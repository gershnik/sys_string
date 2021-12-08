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
            static_assert(std::is_convertible_v<typename Cursor::iterator_category, std::random_access_iterator_tag>,
                          "Underlying cursor must be random access to be wrappable in utf_cursor");
            
            friend utf_cursor<OutputEnc, typename Cursor::reverse_type>;
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
            constexpr utf_cursor() noexcept(std::is_nothrow_constructible_v<Cursor>):
                m_char_idx(0),
                m_current_storage_size(0)
            {}
            
            SYS_STRING_FORCE_INLINE
            constexpr utf_cursor(const Cursor & wrapped) noexcept(std::is_nothrow_copy_constructible_v<Cursor>):
                m_next(wrapped)
            {
                this->load_next();
            }
            
            constexpr value_type operator*() const noexcept
                { return this->m_encoder.begin()[this->m_char_idx]; }

            explicit constexpr operator bool() const noexcept
                { return this->m_char_idx != this->m_encoder.size(); }

            SYS_STRING_FORCE_INLINE
            constexpr utf_cursor & operator++()
            {
                if constexpr (is_forward)
                {
                    if (++this->m_char_idx < this->m_encoder.size())
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

            constexpr utf_cursor operator++(int)
            {
                utf_cursor ret = *this;
                ++(*this);
                return ret;
            }
            
            friend constexpr bool operator==(const utf_cursor & lhs, const utf_cursor & rhs)
                { return lhs.m_current_storage_size == rhs.m_current_storage_size && lhs.m_char_idx == rhs.m_char_idx && lhs.m_next == rhs.m_next; }
            friend constexpr bool operator!=(const utf_cursor & lhs, const utf_cursor & rhs)
                { return !(lhs == rhs); }
            
            friend constexpr bool operator==(const utf_cursor & lhs, std::nullptr_t)
                { return !lhs; }
            friend constexpr bool operator==(std::nullptr_t, const utf_cursor & rhs)
                { return !rhs; }
            friend constexpr bool operator!=(const utf_cursor & lhs, std::nullptr_t rhs)
                { return !(lhs == rhs); }
            friend constexpr bool operator!=(std::nullptr_t lhs, const utf_cursor & rhs)
                { return !(lhs == rhs); }
            
            reverse_type reverse() const
            {
                auto reverse_next = this->m_next.reverse();
                reverse_next += this->m_current_storage_size;
                reverse_type ret(std::move(reverse_next), this->m_encoder, this->m_char_idx, this->m_current_storage_size);
                ++ret;
                return ret;
            }
            
            utf_cursor<utf32, Cursor> char_start() const
                { return this->storage_cursor(); }
            
            size_type storage_pos() const
            {
                if constexpr (is_forward)
                    return this->m_next.position() - this->m_current_storage_size;
                else
                    return this->m_next.position() + this->m_current_storage_size;
            }
            
            size_type storage_size() const noexcept
                { return this->m_current_storage_size; }
            
            const Cursor storage_cursor() const
                { return this->m_next - this->m_current_storage_size; }
            
            friend auto operator<<(std::ostream & str, const utf_cursor & c) -> std::ostream &
            {
                str << c.storage_cursor() << ", char " << c.m_char_idx << " in {";
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
            utf_cursor(Cursor && next,
                       const utf_codepoint_encoder<OutputEnc, false> & encoder,
                       uint8_t char_idx,
                       uint8_t storage_size) noexcept(std::is_nothrow_move_constructible_v<Cursor>):
                m_next(std::move(next)),
                m_encoder(encoder),
                m_char_idx(char_idx),
                m_current_storage_size(storage_size)
            {}
            
            SYS_STRING_FORCE_INLINE
            constexpr void load_next()
            {
                if (this->m_next)
                {
                    auto original_pos = this->m_next.position();
                    auto c = current_utf32(this->m_next);
                    this->m_encoder.put(c);
                    if constexpr (is_forward)
                    {
                        this->m_char_idx = 0;
                        this->m_current_storage_size = uint8_t(this->m_next.position() - original_pos);
                    }
                    else
                    {
                        this->m_char_idx = uint8_t(this->m_encoder.size() - 1);
                        this->m_current_storage_size = uint8_t(original_pos - this->m_next.position());
                    }
                }
                else
                {
                    this->m_encoder.clear();
                    this->m_char_idx = 0;
                    this->m_current_storage_size = 0;
                }
            }
            
        private:
            Cursor m_next;
            utf_codepoint_encoder<OutputEnc, false> m_encoder;
            uint8_t m_char_idx;
            uint8_t m_current_storage_size;
        };

        template<class Cursor>
        class utf_cursor<utf32, Cursor>
        {
            static_assert(std::is_convertible_v<typename Cursor::iterator_category, std::random_access_iterator_tag>,
                          "Underlying cursor must be random access to be wrappable in utf_cursor");
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
            constexpr utf_cursor() = default;

            SYS_STRING_FORCE_INLINE
            constexpr utf_cursor(const Cursor & wrapped) noexcept(std::is_nothrow_copy_constructible_v<Cursor>) :
                m_current(wrapped)
            {
                this->load_next();
            }
            
            constexpr value_type operator*() const noexcept
                { return this->m_value; }

            explicit constexpr operator bool() const
                { return bool(this->m_current); }

            SYS_STRING_FORCE_INLINE
            constexpr utf_cursor & operator++()
            {
                this->m_current += this->m_current_storage_size;
                this->m_current_storage_size = 0;
                this->load_next();
                return *this;
            }

            SYS_STRING_FORCE_INLINE
            constexpr utf_cursor operator++(int)
            {
                utf_cursor ret = *this;
                ++(*this);
                return ret;
            }

            friend constexpr bool operator==(const utf_cursor & lhs, const utf_cursor & rhs)
                { return lhs.m_current == rhs.m_current; }
            friend constexpr bool operator!=(const utf_cursor & lhs, const utf_cursor & rhs)
                { return !(lhs == rhs); }
            
            friend constexpr bool operator==(const utf_cursor & lhs, std::nullptr_t)
                { return !lhs; }
            friend constexpr bool operator==(std::nullptr_t, const utf_cursor & rhs)
                { return !rhs; }
            friend constexpr bool operator!=(const utf_cursor & lhs, std::nullptr_t rhs)
                { return !(lhs == rhs); }
            friend constexpr bool operator!=(std::nullptr_t lhs, const utf_cursor & rhs)
                { return !(lhs == rhs); }
            
            reverse_type reverse() const
                { return this->m_current.reverse(); }
            
            const utf_cursor & char_start() const noexcept
                { return *this; }
            
            size_type storage_pos() const
                { return this->m_current.position(); }
            
            size_type storage_size() const noexcept
                { return this->m_current_storage_size; }
            
            const Cursor storage_cursor() const
                { return this->m_current; }
            
            friend auto operator<<(std::ostream & str, const utf_cursor & c) -> std::ostream &
            {
                str << c.m_current << ", value " << uint32_t(c.m_value);
                return str;
            }
        private:
            SYS_STRING_FORCE_INLINE
            void load_next()
            {
                if (Cursor next = this->m_current)
                {
                    this->m_value = current_utf32(next);
                    if constexpr (is_forward)
                        this->m_current_storage_size = uint8_t(next.position() - this->m_current.position());
                    else
                        this->m_current_storage_size = uint8_t(this->m_current.position() - next.position());
                }
            }
        private:
            Cursor m_current;
            value_type m_value = 0;
            uint8_t m_current_storage_size = 0;
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
        
        template<util::cursor_direction Direction>
        static auto detect_cursor_type(std::add_pointer_t<const char_access> access = nullptr) noexcept
        {
            using util::cursor_begin;
            return cursor_begin<Direction>(*access);
        }
        using access_cursor_type = decltype(detect_cursor_type<util::cursor_direction::forward>());
        using access_reverse_cursor_type = decltype(detect_cursor_type<util::cursor_direction::backward>());
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
        
        using cursor_direction = util::cursor_direction;
        
        static constexpr auto forward = cursor_direction::forward;
        static constexpr auto backward = cursor_direction::backward;
    public:
        utf_view(const Container & src) noexcept(noexcept(char_access(Traits::adapt(src)))) : m_access(Traits::adapt(src))
        {}
        SYS_STRING_FORCE_INLINE iterator begin() const
            { return this->cursor_begin(); }
        SYS_STRING_FORCE_INLINE iterator end() const
            { return this->cursor_end(); }
        SYS_STRING_FORCE_INLINE const_iterator cbegin() const
            { return begin(); }
        SYS_STRING_FORCE_INLINE const_iterator cend() const
            { return end(); }
        SYS_STRING_FORCE_INLINE reverse_iterator rbegin() const
            { return this->cursor_begin<backward>(); }
        SYS_STRING_FORCE_INLINE reverse_iterator rend() const
            { return this->cursor_end<backward>(); }
        SYS_STRING_FORCE_INLINE const_reverse_iterator crbegin() const
            { return rbegin(); }
        SYS_STRING_FORCE_INLINE const_reverse_iterator crend() const
            { return rend(); }
        
        template<cursor_direction Direction = forward>
        SYS_STRING_FORCE_INLINE
        decltype(auto) cursor_begin() const
        {
            using util::cursor_begin, util::utf_cursor;
            return utf_cursor<Enc, decltype(detect_cursor_type<Direction>())>(cursor_begin<Direction>(this->m_access));
        }
        
        template<cursor_direction Direction = forward>
        SYS_STRING_FORCE_INLINE
        decltype(auto) cursor_end() const
        {
            using util::cursor_end, util::utf_cursor;
            return utf_cursor<Enc, decltype(detect_cursor_type<Direction>())>(cursor_end<Direction>(this->m_access));
        }
        
        template<class Func>
        decltype(auto) each(Func func) const
        {
            using util::cursor_begin;
            constexpr auto source_encoding = utf_encoding_of<typename char_access::value_type>;
            return utf_converter<source_encoding, Enc>::for_each_converted(cursor_begin<util::cursor_direction::forward>(this->m_access), func);
        }
        
    private:
        char_access m_access;
    };
}

#endif
