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
#include <sys_string/impl/util/bit_array.h>

#include <sys_string/impl/unicode/mappings_params.h>

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

        class mapper : public mapper_data
        {
        public:
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

        template<class Base>
        class property_lookup : public Base
        {
        public:
            using typename Base::value;

            static inline value get(char32_t c) noexcept
            {
                if (c > Base::max_char)
                    return value(0);
                auto stage2_block_offset = Base::stage1[c / Base::block_len] * Base::block_len;
                auto stage2_char_idx = c % Base::block_len;
                if constexpr (Base::separate_values)
                    return value(Base::values[Base::stage2[stage2_block_offset + stage2_char_idx]]);
                else
                    return value(Base::stage2[stage2_block_offset + stage2_char_idx]);
            }
        };

        template<class Base>
        class trie_lookup : public Base
        {
        public:
            using typename Base::value;

            static inline value get(char32_t c) noexcept
            {
                using entry_type = typename decltype(Base::entries)::value_type;
                constexpr auto value_shift = Base::bits_per_index << 1;
                constexpr uint32_t masks[] = {
                    0x10'0000,
                     0x8'0000, 0x4'0000, 0x2'0000, 0x1'0000,
                       0x8000,   0x4000,   0x2000,   0x1000,
                        0x800,    0x400,    0x200,    0x100,
                         0x80,     0x40,     0x20,     0x10,
                          0x8,      0x4,      0x2,      0x1
                };

                constexpr entry_type index_mask = (1 << Base::bits_per_index) - 1;
                entry_type entry;
                entry_type idx = 0;
                int count = 0;
                for ( ; ; )
                {
                    entry = Base::entries[idx];
                    bool bit = (c & masks[count++]);
                    idx = (entry >> (Base::bits_per_index * bit)) & index_mask;
                    if (idx == index_mask)
                        break;
                }
                entry >>= value_shift;
                if constexpr (Base::separate_values)
                    return value(Base::values[entry]);
                else
                    return value(entry);
            }
        };

        using case_prop = property_lookup<case_prop_data>;
        //using grapheme_cluster_break_prop = property_lookup<grapheme_cluster_break_prop_data>;
        using grapheme_cluster_break_prop = trie_lookup<grapheme_cluster_break_prop_data2>;
    }
}


#endif
