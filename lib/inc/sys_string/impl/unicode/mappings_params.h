//THIS FILE IS GENERATED. PLEASE DO NOT EDIT DIRECTLY

//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_UNICODE_MAPPINGS_DATA_H_INCLUDED
#define HEADER_SYS_STRING_UNICODE_MAPPINGS_DATA_H_INCLUDED

#include <array>
#include <cassert>

namespace sysstr::util::unicode 
{
    struct mapper_data
    {
        static constexpr size_t max_mapped_length = 3;
    };

    struct char_lookup
    {
        char32_t offset:11;
        char32_t value:21;
    };

    struct case_fold_data
    {
        static const char_lookup source_chars[];
        static constexpr size_t source_chars_len = 1557;
        static const char16_t chars[];
    };


    struct to_lower_case_data
    {
        static const char_lookup source_chars[];
        static constexpr size_t source_chars_len = 1460;
        static const char16_t chars[];
    };


    struct to_upper_case_data
    {
        static const char_lookup source_chars[];
        static constexpr size_t source_chars_len = 1552;
        static const char16_t chars[];
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


    class case_prop_lookup : public prop_lookup<case_prop_lookup>
    {
    friend prop_lookup<case_prop_lookup>;
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


    class grapheme_cluster_break_lookup : public prop_lookup<grapheme_cluster_break_lookup>
    {
    friend prop_lookup<grapheme_cluster_break_lookup>;
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

