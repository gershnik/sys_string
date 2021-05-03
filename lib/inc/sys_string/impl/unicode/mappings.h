//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_UNICODE_MAPPINGS_H_INCLUDED
#define HEADER_SYS_STRING_UNICODE_MAPPINGS_H_INCLUDED

#include <sys_string/impl/unicode/utf_encoding.h>

#include <algorithm>
#include <cstdlib>
#include <climits>

namespace sysstr::util
{
    namespace unicode
    {
        struct char_lookup
        {
            char32_t offset:11;
            char32_t value:21;
        };

        class mapper
        {
        public:
            static constexpr size_t max_mapped_length = 3;

            static const mapper case_fold;
            static const mapper to_lower_case;
            static const mapper to_upper_case;

            template<utf_encoding Enc, class OutIt>
            auto map_char(char32_t src, OutIt dest) const noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
            {
                auto lower = std::lower_bound(m_lookup_start, m_lookup_end, char_lookup{0, src}, [](char_lookup lhs, char_lookup rhs) {
                    return lhs.value < rhs.value;
                });
                if (lower == m_lookup_end || lower->value != src) 
                    return write<Enc>(src, dest); 
                auto start = lower->offset;
                auto end = (++lower)->offset; //safe - there is one behind end
                return write<Enc>(m_mapped + start, m_mapped + end, dest);
            }
        private:
            constexpr mapper(const char_lookup * lookup_start, size_t lookup_len, const char16_t * mapped) noexcept:
                m_lookup_start(lookup_start),
                m_lookup_end(lookup_start + lookup_len - 1),
                m_mapped(mapped)
            {}
            template<utf_encoding Enc, class OutIt>
            auto write(char32_t c, OutIt dest) const noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
            {
                if constexpr (Enc == utf32)
                {
                    *dest++ = c;
                    return dest;
                }
                else
                {
                    utf_codepoint_encoder<Enc, false> encoder;
                    encoder.put(c);
                    return std::copy(encoder.begin(), encoder.end(), dest);
                }
            }
            template<utf_encoding Enc, class OutIt>
            auto write(const char16_t * begin, const char16_t * end, OutIt dest) const noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
            {
                if constexpr (Enc == utf16)
                {
                    return std::copy(begin ,end, dest);
                }
                else
                {
                    utf_codepoint_decoder<utf16> decoder;
                    while(begin != end)
                    {
                        decoder.put(*begin++);
                        if (!decoder.done())
                            decoder.put(m_mapped[*begin++]); //no need to bounds check, we know end is good
                        dest = write<Enc>(decoder.value(), dest);
                    }
                    return dest;
                }
            }
        private:
            const char_lookup * m_lookup_start;
            const char_lookup * m_lookup_end;
            const char16_t * m_mapped;
        };

        constexpr size_t props_block_len = 256;
        constexpr size_t props_count_per_byte = 4;

        enum class char_prop : uint8_t
        {
            none            = 0,
            cased           = 0b01,
            case_ignorable  = 0b10
        };
        inline char_prop operator|(char_prop lhs, char_prop rhs) noexcept
            { return char_prop(uint8_t(lhs) | uint8_t(rhs)); }
        inline char_prop operator&(char_prop lhs, char_prop rhs) noexcept
            { return char_prop(uint8_t(lhs) & uint8_t(rhs)); }
        inline char_prop operator^(char_prop lhs, char_prop rhs) noexcept
            { return char_prop(uint8_t(lhs) ^ uint8_t(rhs)); }
        inline char_prop operator~(char_prop val) noexcept
            { return char_prop(~uint8_t(val)); }

        inline char_prop get_char_prop(char32_t c) noexcept
        {
            extern const uint8_t props_stage1[];
            extern const uint8_t props_stage2[];
            extern const char32_t max_props_char;

            if (c > max_props_char)
                return char_prop::none;
            auto stage2_block_offset = props_stage1[c / props_block_len] * (props_block_len / props_count_per_byte);
            auto stage2_char_idx = c % props_block_len;
            auto byte_offset = stage2_block_offset + stage2_char_idx / props_count_per_byte;
            auto prop_byte = props_stage2[byte_offset];
            auto shift = ((props_count_per_byte - 1) - stage2_char_idx % props_count_per_byte) * (CHAR_BIT / props_count_per_byte);
            auto prop_nibble = (prop_byte >> shift) /*& 0x0F*/;
            return char_prop(prop_nibble);
        }

        inline bool isspace(char32_t c) noexcept
        {
            extern const char16_t whitespaces[];
            if (c > 0xFFFFu)
                return false;
            for(auto p = whitespaces; *p; ++p)
                if (*p == c)
                    return true;
            return false;
        }
    }
}

#endif
