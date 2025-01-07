//THIS FILE IS GENERATED. PLEASE DO NOT EDIT DIRECTLY

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
#include <array>
#include <cassert>

namespace sysstr::util::unicode 
{
    struct char_lookup
    {
        char32_t offset:11;
        char32_t value:21;
    };
    
    template<class Derived>
    class mapper 
    {
    public:
        template<utf_encoding Enc, class OutIt>
        static auto map_char(char32_t src, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
        {
            const char_lookup * const lookup_start = Derived::source_chars.data();
            const char_lookup * const lookup_end = lookup_start + Derived::source_chars.size() - 1;
            const char16_t * const mapped = Derived::mapped_chars;
    
            auto lower = std::lower_bound(lookup_start, lookup_end, char_lookup{0, src}, [](char_lookup lhs, char_lookup rhs) {
                return lhs.value < rhs.value;
            });
            if (lower == lookup_end || lower->value != src) 
                return write<Enc>(src, dest); 
            auto start = lower->offset;
            auto end = (++lower)->offset; //safe - there is one behind end
            return write<Enc>(mapped + start, mapped + end, dest);
        }
    private:
        template<utf_encoding Enc, class OutIt>
        static auto write(char32_t c, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
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
        static auto write(const char16_t * begin, const char16_t * end, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
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
                        decoder.put(*begin++); //no need to bounds check, we know end is good
                    dest = write<Enc>(decoder.value(), dest);
                }
                return dest;
            }
        }
    };


    class case_fold_mapper : public mapper<case_fold_mapper>
    {
    friend mapper<case_fold_mapper>;
    private:
        static const std::array<char_lookup, 1558> source_chars;
        static const char16_t mapped_chars[1960];
    public:
        static constexpr size_t max_mapped_length = 3;
        static constexpr size_t data_size = sizeof(source_chars) + sizeof(mapped_chars);
    };


    class to_lower_case_mapper : public mapper<to_lower_case_mapper>
    {
    friend mapper<to_lower_case_mapper>;
    private:
        static const std::array<char_lookup, 1461> source_chars;
        static const char16_t mapped_chars[1744];
    public:
        static constexpr size_t max_mapped_length = 2;
        static constexpr size_t data_size = sizeof(source_chars) + sizeof(mapped_chars);
    };


    class to_upper_case_mapper : public mapper<to_upper_case_mapper>
    {
    friend mapper<to_upper_case_mapper>;
    private:
        static const std::array<char_lookup, 1553> source_chars;
        static const char16_t mapped_chars[1953];
    public:
        static constexpr size_t max_mapped_length = 3;
        static constexpr size_t data_size = sizeof(source_chars) + sizeof(mapped_chars);
    };


    template<class Derived>
    class lookup
    {
    public:
        static bool test(char32_t c) noexcept
        {
            if (c > Derived::max_char)
                return false;
            for(auto p = Derived::chars; *p; ++p)
                if (*p == c)
                    return true;
            return false;
        }
    };


    class is_whitespace : public lookup<is_whitespace>
    {
    friend lookup<is_whitespace>;
    private:
        static const char16_t chars[26];
    public:
        static constexpr char32_t max_char = U'\u3000';
    
        static constexpr size_t data_size = sizeof(chars);
    };


    template<class Derived>
    class prop_lookup
    {
    public:
        static auto get(char32_t c) noexcept
        {
            size_t idx = Derived::values.size();
    
            {
                int char_idx = (c >> 20) & 0xF;
                auto & entry = Derived::entries[idx];
                idx = entry[char_idx];
            }
    
            {
                int char_idx = (c >> 16) & 0xF;
                auto & entry = Derived::entries[idx];
                idx = entry[char_idx];
            }
    
            {
                int char_idx = (c >> 12) & 0xF;
                auto & entry = Derived::entries[idx];
                idx = entry[char_idx];
            }
    
            {
                int char_idx = (c >> 8) & 0xF;
                auto & entry = Derived::entries[idx];
                idx = entry[char_idx];
            }
    
            {
                int char_idx = (c >> 4) & 0xF;
                auto & entry = Derived::entries[idx];
                idx = entry[char_idx];
            }
    
            {
                int char_idx = (c >> 0) & 0xF;
                auto & entry = Derived::entries[idx];
                idx = entry[char_idx];
            }
    
           assert(idx < Derived::values.size());
           return typename Derived::value(Derived::values[idx]);
        }
    };


    class case_prop : public prop_lookup<case_prop>
    {
    friend prop_lookup<case_prop>;
    private:
        using entry_type = std::array<uint16_t, 16>;
        using value_type = uint8_t;
    
        static const std::array<entry_type, 373> entries;
    
        static const std::array<value_type, 4> values;
    
    public:
        enum value : value_type
        {
            none = 0,
            cased = 1,
            case_ignorable = 2
        };
    
        static constexpr size_t data_size = sizeof(entries) + sizeof(values);
    };


    class grapheme_cluster_break_prop : public prop_lookup<grapheme_cluster_break_prop>
    {
    friend prop_lookup<grapheme_cluster_break_prop>;
    private:
        using entry_type = std::array<uint16_t, 16>;
        using value_type = uint8_t;
    
        static const std::array<entry_type, 417> entries;
    
        static const std::array<value_type, 16> values;
    
    public:
        enum value : value_type
        {
            none = 0,
            control = 1,
            extend = 2,
            regional_indicator = 3,
            prepend = 4,
            spacing_mark = 5,
            hangul_l = 6,
            hangul_v = 7,
            hangul_t = 8,
            hangul_lv = 9,
            hangul_lvt = 10,
            extended_pictographic = 11,
            in_cb_consonant = 16,
            in_cb_extend = 32,
            in_cb_linker = 48,
    
            basic_mask = 15,
            in_cb_mask = 48
        };
    
        static constexpr size_t data_size = sizeof(entries) + sizeof(values);
    };

}

#endif

