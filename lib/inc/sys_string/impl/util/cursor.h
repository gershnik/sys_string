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
    enum class cursor_direction : bool
    {
        backward = false,
        forward = true
    };
    constexpr cursor_direction reverse(cursor_direction dir) noexcept
        { return cursor_direction(!bool(dir)); }
    
    template<class Container, cursor_direction Direction>
    class index_cursor
    {
    friend index_cursor<Container, util::reverse(Direction)>;

    private:
        using container_ptr = Container *;
        
    public:
        using size_type = decltype(std::size(std::declval<Container>()));
        using reference = decltype(std::declval<Container>()[0]);
        
        using difference_type = std::make_signed_t<size_type>;
        using value_type = std::remove_cv_t<std::remove_reference_t<reference>>;
        using pointer = std::conditional_t<std::is_reference_v<reference>, std::add_pointer_t<std::remove_reference_t<reference>>, void>;
        
        using iterator_category = std::random_access_iterator_tag;

        using reverse_type = index_cursor<Container, util::reverse(Direction)>;
        
        static constexpr bool is_forward = (Direction == cursor_direction::forward);
        static constexpr bool is_reverse = !is_forward;
    public:
        constexpr index_cursor() noexcept = default;
        
        constexpr index_cursor(Container & cont, size_type idx) noexcept :
            m_cont(&cont),
            m_idx(idx)
        {}
        
        template<cursor_direction SrcDirection>
        constexpr index_cursor(const index_cursor<Container, SrcDirection> & src) noexcept :
            m_cont(src.m_cont),
            m_idx(src.m_idx)
        {}
        
        template<cursor_direction SrcDirection>
        constexpr index_cursor(const index_cursor<Container, SrcDirection> & src, size_type idx) noexcept :
            m_cont(src.m_cont),
            m_idx(idx)
        {}
        
        constexpr reference operator*() const
            { return (*this->m_cont)[this->m_idx - !index_cursor::is_forward]; }
        
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
            if constexpr (index_cursor::is_forward)
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
            { return index_cursor::is_forward ? (lhs.m_idx - rhs.m_idx) : (rhs.m_idx - lhs.m_idx); }

        explicit constexpr operator bool() const
            { return this->m_idx != (index_cursor::is_forward ? std::size(*this->m_cont) : 0); }
        
        
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
            { return is_forward ? lhs.m_idx <=> rhs.m_idx : rhs.m_idx <=> lhs.m_idx; }
    #else
        friend bool operator<(const index_cursor & lhs, const index_cursor & rhs) noexcept
            { return index_cursor::is_forward ? lhs.m_idx < rhs.m_idx : rhs.m_idx < lhs.m_idx; }
        friend bool operator<=(const index_cursor & lhs, const index_cursor & rhs) noexcept
            { return index_cursor::is_forward ? lhs.m_idx <= rhs.m_idx : rhs.m_idx <= lhs.m_idx;; }
        friend bool operator>(const index_cursor & lhs, const index_cursor & rhs) noexcept
            { return index_cursor::is_forward ? lhs.m_idx > rhs.m_idx : rhs.m_idx > lhs.m_idx;; }
        friend bool operator>=(const index_cursor & lhs, const index_cursor & rhs) noexcept
            { return index_cursor::is_forward ? lhs.m_idx >= rhs.m_idx : rhs.m_idx >= lhs.m_idx;; }
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
    
    template<class T>
    using iterator_of = decltype(std::begin(std::declval<std::remove_reference_t<T>>()));
    template<class T>
    using end_iterator_of = decltype(std::end(std::declval<std::remove_reference_t<T>>()));


    template<class It, class EndIt, cursor_direction Direction, class SizeT = std::make_unsigned_t<typename std::iterator_traits<It>::difference_type>>
    class iter_cursor
    {
        friend iter_cursor<It, EndIt, util::reverse(Direction), SizeT>;
    public:
        static constexpr bool is_forward = Direction == cursor_direction::forward;
        static constexpr bool is_reverse = !is_forward;
    private:
        static constexpr bool is_bidi = std::is_convertible_v<typename std::iterator_traits<It>::iterator_category, std::bidirectional_iterator_tag>;
        static constexpr bool is_random = std::is_convertible_v<typename std::iterator_traits<It>::iterator_category, std::random_access_iterator_tag>;

        static_assert(is_bidi || is_forward, "Iterator must be at least bidirectional to support reverse cursor");
        
    public:
        using size_type = SizeT;
        using reference = typename std::iterator_traits<It>::reference;
        using value_type = typename std::iterator_traits<It>::value_type;
        using pointer = typename std::iterator_traits<It>::pointer;

        using difference_type = std::make_signed_t<size_type>;
        
        using iterator_category = typename std::iterator_traits<It>::iterator_category;

        using reverse_type = iter_cursor<It, EndIt, util::reverse(Direction), SizeT>;
        
    public:
        constexpr iter_cursor() = default;
        
        constexpr iter_cursor(It begin, EndIt end, It current) :
            m_begin(std::move(begin)),
            m_end(std::move(end)),
            m_current(std::move(current))
        {}
        
        template<class Container>
        constexpr iter_cursor(Container & cont,
                              std::enable_if_t<std::is_same_v<iterator_of<Container>, It> && std::is_same_v<end_iterator_of<Container>, EndIt>,
                              It> current) :
            m_begin(std::begin(cont)),
            m_end(std::end(cont)),
            m_current(std::move(current))
        {}
        
        template<cursor_direction SrcDirection>
        constexpr iter_cursor(const iter_cursor<It, EndIt, SrcDirection, SizeT> & src) :
            m_begin(src.m_begin),
            m_end(src.m_end),
            m_current(src.m_current)
        {}
        
        constexpr reference operator*() const
        {
            if constexpr (iter_cursor::is_forward)
            {
                return *this->m_current;
            }
            else
            {
                It temp(this->m_current);
                --temp;
                return *temp;
            }
        }
        
        
        iter_cursor & operator++()
        {
            if constexpr (iter_cursor::is_forward)
                ++this->m_current;
            else
                --this->m_current;
            return *this;
        }
        
        iter_cursor operator++(int)
        {
            iter_cursor ret = *this;
            ++(*this);
            return ret;
        }
        
        template<class Cursor=iter_cursor>
        std::enable_if_t<Cursor::is_bidi,
        iter_cursor> & operator--()
        {
            if constexpr (iter_cursor::is_forward)
                --this->m_current;
            else
                ++this->m_current;
            return *this;
        }
        
        template<class Cursor=iter_cursor>
        std::enable_if_t<Cursor::is_bidi,
        iter_cursor> operator--(int)
        {
            iter_cursor ret = *this;
            --(*this);
            return ret;
        }
        
        template<class Cursor=iter_cursor>
        std::enable_if_t<Cursor::is_random,
        iter_cursor> & operator+=(difference_type diff) 
        {
            constexpr difference_type diff_sign = -2 * !iter_cursor::is_forward + 1;
            this->m_current += diff_sign * diff;
            return *this;
        }
        
        template<class Cursor=iter_cursor>
        std::enable_if_t<Cursor::is_random,
        iter_cursor> & operator-=(difference_type diff)
        {
            *this += -diff;
            return *this;
        }
        
        template<class Cursor=iter_cursor>
        friend std::enable_if_t<Cursor::is_random,
        iter_cursor> operator+(const iter_cursor & lhs, difference_type rhs)
        {
            iter_cursor ret(lhs);
            ret += rhs;
            return ret;
        }
        
        template<class Cursor=iter_cursor>
        friend std::enable_if_t<Cursor::is_random,
        iter_cursor> operator+(difference_type lhs, const iter_cursor & rhs)
        {
            iter_cursor ret(rhs);
            ret += lhs;
            return ret;
        }
        
        template<class Cursor=iter_cursor>
        friend std::enable_if_t<Cursor::is_random,
        iter_cursor> operator-(const iter_cursor & lhs, difference_type rhs)
        {
            iter_cursor ret(lhs);
            ret -= rhs;
            return ret;
        }
        
        template<class Cursor=iter_cursor>
        friend std::enable_if_t<Cursor::is_random,
        difference_type> operator-(const iter_cursor & lhs, const iter_cursor & rhs)
        {
            if constexpr (iter_cursor::is_forward)
                return difference_type(lhs.m_current - rhs.m_current);
            else
                return difference_type(rhs.m_current - lhs.m_current);
        }
        
        explicit constexpr operator bool() const
            { return this->m_current != (iter_cursor::is_forward ? this->m_end : this->m_begin); }
        
        
        friend constexpr bool operator==(const iter_cursor & lhs, const iter_cursor & rhs)
            { return lhs.m_current == rhs.m_current; }
        friend constexpr bool operator!=(const iter_cursor & lhs, const iter_cursor & rhs)
            { return !(lhs == rhs); }
        
        friend constexpr bool operator==(const iter_cursor & lhs, std::nullptr_t)
            { return !lhs; }
        friend constexpr bool operator==(std::nullptr_t, const iter_cursor & rhs)
            { return !rhs; }
        friend constexpr bool operator!=(const iter_cursor & lhs, std::nullptr_t rhs)
            { return !(lhs == rhs); }
        friend constexpr bool operator!=(std::nullptr_t lhs, const iter_cursor & rhs)
            { return !(lhs == rhs); }
        
    #if SYS_STRING_USE_SPACESHIP_OPERATOR

        template<class Cursor=iter_cursor>
        friend std::enable_if_t<Cursor::is_random,
        std::strong_ordering> operator<=>(const iter_cursor & lhs, const iter_cursor & rhs)
            { return (lhs - rhs) <=> 0; }
    #else
        template<class Cursor=iter_cursor>
        friend std::enable_if_t<Cursor::is_random,
        bool> operator<(const iter_cursor & lhs, const iter_cursor & rhs)
            { return (lhs - rhs) < 0; }
        template<class Cursor=iter_cursor>
        friend std::enable_if_t<Cursor::is_random,
        bool> operator<=(const iter_cursor & lhs, const iter_cursor & rhs)
            { return (lhs - rhs) <= 0; }
        template<class Cursor=iter_cursor>
        friend std::enable_if_t<Cursor::is_random,
        bool> operator>(const iter_cursor & lhs, const iter_cursor & rhs)
            { return (lhs - rhs) > 0; }
        template<class Cursor=iter_cursor>
        friend std::enable_if_t<Cursor::is_random,
        bool> operator>=(const iter_cursor & lhs, const iter_cursor & rhs)
            { return (lhs - rhs) >= 0; }
    #endif
        
        template<class Cursor=iter_cursor>
        constexpr std::enable_if_t<Cursor::is_bidi,
        reverse_type> reverse() const
            { return reverse_type(*this); }

        
        template<class Cursor=iter_cursor>
        constexpr std::enable_if_t<Cursor::is_random,
        size_type> position() const
            { return size_type(this->m_current - this->m_begin); }
        
        It iterator() const
            { return m_current; }
        
        friend auto operator<<(std::ostream & str, const iter_cursor & c) -> std::ostream &
        {
            if constexpr (is_random)
                return str << c.position();
            else
                return str << "non random iter_cursor";
        }
    private:
        It m_begin {};
        EndIt m_end {};
        It m_current {};
    };

    
    template<cursor_direction Direction>
    struct call_member_cursor_begin
    {
        template<class T>
        auto operator()(T && obj) const -> decltype(std::forward<T>(obj).template cursor_begin<Direction>());
    };
    template<cursor_direction Direction>
    struct call_member_cursor_end
    {
        template<class T>
        auto operator()(T && obj) const -> decltype(std::forward<T>(obj).template cursor_end<Direction>());
    };
    
    
    template<cursor_direction Direction, class T>
    constexpr bool has_member_cursor_begin = std::is_invocable_v<call_member_cursor_begin<Direction>, std::remove_reference_t<T>>;
    template<cursor_direction Direction, class T>
    constexpr bool has_member_cursor_end = std::is_invocable_v<call_member_cursor_end<Direction>, std::remove_reference_t<T>>;

    template<cursor_direction Direction, class T>
    SYS_STRING_FORCE_INLINE
    auto cursor_begin(T && cont) -> decltype(cont.template cursor_begin<Direction>())
    {
        static_assert(noexcept(cont.template cursor_begin<Direction>()), "member cursor_begin must be noexcept");
        return cont.template cursor_begin<Direction>();
    }

    template<cursor_direction Direction, class T>
    SYS_STRING_FORCE_INLINE
    auto cursor_end(T && cont) -> decltype(cont.template cursor_end<Direction>())
    {
        return cont.template cursor_end<Direction>();
    }

    template<cursor_direction Direction, class T>
    SYS_STRING_FORCE_INLINE
    auto cursor_at(T && cont, typename T::size_type size) -> decltype(cont.template cursor_at<Direction>(size))
    {
        return cont.template cursor_at<Direction>(size);
    }

    template<cursor_direction Direction, class T>
    SYS_STRING_FORCE_INLINE
    auto cursor_begin(T && cont) -> std::enable_if_t<!has_member_cursor_begin<Direction, T>,
    iter_cursor<iterator_of<T>, end_iterator_of<T>, Direction, decltype(std::size(cont))>>
    {
        if constexpr (Direction == cursor_direction::forward)
            return iter_cursor<iterator_of<T>, end_iterator_of<T>, Direction, decltype(std::size(cont))>(cont, std::begin(cont));
        else
            return iter_cursor<iterator_of<T>, end_iterator_of<T>, Direction, decltype(std::size(cont))>(cont, std::end(cont));
    }

    template<cursor_direction Direction, class T>
    SYS_STRING_FORCE_INLINE
    auto cursor_end(T && cont) -> std::enable_if_t<!has_member_cursor_end<Direction, T>,
    iter_cursor<iterator_of<T>, end_iterator_of<T>, Direction, decltype(std::size(cont))>>
    {
        if constexpr (Direction == cursor_direction::forward)
            return iter_cursor<iterator_of<T>, end_iterator_of<T>, Direction, decltype(std::size(cont))>(cont, std::end(cont));
        else
            return iter_cursor<iterator_of<T>, end_iterator_of<T>, Direction, decltype(std::size(cont))>(cont, std::begin(cont));
    }

    template<cursor_direction Direction, class T>
    SYS_STRING_FORCE_INLINE
    auto cursor_at(T && cont, decltype(std::size(cont)) pos) -> std::enable_if_t<
        !has_member_cursor_begin<Direction, T> &&
        std::is_convertible_v<typename std::iterator_traits<iterator_of<T>>::iterator_category, std::random_access_iterator_tag>,
    iter_cursor<iterator_of<T>, end_iterator_of<T>, Direction, decltype(std::size(cont))>>
    {
        if constexpr (Direction == cursor_direction::forward)
            return iter_cursor<iterator_of<T>, end_iterator_of<T>, Direction, decltype(std::size(cont))>(cont, std::begin(cont) + pos);
        else
            return iter_cursor<iterator_of<T>, end_iterator_of<T>, Direction, decltype(std::size(cont))>(cont, std::end(cont) - pos);
    }
}

#endif
