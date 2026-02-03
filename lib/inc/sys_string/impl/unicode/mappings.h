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
    class is_whitespace 
    {
    private:
        static const char16_t chars[26];
    public:
        static constexpr char32_t max_char = U'\u3000';
    
        static bool test(char32_t c) noexcept
        {
            if (c > max_char)
                return false;
            for(auto p = chars; *p; ++p)
                if (*p == char16_t(c))
                    return true;
            return false;
        }
    
        static constexpr size_t data_size = sizeof(chars);
    };


    class case_mapper 
    {
    private:
        class lookup 
        {
        private:
            static const std::array<uint32_t, 128> ascii;
    
            static const std::array<uint8_t, 2048> stage1;
    
            static const std::array<std::array<uint16_t, 32>, 65> stage2;
    
            static const std::array<std::array<uint16_t, 16>, 366> stage3;
    
            static const std::array<uint32_t, 752> stage4;
    
        public:
            using value = decltype(stage4)::value_type;
    
            SYS_STRING_FORCE_INLINE
            static auto get(char32_t c) noexcept
            {
                if (c < 128)
                    return value(ascii[c]);
    
                size_t stage_idx = (c >> 9) & 0x7FF;
                size_t base = stage1[stage_idx];
                stage_idx = (c >> 4) & 0x1F;
                base = stage2[base][stage_idx];
                stage_idx = c & 0xF;
                base = stage3[base][stage_idx];
                return value(stage4[base]);
            }
    
            static constexpr size_t data_size =                          
                                                sizeof(stage1) +                         
                                                sizeof(stage2) +                         
                                                sizeof(stage3) +                         
                                                sizeof(stage4) +                              
                                                sizeof(ascii);
        };
    
    
        static const char16_t cased_data[4046];
    
    public:
        static constexpr size_t data_size = lookup::data_size + sizeof(cased_data);
    
        struct fold 
        {
            static constexpr size_t max_mapped_length = 8; 
    
            template<utf_encoding Enc, class OutIt>
            SYS_STRING_FORCE_INLINE
            static auto map_char(char32_t src, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
            {
                auto res = lookup::get(src);
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
    
        struct to_lower
        {
            static constexpr size_t max_mapped_length = 8; 
    
            template<utf_encoding Enc, class OutIt>
            SYS_STRING_FORCE_INLINE
            static auto map_char(char32_t src, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
            {
                auto res = lookup::get(src);
                uint8_t lower_len = (res >> 16) & 0x0F;
                if (lower_len == 0)
                    return write_unsafe<Enc>(src, dest);
                size_t entry_offset = (size_t(src) - (res & 0xFFFF)) & 0x0FFF;
                const char16_t * entry = cased_data + entry_offset;
                return write_unsafe<Enc>(entry, entry + lower_len, dest);
            }
        };
    
        struct to_upper
        {
            static constexpr size_t max_mapped_length = 8; 
    
            template<utf_encoding Enc, class OutIt>
            SYS_STRING_FORCE_INLINE
            static auto map_char(char32_t src, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
            {
                auto res = lookup::get(src);
                uint8_t upper_len = (res >> 20) & 0x0F;
                if (upper_len == 0)
                    return write_unsafe<Enc>(src, dest);
                uint8_t upper_offset = (res >> 16) & 0x0F;
                size_t entry_offset = (size_t(src) - (res & 0xFFFF)) & 0x0FFF;
                const char16_t * entry = cased_data + entry_offset;
                return write_unsafe<Enc>(entry + upper_offset, entry + upper_offset + upper_len, dest);
            }
        };
    
        class props
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
                auto res = lookup::get(c);
                return value(res >> 28);
            }
        };
    };


    class normalizer 
    {
    private:
        class lookup 
        {
        private:
            static const std::array<uint16_t, 128> ascii;
    
            static const std::array<uint8_t, 2048> stage1;
    
            static const std::array<std::array<uint16_t, 32>, 55> stage2;
    
            static const std::array<std::array<uint16_t, 16>, 312> stage3;
    
            static const std::array<uint16_t, 498> stage4;
    
        public:
            using value = decltype(stage4)::value_type;
    
            SYS_STRING_FORCE_INLINE
            static auto get(char32_t c) noexcept
            {
                if (c < 128)
                    return value(ascii[c]);
    
                size_t stage_idx = (c >> 9) & 0x7FF;
                size_t base = stage1[stage_idx];
                stage_idx = (c >> 4) & 0x1F;
                base = stage2[base][stage_idx];
                stage_idx = c & 0xF;
                base = stage3[base][stage_idx];
                return value(stage4[base]);
            }
    
            static constexpr size_t data_size =                          
                                                sizeof(stage1) +                         
                                                sizeof(stage2) +                         
                                                sizeof(stage3) +                         
                                                sizeof(stage4) +                              
                                                sizeof(ascii);
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
            if (res & 0x1000)
            {
                uint32_t shifted_ccc = uint32_t(res & 0xFF) << 21;
                uint32_t val = uint32_t(src) | shifted_ccc;
                *dest = val;
                return ++dest;
            }
    
            size_t value_offset = ((size_t(src) - res) & 0x0FFF);
            uint32_t value = values[value_offset];
            if (value & (1 << 31))
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
            int final = ((value & (1 << 5)) != 0);
    
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
    
            if (res & 0x1000)
                return nullptr;
    
            size_t value_offset = ((size_t(src) - res) & 0x0FFF);
            uint32_t value = values[value_offset];
    
            uint16_t comp_idx = value & 0xFFF;
            if (comp_idx == 0xFFF)
                return nullptr;
    
            auto ret = compositions + comp_idx;
            bool is_last = (ret[0] & (uint32_t(1) << 29));
            if (is_last)
                return nullptr;
            if (ret[0] & (uint32_t(0xFF) << 21))
                return nullptr;
    
            ++ret;
            return ret;
        }
    
        static auto get_comb_class(char32_t c) -> uint8_t
        {
            if (c < 128)
                return 0;
    
            auto res = lookup::get(c);
            if (res == 0)
                return 0;
    
            if (res & 0x1000)
                return uint8_t(res);
    
            size_t value_offset = ((size_t(c) - res) & 0x0FFF);
            uint32_t value = values[value_offset];
    
            // possible but an extra if...
            //if (value & (uint32_t(1) << 30))
            //    return (value >> 12) & 0xFF;
    
            uint16_t comp_idx = value & 0xFFF;
            if (comp_idx == 0xFFF)
                return 0;
    
            auto * comps = compositions + comp_idx;
            return (comps[0] >> 21) & 0xFF;
        }
    
        enum class nfc_qc_status
        {
            bad,
            good,
            stable
        };
    
        static auto get_nfc_qc_status(char32_t c) -> nfc_qc_status
        {
            if (c < 128)
                return nfc_qc_status::stable;
    
            auto res = lookup::get(c);
            if (res == 0)
                return nfc_qc_status::stable;
    
            if (res & 0x1000)
            {
                int is_ccc_zero = uint8_t(res) == 0;
                int is_nfc_qc_yes = !(res >> 8);
                return nfc_qc_status(0 + (is_nfc_qc_yes << is_ccc_zero));
            }
    
            size_t value_offset = ((size_t(c) - res) & 0x0FFF);
            uint32_t value = values[value_offset];
    
            int is_nfc_qc_yes = !(value & (1 << 30));
    
            uint16_t comp_idx = value & 0xFFF;
            if (comp_idx == 0xFFF)
                return nfc_qc_status(0 + (is_nfc_qc_yes << 1));
    
            auto * comps = compositions + comp_idx;
            int is_ccc_zero = !(comps[0] & (uint32_t(0xFF) << 21));
    
            return nfc_qc_status(0 + (is_nfc_qc_yes << is_ccc_zero));
        }
    };

}

#endif

