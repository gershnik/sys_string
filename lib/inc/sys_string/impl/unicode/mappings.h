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

#include <sys_string/impl/unicode/mappings_common.h>
           
#include <algorithm>
#include <cstdlib>
#include <climits>
#include <array>
#include <cassert>

namespace sysstr::util::unicode 
{
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


    template<size_t N, class Derived>
    class trie_lookup;
    
    template<class Derived>
    class trie_lookup<4, Derived>
    {
    public:
        SYS_STRING_FORCE_INLINE
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
    

    

    class case_info : public trie_lookup<4, case_info>
    {
    friend trie_lookup<4,case_info>;
    private:
        using entry_type = std::array<uint16_t, 16>;
        using value_type = uint32_t;
    
        static const std::array<entry_type, 1218> entries;
    
        static const std::array<value_type, 756> values;
    
    public:
    
        using value = value_type;
    
        static constexpr size_t data_size = sizeof(entries) + sizeof(values);
    };
    
    
    extern const char16_t cased_data[];
    
    struct case_fold_mapper 
    {
        static constexpr size_t max_mapped_length = 8; 
    
        template<utf_encoding Enc, class OutIt>
        SYS_STRING_FORCE_INLINE
        static auto map_char(char32_t src, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
        {
            auto res = case_info::get(src);
            uint8_t fold_len = (res >> 24) & 0x0F;
            if (fold_len == 0)
                return write_unsafe<Enc>(src, dest);
            uint8_t fold_offset;
            if (fold_len == 0x0F)
            {
                fold_len = (res >> 16) & 0x0F;
                fold_offset = 0;
            }
            else if (fold_len == 0x0E)
            {
                fold_len = (res >> 20) & 0x0F;
                fold_offset = (res >> 16) & 0x0F;
            }
            else
            {
                fold_offset = ((res >> 16) & 0x0F) + ((res >> 20) & 0x0F);
            }
            size_t entry_offset = (size_t(src) - (res & 0xFFFF)) & 0x0FFF;
            const char16_t * entry = cased_data + entry_offset;
            return write_unsafe<Enc>(entry + fold_offset, entry + fold_offset + fold_len, dest);
        }
    };
    
    struct to_lower_case_mapper 
    {
        static constexpr size_t max_mapped_length = 8; 
    
        template<utf_encoding Enc, class OutIt>
        SYS_STRING_FORCE_INLINE
        static auto map_char(char32_t src, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
        {
            auto res = case_info::get(src);
            uint8_t lower_len = (res >> 16) & 0x0F;
            if (lower_len == 0)
                return write_unsafe<Enc>(src, dest);
            size_t entry_offset = (size_t(src) - (res & 0xFFFF)) & 0x0FFF;
            const char16_t * entry = cased_data + entry_offset;
            return write_unsafe<Enc>(entry, entry + lower_len, dest);
        }
    };
    
    struct to_upper_case_mapper 
    {
        static constexpr size_t max_mapped_length = 8; 
    
        template<utf_encoding Enc, class OutIt>
        SYS_STRING_FORCE_INLINE
        static auto map_char(char32_t src, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
        {
            auto res = case_info::get(src);
            uint8_t upper_len = (res >> 20) & 0x0F;
            if (upper_len == 0)
                return write_unsafe<Enc>(src, dest);
            uint8_t upper_offset = (res >> 16) & 0x0F;
            size_t entry_offset = (size_t(src) - (res & 0xFFFF)) & 0x0FFF;
            const char16_t * entry = cased_data + entry_offset;
            return write_unsafe<Enc>(entry + upper_offset, entry + upper_offset + upper_len, dest);
        }
    };
    
    class case_prop
    {
    public:
        enum value : uint8_t
        {
            none = 0,
            cased = 1,
            case_ignorable = 2
        };
    
        SYS_STRING_FORCE_INLINE
        static auto get(char32_t c) noexcept
        {
            auto res = case_info::get(c);
            return value(res >> 28);
        }
    };


    class normalizer 
    {
    private:
        class lookup : public trie_lookup<4, lookup>
        {
        friend trie_lookup<4,lookup>;
        private:
            using entry_type = std::array<uint16_t, 16>;
            using value_type = uint32_t;
    
            static const std::array<entry_type, 885> entries;
    
            static const std::array<value_type, 501> values;
    
        public:
    
            using value = value_type;
    
            static constexpr size_t data_size = sizeof(entries) + sizeof(values);
        };
    
    
        static const uint32_t compositions[3478];
        static const uint32_t values[2401];
    
    public:
        static constexpr size_t data_size = sizeof(compositions) + 
                                            sizeof(values) +
                                            lookup::data_size;
    
        template<utf_encoding Enc, class OutIt>
        static auto decompose(char32_t src, OutIt dest) noexcept(noexcept(*dest++ = uint32_t())) -> OutIt
        {
            auto res = lookup::get(src);
            if (res == 0)
            {
                *dest = src;
                return ++dest;
            }
            if (res > 0x0FFF)
            {
                uint32_t shifted_ccc = res << 9;
                uint32_t val = uint32_t(src) | shifted_ccc;
                *dest = val;
                return ++dest;
            }
    
            size_t value_offset = ((size_t(src) - res) & 0x0FFF) - 1;
            uint32_t value = values[value_offset];
            if (value & (1 << 30))
            {
                uint32_t shifted_ccc = (value & 0x0FF000) << 9;
                uint32_t val = uint32_t(src) | shifted_ccc;
                *dest = val;
                return ++dest;
            }
    
    
            value >>= 12;
            uint16_t decomp_start = value & 0xFFF;
            value >>= 12;
            uint16_t decomp_idx = value & 0x1F;
            value >>= 5;
            int final = value;
    
            auto * comps = compositions + decomp_start;
    
            uint32_t first = comps[0] & 0x1FFF'FFFF;
            if (!final) 
            {
                dest = decompose<Enc>(first & 0x1F'FFFF, dest);
            }
            else
            {
                *dest = first;
                ++dest;
            }
    
            if (decomp_idx != 0)
            {
                uint32_t second = comps[decomp_idx * 2 - 1] & 0x1FFF'FFFF;
                *dest = second;
                ++dest;
            }
    
            return dest;
        }
    
        static auto get_compositions(char32_t src) -> const uint32_t *
        {
            auto res = lookup::get(src);
            if (res == 0)
                return nullptr;
    
            if (res > 0x0FFF)
                return nullptr;
    
            size_t value_offset = ((size_t(src) - res) & 0x0FFF) - 1;
            uint32_t value = values[value_offset];
    
            uint16_t comp_idx = value & 0xFFF;
            if (comp_idx == 0xFFF)
                return nullptr;
    
            auto ret = compositions + comp_idx;
            bool is_last = (ret[0] >> 29);
            if (is_last)
                return nullptr;
            if (ret[0] >> 21)
                return nullptr;
    
            ++ret;
            return ret;
        }
    
        static auto get_comb_class(char32_t c) -> uint8_t
        {
            auto res = lookup::get(c);
            if (res == 0)
                return 0;
    
            if (res > 0x0FFF)
                return res >> 12;
    
            size_t value_offset = ((size_t(c) - res) & 0x0FFF) - 1;
            uint32_t value = values[value_offset];
    
            if (value & 0x2000000)
                return (value >> 12) & 0xFF;
    
            uint16_t comp_idx = value & 0xFFF;
            if (comp_idx == 0xFFF)
                return 0;
    
            auto * comps = compositions + comp_idx;
            return (comps[0] >> 21) & 0xFF;
        }
    };


    class grapheme_cluster_break_prop : public trie_lookup<4, grapheme_cluster_break_prop>
    {
    friend trie_lookup<4,grapheme_cluster_break_prop>;
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

