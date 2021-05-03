//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
//
// Portions Copyright (c) 2008-2010 Björn Höhrmann <bjoern@hoehrmann.de>.
// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
// USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef HEADER_SYS_STRING_UNICODE_UTF_ENCODING_INCLUDED
#define HEADER_SYS_STRING_UNICODE_UTF_ENCODING_INCLUDED

#include <sys_string/config.h>

#include <type_traits>
#include <cstdint>
#include <cstddef>

namespace sysstr
{

    enum utf_encoding
    {
        utf8  =  8,
        utf16 = 16,
        utf32 = 32
    };

    template<utf_encoding Enc> struct utf_char_type_of;
    template<> struct utf_char_type_of<utf8>    { using type = char; };
    template<> struct utf_char_type_of<utf16>   { using type = char16_t; };
    template<> struct utf_char_type_of<utf32>   { using type = char32_t; };

    template<utf_encoding Enc> using utf_char_of = typename utf_char_type_of<Enc>::type;

    template<class T> struct utf_encoding_value;
    template<> struct utf_encoding_value<char>          { static constexpr utf_encoding value = utf8; };
    template<> struct utf_encoding_value<unsigned char> { static constexpr utf_encoding value = utf8; };
    template<> struct utf_encoding_value<signed char>   { static constexpr utf_encoding value = utf8; };
    #if SYS_STRING_USE_CHAR8
    template<> struct utf_encoding_value<char8_t>       { static constexpr utf_encoding value = utf8; };
    #endif
    template<> struct utf_encoding_value<char16_t>      { static constexpr utf_encoding value = utf16; };
    template<> struct utf_encoding_value<char32_t>      { static constexpr utf_encoding value = utf32; };

    template<class T> constexpr utf_encoding utf_encoding_of = utf_encoding_value<std::remove_cv_t<T>>::value;

    template<class T> struct has_utf_encoding_type          : public std::false_type {};
    template<> struct has_utf_encoding_type<char>           : public std::true_type {};
    template<> struct has_utf_encoding_type<unsigned char>  : public std::true_type {};
    template<> struct has_utf_encoding_type<signed char>    : public std::true_type {};
    #if SYS_STRING_USE_CHAR8
    template<> struct has_utf_encoding_type<char8_t>        : public std::true_type {};
    #endif
    template<> struct has_utf_encoding_type<char16_t>       : public std::true_type {};
    template<> struct has_utf_encoding_type<char32_t>       : public std::true_type {};

    template<class T> constexpr bool has_utf_encoding = has_utf_encoding_type<std::remove_cv_t<T>>::value;

    template<utf_encoding Enc> struct utf_encoding_max_length;
    template<> struct utf_encoding_max_length<utf8>     { static constexpr size_t value = 4; };
    template<> struct utf_encoding_max_length<utf16>    { static constexpr size_t value = 2; };
    template<> struct utf_encoding_max_length<utf32>    { static constexpr size_t value = 1; };

    template<utf_encoding Enc> constexpr size_t utf_encoding_max_length_of = utf_encoding_max_length<Enc>::value;


    template<utf_encoding Enc> class utf_codepoint_decoder;
    template<utf_encoding Enc> class utf_reverse_codepoint_decoder;
    template<utf_encoding Enc, bool Validate> class utf_codepoint_encoder;

    //MARK:- UTF-16

    template<>
    class utf_codepoint_decoder<utf16>
    {
    public:
        constexpr void put(uint16_t c) noexcept
        {
            int is_lead = ((c & 0xFC00) == 0xD800);
            int is_trail = ((c & 0xFC00) == 0xDC00);
            int type = is_lead | (is_trail << 1);
            
            if (m_state != state_need_trail)
                m_value = c;
            else
                m_value = ((m_value - 0xD800) << 10) + (uint32_t(c) - 0xDC00) + 0x0010000;
            m_state = s_state_table[m_state + type];
        }
        
        constexpr bool done() const noexcept
            { return m_state == state_done; }
        
        constexpr bool error() const noexcept
            { return m_state == state_error; }
        
        constexpr uint32_t value() const noexcept
            { return m_value; }
    private:
        static constexpr int state_done = 0;
        static constexpr int state_need_trail = 4;
        static constexpr int state_error = 8;
        
        static constexpr int s_state_table[] = {
        // 0 - normal, 1 - lead, 2 - trail, 3 - unused
        //                     0  1  2  3
        /* state_done       */ 0, 4, 8, 0,
        /* state_need_trail */ 8, 8, 0, 0,
        /* state_error */      0, 4, 8, 0
        };
        
        uint32_t m_value;
        int m_state = state_done;
    };

    template<>
    class utf_reverse_codepoint_decoder<utf16>
    {
    public:
        constexpr void put(uint16_t c) noexcept
        {
            int is_lead = ((c & 0xFC00) == 0xD800);
            int is_trail = ((c & 0xFC00) == 0xDC00);
            int type = is_lead | (is_trail << 1);
            
            if (m_state != state_need_lead)
                m_value = c;
            else
                m_value = ((uint32_t(c) - 0xD800) << 10) + (m_value - 0xDC00) + 0x0010000;
            
            m_state = s_state_table[m_state + type];
        }
        
        constexpr bool done() const noexcept
            { return m_state == state_done; }
        
        constexpr bool error() const noexcept
            { return m_state == state_error; }
        
        constexpr uint32_t value() const noexcept
            { return m_value; }
        
    private:
        static constexpr int state_done = 0;
        static constexpr int state_need_lead = 4;
        static constexpr int state_error = 8;
        
        static constexpr int s_state_table[] = {
        // 0 - normal, 1 - lead, 2 - trail, 3 - unused
        //                    0  1  2  3
        /* state_done      */ 0, 8, 4, 0,
        /* state_need_lead */ 8, 0, 8, 0,
        /* state_error     */ 0, 8, 4, 0
        };
    
        uint32_t m_value;
        int m_state = state_done;
    };

    template<bool Validate>
    class utf_codepoint_encoder<utf16, Validate>
    {
    public:
        constexpr bool put(uint32_t c) noexcept
        {
            if constexpr (Validate)
            {
                if ((c & 0xFFFFF800) == 0x0000D800 || c > 0x010FFFF)
                {
                    m_size = 0;
                    return false;
                }
            }

            if (c < 0x010000)
            {
                m_size = 1;
                m_data[0] = char16_t(c);
            }
            else
            {
                m_size = 2;
                m_data[0] = char16_t(((c - 0x010000) >> 10)    + 0x00D800);
                m_data[1] = char16_t(((c - 0x010000) & 0x03FF) + 0x00DC00);
            }
            return true;
        }
        
        constexpr const char16_t * begin() const noexcept
            { return m_data; }
        constexpr const char16_t * end() const noexcept
            { return m_data + m_size; }
        constexpr uint8_t size() const noexcept
            { return m_size; }

        void clear() noexcept
            { m_size = 0; }
    private:
        char16_t m_data[2] = {};
        uint8_t m_size = 0;
    };

    //MARK:- UTF-8

    template<>
    class utf_codepoint_decoder<utf8>
    {
    public:
        constexpr void put(uint8_t byte) noexcept
        {
            uint32_t type = s_state_table[byte];

            m_value = (m_state > state_reject) ?
                    (byte & 0x3fu) | (m_value << 6) :
                    (0xff >> type) & (byte);

            m_state = s_state_table[256 + m_state + type];
        }
        
        constexpr bool done() const noexcept
            { return m_state == state_accept; }
        
        constexpr bool error() const noexcept
            { return m_state == state_reject; }
        
        constexpr uint32_t value() const noexcept
            { return m_value; }
    private:
        uint32_t m_value = 0;
        uint8_t m_state = state_accept;
        
        
        static constexpr uint8_t state_accept = 0;
        static constexpr uint8_t state_reject = 12;
        
        static constexpr const uint8_t s_state_table[] = {
            // The first part of the table maps bytes to character classes that
            // to reduce the size of the transition table and create bitmasks.
             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
             1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
             8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
            10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,

        // The second part is a transition table that maps a combination
        // of a state of the automaton and a character class to a state.
        //   0  1  2  3  4  5  6  7  8  9 10 11
             0,12,24,36,60,96,84,12,12,12,48,72,
             0,12,24,36,60,96,84,12,12,12,48,72, //unlike original we don't get stuck in error state
            12, 0,12,12,12,12,12, 0,12, 0,12,12,
            12,24,12,12,12,12,12,24,12,24,12,12,
            12,12,12,12,12,12,12,24,12,12,12,12,
            12,24,12,12,12,12,12,12,12,24,12,12,
            12,12,12,12,12,12,12,36,12,36,12,12,
            12,36,12,12,12,12,12,36,12,36,12,12,
            12,36,12,12,12,12,12,12,12,12,12,12,
        };
    };

    template<>
    class utf_reverse_codepoint_decoder<utf8>
    {
    public:
        constexpr void put(uint8_t byte) noexcept
        {
            uint32_t type = s_state_table[byte];
            uint8_t next_state = s_state_table[256 + m_state + type];

            if (next_state <= state_reject)
            {
                m_value |= (((0xff >> type) & (byte)) << m_shift);
                m_shift = 0;
            }
            else
            {
                m_value |= ((byte & 0x3fu) << m_shift);
                m_shift += 6;
            }
            m_state = next_state;
        }
        
        constexpr bool done() const noexcept
            { return m_state == state_accept; }
        
        constexpr bool error() const noexcept
            { return m_state == state_reject; }
        
        constexpr uint32_t value() const noexcept
            { return m_value; }
    private:
        uint32_t m_value = 0;
        uint8_t m_shift = 0;
        uint8_t m_state = state_accept;
        
        static constexpr uint8_t state_accept = 0;
        static constexpr uint8_t state_reject = 12;
        
        static constexpr const uint8_t s_state_table[] = {
        // The first part of the table maps bytes to character classes that
        // to reduce the size of the transition table and create bitmasks.
             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
             1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
             8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
            10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,
            
        // The second part is a transition table that maps a combination
        // of a state of the automaton and a character class to a state.
        //   0  1  2  3  4  5  6  7  8  9 10 11
             0,24,12,12,12,12,12,24,12,24,12,12,
             0,24,12,12,12,12,12,24,12,24,12,12, //we don't get stuck in error state
            12,36, 0,12,12,12,12,48,12,36,12,12,
            12,60,12, 0, 0,12,12,72,12,72,12,12,
            12,60,12, 0,12,12,12,72,12,72, 0,12,
            12,12,12,12,12, 0, 0,12,12,12,12,12,
            12,12,12,12,12,12,12,12,12,12,12, 0,
        };
    };

    template<bool Validate>
    class utf_codepoint_encoder<utf8, Validate>
    {
    public:
        constexpr bool put(uint32_t c) noexcept
        {
            if constexpr (Validate)
            {
                if ((c & 0xFFFFF800) == 0x0000D800 || c > 0x010FFFF)
                {
                    m_size = 0;
                    return false;
                }
            }

            int size = 1;
            size += (c > 0x00007f);
            size += (c > 0x0007FF);
            size += (c > 0x00FFFF);
            
            constexpr uint32_t initial_or_masks[]  = {0b00000000u, 0b11000000u, 0b11100000u, 0b11110000u};
            constexpr uint32_t initial_and_masks[] = {0b11111111u, 0b00011111u, 0b00001111u, 0b00000111u};
            uint32_t initial_or_mask = initial_or_masks[size - 1];
            uint32_t initial_and_mask = initial_and_masks[size - 1];
            int shift = 6 * (size - 1);
            
            m_size = uint8_t(size);
            m_data[0] = uint8_t(initial_or_mask | ((c >> shift) & initial_and_mask));
            for(int i = 1; i < size; ++i)
            {
                shift -= 6;
                m_data[i] = uint8_t(0b10000000u | ((c >> shift)  & 0b00111111u));
            }
            return true;
        }
        constexpr const char * begin() const noexcept
            { return m_data; }
        constexpr const char * end() const noexcept
            { return m_data + m_size; }
        constexpr uint8_t size() const noexcept
            { return m_size; }

        void clear() noexcept
            { m_size = 0; }
    private:
        char m_data[4] = {};
        uint8_t m_size = 0;
    };

    //MARK:- UTF-32

    template<>
    class utf_codepoint_decoder<utf32>
    {
    public:
        constexpr bool put(uint32_t c) noexcept
        {
            m_value = c;
            return !( (c & 0xFFFFF800) == 0x0000D800 || c > 0x010FFFF );
        }
        
        constexpr uint32_t value() const noexcept
            { return m_value; }
    private:
        uint32_t m_value;
    };

    template<bool Validate>
    class utf_codepoint_encoder<utf32, Validate>
    {
    public:
        constexpr bool put(uint32_t c) noexcept
        {
            m_val = c;
            
            if constexpr (Validate)
                return !( (c & 0xFFFFF800) == 0x0000D800 || c > 0x010FFFF );
            
            return true;
        }
        
        constexpr const char32_t * begin() const noexcept
            { return &m_val; }
        constexpr const char32_t * end() const noexcept
            { return &m_val + 1; }
        constexpr uint8_t size() const noexcept
            { return 1; }
    private:
        char32_t m_val = 0;
    };
}

#endif


