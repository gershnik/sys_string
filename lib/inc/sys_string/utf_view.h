//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_UTF_VIEW_H_INCLUDED
#define HEADER_SYS_STRING_UTF_VIEW_H_INCLUDED

#include <sys_string/impl/unicode/utf_util.h>
#include <sys_string/impl/util/cursor.h>

#include <array>

namespace sysstr
{
    template<class Container>
    struct utf_access_traits;

    template<utf_encoding Enc, class Container, class Traits = utf_access_traits<Container>>
    class utf_view;

    namespace util
    {
        template<class Cursor>
        SYS_STRING_FORCE_INLINE
        char32_t current_utf32(Cursor & cursor) 
        {
            if constexpr (Cursor::is_forward)
                return utf32_input<utf_encoding_of<typename Cursor::value_type>>::read(cursor);
            else
                return utf32_input<utf_encoding_of<typename Cursor::value_type>>::read_reversed(cursor);
        }

        template<utf_encoding OutputEnc, class Cursor>
        class utf_cursor
        {
        public:
            using size_type = typename Cursor::size_type;
            using difference_type = typename Cursor::difference_type;
            using value_type = utf_char_of<OutputEnc>;
            using reference = const value_type;
            using pointer = void;
            
            using iterator_category = std::forward_iterator_tag;
            
            using reverse_type = utf_cursor<OutputEnc, typename Cursor::reverse_type>;
            
            static constexpr bool is_reverse = Cursor::is_reverse;
            static constexpr bool is_forward = Cursor::is_forward;
        public:
            constexpr utf_cursor() noexcept:
                m_char_idx(0)
            {}
            SYS_STRING_FORCE_INLINE
            constexpr utf_cursor(const Cursor & wrapped) noexcept:
                m_next(wrapped)
            {
                this->load_next();
            }
            
            constexpr value_type operator*() const noexcept
                { return this->m_encoder.begin()[this->m_char_idx]; }

            explicit constexpr operator bool() const noexcept
                { return this->m_char_idx != this->m_encoder.size(); }

            SYS_STRING_FORCE_INLINE
            constexpr utf_cursor & operator++() noexcept
            {
                if constexpr (is_forward)
                {
                    if (++this->m_char_idx != this->m_encoder.size())
                        return *this;
                }
                else
                {
                    if (this->m_char_idx > 0)
                    {
                        --this->m_char_idx;
                        return *this;
                    }
                }

                this->load_next();
                return *this;
            }

            constexpr utf_cursor operator++(int) noexcept
            {
                utf_cursor ret = *this;
                ++(*this);
                return ret;
            }
            
            friend constexpr bool operator==(const utf_cursor & lhs, const utf_cursor & rhs) noexcept
                { return lhs.m_char_idx == rhs.m_char_idx && lhs.m_current == rhs.m_current; }
            friend constexpr bool operator!=(const utf_cursor & lhs, const utf_cursor & rhs) noexcept
                { return !(lhs == rhs); }
            
            friend constexpr bool operator==(const utf_cursor & lhs, std::nullptr_t) noexcept
                { return !lhs; }
            friend constexpr bool operator==(std::nullptr_t, const utf_cursor & rhs) noexcept
                { return !rhs; }
            friend constexpr bool operator!=(const utf_cursor & lhs, std::nullptr_t rhs) noexcept
                { return !(lhs == rhs); }
            friend constexpr bool operator!=(std::nullptr_t lhs, const utf_cursor & rhs) noexcept
                { return !(lhs == rhs); }
            
            reverse_type reverse() const noexcept
                { return this->m_current.reverse(); }
            
            utf_cursor<utf32, Cursor> char_start() const noexcept
                { return this->m_current; }
            
            size_type storage_pos() const noexcept
                { return this->m_current.position(); }
            
            size_type storage_size() const noexcept
                { return this->m_next - this->m_current; }
            
            const Cursor storage_cursor() const noexcept
                { return this->m_current; }
            
            friend auto operator<<(std::ostream & str, const utf_cursor & c) -> std::ostream &
            {
                str << c.m_current << ", char " << c.m_char_idx << " in {";
                for (uint8_t i = 0; i < c.m_encoder.size(); ++i)
                {
                    if (i)
                        str << ',';
                    str << unsigned(c.m_encoder.begin()[i]);
                }
                str << "}";
                return str;
            }
        private:
            SYS_STRING_FORCE_INLINE
            constexpr void load_next() noexcept
            {
                this->m_current = this->m_next;

                if (this->m_next)
                {
                    auto c = current_utf32(this->m_next);
                    this->m_encoder.put(c);
                    this->m_char_idx = is_forward ? 0 : this->m_encoder.size() - 1;
                }
                else
                {
                    this->m_encoder.clear();
                    this->m_char_idx = 0;
                }
            }

        private:
            Cursor m_current;
            Cursor m_next;
            utf_codepoint_encoder<OutputEnc, false> m_encoder;
            unsigned m_char_idx;
        };

        template<class Cursor>
        class utf_cursor<utf32, Cursor>
        {
        public:
            using size_type = typename Cursor::size_type;
            using difference_type = typename Cursor::difference_type;
            using value_type = char32_t;
            using reference = const char32_t;
            using pointer = void;
            
            using iterator_category = std::forward_iterator_tag;
            
            using reverse_type = utf_cursor<utf32, typename Cursor::reverse_type>;
            
            static constexpr bool is_reverse = Cursor::is_reverse;
            static constexpr bool is_forward = Cursor::is_forward;
        public:
            constexpr utf_cursor() noexcept = default;

            SYS_STRING_FORCE_INLINE
            constexpr utf_cursor(const Cursor & wrapped) noexcept :
                m_current(wrapped),
                m_next(wrapped),
                m_value(wrapped ? current_utf32(m_next) : 0)
            {
            }
            
            constexpr value_type operator*() const noexcept
                { return this->m_value; }

            explicit constexpr operator bool() const noexcept
                { return bool(this->m_current); }

            SYS_STRING_FORCE_INLINE
            constexpr utf_cursor & operator++() noexcept
            {
                this->m_current = this->m_next;
                if (this->m_next)
                    this->m_value = current_utf32(this->m_next);
                return *this;
            }

            SYS_STRING_FORCE_INLINE
            constexpr utf_cursor operator++(int) noexcept
            {
                utf_cursor ret = *this;
                ++(*this);
                return ret;
            }

            friend constexpr bool operator==(const utf_cursor & lhs, const utf_cursor & rhs) noexcept
                { return lhs.m_current == rhs.m_current; }
            friend constexpr bool operator!=(const utf_cursor & lhs, const utf_cursor & rhs) noexcept
                { return !(lhs == rhs); }
            
            friend constexpr bool operator==(const utf_cursor & lhs, std::nullptr_t) noexcept
                { return !lhs; }
            friend constexpr bool operator==(std::nullptr_t, const utf_cursor & rhs) noexcept
                { return !rhs; }
            friend constexpr bool operator!=(const utf_cursor & lhs, std::nullptr_t rhs) noexcept
                { return !(lhs == rhs); }
            friend constexpr bool operator!=(std::nullptr_t lhs, const utf_cursor & rhs) noexcept
                { return !(lhs == rhs); }
            
            reverse_type reverse() const noexcept
                { return this->m_current.reverse(); }
            
            const utf_cursor & char_start() const noexcept
                { return *this; }
            
            size_type storage_pos() const noexcept
                { return this->m_current.position(); }
            
            size_type storage_size() const noexcept
                { return this->m_next - this->m_current; }
            
            const Cursor storage_cursor() const noexcept
                { return this->m_current; }
            
            friend auto operator<<(std::ostream & str, const utf_cursor & c) -> std::ostream &
            {
                str << c.m_current << ", value " << uint32_t(c.m_value);
                return str;
            }
        private:
            Cursor m_current;
            Cursor m_next;
            value_type m_value = 0;
        };
    }

    template<class Container>
    struct utf_access_traits
    {
        using char_access = std::add_lvalue_reference_t<Container>;
        
        static decltype(auto) adapt(std::add_lvalue_reference_t<Container> src) noexcept
        {
            return src;
        }
    };

    template<class Char, size_t N>
    struct utf_access_traits<Char [N]>
    {
        using char_access = std::basic_string_view<const Char>;
        
        static constexpr std::basic_string_view<const Char> adapt(Char (& src) [N]) noexcept
            { return src; }
    };

    template<utf_encoding Enc, class Container, class Traits>
    class utf_view
    {
    private:
        using char_access = typename Traits::char_access;
        
        template<bool Forward>
        static auto detect_cursor_type(std::add_pointer_t<const char_access> access = nullptr) noexcept
        {
            using util::cursor_begin;
            return cursor_begin<Forward>(*access);
        }
        using access_cursor_type = decltype(detect_cursor_type<true>());
        using access_reverse_cursor_type = decltype(detect_cursor_type<false>());
    public:
        using iterator = util::utf_cursor<Enc, access_cursor_type>;
        using const_iterator = iterator;
        using reverse_iterator = util::utf_cursor<Enc, access_reverse_cursor_type>;
        using const_reverse_iterator = reverse_iterator;
        
        using value_type = typename iterator::value_type;
        using reference = typename iterator::reference;
        using const_reference = reference;
        using pointer = typename iterator::pointer;
        using const_pointer = pointer;
        
        using cursor_type = util::utf_cursor<Enc, access_cursor_type>;
        using reverse_cursor_type = util::utf_cursor<Enc, access_reverse_cursor_type>;
    public:
        utf_view(const Container & src) noexcept : m_access(Traits::adapt(src))
        {}
        SYS_STRING_FORCE_INLINE iterator begin() const noexcept
            { return this->cursor_begin<true>(); }
        SYS_STRING_FORCE_INLINE iterator end() const noexcept
            { return this->cursor_end<true>(); }
        SYS_STRING_FORCE_INLINE const_iterator cbegin() const noexcept
            { return begin(); }
        SYS_STRING_FORCE_INLINE const_iterator cend() const noexcept
            { return end(); }
        SYS_STRING_FORCE_INLINE reverse_iterator rbegin() const noexcept
            { return this->cursor_begin<false>(); }
        SYS_STRING_FORCE_INLINE reverse_iterator rend() const noexcept
            { return this->cursor_end<false>(); }
        SYS_STRING_FORCE_INLINE const_reverse_iterator crbegin() const noexcept
            { return rbegin(); }
        SYS_STRING_FORCE_INLINE const_reverse_iterator crend() const noexcept
            { return rend(); }
        
        template<bool Forward>
        SYS_STRING_FORCE_INLINE
        decltype(auto) cursor_begin() const noexcept
        {
            using util::cursor_begin, util::utf_cursor;
            return utf_cursor<Enc, decltype(detect_cursor_type<Forward>())>(cursor_begin<Forward>(this->m_access));
        }
        
        template<bool Forward>
        SYS_STRING_FORCE_INLINE
        decltype(auto) cursor_end() const noexcept
        {
            using util::cursor_end, util::utf_cursor;
            return utf_cursor<Enc, decltype(detect_cursor_type<Forward>())>(cursor_end<Forward>(this->m_access));
        }
    private:
        char_access m_access;
    };
}

#endif
