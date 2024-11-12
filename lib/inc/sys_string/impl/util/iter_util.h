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

namespace sysstr::util
{
    enum class iter_direction : bool
    {
        backward = false,
        forward = true
    };
    constexpr iter_direction reverse(iter_direction dir) noexcept
        { return iter_direction(!bool(dir)); }

    
    template<class T, class Base>
    concept reverse_iterator_of =
        std::input_iterator<T> &&
        std::input_iterator<Base> &&
        std::is_constructible_v<T, Base> &&
    requires(T t) {
      
        { t.base() } -> std::same_as<Base>;
    };


    template<class T>
    concept indexable = requires(T & t, decltype(std::size(t)) n)
    {
        t[n];
    };

    template<class Container, iter_direction Direction>
    class index_iterator
    {
        static_assert(indexable<Container>);

        friend index_iterator<Container, reverse(Direction)>;
    private:
        using container_ptr = Container *;
        
    public:
        using size_type = decltype(std::size(std::declval<Container>()));
        using reference = decltype(std::declval<Container>()[0]);
        using difference_type = std::make_signed_t<size_type>;
        using value_type = std::remove_cv_t<std::remove_reference_t<reference>>;

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
