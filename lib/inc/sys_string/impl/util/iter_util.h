//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_ITER_UTIL_H_INCLUDED
#define HEADER_SYS_STRING_ITER_UTIL_H_INCLUDED

#include <sys_string/config.h>

#include <iterator>
#include <ranges>

namespace sysstr::ranges //non-standard extensions to std::ranges
{
    template<class T, class Base>
    concept reverse_iterator_of =
        std::input_iterator<T> &&
        std::input_iterator<Base> &&
        std::is_constructible_v<T, Base> &&
    requires(T t) {
      
        { t.base() } -> std::same_as<Base>;
    };

    template<std::ranges::range R>
    using reverse_iterator_t = decltype(std::ranges::rbegin(std::declval<R&>()));
    template<std::ranges::range R>
    using const_reverse_iterator_t = decltype(std::ranges::crbegin(std::declval<R&>()));
    template<std::ranges::range R>
    using reverse_sentinel_t = decltype(std::ranges::rend(std::declval<R&>()));
    template<std::ranges::range R>
    using const_reverse_sentinel_t = decltype(std::ranges::crend(std::declval<R&>()));

    template<class R>
    concept reverse_traversable_range = std::ranges::range<R> && requires(R & r) {
        std::ranges::rbegin(r);
        std::ranges::rend(r);
        std::ranges::crbegin(r);
        std::ranges::crend(r);
    };

    template<class R>
    concept common_reverse_traversable_range = reverse_traversable_range<R> && 
                                                std::ranges::common_range<R> &&
                                                std::same_as<reverse_iterator_t<R>, reverse_sentinel_t<R>>;

    template<class R>
    concept standard_reverse_traversable_range = common_reverse_traversable_range<R> &&
                                            reverse_iterator_of<reverse_iterator_t<R>, std::ranges::iterator_t<R>>;

    
    template<class R>
    concept custom_reverse_traversable_range = reverse_traversable_range<R> && requires(R & r) 
    {
        { r.reverse(std::declval<std::ranges::iterator_t<R>>()) } -> std::same_as<reverse_iterator_t<R>>;
        { r.reverse(std::declval<reverse_iterator_t<R>>()) } -> std::same_as<std::ranges::iterator_t<R>>;
    };


    template<standard_reverse_traversable_range R>
    decltype(auto) make_reverse_iterator(R && /*range*/, std::ranges::iterator_t<std::remove_reference_t<R>> it)
        { return reverse_iterator_t<R>(it); }

    template<standard_reverse_traversable_range R>
    decltype(auto) make_reverse_iterator(R && /*range*/, reverse_iterator_t<std::remove_reference_t<R>> it)
        { return it.base(); }

    template<custom_reverse_traversable_range R>
    decltype(auto) make_reverse_iterator(R && range, std::ranges::iterator_t<std::remove_reference_t<R>> it)
        { return range.reverse(it); }

    template<custom_reverse_traversable_range R>
    decltype(auto) make_reverse_iterator(R && range, reverse_iterator_t<std::remove_reference_t<R>> it)
        { return range.reverse(it); }

    template<class R>
    concept reversible_range = reverse_traversable_range<R> && requires(R & r) {
        make_reverse_iterator(r, std::declval<std::ranges::iterator_t<std::remove_reference_t<R>>>());
        make_reverse_iterator(r, std::declval<reverse_iterator_t<std::remove_reference_t<R>>>());
    };
}

namespace sysstr::util
{
    enum class iter_direction : bool
    {
        backward = false,
        forward = true
    };
    constexpr iter_direction reverse(iter_direction dir) noexcept
        { return iter_direction(!bool(dir)); }


    template<class T>
    concept indexable = requires(T & t, decltype(std::size(t)) n)
    {
        t[n];
    };

    template<class Container, iter_direction Direction>
    class index_iterator
    {
        //We cannot declare ourselves as template<indexable Container> becuase iterators are often declared as nested
        //types and within a type the Container is incomplete. Sigh. 
        static_assert(indexable<Container>);

        friend index_iterator<Container, reverse(Direction)>;
    private:
        using container_ptr = Container *;
        
    public:
        using size_type = decltype(std::size(std::declval<Container>()));
        using reference = decltype(std::declval<Container>()[0]);
        using difference_type = std::make_signed_t<size_type>;
        using value_type = std::remove_cvref_t<reference>;

        static constexpr bool is_forward = (Direction == iter_direction::forward);
        static constexpr bool is_reverse = !is_forward;
    public:
        constexpr index_iterator() noexcept = default;
        
        constexpr index_iterator(Container & cont, size_type idx) noexcept :
            m_cont(&cont),
            m_idx(idx)
        {}
        
        template<iter_direction SrcDirection>
        constexpr index_iterator(const index_iterator<Container, SrcDirection> & src) noexcept :
            m_cont(src.m_cont),
            m_idx(src.m_idx)
        {}
        
        template<iter_direction SrcDirection>
        constexpr index_iterator(const index_iterator<Container, SrcDirection> & src, size_type idx) noexcept :
            m_cont(src.m_cont),
            m_idx(idx)
        {}
        
        constexpr reference operator*() const
            { return (*this->m_cont)[this->m_idx - !index_iterator::is_forward]; }

        constexpr reference operator[](difference_type idx) const
        { 
            if constexpr (index_iterator::is_forward)
                return (*this->m_cont)[this->m_idx + idx];
            else
                return (*this->m_cont)[this->m_idx - 1 - idx];
        }
        
        constexpr index_iterator & operator++() noexcept
        {
            (*this) += 1;
            return *this;
        }
        constexpr index_iterator operator++(int) noexcept
        {
            index_iterator ret = *this;
            (*this) += 1;
            return ret;
        }
        constexpr index_iterator & operator+=(difference_type diff) noexcept
        {
            if constexpr (index_iterator::is_forward)
                this->m_idx += diff;
            else
                this->m_idx -= diff;
            return *this;
        }
        index_iterator & operator--() noexcept
        {
            (*this) += -1;
            return *this;
        }
        index_iterator operator--(int) noexcept
        {
            index_iterator ret = *this;
            (*this) += -1;
            return ret;
        }
        index_iterator & operator-=(difference_type diff) noexcept
        {
            (*this) += -diff;
            return *this;
        }
        friend constexpr index_iterator operator+(const index_iterator & lhs, difference_type rhs) noexcept
        {
            index_iterator ret(lhs);
            ret += rhs;
            return ret;
        }
        
        friend constexpr index_iterator operator+(difference_type lhs, const index_iterator & rhs) noexcept
        {
            index_iterator ret(rhs);
            ret += lhs;
            return ret;
        }
        
        friend constexpr difference_type operator-(const index_iterator & lhs, const index_iterator & rhs) noexcept
            { return index_iterator::is_forward ? (lhs.m_idx - rhs.m_idx) : (rhs.m_idx - lhs.m_idx); }
        
        friend constexpr difference_type operator-(const index_iterator & lhs, std::default_sentinel_t) noexcept
            { return index_iterator::is_forward ? (lhs.m_idx - std::size(*lhs.m_cont)) : -lhs.m_idx; }

        friend constexpr difference_type operator-(std::default_sentinel_t, const index_iterator & rhs) noexcept
            { return index_iterator::is_forward ? (std::size(*rhs.m_cont) - rhs.m_idx) : rhs.m_idx; }
        
        friend constexpr index_iterator operator-(const index_iterator & lhs, difference_type rhs) noexcept
        {
            index_iterator ret(lhs);
            ret -= rhs;
            return ret;
        }

        friend constexpr bool operator==(const index_iterator & lhs, const index_iterator & rhs) noexcept
            { return lhs.m_idx == rhs.m_idx; }
        friend constexpr bool operator!=(const index_iterator & lhs, const index_iterator & rhs) noexcept
            { return !(lhs == rhs); }
        
        friend constexpr bool operator==(const index_iterator & lhs, std::default_sentinel_t) noexcept
            { return lhs.m_idx == (index_iterator::is_forward ? std::size(*lhs.m_cont) : 0); }
        friend constexpr bool operator==(std::default_sentinel_t, const index_iterator & rhs) noexcept
            { return rhs == std::default_sentinel; }
        friend constexpr bool operator!=(const index_iterator & lhs, std::default_sentinel_t) noexcept
            { return !(lhs == std::default_sentinel); }
        friend constexpr bool operator!=(std::default_sentinel_t, const index_iterator & rhs) noexcept
            { return !(rhs == std::default_sentinel); }
        
        friend constexpr std::strong_ordering operator<=>(const index_iterator & lhs, const index_iterator & rhs) noexcept
            { return is_forward ? lhs.m_idx <=> rhs.m_idx : rhs.m_idx <=> lhs.m_idx; }
        
        
        constexpr size_type index() const noexcept
            { return this->m_idx; }

        container_ptr container() const noexcept
            { return m_cont; }
        
        index_iterator<Container, iter_direction::forward> base() const requires(is_reverse)
            { return index_iterator<Container, iter_direction::forward>(*this); }
        
    private:
        container_ptr m_cont = nullptr;
        size_type m_idx = 0;
    };
}

#endif
