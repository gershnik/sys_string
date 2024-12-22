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

namespace sysstr::util::unicode 
{
    struct mapper_data
    {
        static constexpr size_t max_mapped_length = 3;
    };

    
    class case_prop_data
    {
    protected:
        static constexpr size_t block_len = 256;
        static constexpr char32_t max_char = U'\U000E01EF';

        static const std::array<uint8_t, 3586> stage1;
        static const std::array<uint8_t, 25856> stage2;
    
        static constexpr bool separate_values = false;
    
    public:
        enum value : decltype(stage2)::value_type
        {
            none = 0,
            cased = 1,
            case_ignorable = 2
        };
    
        static constexpr size_t data_size = sizeof(stage1) + sizeof(stage2);
    
    };
    
    
    class grapheme_cluster_break_prop_data
    {
    protected:
        static constexpr size_t block_len = 256;
        static constexpr char32_t max_char = U'\U0001FFFD';

        static const std::array<uint8_t, 3600> stage1;
        static const std::array<uint8_t, 27136> stage2;
    
        static const std::array<uint8_t, 17> values;

        static constexpr bool separate_values = true;
    
    public:
        enum value : decltype(stage2)::value_type
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
    
        static constexpr size_t data_size = sizeof(stage1) + sizeof(stage2) + sizeof(values);
    
    };
    
    
    class grapheme_cluster_break_prop_data2
    {
    protected:
        using entry_type = std::array<uint8_t, 128>;
        using value_type = uint8_t;

        static const std::array<entry_type, 177> entries;
    
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

