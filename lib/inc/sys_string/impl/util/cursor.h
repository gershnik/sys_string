//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_CURSOR_H_INCLUDED
#define HEADER_SYS_STRING_CURSOR_H_INCLUDED

#include <sys_string/config.h>

#include <type_traits>
#include <iterator>
#include <ostream>

namespace sysstr::util
{
    template<class Container, bool Forward>
    class index_cursor
    {
        static_assert(noexcept(std::size(std::declval<Container>())), "std::size(Container) must be noexcept");
        static_assert(noexcept(std::declval<Container>()[0]), "Container[] must be noexcept");
        
    friend index_cursor<Container, !Forward>;

    private:
        using container_ptr = Container *;
        
    public:
        using size_type = decltype(std::size(std::declval<Container>()));
        using reference = decltype(std::declval<Container>()[0]);
        
        using difference_type = std::make_signed_t<size_type>;
        using value_type = std::remove_cv_t<std::remove_reference_t<reference>>;
        using pointer = std::conditional_t<std::is_reference_v<reference>, std::add_pointer_t<std::remove_reference_t<reference>>, void>;
        
        using iterator_category = std::random_access_iterator_tag;

        using reverse_type = index_cursor<Container, !Forward>;
        
        static constexpr bool is_forward = Forward;
        static constexpr bool is_reverse = !Forward;
    public:
        constexpr index_cursor() noexcept = default;
        
        constexpr index_cursor(Container & cont, size_type idx) noexcept :
            m_cont(&cont),
            m_idx(idx)
        {}
        
        template<bool SrcForward>
        constexpr index_cursor(const index_cursor<Container, SrcForward> & src) noexcept :
            m_cont(src.m_cont),
            m_idx(src.m_idx)
        {}
        
        template<bool SrcForward>
        constexpr index_cursor(const index_cursor<Container, SrcForward> & src, size_type idx) noexcept :
            m_cont(src.m_cont),
            m_idx(idx)
        {}
        
        constexpr reference operator*() const noexcept
            { return (*this->m_cont)[this->m_idx - !Forward]; }
        
        index_cursor & operator++() noexcept
        {
            (*this) += 1;
            return *this;
        }
        index_cursor operator++(int) noexcept
        {
            index_cursor ret = *this;
            (*this) += 1;
            return ret;
        }
        index_cursor & operator+=(difference_type diff) noexcept
        {
            if constexpr (Forward)
                this->m_idx += diff;
            else
                this->m_idx -= diff;
            return *this;
        }
        index_cursor & operator--() noexcept
        {
            (*this) += -1;
            return *this;
        }
        index_cursor operator--(int) noexcept
        {
            index_cursor ret = *this;
            (*this) += -1;
            return ret;
        }
        index_cursor & operator-=(difference_type diff) noexcept
        {
            (*this) += -diff;
            return *this;
        }
        
        friend constexpr index_cursor operator+(const index_cursor & lhs, difference_type rhs) noexcept
        {
            index_cursor ret(lhs);
            ret += rhs;
            return ret;
        }
        
        friend constexpr index_cursor operator+(difference_type lhs, const index_cursor & rhs) noexcept
        {
            index_cursor ret(rhs);
            ret += lhs;
            return ret;
        }
        
        friend constexpr index_cursor operator-(const index_cursor & lhs, difference_type rhs) noexcept
        {
            index_cursor ret(lhs);
            ret -= rhs;
            return ret;
        }
        
        friend constexpr difference_type operator-(const index_cursor & lhs, const index_cursor & rhs) noexcept
            { return Forward ? (lhs.m_idx - rhs.m_idx) : (rhs.m_idx - lhs.m_idx); }

        explicit constexpr operator bool() const noexcept
            { return this->m_idx != (Forward ? std::size(*this->m_cont) : 0); }
        
        
        friend constexpr bool operator==(const index_cursor & lhs, const index_cursor & rhs) noexcept
            { return lhs.m_idx == rhs.m_idx; }
        friend constexpr bool operator!=(const index_cursor & lhs, const index_cursor & rhs) noexcept
            { return !(lhs == rhs); }
        
        friend constexpr bool operator==(const index_cursor & lhs, std::nullptr_t) noexcept
            { return !lhs; }
        friend constexpr bool operator==(std::nullptr_t, const index_cursor & rhs) noexcept
            { return !rhs; }
        friend constexpr bool operator!=(const index_cursor & lhs, std::nullptr_t rhs) noexcept
            { return !(lhs == rhs); }
        friend constexpr bool operator!=(std::nullptr_t lhs, const index_cursor & rhs) noexcept
            { return !(lhs == rhs); }
        
    #if SYS_STRING_USE_SPACESHIP_OPERATOR

        friend std::strong_ordering operator<=>(const index_cursor & lhs, const index_cursor & rhs) noexcept
            { return Forward ? lhs.m_idx <=> rhs.m_idx : rhs.m_idx <=> lhs.m_idx; }
    #else
        friend bool operator<(const index_cursor & lhs, const index_cursor & rhs) noexcept
            { return Forward ? lhs.m_idx < rhs.m_idx : rhs.m_idx < lhs.m_idx; }
        friend bool operator<=(const index_cursor & lhs, const index_cursor & rhs) noexcept
            { return Forward ? lhs.m_idx <= rhs.m_idx : rhs.m_idx <= lhs.m_idx;; }
        friend bool operator>(const index_cursor & lhs, const index_cursor & rhs) noexcept
            { return Forward ? lhs.m_idx > rhs.m_idx : rhs.m_idx > lhs.m_idx;; }
        friend bool operator>=(const index_cursor & lhs, const index_cursor & rhs) noexcept
            { return Forward ? lhs.m_idx >= rhs.m_idx : rhs.m_idx >= lhs.m_idx;; }
    #endif
        
        
        constexpr reverse_type reverse() const noexcept
            { return reverse_type(*this); }

        constexpr size_type position() const noexcept
            { return this->m_idx; }

        container_ptr container() const noexcept
            { return m_cont; }
        
        friend auto operator<<(std::ostream & str, const index_cursor & c) -> std::ostream &
            { return str << c.m_idx; }
        
    private:
        container_ptr m_cont = nullptr;
        size_type m_idx = 0;
    };

    template<class Container, bool Forward>
    class iter_cursor
    {
    private:
        using iter = decltype(std::begin(std::declval<Container>()));

        static_assert(std::is_convertible_v<typename std::iterator_traits<iter>::iterator_category, std::random_access_iterator_tag>,
                      "currently iter_cursor only supports random access iterators");
        static_assert(std::is_nothrow_constructible_v<iter>);
        static_assert(std::is_nothrow_copy_constructible_v<iter>);
        static_assert(std::is_nothrow_move_constructible_v<iter>);
        static_assert(noexcept(std::declval<std::add_lvalue_reference_t<iter>>() += 0), "Container's iterator += must be noexcept");
        static_assert(noexcept(*std::declval<iter>()), "Container's iterator dereference must be noexcept");
        static_assert(noexcept(std::declval<iter>() == std::declval<iter>()), "Container's iterator equality comparison be noexcept");
        
        
        friend iter_cursor<Container, !Forward>;

    public:
        using size_type = decltype(std::size(std::declval<Container>()));
        using reference = typename std::iterator_traits<iter>::reference;
        using value_type = typename std::iterator_traits<iter>::value_type;
        using pointer = typename std::iterator_traits<iter>::pointer;

        using difference_type = std::make_signed_t<size_type>; //for pointer iterators this is more accurate than what traits returns
        
        using iterator_category = std::random_access_iterator_tag;

        using reverse_type = iter_cursor<Container, !Forward>;
        
        static constexpr bool is_forward = Forward;
        static constexpr bool is_reverse = !Forward;
        
    public:
        constexpr iter_cursor() noexcept = default;
        
        constexpr iter_cursor(iter begin, iter end, iter current) noexcept :
            m_begin(begin),
            m_end(end),
            m_current(current)
        {}
        
        constexpr iter_cursor(Container & cont, iter current) noexcept :
            m_begin(std::begin(cont)),
            m_end(std::end(cont)),
            m_current(current)
        {}
        
        template<bool SrcForward>
        constexpr iter_cursor(const iter_cursor<Container, SrcForward> & src) noexcept :
            m_begin(src.m_begin),
            m_end(src.m_end),
            m_current(src.m_current)
        {}
        
        constexpr reference operator*() const noexcept
            { return *(this->m_current - !Forward); }
        
        
        iter_cursor & operator++() noexcept
        {
            (*this) += 1;
            return *this;
        }
        iter_cursor operator++(int) noexcept
        {
            index_cursor ret = *this;
            (*this) += 1;
            return ret;
        }
        iter_cursor & operator+=(difference_type diff) noexcept
        {
            constexpr difference_type diff_sign = -2 * !Forward + 1;
            this->m_current += diff_sign * diff;
            return *this;
        }
        iter_cursor & operator--() noexcept
        {
            (*this) += -1;
            return *this;
        }
        iter_cursor operator--(int) noexcept
        {
            iter_cursor ret = *this;
            (*this) += -1;
            return ret;
        }
        iter_cursor & operator-=(difference_type diff) noexcept
        {
            (*this) += -diff;
            return *this;
        }
        
        friend constexpr iter_cursor operator+(const iter_cursor & lhs, difference_type rhs) noexcept
        {
            iter_cursor ret(lhs);
            ret += rhs;
            return ret;
        }
        
        friend constexpr iter_cursor operator+(difference_type lhs, const iter_cursor & rhs) noexcept
        {
            iter_cursor ret(rhs);
            ret += lhs;
            return ret;
        }
        
        friend constexpr iter_cursor operator-(const iter_cursor & lhs, difference_type rhs) noexcept
        {
            iter_cursor ret(lhs);
            ret -= rhs;
            return ret;
        }
        friend constexpr difference_type operator-(const iter_cursor & lhs, const iter_cursor & rhs) noexcept
        {
            return (2 * Forward - 1) * difference_type(lhs.m_current - rhs.m_current);
        }
        
        explicit constexpr operator bool() const noexcept
            { return this->m_current != (Forward ? this->m_end : this->m_begin); }
        
        
        friend constexpr bool operator==(const iter_cursor & lhs, const iter_cursor & rhs) noexcept
            { return lhs.m_current == rhs.m_current; }
        friend constexpr bool operator!=(const iter_cursor & lhs, const iter_cursor & rhs) noexcept
            { return !(lhs == rhs); }
        
        friend constexpr bool operator==(const iter_cursor & lhs, std::nullptr_t) noexcept
            { return !lhs; }
        friend constexpr bool operator==(std::nullptr_t, const iter_cursor & rhs) noexcept
            { return !rhs; }
        friend constexpr bool operator!=(const iter_cursor & lhs, std::nullptr_t rhs) noexcept
            { return !(lhs == rhs); }
        friend constexpr bool operator!=(std::nullptr_t lhs, const iter_cursor & rhs) noexcept
            { return !(lhs == rhs); }
        
    #if SYS_STRING_USE_SPACESHIP_OPERATOR

        friend std::strong_ordering operator<=>(const iter_cursor & lhs, const iter_cursor & rhs) noexcept
            { return (lhs - rhs) <=> 0; }
    #else
        friend bool operator<(const iter_cursor & lhs, const iter_cursor & rhs) noexcept
            { return (lhs - rhs) < 0; }
        friend bool operator<=(const iter_cursor & lhs, const iter_cursor & rhs) noexcept
            { return (lhs - rhs) <= 0; }
        friend bool operator>(const iter_cursor & lhs, const iter_cursor & rhs) noexcept
            { return (lhs - rhs) > 0; }
        friend bool operator>=(const iter_cursor & lhs, const iter_cursor & rhs) noexcept
            { return (lhs - rhs) >= 0; }
    #endif
        
        
        constexpr reverse_type reverse() const noexcept
            { return reverse_type(*this); }

        

        constexpr size_type position() const noexcept
            { return size_type(this->m_current - this->m_begin); }
        
        iter iterator() const noexcept
            { return m_current; }
        
        friend auto operator<<(std::ostream & str, const iter_cursor & c) -> std::ostream &
            { return str << c.position(); }
    private:
        iter m_begin {};
        iter m_end {};
        iter m_current {};
    };

    
    template<bool Forward>
    struct call_member_cursor_begin
    {
        template<class T>
        auto operator()(T && obj) const -> decltype(std::forward<T>(obj).template cursor_begin<Forward>());
    };

    template<bool Forward, class T>
    constexpr bool has_member_cursor_begin = std::is_invocable_v<call_member_cursor_begin<Forward>, std::remove_reference_t<T>>;

    template<bool Forward, class T>
    SYS_STRING_FORCE_INLINE
    auto cursor_begin(T && cont) noexcept -> decltype(cont.template cursor_begin<Forward>())
    {
        static_assert(noexcept(cont.template cursor_begin<Forward>()), "member cursor_begin must be noexcept");
        return cont.template cursor_begin<Forward>();
    }

    template<bool Forward, class T>
    SYS_STRING_FORCE_INLINE
    auto cursor_end(T && cont) noexcept -> decltype(cont.template cursor_end<Forward>())
    {
        static_assert(noexcept(cont.template cursor_end<Forward>()), "member cursor_begin must be noexcept");
        return cont.template cursor_end<Forward>();
    }

    template<bool Forward, class T>
    SYS_STRING_FORCE_INLINE
    auto cursor_at(T && cont, typename T::size_type size) noexcept -> decltype(cont.template cursor_at<Forward>(size))
    {
        static_assert(noexcept(cont.template cursor_at<Forward>(size)), "member cursor_begin must be noexcept");
        return cont.template cursor_at<Forward>(size);
    }

    template<bool Forward, class T>
    SYS_STRING_FORCE_INLINE
    auto cursor_begin(T && cont) noexcept -> std::enable_if_t<!has_member_cursor_begin<Forward, T>,
    iter_cursor<std::remove_reference_t<T>, Forward>>
    {
        if constexpr (Forward)
            return iter_cursor<std::remove_reference_t<T>, Forward>(cont, std::begin(cont));
        else
            return iter_cursor<std::remove_reference_t<T>, Forward>(cont, std::end(cont));
    }

    template<bool Forward, class T>
    SYS_STRING_FORCE_INLINE
    auto cursor_end(T && cont) noexcept -> std::enable_if_t<!has_member_cursor_begin<Forward, T>,
    iter_cursor<std::remove_reference_t<T>, Forward>>
    {
        if constexpr (Forward)
            return iter_cursor<std::remove_reference_t<T>, Forward>(cont, std::end(cont));
        else
            return iter_cursor<std::remove_reference_t<T>, Forward>(cont, std::begin(cont));
    }

    template<bool Forward, class T>
    SYS_STRING_FORCE_INLINE
    auto cursor_at(T && cont, decltype(std::size(cont)) pos) noexcept -> std::enable_if_t<!has_member_cursor_begin<Forward, T>,
    iter_cursor<std::remove_reference_t<T>, Forward>>
    {
        if constexpr (Forward)
            return iter_cursor<std::remove_reference_t<T>, Forward>(cont, std::begin(cont) + pos);
        else
            return iter_cursor<std::remove_reference_t<T>, Forward>(cont, std::end(cont) - pos);
    }
}

#endif
