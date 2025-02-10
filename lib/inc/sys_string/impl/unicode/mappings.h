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
                if (*p == c)
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
    
            static const std::array<std::array<uint16_t, 32>, 64> stage2;
    
            static const std::array<std::array<uint16_t, 16>, 359> stage3;
    
            static const std::array<uint32_t, 754> stage4;
    
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
    
    
        static const char16_t cased_data[3940];
    
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
    
            static const std::array<std::array<uint16_t, 32>, 54> stage2;
    
            static const std::array<std::array<uint16_t, 16>, 308> stage3;
    
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


    class grapheme_cluster_break_prop 
    {
    private:
        static const std::array<uint8_t, 128> ascii;
    
        static const std::array<uint8_t, 2048> stage1;
    
        static const std::array<std::array<uint16_t, 32>, 74> stage2;
    
        static const std::array<std::array<uint8_t, 16>, 286> stage3;
    
        static const std::array<uint8_t, 16> stage4;
    
    public:
        enum value : decltype(stage4)::value_type
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



    inline constexpr char16_t is_whitespace::chars[26] = 
        u"\u0009\u000A\u000B\u000C\u000D\u0020\u0085\u00A0\u1680\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007"
        u"\u2008\u2009\u200A\u2028\u2029\u202F\u205F\u3000";

    inline constexpr char16_t case_mapper::cased_data[3940] = 
        u"\u0061\u0062\u0063\u0064\u0065\u0066\u0067\u0068\u0069\u006A\u006B\u006C\u006D\u006E\u006F\u0070\u0071"
        u"\u0072\u0073\u0074\u0075\u0076\u0077\u0078\u0079\u007A\u0041\u0042\u0043\u0044\u0045\u0046\u0047\u0048"
        u"\u0049\u004A\u004B\u004C\u004D\u004E\u004F\u0050\u0051\u0052\u0053\u0054\u0055\u0056\u0057\u0058\u0059"
        u"\u005A\u039C\u03BC\u00E0\u00E1\u00E2\u00E3\u00E4\u00E5\u00E6\u00E7\u00E8\u00E9\u00EA\u00EB\u00EC\u00ED"
        u"\u00EE\u00EF\u00F0\u00F1\u00F2\u00F3\u00F4\u00F5\u00F6\u00F8\u00F9\u00FA\u00FB\u00FC\u00FD\u00FE\u0053"
        u"\u0053\u0073\u0073\u00C0\u00C1\u00C2\u00C3\u00C4\u00C5\u00C6\u00C7\u00C8\u00C9\u00CA\u00CB\u00CC\u00CD"
        u"\u00CE\u00CF\u00D0\u00D1\u00D2\u00D3\u00D4\u00D5\u00D6\u00D8\u00D9\u00DA\u00DB\u00DC\u00DD\u00DE\u0178"
        u"\u0101\u0100\u0103\u0102\u0105\u0104\u0107\u0106\u0109\u0108\u010B\u010A\u010D\u010C\u010F\u010E\u0111"
        u"\u0110\u0113\u0112\u0115\u0114\u0117\u0116\u0119\u0118\u011B\u011A\u011D\u011C\u011F\u011E\u0121\u0120"
        u"\u0123\u0122\u0125\u0124\u0127\u0126\u0129\u0128\u012B\u012A\u012D\u012C\u012F\u012E\u0069\u0307\u0049"
        u"\u0133\u0132\u0135\u0134\u0137\u0136\u013A\u0139\u013C\u013B\u013E\u013D\u0140\u013F\u0142\u0141\u0144"
        u"\u0143\u0146\u0145\u0148\u0147\u02BC\u004E\u02BC\u006E\u014B\u014A\u014D\u014C\u014F\u014E\u0151\u0150"
        u"\u0153\u0152\u0155\u0154\u0157\u0156\u0159\u0158\u015B\u015A\u015D\u015C\u015F\u015E\u0161\u0160\u0163"
        u"\u0162\u0165\u0164\u0167\u0166\u0169\u0168\u016B\u016A\u016D\u016C\u016F\u016E\u0171\u0170\u0173\u0172"
        u"\u0175\u0174\u0177\u0176\u00FF\u017A\u0179\u017C\u017B\u017E\u017D\u0053\u0073\u0243\u0253\u0183\u0182"
        u"\u0185\u0184\u0254\u0188\u0187\u0256\u0257\u018C\u018B\u01DD\u0259\u025B\u0192\u0191\u0260\u0263\u01F6"
        u"\u0269\u0268\u0199\u0198\u023D\uA7DC\u026F\u0272\u0220\u0275\u01A1\u01A0\u01A3\u01A2\u01A5\u01A4\u0280"
        u"\u01A8\u01A7\u0283\u01AD\u01AC\u0288\u01B0\u01AF\u028A\u028B\u01B4\u01B3\u01B6\u01B5\u0292\u01B9\u01B8"
        u"\u01BD\u01BC\u01F7\u01C6\u01C6\u01C4\u01C4\u01C9\u01C9\u01C7\u01C7\u01CC\u01CC\u01CA\u01CA\u01CE\u01CD"
        u"\u01D0\u01CF\u01D2\u01D1\u01D4\u01D3\u01D6\u01D5\u01D8\u01D7\u01DA\u01D9\u01DC\u01DB\u018E\u01DF\u01DE"
        u"\u01E1\u01E0\u01E3\u01E2\u01E5\u01E4\u01E7\u01E6\u01E9\u01E8\u01EB\u01EA\u01ED\u01EC\u01EF\u01EE\u004A"
        u"\u030C\u006A\u030C\u01F3\u01F3\u01F1\u01F1\u01F5\u01F4\u0195\u01BF\u01F9\u01F8\u01FB\u01FA\u01FD\u01FC"
        u"\u01FF\u01FE\u0201\u0200\u0203\u0202\u0205\u0204\u0207\u0206\u0209\u0208\u020B\u020A\u020D\u020C\u020F"
        u"\u020E\u0211\u0210\u0213\u0212\u0215\u0214\u0217\u0216\u0219\u0218\u021B\u021A\u021D\u021C\u021F\u021E"
        u"\u019E\u0223\u0222\u0225\u0224\u0227\u0226\u0229\u0228\u022B\u022A\u022D\u022C\u022F\u022E\u0231\u0230"
        u"\u0233\u0232\u2C65\u023C\u023B\u019A\u2C66\u2C7E\u2C7F\u0242\u0241\u0180\u0289\u028C\u0247\u0246\u0249"
        u"\u0248\u024B\u024A\u024D\u024C\u024F\u024E\u2C6F\u2C6D\u2C70\u0181\u0186\u0189\u018A\u018F\u0190\uA7AB"
        u"\u0193\uA7AC\u0194\uA7CB\uA78D\uA7AA\u0197\u0196\uA7AE\u2C62\uA7AD\u019C\u2C6E\u019D\u019F\u2C64\u01A6"
        u"\uA7C5\u01A9\uA7B1\u01AE\u0244\u01B1\u01B2\u0245\u01B7\uA7B2\uA7B0\u0399\u03B9\u0371\u0370\u0373\u0372"
        u"\u0377\u0376\u03FD\u03FE\u03FF\u03F3\u03AC\u03AD\u03AE\u03AF\u03CC\u03CD\u03CE\u0399\u0308\u0301\u03B9"
        u"\u0308\u0301\u03B1\u03B2\u03B3\u03B4\u03B5\u03B6\u03B7\u03B8\u03B9\u03BA\u03BB\u03BC\u03BD\u03BE\u03BF"
        u"\u03C0\u03C1\u03C3\u03C4\u03C5\u03C6\u03C7\u03C8\u03C9\u03CA\u03CB\u0386\u0388\u0389\u038A\u03A5\u0308"
        u"\u0301\u03C5\u0308\u0301\u0391\u0392\u0393\u0394\u0395\u0396\u0397\u0398\u0399\u039A\u039B\u039C\u039D"
        u"\u039E\u039F\u03A0\u03A1\u03A3\u03C3\u03A3\u03A4\u03A5\u03A6\u03A7\u03A8\u03A9\u03AA\u03AB\u038C\u038E"
        u"\u038F\u03D7\u0392\u03B2\u0398\u03B8\u03A6\u03C6\u03A0\u03C0\u03CF\u03D9\u03D8\u03DB\u03DA\u03DD\u03DC"
        u"\u03DF\u03DE\u03E1\u03E0\u03E3\u03E2\u03E5\u03E4\u03E7\u03E6\u03E9\u03E8\u03EB\u03EA\u03ED\u03EC\u03EF"
        u"\u03EE\u039A\u03BA\u03A1\u03C1\u03F9\u037F\u03B8\u0395\u03B5\u03F8\u03F7\u03F2\u03FB\u03FA\u037B\u037C"
        u"\u037D\u0450\u0451\u0452\u0453\u0454\u0455\u0456\u0457\u0458\u0459\u045A\u045B\u045C\u045D\u045E\u045F"
        u"\u0430\u0431\u0432\u0433\u0434\u0435\u0436\u0437\u0438\u0439\u043A\u043B\u043C\u043D\u043E\u043F\u0440"
        u"\u0441\u0442\u0443\u0444\u0445\u0446\u0447\u0448\u0449\u044A\u044B\u044C\u044D\u044E\u044F\u0410\u0411"
        u"\u0412\u0413\u0414\u0415\u0416\u0417\u0418\u0419\u041A\u041B\u041C\u041D\u041E\u041F\u0420\u0421\u0422"
        u"\u0423\u0424\u0425\u0426\u0427\u0428\u0429\u042A\u042B\u042C\u042D\u042E\u042F\u0400\u0401\u0402\u0403"
        u"\u0404\u0405\u0406\u0407\u0408\u0409\u040A\u040B\u040C\u040D\u040E\u040F\u0461\u0460\u0463\u0462\u0465"
        u"\u0464\u0467\u0466\u0469\u0468\u046B\u046A\u046D\u046C\u046F\u046E\u0471\u0470\u0473\u0472\u0475\u0474"
        u"\u0477\u0476\u0479\u0478\u047B\u047A\u047D\u047C\u047F\u047E\u0481\u0480\u048B\u048A\u048D\u048C\u048F"
        u"\u048E\u0491\u0490\u0493\u0492\u0495\u0494\u0497\u0496\u0499\u0498\u049B\u049A\u049D\u049C\u049F\u049E"
        u"\u04A1\u04A0\u04A3\u04A2\u04A5\u04A4\u04A7\u04A6\u04A9\u04A8\u04AB\u04AA\u04AD\u04AC\u04AF\u04AE\u04B1"
        u"\u04B0\u04B3\u04B2\u04B5\u04B4\u04B7\u04B6\u04B9\u04B8\u04BB\u04BA\u04BD\u04BC\u04BF\u04BE\u04CF\u04C2"
        u"\u04C1\u04C4\u04C3\u04C6\u04C5\u04C8\u04C7\u04CA\u04C9\u04CC\u04CB\u04CE\u04CD\u04C0\u04D1\u04D0\u04D3"
        u"\u04D2\u04D5\u04D4\u04D7\u04D6\u04D9\u04D8\u04DB\u04DA\u04DD\u04DC\u04DF\u04DE\u04E1\u04E0\u04E3\u04E2"
        u"\u04E5\u04E4\u04E7\u04E6\u04E9\u04E8\u04EB\u04EA\u04ED\u04EC\u04EF\u04EE\u04F1\u04F0\u04F3\u04F2\u04F5"
        u"\u04F4\u04F7\u04F6\u04F9\u04F8\u04FB\u04FA\u04FD\u04FC\u04FF\u04FE\u0501\u0500\u0503\u0502\u0505\u0504"
        u"\u0507\u0506\u0509\u0508\u050B\u050A\u050D\u050C\u050F\u050E\u0511\u0510\u0513\u0512\u0515\u0514\u0517"
        u"\u0516\u0519\u0518\u051B\u051A\u051D\u051C\u051F\u051E\u0521\u0520\u0523\u0522\u0525\u0524\u0527\u0526"
        u"\u0529\u0528\u052B\u052A\u052D\u052C\u052F\u052E\u0561\u0562\u0563\u0564\u0565\u0566\u0567\u0568\u0569"
        u"\u056A\u056B\u056C\u056D\u056E\u056F\u0570\u0571\u0572\u0573\u0574\u0575\u0576\u0577\u0578\u0579\u057A"
        u"\u057B\u057C\u057D\u057E\u057F\u0580\u0581\u0582\u0583\u0584\u0585\u0586\u0531\u0532\u0533\u0534\u0535"
        u"\u0536\u0537\u0538\u0539\u053A\u053B\u053C\u053D\u053E\u053F\u0540\u0541\u0542\u0543\u0544\u0545\u0546"
        u"\u0547\u0548\u0549\u054A\u054B\u054C\u054D\u054E\u054F\u0550\u0551\u0552\u0553\u0554\u0555\u0556\u0535"
        u"\u0552\u0565\u0582\u2D00\u2D01\u2D02\u2D03\u2D04\u2D05\u2D06\u2D07\u2D08\u2D09\u2D0A\u2D0B\u2D0C\u2D0D"
        u"\u2D0E\u2D0F\u2D10\u2D11\u2D12\u2D13\u2D14\u2D15\u2D16\u2D17\u2D18\u2D19\u2D1A\u2D1B\u2D1C\u2D1D\u2D1E"
        u"\u2D1F\u2D20\u2D21\u2D22\u2D23\u2D24\u2D25\u2D27\u2D2D\u1C90\u1C91\u1C92\u1C93\u1C94\u1C95\u1C96\u1C97"
        u"\u1C98\u1C99\u1C9A\u1C9B\u1C9C\u1C9D\u1C9E\u1C9F\u1CA0\u1CA1\u1CA2\u1CA3\u1CA4\u1CA5\u1CA6\u1CA7\u1CA8"
        u"\u1CA9\u1CAA\u1CAB\u1CAC\u1CAD\u1CAE\u1CAF\u1CB0\u1CB1\u1CB2\u1CB3\u1CB4\u1CB5\u1CB6\u1CB7\u1CB8\u1CB9"
        u"\u1CBA\u1CBD\u1CBE\u1CBF\uAB70\uAB71\uAB72\uAB73\uAB74\uAB75\uAB76\uAB77\uAB78\uAB79\uAB7A\uAB7B\uAB7C"
        u"\uAB7D\uAB7E\uAB7F\uAB80\uAB81\uAB82\uAB83\uAB84\uAB85\uAB86\uAB87\uAB88\uAB89\uAB8A\uAB8B\uAB8C\uAB8D"
        u"\uAB8E\uAB8F\uAB90\uAB91\uAB92\uAB93\uAB94\uAB95\uAB96\uAB97\uAB98\uAB99\uAB9A\uAB9B\uAB9C\uAB9D\uAB9E"
        u"\uAB9F\uABA0\uABA1\uABA2\uABA3\uABA4\uABA5\uABA6\uABA7\uABA8\uABA9\uABAA\uABAB\uABAC\uABAD\uABAE\uABAF"
        u"\uABB0\uABB1\uABB2\uABB3\uABB4\uABB5\uABB6\uABB7\uABB8\uABB9\uABBA\uABBB\uABBC\uABBD\uABBE\uABBF\u13F8"
        u"\u13F9\u13FA\u13FB\u13FC\u13FD\u13F0\u13F1\u13F2\u13F3\u13F4\u13F5\u0412\u0432\u0414\u0434\u041E\u043E"
        u"\u0421\u0441\u0422\u0442\u0422\u0442\u042A\u044A\u0462\u0463\uA64A\uA64B\u1C8A\u1C89\u10D0\u10D1\u10D2"
        u"\u10D3\u10D4\u10D5\u10D6\u10D7\u10D8\u10D9\u10DA\u10DB\u10DC\u10DD\u10DE\u10DF\u10E0\u10E1\u10E2\u10E3"
        u"\u10E4\u10E5\u10E6\u10E7\u10E8\u10E9\u10EA\u10EB\u10EC\u10ED\u10EE\u10EF\u10F0\u10F1\u10F2\u10F3\u10F4"
        u"\u10F5\u10F6\u10F7\u10F8\u10F9\u10FA\u10FD\u10FE\u10FF\uA77D\u2C63\uA7C6\u1E01\u1E00\u1E03\u1E02\u1E05"
        u"\u1E04\u1E07\u1E06\u1E09\u1E08\u1E0B\u1E0A\u1E0D\u1E0C\u1E0F\u1E0E\u1E11\u1E10\u1E13\u1E12\u1E15\u1E14"
        u"\u1E17\u1E16\u1E19\u1E18\u1E1B\u1E1A\u1E1D\u1E1C\u1E1F\u1E1E\u1E21\u1E20\u1E23\u1E22\u1E25\u1E24\u1E27"
        u"\u1E26\u1E29\u1E28\u1E2B\u1E2A\u1E2D\u1E2C\u1E2F\u1E2E\u1E31\u1E30\u1E33\u1E32\u1E35\u1E34\u1E37\u1E36"
        u"\u1E39\u1E38\u1E3B\u1E3A\u1E3D\u1E3C\u1E3F\u1E3E\u1E41\u1E40\u1E43\u1E42\u1E45\u1E44\u1E47\u1E46\u1E49"
        u"\u1E48\u1E4B\u1E4A\u1E4D\u1E4C\u1E4F\u1E4E\u1E51\u1E50\u1E53\u1E52\u1E55\u1E54\u1E57\u1E56\u1E59\u1E58"
        u"\u1E5B\u1E5A\u1E5D\u1E5C\u1E5F\u1E5E\u1E61\u1E60\u1E63\u1E62\u1E65\u1E64\u1E67\u1E66\u1E69\u1E68\u1E6B"
        u"\u1E6A\u1E6D\u1E6C\u1E6F\u1E6E\u1E71\u1E70\u1E73\u1E72\u1E75\u1E74\u1E77\u1E76\u1E79\u1E78\u1E7B\u1E7A"
        u"\u1E7D\u1E7C\u1E7F\u1E7E\u1E81\u1E80\u1E83\u1E82\u1E85\u1E84\u1E87\u1E86\u1E89\u1E88\u1E8B\u1E8A\u1E8D"
        u"\u1E8C\u1E8F\u1E8E\u1E91\u1E90\u1E93\u1E92\u1E95\u1E94\u0048\u0331\u0068\u0331\u0054\u0308\u0074\u0308"
        u"\u0057\u030A\u0077\u030A\u0059\u030A\u0079\u030A\u0041\u02BE\u0061\u02BE\u1E60\u1E61\u00DF\u0073\u0073"
        u"\u1EA1\u1EA0\u1EA3\u1EA2\u1EA5\u1EA4\u1EA7\u1EA6\u1EA9\u1EA8\u1EAB\u1EAA\u1EAD\u1EAC\u1EAF\u1EAE\u1EB1"
        u"\u1EB0\u1EB3\u1EB2\u1EB5\u1EB4\u1EB7\u1EB6\u1EB9\u1EB8\u1EBB\u1EBA\u1EBD\u1EBC\u1EBF\u1EBE\u1EC1\u1EC0"
        u"\u1EC3\u1EC2\u1EC5\u1EC4\u1EC7\u1EC6\u1EC9\u1EC8\u1ECB\u1ECA\u1ECD\u1ECC\u1ECF\u1ECE\u1ED1\u1ED0\u1ED3"
        u"\u1ED2\u1ED5\u1ED4\u1ED7\u1ED6\u1ED9\u1ED8\u1EDB\u1EDA\u1EDD\u1EDC\u1EDF\u1EDE\u1EE1\u1EE0\u1EE3\u1EE2"
        u"\u1EE5\u1EE4\u1EE7\u1EE6\u1EE9\u1EE8\u1EEB\u1EEA\u1EED\u1EEC\u1EEF\u1EEE\u1EF1\u1EF0\u1EF3\u1EF2\u1EF5"
        u"\u1EF4\u1EF7\u1EF6\u1EF9\u1EF8\u1EFB\u1EFA\u1EFD\u1EFC\u1EFF\u1EFE\u1F08\u1F09\u1F0A\u1F0B\u1F0C\u1F0D"
        u"\u1F0E\u1F0F\u1F00\u1F01\u1F02\u1F03\u1F04\u1F05\u1F06\u1F07\u1F18\u1F19\u1F1A\u1F1B\u1F1C\u1F1D\u1F10"
        u"\u1F11\u1F12\u1F13\u1F14\u1F15\u1F28\u1F29\u1F2A\u1F2B\u1F2C\u1F2D\u1F2E\u1F2F\u1F20\u1F21\u1F22\u1F23"
        u"\u1F24\u1F25\u1F26\u1F27\u1F38\u1F39\u1F3A\u1F3B\u1F3C\u1F3D\u1F3E\u1F3F\u1F30\u1F31\u1F32\u1F33\u1F34"
        u"\u1F35\u1F36\u1F37\u1F48\u1F49\u1F4A\u1F4B\u1F4C\u1F4D\u1F40\u1F41\u1F42\u1F43\u1F44\u1F45\u03A5\u0313"
        u"\u03C5\u0313\u1F59\u03A5\u0313\u0300\u03C5\u0313\u0300\u1F5B\u03A5\u0313\u0301\u03C5\u0313\u0301\u1F5D"
        u"\u03A5\u0313\u0342\u03C5\u0313\u0342\u1F5F\u1F51\u1F53\u1F55\u1F57\u1F68\u1F69\u1F6A\u1F6B\u1F6C\u1F6D"
        u"\u1F6E\u1F6F\u1F60\u1F61\u1F62\u1F63\u1F64\u1F65\u1F66\u1F67\u1FBA\u1FBB\u1FC8\u1FC9\u1FCA\u1FCB\u1FDA"
        u"\u1FDB\u1FF8\u1FF9\u1FEA\u1FEB\u1FFA\u1FFB\u1F08\u0399\u1F00\u03B9\u1F09\u0399\u1F01\u03B9\u1F0A\u0399"
        u"\u1F02\u03B9\u1F0B\u0399\u1F03\u03B9\u1F0C\u0399\u1F04\u03B9\u1F0D\u0399\u1F05\u03B9\u1F0E\u0399\u1F06"
        u"\u03B9\u1F0F\u0399\u1F07\u03B9\u1F80\u1F08\u0399\u1F00\u03B9\u1F81\u1F09\u0399\u1F01\u03B9\u1F82\u1F0A"
        u"\u0399\u1F02\u03B9\u1F83\u1F0B\u0399\u1F03\u03B9\u1F84\u1F0C\u0399\u1F04\u03B9\u1F85\u1F0D\u0399\u1F05"
        u"\u03B9\u1F86\u1F0E\u0399\u1F06\u03B9\u1F87\u1F0F\u0399\u1F07\u03B9\u1F28\u0399\u1F20\u03B9\u1F29\u0399"
        u"\u1F21\u03B9\u1F2A\u0399\u1F22\u03B9\u1F2B\u0399\u1F23\u03B9\u1F2C\u0399\u1F24\u03B9\u1F2D\u0399\u1F25"
        u"\u03B9\u1F2E\u0399\u1F26\u03B9\u1F2F\u0399\u1F27\u03B9\u1F90\u1F28\u0399\u1F20\u03B9\u1F91\u1F29\u0399"
        u"\u1F21\u03B9\u1F92\u1F2A\u0399\u1F22\u03B9\u1F93\u1F2B\u0399\u1F23\u03B9\u1F94\u1F2C\u0399\u1F24\u03B9"
        u"\u1F95\u1F2D\u0399\u1F25\u03B9\u1F96\u1F2E\u0399\u1F26\u03B9\u1F97\u1F2F\u0399\u1F27\u03B9\u1F68\u0399"
        u"\u1F60\u03B9\u1F69\u0399\u1F61\u03B9\u1F6A\u0399\u1F62\u03B9\u1F6B\u0399\u1F63\u03B9\u1F6C\u0399\u1F64"
        u"\u03B9\u1F6D\u0399\u1F65\u03B9\u1F6E\u0399\u1F66\u03B9\u1F6F\u0399\u1F67\u03B9\u1FA0\u1F68\u0399\u1F60"
        u"\u03B9\u1FA1\u1F69\u0399\u1F61\u03B9\u1FA2\u1F6A\u0399\u1F62\u03B9\u1FA3\u1F6B\u0399\u1F63\u03B9\u1FA4"
        u"\u1F6C\u0399\u1F64\u03B9\u1FA5\u1F6D\u0399\u1F65\u03B9\u1FA6\u1F6E\u0399\u1F66\u03B9\u1FA7\u1F6F\u0399"
        u"\u1F67\u03B9\u1FB8\u1FB9\u1FBA\u0399\u1F70\u03B9\u0391\u0399\u03B1\u03B9\u0386\u0399\u03AC\u03B9\u0391"
        u"\u0342\u03B1\u0342\u0391\u0342\u0399\u03B1\u0342\u03B9\u1FB0\u1FB1\u1F70\u1F71\u1FB3\u0391\u0399\u03B1"
        u"\u03B9\u0399\u03B9\u1FCA\u0399\u1F74\u03B9\u0397\u0399\u03B7\u03B9\u0389\u0399\u03AE\u03B9\u0397\u0342"
        u"\u03B7\u0342\u0397\u0342\u0399\u03B7\u0342\u03B9\u1F72\u1F73\u1F74\u1F75\u1FC3\u0397\u0399\u03B7\u03B9"
        u"\u1FD8\u1FD9\u0399\u0308\u0300\u03B9\u0308\u0300\u0399\u0308\u0301\u03B9\u0308\u0301\u0399\u0342\u03B9"
        u"\u0342\u0399\u0308\u0342\u03B9\u0308\u0342\u1FD0\u1FD1\u1F76\u1F77\u1FE8\u1FE9\u03A5\u0308\u0300\u03C5"
        u"\u0308\u0300\u03A5\u0308\u0301\u03C5\u0308\u0301\u03A1\u0313\u03C1\u0313\u1FEC\u03A5\u0342\u03C5\u0342"
        u"\u03A5\u0308\u0342\u03C5\u0308\u0342\u1FE0\u1FE1\u1F7A\u1F7B\u1FE5\u1FFA\u0399\u1F7C\u03B9\u03A9\u0399"
        u"\u03C9\u03B9\u038F\u0399\u03CE\u03B9\u03A9\u0342\u03C9\u0342\u03A9\u0342\u0399\u03C9\u0342\u03B9\u1F78"
        u"\u1F79\u1F7C\u1F7D\u1FF3\u03A9\u0399\u03C9\u03B9\u03C9\u006B\u00E5\u214E\u2132\u2170\u2171\u2172\u2173"
        u"\u2174\u2175\u2176\u2177\u2178\u2179\u217A\u217B\u217C\u217D\u217E\u217F\u2160\u2161\u2162\u2163\u2164"
        u"\u2165\u2166\u2167\u2168\u2169\u216A\u216B\u216C\u216D\u216E\u216F\u2184\u2183\u24D0\u24D1\u24D2\u24D3"
        u"\u24D4\u24D5\u24D6\u24D7\u24D8\u24D9\u24DA\u24DB\u24DC\u24DD\u24DE\u24DF\u24E0\u24E1\u24E2\u24E3\u24E4"
        u"\u24E5\u24E6\u24E7\u24E8\u24E9\u24B6\u24B7\u24B8\u24B9\u24BA\u24BB\u24BC\u24BD\u24BE\u24BF\u24C0\u24C1"
        u"\u24C2\u24C3\u24C4\u24C5\u24C6\u24C7\u24C8\u24C9\u24CA\u24CB\u24CC\u24CD\u24CE\u24CF\u2C30\u2C31\u2C32"
        u"\u2C33\u2C34\u2C35\u2C36\u2C37\u2C38\u2C39\u2C3A\u2C3B\u2C3C\u2C3D\u2C3E\u2C3F\u2C40\u2C41\u2C42\u2C43"
        u"\u2C44\u2C45\u2C46\u2C47\u2C48\u2C49\u2C4A\u2C4B\u2C4C\u2C4D\u2C4E\u2C4F\u2C50\u2C51\u2C52\u2C53\u2C54"
        u"\u2C55\u2C56\u2C57\u2C58\u2C59\u2C5A\u2C5B\u2C5C\u2C5D\u2C5E\u2C5F\u2C00\u2C01\u2C02\u2C03\u2C04\u2C05"
        u"\u2C06\u2C07\u2C08\u2C09\u2C0A\u2C0B\u2C0C\u2C0D\u2C0E\u2C0F\u2C10\u2C11\u2C12\u2C13\u2C14\u2C15\u2C16"
        u"\u2C17\u2C18\u2C19\u2C1A\u2C1B\u2C1C\u2C1D\u2C1E\u2C1F\u2C20\u2C21\u2C22\u2C23\u2C24\u2C25\u2C26\u2C27"
        u"\u2C28\u2C29\u2C2A\u2C2B\u2C2C\u2C2D\u2C2E\u2C2F\u2C61\u2C60\u026B\u1D7D\u027D\u023A\u023E\u2C68\u2C67"
        u"\u2C6A\u2C69\u2C6C\u2C6B\u0251\u0271\u0250\u0252\u2C73\u2C72\u2C76\u2C75\u023F\u0240\u2C81\u2C80\u2C83"
        u"\u2C82\u2C85\u2C84\u2C87\u2C86\u2C89\u2C88\u2C8B\u2C8A\u2C8D\u2C8C\u2C8F\u2C8E\u2C91\u2C90\u2C93\u2C92"
        u"\u2C95\u2C94\u2C97\u2C96\u2C99\u2C98\u2C9B\u2C9A\u2C9D\u2C9C\u2C9F\u2C9E\u2CA1\u2CA0\u2CA3\u2CA2\u2CA5"
        u"\u2CA4\u2CA7\u2CA6\u2CA9\u2CA8\u2CAB\u2CAA\u2CAD\u2CAC\u2CAF\u2CAE\u2CB1\u2CB0\u2CB3\u2CB2\u2CB5\u2CB4"
        u"\u2CB7\u2CB6\u2CB9\u2CB8\u2CBB\u2CBA\u2CBD\u2CBC\u2CBF\u2CBE\u2CC1\u2CC0\u2CC3\u2CC2\u2CC5\u2CC4\u2CC7"
        u"\u2CC6\u2CC9\u2CC8\u2CCB\u2CCA\u2CCD\u2CCC\u2CCF\u2CCE\u2CD1\u2CD0\u2CD3\u2CD2\u2CD5\u2CD4\u2CD7\u2CD6"
        u"\u2CD9\u2CD8\u2CDB\u2CDA\u2CDD\u2CDC\u2CDF\u2CDE\u2CE1\u2CE0\u2CE3\u2CE2\u2CEC\u2CEB\u2CEE\u2CED\u2CF3"
        u"\u2CF2\u10A0\u10A1\u10A2\u10A3\u10A4\u10A5\u10A6\u10A7\u10A8\u10A9\u10AA\u10AB\u10AC\u10AD\u10AE\u10AF"
        u"\u10B0\u10B1\u10B2\u10B3\u10B4\u10B5\u10B6\u10B7\u10B8\u10B9\u10BA\u10BB\u10BC\u10BD\u10BE\u10BF\u10C0"
        u"\u10C1\u10C2\u10C3\u10C4\u10C5\u10C7\u10CD\uA641\uA640\uA643\uA642\uA645\uA644\uA647\uA646\uA649\uA648"
        u"\uA64B\uA64A\uA64D\uA64C\uA64F\uA64E\uA651\uA650\uA653\uA652\uA655\uA654\uA657\uA656\uA659\uA658\uA65B"
        u"\uA65A\uA65D\uA65C\uA65F\uA65E\uA661\uA660\uA663\uA662\uA665\uA664\uA667\uA666\uA669\uA668\uA66B\uA66A"
        u"\uA66D\uA66C\uA681\uA680\uA683\uA682\uA685\uA684\uA687\uA686\uA689\uA688\uA68B\uA68A\uA68D\uA68C\uA68F"
        u"\uA68E\uA691\uA690\uA693\uA692\uA695\uA694\uA697\uA696\uA699\uA698\uA69B\uA69A\uA723\uA722\uA725\uA724"
        u"\uA727\uA726\uA729\uA728\uA72B\uA72A\uA72D\uA72C\uA72F\uA72E\uA733\uA732\uA735\uA734\uA737\uA736\uA739"
        u"\uA738\uA73B\uA73A\uA73D\uA73C\uA73F\uA73E\uA741\uA740\uA743\uA742\uA745\uA744\uA747\uA746\uA749\uA748"
        u"\uA74B\uA74A\uA74D\uA74C\uA74F\uA74E\uA751\uA750\uA753\uA752\uA755\uA754\uA757\uA756\uA759\uA758\uA75B"
        u"\uA75A\uA75D\uA75C\uA75F\uA75E\uA761\uA760\uA763\uA762\uA765\uA764\uA767\uA766\uA769\uA768\uA76B\uA76A"
        u"\uA76D\uA76C\uA76F\uA76E\uA77A\uA779\uA77C\uA77B\u1D79\uA77F\uA77E\uA781\uA780\uA783\uA782\uA785\uA784"
        u"\uA787\uA786\uA78C\uA78B\u0265\uA791\uA790\uA793\uA792\uA7C4\uA797\uA796\uA799\uA798\uA79B\uA79A\uA79D"
        u"\uA79C\uA79F\uA79E\uA7A1\uA7A0\uA7A3\uA7A2\uA7A5\uA7A4\uA7A7\uA7A6\uA7A9\uA7A8\u0266\u025C\u0261\u026C"
        u"\u026A\u029E\u0287\u029D\uAB53\uA7B5\uA7B4\uA7B7\uA7B6\uA7B9\uA7B8\uA7BB\uA7BA\uA7BD\uA7BC\uA7BF\uA7BE"
        u"\uA7C1\uA7C0\uA7C3\uA7C2\uA794\u0282\u1D8E\uA7C8\uA7C7\uA7CA\uA7C9\u0264\uA7CD\uA7CC\uA7D1\uA7D0\uA7D7"
        u"\uA7D6\uA7D9\uA7D8\uA7DB\uA7DA\u019B\uA7F6\uA7F5\uA7B3\u13A0\u13A1\u13A2\u13A3\u13A4\u13A5\u13A6\u13A7"
        u"\u13A8\u13A9\u13AA\u13AB\u13AC\u13AD\u13AE\u13AF\u13B0\u13B1\u13B2\u13B3\u13B4\u13B5\u13B6\u13B7\u13B8"
        u"\u13B9\u13BA\u13BB\u13BC\u13BD\u13BE\u13BF\u13C0\u13C1\u13C2\u13C3\u13C4\u13C5\u13C6\u13C7\u13C8\u13C9"
        u"\u13CA\u13CB\u13CC\u13CD\u13CE\u13CF\u13D0\u13D1\u13D2\u13D3\u13D4\u13D5\u13D6\u13D7\u13D8\u13D9\u13DA"
        u"\u13DB\u13DC\u13DD\u13DE\u13DF\u13E0\u13E1\u13E2\u13E3\u13E4\u13E5\u13E6\u13E7\u13E8\u13E9\u13EA\u13EB"
        u"\u13EC\u13ED\u13EE\u13EF\u0046\u0046\u0066\u0066\u0046\u0049\u0066\u0069\u0046\u004C\u0066\u006C\u0046"
        u"\u0046\u0049\u0066\u0066\u0069\u0046\u0046\u004C\u0066\u0066\u006C\u0053\u0054\u0073\u0074\u0053\u0054"
        u"\u0073\u0074\u0544\u0546\u0574\u0576\u0544\u0535\u0574\u0565\u0544\u053B\u0574\u056B\u054E\u0546\u057E"
        u"\u0576\u0544\u053D\u0574\u056D\uFF41\uFF42\uFF43\uFF44\uFF45\uFF46\uFF47\uFF48\uFF49\uFF4A\uFF4B\uFF4C"
        u"\uFF4D\uFF4E\uFF4F\uFF50\uFF51\uFF52\uFF53\uFF54\uFF55\uFF56\uFF57\uFF58\uFF59\uFF5A\uFF21\uFF22\uFF23"
        u"\uFF24\uFF25\uFF26\uFF27\uFF28\uFF29\uFF2A\uFF2B\uFF2C\uFF2D\uFF2E\uFF2F\uFF30\uFF31\uFF32\uFF33\uFF34"
        u"\uFF35\uFF36\uFF37\uFF38\uFF39\uFF3A\U00010428\U00010429\U0001042A\U0001042B\U0001042C\U0001042D\U0001042E"
        u"\U0001042F\U00010430\U00010431\U00010432\U00010433\U00010434\U00010435\U00010436\U00010437\U00010438"
        u"\U00010439\U0001043A\U0001043B\U0001043C\U0001043D\U0001043E\U0001043F\U00010440\U00010441\U00010442"
        u"\U00010443\U00010444\U00010445\U00010446\U00010447\U00010448\U00010449\U0001044A\U0001044B\U0001044C"
        u"\U0001044D\U0001044E\U0001044F\U00010400\U00010401\U00010402\U00010403\U00010404\U00010405\U00010406"
        u"\U00010407\U00010408\U00010409\U0001040A\U0001040B\U0001040C\U0001040D\U0001040E\U0001040F\U00010410"
        u"\U00010411\U00010412\U00010413\U00010414\U00010415\U00010416\U00010417\U00010418\U00010419\U0001041A"
        u"\U0001041B\U0001041C\U0001041D\U0001041E\U0001041F\U00010420\U00010421\U00010422\U00010423\U00010424"
        u"\U00010425\U00010426\U00010427\U000104D8\U000104D9\U000104DA\U000104DB\U000104DC\U000104DD\U000104DE"
        u"\U000104DF\U000104E0\U000104E1\U000104E2\U000104E3\U000104E4\U000104E5\U000104E6\U000104E7\U000104E8"
        u"\U000104E9\U000104EA\U000104EB\U000104EC\U000104ED\U000104EE\U000104EF\U000104F0\U000104F1\U000104F2"
        u"\U000104F3\U000104F4\U000104F5\U000104F6\U000104F7\U000104F8\U000104F9\U000104FA\U000104FB\U000104B0"
        u"\U000104B1\U000104B2\U000104B3\U000104B4\U000104B5\U000104B6\U000104B7\U000104B8\U000104B9\U000104BA"
        u"\U000104BB\U000104BC\U000104BD\U000104BE\U000104BF\U000104C0\U000104C1\U000104C2\U000104C3\U000104C4"
        u"\U000104C5\U000104C6\U000104C7\U000104C8\U000104C9\U000104CA\U000104CB\U000104CC\U000104CD\U000104CE"
        u"\U000104CF\U000104D0\U000104D1\U000104D2\U000104D3\U00010597\U00010598\U00010599\U0001059A\U0001059B"
        u"\U0001059C\U0001059D\U0001059E\U0001059F\U000105A0\U000105A1\U000105A3\U000105A4\U000105A5\U000105A6"
        u"\U000105A7\U000105A8\U000105A9\U000105AA\U000105AB\U000105AC\U000105AD\U000105AE\U000105AF\U000105B0"
        u"\U000105B1\U000105B3\U000105B4\U000105B5\U000105B6\U000105B7\U000105B8\U000105B9\U000105BB\U000105BC"
        u"\U00010570\U00010571\U00010572\U00010573\U00010574\U00010575\U00010576\U00010577\U00010578\U00010579"
        u"\U0001057A\U0001057C\U0001057D\U0001057E\U0001057F\U00010580\U00010581\U00010582\U00010583\U00010584"
        u"\U00010585\U00010586\U00010587\U00010588\U00010589\U0001058A\U0001058C\U0001058D\U0001058E\U0001058F"
        u"\U00010590\U00010591\U00010592\U00010594\U00010595\U00010CC0\U00010CC1\U00010CC2\U00010CC3\U00010CC4"
        u"\U00010CC5\U00010CC6\U00010CC7\U00010CC8\U00010CC9\U00010CCA\U00010CCB\U00010CCC\U00010CCD\U00010CCE"
        u"\U00010CCF\U00010CD0\U00010CD1\U00010CD2\U00010CD3\U00010CD4\U00010CD5\U00010CD6\U00010CD7\U00010CD8"
        u"\U00010CD9\U00010CDA\U00010CDB\U00010CDC\U00010CDD\U00010CDE\U00010CDF\U00010CE0\U00010CE1\U00010CE2"
        u"\U00010CE3\U00010CE4\U00010CE5\U00010CE6\U00010CE7\U00010CE8\U00010CE9\U00010CEA\U00010CEB\U00010CEC"
        u"\U00010CED\U00010CEE\U00010CEF\U00010CF0\U00010CF1\U00010CF2\U00010C80\U00010C81\U00010C82\U00010C83"
        u"\U00010C84\U00010C85\U00010C86\U00010C87\U00010C88\U00010C89\U00010C8A\U00010C8B\U00010C8C\U00010C8D"
        u"\U00010C8E\U00010C8F\U00010C90\U00010C91\U00010C92\U00010C93\U00010C94\U00010C95\U00010C96\U00010C97"
        u"\U00010C98\U00010C99\U00010C9A\U00010C9B\U00010C9C\U00010C9D\U00010C9E\U00010C9F\U00010CA0\U00010CA1"
        u"\U00010CA2\U00010CA3\U00010CA4\U00010CA5\U00010CA6\U00010CA7\U00010CA8\U00010CA9\U00010CAA\U00010CAB"
        u"\U00010CAC\U00010CAD\U00010CAE\U00010CAF\U00010CB0\U00010CB1\U00010CB2\U00010D70\U00010D71\U00010D72"
        u"\U00010D73\U00010D74\U00010D75\U00010D76\U00010D77\U00010D78\U00010D79\U00010D7A\U00010D7B\U00010D7C"
        u"\U00010D7D\U00010D7E\U00010D7F\U00010D80\U00010D81\U00010D82\U00010D83\U00010D84\U00010D85\U00010D50"
        u"\U00010D51\U00010D52\U00010D53\U00010D54\U00010D55\U00010D56\U00010D57\U00010D58\U00010D59\U00010D5A"
        u"\U00010D5B\U00010D5C\U00010D5D\U00010D5E\U00010D5F\U00010D60\U00010D61\U00010D62\U00010D63\U00010D64"
        u"\U00010D65\U000118C0\U000118C1\U000118C2\U000118C3\U000118C4\U000118C5\U000118C6\U000118C7\U000118C8"
        u"\U000118C9\U000118CA\U000118CB\U000118CC\U000118CD\U000118CE\U000118CF\U000118D0\U000118D1\U000118D2"
        u"\U000118D3\U000118D4\U000118D5\U000118D6\U000118D7\U000118D8\U000118D9\U000118DA\U000118DB\U000118DC"
        u"\U000118DD\U000118DE\U000118DF\U000118A0\U000118A1\U000118A2\U000118A3\U000118A4\U000118A5\U000118A6"
        u"\U000118A7\U000118A8\U000118A9\U000118AA\U000118AB\U000118AC\U000118AD\U000118AE\U000118AF\U000118B0"
        u"\U000118B1\U000118B2\U000118B3\U000118B4\U000118B5\U000118B6\U000118B7\U000118B8\U000118B9\U000118BA"
        u"\U000118BB\U000118BC\U000118BD\U000118BE\U000118BF\U00016E60\U00016E61\U00016E62\U00016E63\U00016E64"
        u"\U00016E65\U00016E66\U00016E67\U00016E68\U00016E69\U00016E6A\U00016E6B\U00016E6C\U00016E6D\U00016E6E"
        u"\U00016E6F\U00016E70\U00016E71\U00016E72\U00016E73\U00016E74\U00016E75\U00016E76\U00016E77\U00016E78"
        u"\U00016E79\U00016E7A\U00016E7B\U00016E7C\U00016E7D\U00016E7E\U00016E7F\U00016E40\U00016E41\U00016E42"
        u"\U00016E43\U00016E44\U00016E45\U00016E46\U00016E47\U00016E48\U00016E49\U00016E4A\U00016E4B\U00016E4C"
        u"\U00016E4D\U00016E4E\U00016E4F\U00016E50\U00016E51\U00016E52\U00016E53\U00016E54\U00016E55\U00016E56"
        u"\U00016E57\U00016E58\U00016E59\U00016E5A\U00016E5B\U00016E5C\U00016E5D\U00016E5E\U00016E5F\U0001E922"
        u"\U0001E923\U0001E924\U0001E925\U0001E926\U0001E927\U0001E928\U0001E929\U0001E92A\U0001E92B\U0001E92C"
        u"\U0001E92D\U0001E92E\U0001E92F\U0001E930\U0001E931\U0001E932\U0001E933\U0001E934\U0001E935\U0001E936"
        u"\U0001E937\U0001E938\U0001E939\U0001E93A\U0001E93B\U0001E93C\U0001E93D\U0001E93E\U0001E93F\U0001E940"
        u"\U0001E941\U0001E942\U0001E943\U0001E900\U0001E901\U0001E902\U0001E903\U0001E904\U0001E905\U0001E906"
        u"\U0001E907\U0001E908\U0001E909\U0001E90A\U0001E90B\U0001E90C\U0001E90D\U0001E90E\U0001E90F\U0001E910"
        u"\U0001E911\U0001E912\U0001E913\U0001E914\U0001E915\U0001E916\U0001E917\U0001E918\U0001E919\U0001E91A"
        u"\U0001E91B\U0001E91C\U0001E91D\U0001E91E\U0001E91F\U0001E920\U0001E921";
    
    inline constexpr std::array<uint32_t, 128> case_mapper::lookup::ascii({{
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x20000000, 
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x20000000, 0x00000000, 0x00000000, 0x00000000, 
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x20000000, 0x00000000, 
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x1F010041, 0x1F010041, 0x1F010041, 0x1F010041, 0x1F010041, 
        0x1F010041, 0x1F010041, 0x1F010041, 0x1F010041, 0x1F010041, 0x1F010041, 0x1F010041, 0x1F010041, 0x1F010041, 0x1F010041, 
        0x1F010041, 0x1F010041, 0x1F010041, 0x1F010041, 0x1F010041, 0x1F010041, 0x1F010041, 0x1F010041, 0x1F010041, 0x1F010041, 
        0x1F010041, 0x00000000, 0x00000000, 0x00000000, 0x20000000, 0x00000000, 0x20000000, 0x10100047, 0x10100047, 0x10100047, 
        0x10100047, 0x10100047, 0x10100047, 0x10100047, 0x10100047, 0x10100047, 0x10100047, 0x10100047, 0x10100047, 0x10100047, 
        0x10100047, 0x10100047, 0x10100047, 0x10100047, 0x10100047, 0x10100047, 0x10100047, 0x10100047, 0x10100047, 0x10100047, 
        0x10100047, 0x10100047, 0x10100047, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
    }});
    
    inline constexpr std::array<uint8_t, 2048> case_mapper::lookup::stage1({{
        23, 54, 55, 60, 40, 49, 59, 30, 36,  6,  0, 10, 43, 39, 37, 57, 44,  0, 20,  0,  0,  0, 56, 33, 45,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 41,  0, 14, 42, 46, 38,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0, 12,  0, 62,  1, 18, 58,  4,  0, 50, 25, 22, 47, 34, 28, 29, 35, 51, 31, 15,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0, 32,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  0,  0,  0, 
         0, 16,  7, 27,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  3,  0,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0,  0,  0, 13,  5, 26, 52, 53,  0, 63,  0, 11, 
        61, 21, 17,  0, 19,  0,  0,  0,  8,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0
    }});
    
    inline constexpr std::array<std::array<uint16_t, 32>, 64> case_mapper::lookup::stage2({{
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  17}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 323}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 351, 358, 358, 349,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  37,  84, 317,   0,  20,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   7,   0,   0,   0,   0, 155, 155, 155, 155, 155, 154}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 315,   0,  15,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 142, 135, 142, 135, 142, 135,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   3, 340,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  95,   0,  94,   0,  94,   0,  94,   0,   0,   0,  67,  43, 321,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 138, 141,  53,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 209,  71,   0,   0,   0,   0,   0,   0,   0,   0,   0, 101, 315,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 347, 347, 344, 347, 330,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  32,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  76, 304,   0,   0,  46, 296,  18,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 324,   0,   0,   0, 330, 320,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  17,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 287,   0,   0,   0,   0,   0,   0,   0,   0,  46,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 330,   0,   0, 276, 275, 274, 195, 194,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  40, 251, 185, 184,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   0,   0,   0,  13,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  15,   0,   0,   0,   0,  13,   0,   0,   0,   0,  51, 287,   0,   0,  97,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  28,  65, 227, 226, 158, 157, 225, 225, 225, 265, 160, 218, 218, 219, 156, 221, 220, 159,  60, 161, 228, 210}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   6, 320,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0, 283, 281, 280, 279, 201, 200, 199, 198,   0,   0,  69,   0,   2, 284, 282, 204, 202,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,  95,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0, 286, 285, 205, 203,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   1, 347,   0,   0,   0,   0, 310,   0}},
        {{  0,   0,   0,  33,  96,   2,   0,   0,   0,   0,   0,  80, 298,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  98, 287,  11,   0,   0}},
        {{  0,   0,   0,  82, 287,   0,   0,   0,   0,   0,  14, 329,   0,   0,   0,   0,   0,   5, 100,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0, 106,  50,   0,   0,   0,   0,   0,   0, 107,  45,   0,   0,   0,   0,  29,   0,  56,   0,   0,   0, 122, 325, 337, 347, 343,  41,   0,   0,   0}},
        {{  0,   0,   0, 333,   0,   0,   0,   0,   0, 101, 335, 299,   0,   0,   0,   0,   0,   0,   0, 117, 329,   0,   0,   0,   0, 308,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0, 347, 292, 327,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   1, 309, 105,   0,   0,   0,   0,   0,   0,   0,   0,   1,  81,   0, 304,   0,   0,  15, 287,   0,  47, 324,   0,   0,   0,  17, 288, 296, 108,   0}},
        {{  0,   0,   1, 336,   0,   0,   0,   0,   0,   0, 278, 277, 197, 196,   0,   0,   0,   0,   0,  16,  73,   0,   0,   0,   0,   0,   0,   0,   0,  70, 287,   0}},
        {{  0,   0,   7, 301,   0,  30, 287, 113,  93,   4, 253, 253, 252, 187, 187, 186,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,  13, 322,   0,   0,   0,  32, 208, 246, 246, 245,   0, 319, 302,  62, 142, 142, 144, 358, 358, 358, 355, 147, 143, 145, 358, 358, 347, 347, 347, 347}},
        {{  0,   0,  25, 110,  74,   0,   0, 290,   0,   0,   0, 300, 105,   4,  11,  77,   0,   0,   0, 142, 142, 148, 146, 217, 217, 217, 217, 217,   0,   0,  55,   0}},
        {{  0,  36,   0,   0,   0,  45, 297,  83,   0,   0,  34, 347, 346,   0,   0,   0, 320,   0,   0,  64,  88,   0,  17, 320, 304,   0,  99,   0,   0,   0,  44, 304}},
        {{  0,  51, 344,   0,   0,  23,   0,   0,  27, 306,   0,   0,  26, 347, 347, 347, 315,   0,   0,  19, 120, 119,  94, 105, 105,   0,   0,   8, 114,   0,  94,   2}},
        {{  0,  54,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  8,   0,   0,   0, 255, 255, 254, 318, 256, 257,   0,   0,   0,   0,   0, 304, 347, 347, 314, 151, 258, 258, 258, 350, 259, 260, 261, 263, 262, 264,   0, 102}},
        {{ 17,   0,   0,   0,  73,   0,   0,   0,  57,   0,  22,   0,   0,   0,   0,   0,   0,   0, 316,  90,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{ 17,  29,  63,   0,   0,   0, 326, 124,   0, 356,   0,   0,   0, 347, 347, 287,  85, 127,  59, 150,  52,   0, 250, 183,  72,   0,   0,   0,   0,   0,   0,   0}},
        {{ 54,   0,  20, 115,   0,   0,   0,   0,   0,  25,   0,   0,   0,   0,   0,  12,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{ 91,   0,  58,   0,   0,   0,   0,   0,   0,   0,   0,   0,  66,   0, 347, 305,   0,   0,  48,   0,  39, 304,   0,   0, 315,   0,   0,  75,   1,   0,  57,   0}},
        {{105,   0,   0,  33, 330,   0,   0, 295, 304,   0,   0,  78,  89,   0,   0,   0, 315,   0,  38, 324,   0,   0,   0,  73, 304,   0,   0,  50,  24,   0,   0,   0}},
        {{105,   0, 347, 347, 347, 347, 347, 347,   0,   0,   0,   0,   0,   0,   0,   0, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347,   0}},
        {{108,   0,   0,   8, 109, 105,   0, 307, 108,   0,   0,   8, 116,   0,  94,  21, 105,   0,   0,   9, 114,  57,  94,   0,  88,   0,   0,   0, 289,   0,   0,   0}},
        {{112,   0,   0,  31,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  57,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{121,   0,   0,  79,  34, 118,   0,   0,  21, 332,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{142, 142, 142, 142, 142, 131, 142, 142, 142, 140,  86, 136, 128, 142, 142, 142, 133, 130, 142, 137, 129, 125, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142}},
        {{142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 132, 142, 125, 139, 142, 139, 142, 131, 142, 131, 141, 142, 141, 142, 134, 142, 134, 142, 126,   0,   0,   0}},
        {{223, 223, 222, 224, 163, 162, 164, 152, 165, 149, 142, 354, 352, 347, 353, 347, 347, 347, 347, 347, 348, 347, 347, 229,  68, 216, 234, 215, 167, 207, 233, 206}},
        {{232, 232, 232, 166, 166, 166, 231, 231, 230, 235, 235, 235, 236, 235, 235, 235, 235, 235, 235, 103, 238, 237, 153, 168, 169, 123, 347, 345, 111,   0,   0,  61}},
        {{241, 241, 241, 170, 170, 170, 240, 239, 243, 243, 243, 243, 243, 243, 242, 313, 172, 172, 171,   0,   0,   0,   1,   1,   0,   0,   0,   0,   0,   0, 347, 347}},
        {{249, 249, 249, 249, 249, 249, 249, 249, 249, 248, 247, 247, 247, 247, 247, 247, 180, 179, 182, 182, 181, 214, 178, 177, 213, 212, 211, 176, 312, 175, 174,  87}},
        {{269, 268, 266, 189, 188,   0,   0,   0,   0,   0,   0, 270, 269, 267, 191, 190,   0,   0,   0,   0,   0,   0,   0, 273, 272, 271, 193, 192,   0,   0,   0,   0}},
        {{294,   0,   0,  10, 293,  57,  94,   0, 105,   0,   0,   9,  42,   0,  94,   0, 304,   0,   0,  15, 114,   0,  94,   0, 105,   0,   0,   0,  18,  96,   0,   0}},
        {{327, 342,   0,   0, 291, 347,   0, 287,   0,   0,   0,   0,   0,  49, 339,   0,   1, 105,   0, 347, 341,   0,   0,   0,   0,   0,  51, 287,   0,   0,  17, 328}},
        {{334, 338, 311, 358, 358, 358, 357,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 344,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{347,  73, 347,   0,   0,  92,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,  35,  18, 104, 244, 303, 173,   0, 287,   0,   3,   0,   0,   0,   0,  73,  23}},
        {{347, 347, 347, 331, 347, 347, 343,  54,  61,  17, 123,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}}
    }});
    
    inline constexpr std::array<std::array<uint16_t, 16>, 359> case_mapper::lookup::stage3({{
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,  751}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,    0,  751}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,  751,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,  751,  751}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,    0,    0,  751}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,    0,  751,  751}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,  751,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,  751,  751,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,  751,  751,  751}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,    0,  751,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,  751,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,  751,    0,  751,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,    0,  751,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,  751,  751,  751,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751,  751}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,  751,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,  751,  751,  751,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,  751,  751,  751,  751,    0,    0,  751}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751,  751,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751,  751,  751}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,  751,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,  751,    0,    1,    0,    0,  751,    0,  751}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,  751,  751,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,  751,  751,    0,    0,    0,    0,  751,  751}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,  751,  751,  751,    0,    0,    0,    0,  751}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751,  751,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751,  751,  751,  751}},
        {{   0,    0,    0,    0,    0,    0,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,  751,    0,    0,    0,    0,    0,    0,  751,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,  751,  751,    0,    0,  751,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,  751,  751,  751,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751,    0,  751,  751,  751}},
        {{   0,    0,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751,  751,  751,  751,  751}},
        {{   0,    0,    0,    0,    0,    0,  497,  497,  497,  497,  497,  497,  497,  497,  497,  497}},
        {{   0,    0,    0,    0,    0,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,  751,    0,    0,    0,    0,    0,  751,  751,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,  751,    0,    0,  751,  751,  751,  751,  751,  751,  751}},
        {{   0,    0,    0,    0,    0,    0,  751,    0,  751,  751,    0,    0,    0,  751,    0,  751}},
        {{   0,    0,    0,    0,    0,    0,  751,    0,  751,  751,  751,  751,  751,  751,  751,    0}},
        {{   0,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751,  751,  751,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751,  751,  751,  751,    0,    0}},
        {{   0,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751,  751,  751,  751,    0,  751}},
        {{   0,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751,  751,  751,  751,  751,    0}},
        {{   0,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751}},
        {{   0,    0,    0,    0,    0,    1,    1,    1,    1,    1,    0,    0,    0,    0,   62,    0}},
        {{   0,    0,    0,    0,    0,    1,    1,    1,    1,    1,    1,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,  751,    0,    0,  751,    0,    0,    0,    0,  751,    0,    0}},
        {{   0,    0,    0,    0,    0,  751,    0,  751,    0,  751,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,    0,  751,  751,    0,    0,    0,    0,    0,  751,    0,    0,    0}},
        {{   0,    0,    0,    0,    1,    0,  492,    0,    1,    0,  493,  493,    1,    1,    0,    1}},
        {{   0,    0,    0,    0,  455,  750,   11,  454,  749,   10,  453,  748,    9,  452,    9,  452}},
        {{   0,    0,    0,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,  751,    0,    0,    0,  751,  751,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,  751,    0,    0,  751,    0,    0,  751,  751,  751,  751,  751,    0}},
        {{   0,    0,    0,    0,  751,    0,  751,  751,  751,  751,  751,    0,  751,    0,    0,    0}},
        {{   0,    0,    0,    0,  751,  325,    0,  751,  751,    0,    1,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,  751,  751,    0,  751,  751,  751,  751,  751,  751,  751,    0,    0}},
        {{   0,    0,    0,    0,  751,  751,  464,  751,  465,  465,  465,    0,  466,    0,  467,  467}},
        {{   0,    0,    0,    0,  751,  751,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,    0,  751,  751,  751,  751,    0,    0,  751,  751,    0,    0,    0,    0}},
        {{   0,    0,    0,  348,  347,  346,  345,  344,    0,    0,    0,    0,    0,    0,  751,    0}},
        {{   0,    0,    0,  496,   64,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0,  751,    0,    0,    0}},
        {{   0,    0,    0,  751,    0,    0,  751,  751,  751,  751,    0,    0,  751,  751,    0,    0}},
        {{   0,    0,    0,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,  751,  751,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,  751,  751,  751,  751,    0,    0,  751,  751,    0,    0,  751,    0,    0}},
        {{   0,    0,    0,  751,  751,  751,  751,  751,  751,    0,    0,  751,  751,  751,  751,    0}},
        {{   0,    0,    0,  751,  751,  751,  751,  751,  751,    0,  751,    0,    0,    0,    0,  751}},
        {{   0,    0,    0,  751,  751,  751,  751,  751,  751,  751,  751,    0,    0,    0,    0,    0}},
        {{   0,    0,    0,  751,  751,  751,  751,  751,  751,  751,  751,    0,    0,  751,    0,  751}},
        {{   0,    0,    0,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,    0,    0,  751}},
        {{   0,    0,    0,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751}},
        {{   0,    0,    1,    0,    0,    0,    0,    1,    0,    0,    1,    1,    1,    1,    1,    1}},
        {{   0,    0,    1,    0,    0,    1,    1,    0,    0,    1,    1,    1,    1,    0,    1,    1}},
        {{   0,    0,  359,  358,  357,    0,  356,  432,  478,  478,  478,  478,  401,  751,  751,    0}},
        {{   0,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,    0,    0}},
        {{   0,    0,  751,    0,    0,    0,    0,    0,    0,  751,  751,  751,    0,    0,    0,    0}},
        {{   0,    0,  751,    0,    0,    0,  751,    0,    0,    0,    0,  751,    0,    0,    0,    0}},
        {{   0,    0,  751,    0,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,  751,    0,    0,  751,  751,    0,    0,    0,    0,    0,    0,  751,    0,    0}},
        {{   0,    0,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,  751,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,  751,  751,  751,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,  751,  751,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,    0,  751,  751,  751,  751,    0,    0,    0,    0,    0,    0,  751,  751,    0,  751}},
        {{   0,    0,  751,  751,  751,  751,    0,    0,  751,  751,    0,  751,  751,  751,    0,    0}},
        {{   0,    0,  751,  751,  751,  751,    0,  751,  751,  751,  751,  751,    0,    0,    0,    0}},
        {{   0,    0,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751}},
        {{   0,    0,  752,  752,  752,  512,   79,    0,  752,  752,    1,    0,    0,    0,    0,    0}},
        {{   0,  464,  464,  464,  464,  464,  464,  464,  464,  464,  464,  464,  464,  464,  464,  464}},
        {{   0,  474,  474,  474,  474,  474,  474,  474,  474,  474,  474,  474,  474,  474,  474,  474}},
        {{   0,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,  751,    0,    0,  751,  751,  751,  751,  751,  751,  751,    0,    0,    0,    0,    0}},
        {{   0,  751,    0,    0,  751,  751,  751,  751,  751,  751,  751,  751,  751,    0,    0,    0}},
        {{   0,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,  751,  751,    0,    0,    0,    0,  751,  751,    0,    0,  751,  751,  751,    0,    0}},
        {{   0,  751,  751,    0,    0,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,  751,  751,    0,  751,  751,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,  751,  751,  751,    0,  751,  751,    0,    0,    0,    0,    0,  751,  751,  751,  751}},
        {{   0,  751,  751,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,  751,  751,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,  751,    0,    0}},
        {{   0,  751,  751,  751,  751,  751,    0,    0,    0,    0,    0,  751,    0,    0,    0,    0}},
        {{   0,  751,  751,  751,  751,  751,    0,  751,  751,    0,    0,    0,    0,  751,    0,    0}},
        {{   0,  751,  751,  751,  751,  751,  751,    0,    0,    0,  751,    0,  751,  751,    0,  751}},
        {{   0,  751,  751,  751,  751,  751,  751,    0,    0,  751,  751,  751,    0,    0,    0,    0}},
        {{   0,  751,  751,  751,  751,  751,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{   0,  751,  751,  751,  751,  751,  751,  751,  751,    0,    0,    0,    0,  751,    0,    0}},
        {{   0,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,    0,    0,    0,    0,    0}},
        {{   0,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,    0}},
        {{   0,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751}},
        {{   0,  752,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  752}},
        {{   1,    0,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1}},
        {{   1,    1,    1,    0,    1,    1,    1,    1,    1,    1,    1,    1,    0,    0,    0,    0}},
        {{   1,    1,    1,    1,    0,    1,    0,    0,    0,    1,    1,    1,    1,    1,    0,    0}},
        {{   1,    1,    1,    1,    0,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1}},
        {{   1,    1,    1,    1,    1,    0,    1,    0,    0,    0,    1,    1,    1,    1,    1,    1}},
        {{   1,    1,    1,    1,    1,    0,    1,    1,    1,    1,    1,    1,    1,    0,    1,    1}},
        {{   1,    1,    1,    1,    1,    0,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1}},
        {{   1,    1,    1,    1,    1,    1,    0,    0,    1,    1,    1,    1,    1,    1,    1,    1}},
        {{   1,    1,    1,    1,    1,    1,    0,    1,    1,    1,    1,    0,    0,    1,    1,    1}},
        {{   1,    1,    1,    1,    1,    1,    1,    1,    1,    0,    1,    1,    1,    1,    1,    1}},
        {{   1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    0,    0,    0,    0,    0,    0}},
        {{   1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    0,    1,    0,    1,    1,    1}},
        {{   1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    0,    1,    1,    1,    1,    0}},
        {{   1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    0,    1,    1,    1,    1,    1}},
        {{   1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    0,    1,    1,    1,    1}},
        {{   1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    0,    1,    1}},
        {{   1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    0}},
        {{   1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1}},
        {{   1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,   54,    1}},
        {{   1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,  752,  752,  752,  752}},
        {{   1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,  752,  752,  752,  752,  752}},
        {{   1,    1,    1,    1,    1,    1,    1,    1,    1,  752,  751,  751,    0,    0,    0,    0}},
        {{   1,    1,    1,    1,    1,    1,    1,    1,  752,   52,    1,    1,    1,   53,    1,    1}},
        {{   1,    1,    1,   28,    1,    1,    1,    1,    1,    1,    1,  751,  752,  752,  752,  752}},
        {{   1,    1,   26,    1,    0,    1,    1,    1,    1,    1,    1,    1,    1,   27,   27,    1}},
        {{   1,    1,  494,    1,    1,    0,    0,    0,    0,    1,    0,    0,    1,    1,    1,    1}},
        {{   1,    1,  504,   71,  504,   71,  504,   71,  504,   71,  504,   71,  504,   71,  504,   71}},
        {{   1,   20,   20,    1,    1,   21,    1,    1,    1,    1,    1,    1,    1,   22,    1,    1}},
        {{   1,   36,   36,   36,   36,   36,   36,   36,   36,   36,   36,   36,   36,   36,   36,   36}},
        {{   2,    2,    2,    2,    2,    2,    0,    0,  445,  445,  445,  445,  445,  445,    0,    0}},
        {{   2,    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,    2}},
        {{   3,  446,  446,    3,  446,    3,  446,  446,    3,  446,  446,  446,    3,    1,  447,  447}},
        {{   5,    5,    5,    5,    5,    5,    5,    0,    6,    6,    6,    6,    6,    6,    6,    6}},
        {{   5,    5,    5,    5,    5,    5,    5,    5,    5,    5,    5,    5,    5,    5,    5,    5}},
        {{   5,  448,  448,  448,    5,  448,    5,  448,  448,    5,    1,    0,  450,    7,    1,    8}},
        {{   6,  449,    6,  449,    6,  449,    6,  449,    6,  352,  447,    4,  447,    4,  447,    4}},
        {{   9,  452,    9,  452,    9,  452,    9,  452,    9,  452,    9,  452,    9,    9,  452,    9}},
        {{  12,   12,   12,   12,   12,    1,   13,   13,    1,   14,    1,   15,   15,    1,    1,    1}},
        {{  12,  455,   12,  455,  455,  455,  455,   12,  455,   12,  455,   12,  455,   12,  455,   12}},
        {{  16,   16,    1,   17,   17,   17,   17,    1,   18,   18,   18,   18,   18,    1,    1,   19}},
        {{  23,    1,   24,   24,    1,    1,    1,   25,   25,   25,   25,   25,   25,    1,    1,    1}},
        {{  31,   31,   31,   31,   31,   31,   31,   31,   31,   31,   31,   31,   31,   31,   31,   31}},
        {{  34,   34,  331,   33,   33,   33,   33,   33,   33,   33,   33,   33,   33,   33,   33,  460}},
        {{  36,   36,   36,   36,   36,   36,   36,   36,   36,   36,   36,   36,   36,   36,   36,   36}},
        {{  36,   36,   36,   36,   36,   36,   36,  355,    1,    0,    0,    0,    0,    0,    0,    0}},
        {{  37,   37,   37,   37,   37,   37,   37,   37,   37,   37,   37,   37,   37,   37,   37,   37}},
        {{  43,   43,   43,   43,   43,   43,    0,   44,    0,    0,    0,    0,    0,   45,    0,    0}},
        {{  43,   43,   43,   43,   43,   43,   43,   43,   43,   43,   43,   43,   43,   43,   43,   43}},
        {{  46,   46,   46,   46,   46,   46,   46,   46,   46,   46,   46,    0,    0,    0,    0,    0}},
        {{  49,   49,  435,  434,  361,   48,  360,  433,  479,  479,  479,  479,  479,  751,  751,  751}},
        {{  50,   50,  438,  437,    0,    0,  362,  436,  480,  480,  480,  480,    0,  751,  751,  751}},
        {{  51,   51,  370,  369,  368,    0,  367,  440,  482,  482,  482,  482,  403,  751,  341,  751}},
        {{  56,   56,   56,   56,   56,   56,   56,   56,   56,   56,   56,   56,   56,   56,    0,    0}},
        {{  56,   56,   56,   56,   56,   56,   56,   56,  486,  486,  486,  486,  486,  486,  486,  486}},
        {{  58,   58,   58,   58,   58,   58,    0,    0,  488,  488,  488,  488,  488,  488,    0,    0}},
        {{  58,   58,   58,   58,   58,   58,   58,   58,  487,  487,  487,  487,  487,  487,  487,  487}},
        {{  59,   59,   59,   59,   59,   59,    0,    0,  490,  490,  490,  490,  490,  490,    0,    0}},
        {{  59,   59,   59,   59,   59,   59,   59,   59,  489,  489,  489,  489,  489,  489,  489,  489}},
        {{  63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63}},
        {{  65,   65,   65,   65,   65,   65,   65,   65,   65,   65,    0,    0,    0,    0,    0,    0}},
        {{  65,   65,   65,   65,   65,   65,   65,   65,   65,   65,   65,   65,   65,   65,   65,   65}},
        {{  66,   66,   66,   66,   66,   66,   66,   66,   66,   66,   66,    0,  752,   67,   67,   67}},
        {{  66,   66,   66,   66,   66,   66,   66,   66,   66,   66,   66,   66,   66,   66,   66,   66}},
        {{  95,   94,   93,   92,   91,   90,   89,   88,   87,   86,   85,   84,   83,   82,   81,   80}},
        {{ 111,  110,  109,  108,  107,  106,  105,  104,  103,  102,  101,  100,   99,   98,   97,   96}},
        {{ 119,  118,  117,  116,  115,  114,  113,  112,  111,  110,  109,  108,    0,    0,    0,    0}},
        {{ 135,  134,  133,  132,  131,  130,  129,  128,  127,  126,  125,  124,  123,  122,  121,  120}},
        {{ 152,  151,    0,  151,  150,  149,  148,  147,  146,  145,    0,  145,  144,    0,    0,    0}},
        {{ 166,  165,    0,  165,  164,  163,  162,  161,  160,  159,  158,  157,  156,  155,  154,  153}},
        {{ 179,  178,  177,  176,  751,  751,  751,  751,  751,  751,  751,  751,    0,    0,    0,    0}},
        {{ 195,  194,  193,  192,  191,  190,  189,  188,  187,  186,  185,  184,  183,  182,  181,  180}},
        {{ 225,  224,  223,  222,  221,  220,  219,  218,  217,  216,  215,  214,  213,  212,  211,  210}},
        {{ 241,  240,  239,  238,  237,  236,  235,  234,  233,  232,  231,  230,  229,  228,  227,  226}},
        {{ 244,  243,  242,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 260,  259,  258,  257,  256,  255,  254,  253,  252,  251,  250,  249,  248,  247,  246,  245}},
        {{ 276,  275,  274,  273,  272,  271,  270,  269,  268,  267,  266,  265,  264,  263,  262,  261}},
        {{ 292,  291,  290,  289,  288,  287,  286,  285,  284,  283,  282,  281,  280,  279,  278,  277}},
        {{ 304,  303,  302,  301,  300,  299,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 308,  307,  306,  305,  304,  303,  302,  301,  300,  299,  298,  297,  296,  295,  294,  293}},
        {{ 320,  319,  318,  317,  316,  315,  314,  313,  312,  311,  310,  309,  308,  307,  306,  305}},
        {{ 324,  323,  322,  321,  320,  319,  318,  317,  316,  315,  314,  313,  312,  311,  310,  309}},
        {{ 329,  328,   30,   30,  457,  327,    0,  457,   30,  457,  457,   30,    1,  458,  458,  458}},
        {{ 330,  329,    1,    1,    1,  331,  330,   32,  459,   32,  459,   32,  459,   32,  459,   32}},
        {{ 340,  339,  338,  337,  336,  335,  334,  333,  332,  475,   47,    0,    0,    0,    0,    0}},
        {{ 351,  350,  349,  429,  428,  346,  345,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 354,  449,  747,    5,  448,    5,  448,  448,  448,    5,  448,    5,  448,    5,  448,    5}},
        {{ 378,  377,  376,  375,  374,  373,  372,  371,  411,  410,  409,  408,  407,  406,  405,  404}},
        {{ 386,  385,  384,  383,  382,  381,  380,  379,  419,  418,  417,  416,  415,  414,  413,  412}},
        {{ 394,  393,  392,  391,  390,  389,  388,  387,  427,  426,  425,  424,  423,  422,  421,  420}},
        {{ 397,   60,  443,   58,  442,   57,  441,   55,    0,  483,    0,  484,    0,  485,    0,  486}},
        {{ 430,   34,   34,   34,   34,   34,   34,   34,   34,   34,   34,   34,   34,   34,   34,   34}},
        {{ 431,  462,  462,  462,  462,  462,  462,  462,  462,  462,  462,  462,  462,  462,  462,  462}},
        {{ 444,  444,  444,  444,  444,  444,  444,  444,  444,  444,  444,  444,  444,  444,  444,  444}},
        {{ 447,    4,  447,    4,  447,    4,  447,    4,  447,    4,  447,    4,  447,    4,  447,    4}},
        {{ 447,    4,  447,    4,  447,    4,  447,    4,  447,  447,    4,  447,    4,  447,    4,  326}},
        {{ 447,    4,  447,    4,  447,    4,  447,  447,    4,  447,    1,    1,  448,    5,  448,  448}},
        {{ 447,  447,    4,  447,  447,    4,  447,  447,  447,    4,    4,    4,  447,  447,    4,  447}},
        {{ 448,    1,  449,    6,  449,    6,  449,    6,  449,    6,  449,    6,  449,    6,  449,    6}},
        {{ 448,    5,  448,    5,  448,    5,  448,    5,  448,    5,  448,    5,  448,    5,  448,    5}},
        {{ 449,    6,  449,    6,    1,    1,    1,    1,    1,    1,  455,  455,   12,  455,  455,   12}},
        {{ 449,    6,  449,    6,  449,    6,  449,    6,  449,    6,  449,    6,  449,    6,  449,    6}},
        {{ 450,  450,  450,  450,  450,  450,  450,    0,  451,  451,  451,  451,  451,  451,  451,  353}},
        {{ 450,  450,  450,  450,  450,  450,  450,  450,  450,  450,  450,  450,  450,  450,  450,  450}},
        {{ 452,    9,  452,    9,  452,    9,  452,    9,  452,    9,  452,    9,  452,    9,  452,    9}},
        {{ 456,   29,  456,   29,  751,  751,  457,   30,    0,    0,  752,   33,   33,   33,    0,  461}},
        {{ 458,   31,    0,  751,  751,  751,  751,  751,  751,  751,  463,   35,  463,   35,  463,   35}},
        {{ 458,   31,  458,   31,  458,   31,  458,   31,  458,   31,  458,   31,  458,   31,  458,   31}},
        {{ 458,  458,  458,  458,  458,  458,  458,  458,  458,  458,  458,  458,  458,  458,  458,  458}},
        {{ 459,   32,  459,   32,  459,   32,  459,   32,  459,   32,  459,   32,  459,   32,  459,   32}},
        {{ 462,  462,    0,  463,  463,  463,  463,  463,  463,  463,  463,  463,   35,   35,   35,   35}},
        {{ 463,   35,  463,   35,  463,   35,  463,   35,  463,   35,  463,   35,  463,   35,  463,   35}},
        {{ 463,  463,   35,  463,   35,  463,   35,  463,   35,  463,   35,  463,   35,  463,   35,   35}},
        {{ 464,  464,  464,  464,  464,  464,  464,    0,    0,  751,    0,    0,    0,    0,    0,  751}},
        {{ 464,  464,  464,  464,  464,  464,  464,  464,  464,  464,  464,  464,  464,  464,  464,  464}},
        {{ 468,    1,  469,   38,    1,  470,   39,    1,    1,    1,    1,    1,  752,  752,  471,  471}},
        {{ 468,   37,  468,  468,  468,   37,   37,  468,   37,  468,   37,  468,   37,  468,  468,  468}},
        {{ 468,  468,  468,  468,  468,  468,  468,  468,  468,  468,  468,  468,  468,  468,  468,  468}},
        {{ 471,   40,  471,   40,    1,    0,    0,    0,    0,    0,    0,  472,   41,  472,   41,  751}},
        {{ 471,   40,  471,   40,  471,   40,  471,   40,  471,   40,  471,   40,  471,   40,  471,   40}},
        {{ 474,  474,  474,  474,  474,  474,  474,  474,  474,  474,  474,    0,    0,    0,  751,    0}},
        {{ 476,  476,  476,  476,  476,  476,  476,  476,  476,  476,  476,    0,    0,  477,  477,  477}},
        {{ 476,  476,  476,  476,  476,  476,  476,  476,  476,  476,  476,  476,  476,  476,  476,  476}},
        {{ 487,   58,  487,   58,  487,   58,  487,   58,  487,   58,  487,   58,  487,   58,  487,   58}},
        {{ 491,   61,  491,   61,  491,   61,  400,  399,  398,  396,  395,  342,    1,    1,  343,    1}},
        {{ 491,   61,  491,   61,  491,   61,  491,   61,  491,   61,  491,   61,  491,   61,  491,   61}},
        {{ 495,  495,  495,  495,  495,  495,  495,  495,  495,  495,  495,  495,  495,  495,  495,  495}},
        {{ 497,  497,  497,  497,  497,  497,  497,  497,  497,  497,  497,  497,  497,  497,  497,  497}},
        {{ 498,  498,  498,  498,  498,  498,    0,  499,    0,    0,    0,    0,    0,  500,    0,    0}},
        {{ 498,  498,  498,  498,  498,  498,  498,  498,  498,  498,  498,  498,  498,  498,  498,  498}},
        {{ 501,   68,  501,   68,  501,   68,  501,   68,  501,   68,  501,   68,  501,   68,    0,  751}},
        {{ 501,   68,  501,   68,  501,   68,  501,   68,  501,   68,  501,   68,  501,   68,  501,   68}},
        {{ 502,   69,  502,   69,  502,   69,  502,   69,  502,   69,  502,   69,  502,   69,  502,   69}},
        {{ 502,   69,  502,   69,  502,   69,  502,   69,  502,   69,  502,   69,  752,  752,  751,  751}},
        {{ 504,   71,  504,   71,  504,   71,  504,   71,  504,   71,  504,   71,  504,   71,  504,   71}},
        {{ 505,   72,  505,   72,  505,   72,  505,   72,  751,  751,  751,  506,   73,  506,    1,    0}},
        {{ 507,   74,  507,   74,   74,    1,  508,   75,  508,   75,  508,   75,  508,   75,  508,   75}},
        {{ 508,   75,  508,   75,  508,   75,  508,   75,  508,   75,  508,  508,  508,  508,  508,    1}},
        {{ 509,   76,  509,   76,  509,  509,  509,  509,   76,  509,   76,  509,  509,   76,    0,    0}},
        {{ 509,  509,  509,  509,  509,   76,  509,   76,  509,   76,  509,   76,  509,   76,  509,   76}},
        {{ 510,   77,    0,    1,    0,    1,  511,   78,  511,   78,  511,   78,  511,    0,    0,    0}},
        {{ 513,    5,  448,    5,  448,    5,  448,    5,    1,  449,    6,  449,    6,  449,    6,  449}},
        {{ 521,  520,  519,  518,  517,  516,  515,  514,  119,  118,  117,  116,  115,  114,  113,  112}},
        {{ 537,  536,  535,  534,    0,    0,    0,    0,  143,  142,  141,  140,  139,  138,  137,  136}},
        {{ 537,  536,  535,  534,  533,  532,  531,  530,  529,  528,  527,  526,  525,  524,  523,  522}},
        {{ 553,  552,  551,  550,  549,  548,  547,  546,  545,  544,  543,  542,  541,  540,  539,  538}},
        {{ 569,  568,  567,  566,  565,  564,  563,  562,  561,  560,  559,  558,  557,  556,  555,  554}},
        {{ 573,  572,  571,    0,  571,  570,    0,  175,  174,  173,  172,  171,  170,  169,  168,  167}},
        {{ 587,  586,  585,  584,  583,  582,  581,  580,  579,  578,  577,    0,  577,  576,  575,  574}},
        {{ 601,  600,  599,  598,  597,  596,  595,  594,  593,  592,  591,    0,  591,  590,  589,  588}},
        {{ 603,  602,  209,  208,  207,  206,  205,  204,  203,  202,  201,  200,  199,  198,  197,  196}},
        {{ 619,  618,  617,  616,  615,  614,  613,  612,  611,  610,  609,  608,  607,  606,  605,  604}},
        {{ 635,  634,  633,  632,  631,  630,  629,  628,  627,  626,  625,  624,  623,  622,  621,  620}},
        {{ 651,  650,  649,  648,  647,  646,  645,  644,  643,  642,  641,  640,  639,  638,  637,  636}},
        {{ 667,  666,  665,  664,  663,  662,  661,  660,  659,  658,  657,  656,  655,  654,  653,  652}},
        {{ 670,  669,  668,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 686,  685,  684,  683,  682,  681,  680,  679,  678,  677,  676,  675,  674,  673,  672,  671}},
        {{ 702,  701,  700,  699,  698,  697,  696,  695,  694,  693,  692,  691,  690,  689,  688,  687}},
        {{ 706,  705,  704,  703,  702,  701,    0,    0,    0,  751,  751,  751,  751,  751,    0,  751}},
        {{ 718,  717,  716,  715,  714,  713,  712,  711,  710,  709,  708,  707,  706,  705,  704,  703}},
        {{ 722,  721,  720,  719,  718,  717,  716,  715,  714,  713,  712,  711,  710,  709,  708,  707}},
        {{ 730,  729,  728,  727,  726,  725,  724,  723,  722,  721,  720,  719,  718,  717,  716,  715}},
        {{ 746,  745,  744,  743,  742,  741,  740,  739,  738,  737,  736,  735,  734,  733,  732,  731}},
        {{ 751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,    0}},
        {{ 751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,    0,    0}},
        {{ 751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,    0,    0,    0}},
        {{ 751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751}},
        {{ 751,    0,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751,  751,  751,  751,  751}},
        {{ 751,    0,    0,    0,    0,    0,  751,  751,  751,    0,  751,  751,  751,  751,    0,    0}},
        {{ 751,    0,    0,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,    0,    0,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751}},
        {{ 751,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,    0,  751,    0,    0,  751,  751,  751,  751,  751,  751,  751,  751,    0,    0,    0}},
        {{ 751,    0,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,    0,  751,  751,    0,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,    0,  751,  751,  751,    0,    0,  751,  751,    0,    0,    0,    0,    0,  751,  751}},
        {{ 751,    0,  751,  751,  751,  751,  751,  751,    0,  751,  751,    0,    0,  751,  751,    0}},
        {{ 751,    0,  751,  751,  751,  751,  751,  751,  751,    0,    0,    0,    0,  751,    0,    0}},
        {{ 751,   46,   46,   46,   46,   46,   46,   46,   46,   46,   46,   46,   46,   46,   46,   46}},
        {{ 751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  751}},
        {{ 751,  751,    0,    0,    0,    0,    0,  751,  751,  751,  751,  751,  751,  751,  751,  751}},
        {{ 751,  751,    0,    0,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,  751,    0,    0,    0,  751,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,  751,    0,    0,  751,    0,  751,  751,    0,    0,    0,    0,    0,    0,  751,    0}},
        {{ 751,  751,    0,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,  751,    0,  751,  751,    0,  751,  751,  751,  751,  751,    0,    0,    0,    0,    0}},
        {{ 751,  751,  366,  365,  364,    0,  363,  439,  481,  481,  481,  481,  402,  751,  751,  751}},
        {{ 751,  751,  473,   42,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,  751,  503,   70,  503,   70,  503,   70,  503,   70,  503,   70,  503,   70,  503,   70}},
        {{ 751,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,  751,  751,    0,    0,    0,    0,  751,  751,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,  751,  751,    0,    0,  751,  751,  751,  751,  751,  751,  751,    0,    0,    0,    0}},
        {{ 751,  751,  751,    0,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,    0,  751}},
        {{ 751,  751,  751,    0,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751}},
        {{ 751,  751,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,  751,  751,  751,    0,    0,    0,  751,    0,    0,    0,    0,    0,  751,    0,    0}},
        {{ 751,  751,  751,  751,    0,    0,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,  751,  751,  751,    0,  751,  751,  751,  751,  751,  751,  751,    0,  751,  751,    0}},
        {{ 751,  751,  751,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,  751,  751,  751,  751,    0,  751,  751,    0,    0,    0,    0,    0,  751,  751,  751}},
        {{ 751,  751,  751,  751,  751,    0,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751}},
        {{ 751,  751,  751,  751,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,  751,  751,  751,  751,  751,    0,    0,    0,    0,  751,    0,    0,  751,    0,    0}},
        {{ 751,  751,  751,  751,  751,  751,    0,  751,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,  751,  751,  751,  751,  751,  751,    0,    0,    0,    0,    0,    0,    0,    0,    0}},
        {{ 751,  751,  751,  751,  751,  751,  751,    0,    0,    0,    0,  751,  751,  751,  751,  751}},
        {{ 751,  751,  751,  751,  751,  751,  751,    0,  751,  751,    0,    0,    0,    0,    0,    0}},
        {{ 751,  751,  751,  751,  751,  751,  751,    0,  751,  751,  751,  751,  751,  751,    0,  751}},
        {{ 751,  751,  751,  751,  751,  751,  751,    0,  751,  751,  751,  751,  751,  751,  751,  751}},
        {{ 751,  751,  751,  751,  751,  751,  751,  751,    0,    0,  751,  751,  751,  751,  751,  751}},
        {{ 751,  751,  751,  751,  751,  751,  751,  751,    0,  751,  751,    0,    0,    0,    0,    0}},
        {{ 751,  751,  751,  751,  751,  751,  751,  751,    0,  751,  751,  751,  751,  751,  751,  751}},
        {{ 751,  751,  751,  751,  751,  751,  751,  751,  751,    0,    0,  751,  751,  751,  751,  751}},
        {{ 751,  751,  751,  751,  751,  751,  751,  751,  751,    0,  751,  751,  751,  751,    0,    0}},
        {{ 751,  751,  751,  751,  751,  751,  751,  751,  751,  751,    0,    0,    0,  751,  751,  751}},
        {{ 751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,    0,    0,    0,    0,    0}},
        {{ 751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,    0,  751,    0,    0,    0}},
        {{ 751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,    0,    0,    0}},
        {{ 751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,    0,    0}},
        {{ 751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,    0,  751}},
        {{ 751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,    0}},
        {{ 751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751}},
        {{ 751,  751,  751,  751,  751,  753,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751}},
        {{ 752,    0,  752,  752,  752,  752,  752,  752,  752,  752,  752,    0,    0,    0,    0,    0}},
        {{ 752,    1,    1,    1,    1,    1,    1,    1,    1,  505,   72,  505,   72,  505,  505,   72}},
        {{ 752,  751,  751,  752,  752,  752,    0,  752,  752,  752,  752,  752,  752,  752,  752,  752}},
        {{ 752,  752,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751}},
        {{ 752,  752,  752,  752,  752,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751,  751}},
        {{ 752,  752,  752,  752,  752,  752,  752,  752,  752,  751,  751,  751,  751,  751,  751,  751}},
        {{ 752,  752,  752,  752,  752,  752,  752,  752,  752,  752,  752,    1,    1,    1,    1,    1}},
        {{ 752,  752,  752,  752,  752,  752,  752,  752,  752,  752,  752,  752,  752,    0,    0,    0}},
        {{ 752,  752,  752,  752,  752,  752,  752,  752,  752,  752,  752,  752,  752,  752,    0,    0}},
        {{ 752,  752,  752,  752,  752,  752,  752,  752,  752,  752,  752,  752,  752,  752,  752,  752}}
    }});
    
    inline constexpr std::array<uint32_t, 754> case_mapper::lookup::stage4({{
        0x00000000, 0x10000000, 0x10010F5C, 0x10100085, 0x10100086, 0x10100088, 0x10100089, 0x1010008A, 0x1010008B, 0x1010008C, 
        0x1010008D, 0x1010008E, 0x1010008F, 0x10100090, 0x10100091, 0x10100092, 0x10100095, 0x10100096, 0x10100097, 0x10100099, 
        0x1010009A, 0x1010009C, 0x101000A3, 0x101000A5, 0x101000A6, 0x101000A9, 0x101000AE, 0x101000B8, 0x10100111, 0x10100187, 
        0x10100189, 0x1010018A, 0x1010018B, 0x1010018C, 0x1010018D, 0x10100192, 0x1010019D, 0x101003B6, 0x101003B7, 0x101003B8, 
        0x101003BF, 0x101003C6, 0x101003C9, 0x101003D5, 0x101003D6, 0x101003DB, 0x10100460, 0x101007D7, 0x10100825, 0x10100832, 
        0x1010083E, 0x10100860, 0x10100897, 0x1010089A, 0x101008AA, 0x10100902, 0x10100906, 0x10100907, 0x1010090C, 0x10100910, 
        0x10100911, 0x1010091B, 0x1010095B, 0x1010096C, 0x1010096F, 0x10100CA0, 0x10100CBA, 0x10100CBC, 0x10100CED, 0x10100CFF, 
        0x10100D85, 0x10100D87, 0x10100D90, 0x10100D93, 0x10100D95, 0x10100D96, 0x10100D97, 0x10100D99, 0x10100D9D, 0x10100DB5, 
        0x102008B6, 0x102008B7, 0x102008B8, 0x102008B9, 0x102008BA, 0x102008BB, 0x102008BC, 0x102008BD, 0x102008BE, 0x102008BF, 
        0x102008C0, 0x102008C1, 0x102008C2, 0x102008C3, 0x102008C4, 0x102008C5, 0x102008C6, 0x102008C7, 0x102008C8, 0x102008C9, 
        0x102008CA, 0x102008CB, 0x102008CC, 0x102008CD, 0x102008CE, 0x102008CF, 0x102008D0, 0x102008D1, 0x102008D2, 0x102008D3, 
        0x102008D4, 0x102008D5, 0x102008D6, 0x102008D7, 0x102008D8, 0x102008D9, 0x102008DA, 0x102008DB, 0x102008DC, 0x102008DD, 
        0x102008DE, 0x102008DF, 0x102008E0, 0x102008E1, 0x102008E2, 0x102008E3, 0x102008E4, 0x102008E5, 0x102008E6, 0x102008E7, 
        0x102008E8, 0x102008E9, 0x102008EA, 0x102008EB, 0x102008EC, 0x102008ED, 0x102008EE, 0x102008EF, 0x102008F0, 0x102008F1, 
        0x102008F2, 0x102008F3, 0x102008F4, 0x102008F5, 0x10200907, 0x10200908, 0x10200909, 0x1020090A, 0x1020090B, 0x1020090C, 
        0x1020090D, 0x1020090E, 0x1020090F, 0x10200910, 0x10200911, 0x10200912, 0x10200913, 0x10200914, 0x10200915, 0x10200916, 
        0x10200917, 0x10200918, 0x10200919, 0x1020091A, 0x1020091B, 0x1020091C, 0x1020091D, 0x1020091E, 0x1020091F, 0x10200920, 
        0x10200921, 0x10200922, 0x10200923, 0x10200924, 0x10200925, 0x10200926, 0x102009E2, 0x102009E3, 0x102009E4, 0x102009E5, 
        0x102009E6, 0x102009E7, 0x102009E8, 0x102009E9, 0x102009EA, 0x102009EB, 0x102009EC, 0x102009ED, 0x102009EE, 0x102009EF, 
        0x102009F0, 0x102009F1, 0x102009F2, 0x102009F3, 0x102009F4, 0x102009F5, 0x102009F6, 0x102009F7, 0x102009F8, 0x102009F9, 
        0x102009FA, 0x102009FB, 0x102009FC, 0x102009FD, 0x102009FE, 0x102009FF, 0x10200A00, 0x10200A01, 0x10200A02, 0x10200A03, 
        0x10200A86, 0x10200A87, 0x10200A88, 0x10200A89, 0x10200A8A, 0x10200A8B, 0x10200A8C, 0x10200A8D, 0x10200A8E, 0x10200A8F, 
        0x10200A90, 0x10200A91, 0x10200A92, 0x10200A93, 0x10200A94, 0x10200A95, 0x10200A96, 0x10200A97, 0x10200A98, 0x10200A99, 
        0x10200A9A, 0x10200A9B, 0x10200A9C, 0x10200A9D, 0x10200A9E, 0x10200A9F, 0x10200AA0, 0x10200AA1, 0x10200AA2, 0x10200AA3, 
        0x10200AA4, 0x10200AA5, 0x10200F71, 0x10200F72, 0x10200F73, 0x10200F74, 0x10200F75, 0x10200F76, 0x10200F77, 0x10200F78, 
        0x10200F79, 0x10200F7A, 0x10200F7B, 0x10200F7C, 0x10200F7D, 0x10200F7E, 0x10200F7F, 0x10200F80, 0x10200F81, 0x10200F82, 
        0x10200F83, 0x10200F84, 0x10200F85, 0x10200F86, 0x10200F87, 0x10200F88, 0x10200F89, 0x10200F8A, 0x10200F8B, 0x10200F8C, 
        0x10200F8D, 0x10200F8E, 0x10200F8F, 0x10200F90, 0x10200F91, 0x10200F92, 0x10200F93, 0x10200F94, 0x10200F95, 0x10200F96, 
        0x10200F97, 0x10200F98, 0x10200F99, 0x10200F9A, 0x10200F9B, 0x10200F9C, 0x10200F9D, 0x10200F9E, 0x10200F9F, 0x10200FA0, 
        0x10200FA1, 0x10200FA2, 0x10200FA3, 0x10200FA6, 0x10200FA7, 0x10200FA8, 0x10200FA9, 0x10200FAA, 0x10200FAB, 0x10200FAC, 
        0x10200FAD, 0x10200FAE, 0x10200FAF, 0x10200FB0, 0x10200FB1, 0x10200FB2, 0x10200FB3, 0x10200FB4, 0x10200FB5, 0x10200FB6, 
        0x10200FB7, 0x10200FB8, 0x10200FB9, 0x10200FBA, 0x10200FBB, 0x10200FBC, 0x10200FBD, 0x10200FBE, 0x10200FBF, 0x10200FC0, 
        0x10200FC1, 0x10200FC2, 0x10200FC3, 0x10200FC4, 0x10200FC5, 0x11100081, 0x11100086, 0x11100189, 0x1110018A, 0x1110018B, 
        0x1110018C, 0x1110018D, 0x111007D8, 0x111007D9, 0x111007DA, 0x111007DB, 0x111007DC, 0x111007DD, 0x111007DE, 0x111007DF, 
        0x111007E0, 0x1110084D, 0x1110090C, 0x1201090D, 0x12200054, 0x12200057, 0x1220005A, 0x1220005D, 0x12200060, 0x12200067, 
        0x1220006A, 0x1220006D, 0x12200089, 0x1220008B, 0x1220008C, 0x1220019D, 0x1220081A, 0x1220081C, 0x1220081F, 0x12200822, 
        0x12200825, 0x12200828, 0x12200836, 0x12200847, 0x12200849, 0x1220084C, 0x1220084F, 0x12200858, 0x1220085A, 0x1220085D, 
        0x12200860, 0x12200883, 0x12200886, 0x12200889, 0x1220088C, 0x1220088F, 0x12200892, 0x12200895, 0x12200898, 0x122008BB, 
        0x122008BE, 0x122008C1, 0x122008C4, 0x122008C7, 0x122008CA, 0x122008CD, 0x122008D0, 0x122008F3, 0x122008F6, 0x122008F9, 
        0x122008FC, 0x122008FF, 0x12200902, 0x12200905, 0x12200908, 0x1220090F, 0x12200912, 0x12200914, 0x12200915, 0x12200918, 
        0x1220091B, 0x12210812, 0x1221083F, 0x12210850, 0x12210864, 0x12210868, 0x1221086C, 0x12210870, 0x12210874, 0x12210878, 
        0x1221087C, 0x12210880, 0x1221089C, 0x122108A0, 0x122108A4, 0x122108A8, 0x122108AC, 0x122108B0, 0x122108B4, 0x122108B8, 
        0x122108D4, 0x122108D8, 0x122108DC, 0x122108E0, 0x122108E4, 0x122108E8, 0x122108EC, 0x122108F0, 0x1330005F, 0x13300064, 
        0x13300192, 0x13300196, 0x13300817, 0x13300822, 0x1330082D, 0x13300832, 0x13300833, 0x13300839, 0x1330083E, 0x13300844, 
        0x13300855, 0x13300907, 0x1330090C, 0x13300911, 0x1E10012D, 0x1E100F5E, 0x1F010085, 0x1F010086, 0x1F010088, 0x1F010089, 
        0x1F01008A, 0x1F01008B, 0x1F01008C, 0x1F01008D, 0x1F01008E, 0x1F01008F, 0x1F010187, 0x1F010189, 0x1F01018A, 0x1F01018B, 
        0x1F01018C, 0x1F01018D, 0x1F010191, 0x1F010192, 0x1F010193, 0x1F010194, 0x1F010195, 0x1F010196, 0x1F0103B6, 0x1F0103B7, 
        0x1F0103B8, 0x1F0103BF, 0x1F0103C6, 0x1F0103C9, 0x1F01045A, 0x1F0107D7, 0x1F0107DC, 0x1F0107DE, 0x1F010812, 0x1F01081D, 
        0x1F01082E, 0x1F01083F, 0x1F010850, 0x1F010903, 0x1F010904, 0x1F010905, 0x1F010906, 0x1F01090C, 0x1F01090E, 0x1F010910, 
        0x1F010912, 0x1F01091B, 0x1F010937, 0x1F01093A, 0x1F010940, 0x1F01096C, 0x1F01096F, 0x1F010CA0, 0x1F010CB2, 0x1F010CB3, 
        0x1F010CB8, 0x1F010CED, 0x1F010CFF, 0x1F010D85, 0x1F010D87, 0x1F010D90, 0x1F010D93, 0x1F010D95, 0x1F010D96, 0x1F010D97, 
        0x1F010D99, 0x1F010D9D, 0x1F010DB5, 0x1F020089, 0x1F0208DE, 0x1F0208DF, 0x1F0208E0, 0x1F0208E1, 0x1F0208E2, 0x1F0208E3, 
        0x1F0208E4, 0x1F0208E5, 0x1F0208E6, 0x1F0208E7, 0x1F0208E8, 0x1F0208E9, 0x1F0208EA, 0x1F0208EB, 0x1F0208EC, 0x1F0208ED, 
        0x1F0208EE, 0x1F0208EF, 0x1F0208F0, 0x1F0208F1, 0x1F0208F2, 0x1F0208F3, 0x1F0208F4, 0x1F0208F5, 0x1F0208F6, 0x1F0208F7, 
        0x1F0208F8, 0x1F0208F9, 0x1F0208FA, 0x1F0208FB, 0x1F0208FC, 0x1F0208FD, 0x1F0208FE, 0x1F0208FF, 0x1F020900, 0x1F020901, 
        0x1F020902, 0x1F020903, 0x1F020904, 0x1F020905, 0x1F020906, 0x1F020907, 0x1F020908, 0x1F020909, 0x1F02090A, 0x1F02090B, 
        0x1F02090C, 0x1F02090D, 0x1F02090E, 0x1F02090F, 0x1F020910, 0x1F020911, 0x1F020912, 0x1F020913, 0x1F020914, 0x1F020915, 
        0x1F020926, 0x1F020927, 0x1F020928, 0x1F020929, 0x1F02092A, 0x1F02092B, 0x1F02092C, 0x1F02092D, 0x1F02092E, 0x1F02092F, 
        0x1F020930, 0x1F020931, 0x1F020932, 0x1F020933, 0x1F020934, 0x1F020935, 0x1F020936, 0x1F020937, 0x1F020938, 0x1F020939, 
        0x1F02093A, 0x1F02093B, 0x1F02093C, 0x1F02093D, 0x1F02093E, 0x1F02093F, 0x1F020940, 0x1F020941, 0x1F020942, 0x1F020943, 
        0x1F020944, 0x1F020945, 0x1F020A04, 0x1F020A05, 0x1F020A06, 0x1F020A07, 0x1F020A08, 0x1F020A09, 0x1F020A0A, 0x1F020A0B, 
        0x1F020A0C, 0x1F020A0D, 0x1F020A0E, 0x1F020A0F, 0x1F020A10, 0x1F020A11, 0x1F020A12, 0x1F020A13, 0x1F020A14, 0x1F020A15, 
        0x1F020A16, 0x1F020A17, 0x1F020A18, 0x1F020A19, 0x1F020A1A, 0x1F020A1B, 0x1F020A1C, 0x1F020A1D, 0x1F020A1E, 0x1F020A1F, 
        0x1F020A20, 0x1F020A21, 0x1F020A22, 0x1F020A23, 0x1F020A24, 0x1F020A25, 0x1F020AA6, 0x1F020AA7, 0x1F020AA8, 0x1F020AA9, 
        0x1F020AAA, 0x1F020AAB, 0x1F020AAC, 0x1F020AAD, 0x1F020AAE, 0x1F020AAF, 0x1F020AB0, 0x1F020AB1, 0x1F020AB2, 0x1F020AB3, 
        0x1F020AB4, 0x1F020AB5, 0x1F020AB6, 0x1F020AB7, 0x1F020AB8, 0x1F020AB9, 0x1F020ABA, 0x1F020ABB, 0x1F020ABC, 0x1F020ABD, 
        0x1F020ABE, 0x1F020ABF, 0x1F020AC0, 0x1F020AC1, 0x1F020AC2, 0x1F020AC3, 0x1F020AC4, 0x1F020AC5, 0x1F020F97, 0x1F020F98, 
        0x1F020F99, 0x1F020F9A, 0x1F020F9B, 0x1F020F9C, 0x1F020F9D, 0x1F020F9E, 0x1F020F9F, 0x1F020FA0, 0x1F020FA1, 0x1F020FA2, 
        0x1F020FA3, 0x1F020FA4, 0x1F020FA5, 0x1F020FA6, 0x1F020FA7, 0x1F020FA8, 0x1F020FA9, 0x1F020FAA, 0x1F020FAB, 0x1F020FAC, 
        0x1F020FAD, 0x1F020FAE, 0x1F020FAF, 0x1F020FB0, 0x1F020FB1, 0x1F020FB2, 0x1F020FB3, 0x1F020FB4, 0x1F020FB5, 0x1F020FB6, 
        0x1F020FB7, 0x1F020FB8, 0x1F020FB9, 0x1F020FBA, 0x1F020FBB, 0x1F020FBC, 0x1F020FBD, 0x1F020FBE, 0x1F020FBF, 0x1F020FC0, 
        0x1F020FC1, 0x1F020FC2, 0x1F020FC3, 0x1F020FC4, 0x1F020FC5, 0x1F020FC6, 0x1F020FC7, 0x1F020FC8, 0x1F020FC9, 0x1F020FCA, 
        0x1F020FCB, 0x1F020FCC, 0x1F020FCD, 0x1F020FCE, 0x1F020FCF, 0x1F020FD0, 0x1F020FD1, 0x1F020FD2, 0x1F020FD3, 0x1F020FD4, 
        0x1F020FD5, 0x1F020FD6, 0x1F020FD7, 0x1F020FD8, 0x1F020FD9, 0x1F020FDA, 0x1F020FDB, 0x1F020FDC, 0x1F020FDD, 0x1F020FDE, 
        0x1F020FDF, 0x1F020FE0, 0x1F020FE1, 0x1F020FE2, 0x1F020FE3, 0x1F020FE4, 0x1F020FE5, 0x1F110089, 0x1F11008D, 0x1F11008E, 
        0x1F11008F, 0x20000000, 0x30000000, 0x3110015E
    }});
    

    inline constexpr uint32_t normalizer::compositions[] = {
        0x2000003B, 0x0000003C, 0x20200338, 0x0000226E, 0x0000003D, 0x20200338, 0x00002260, 0x0000003E, 0x20200338, 0x0000226F, 
        0x00000041, 0x1CC00300, 0x000000C0, 0x1CC00301, 0x000000C1, 0x1CC00302, 0x000000C2, 0x1CC00303, 0x000000C3, 0x1CC00308, 
        0x000000C4, 0x1CC0030A, 0x000000C5, 0x1CC00304, 0x00000100, 0x1CC00306, 0x00000102, 0x19400328, 0x00000104, 0x1CC0030C, 
        0x000001CD, 0x1CC0030F, 0x00000200, 0x1CC00311, 0x00000202, 0x1CC00307, 0x00000226, 0x1B800325, 0x00001E00, 0x1B800323, 
        0x00001EA0, 0x3CC00309, 0x00001EA2, 0x00000042, 0x1CC00307, 0x00001E02, 0x1B800323, 0x00001E04, 0x3B800331, 0x00001E06, 
        0x00000043, 0x19400327, 0x000000C7, 0x1CC00301, 0x00000106, 0x1CC00302, 0x00000108, 0x1CC00307, 0x0000010A, 0x3CC0030C, 
        0x0000010C, 0x00000044, 0x1CC0030C, 0x0000010E, 0x1CC00307, 0x00001E0A, 0x1B800323, 0x00001E0C, 0x1B800331, 0x00001E0E, 
        0x19400327, 0x00001E10, 0x3B80032D, 0x00001E12, 0x00000045, 0x1CC00300, 0x000000C8, 0x1CC00301, 0x000000C9, 0x1CC00302, 
        0x000000CA, 0x1CC00308, 0x000000CB, 0x1CC00304, 0x00000112, 0x1CC00306, 0x00000114, 0x1CC00307, 0x00000116, 0x19400328, 
        0x00000118, 0x1CC0030C, 0x0000011A, 0x1CC0030F, 0x00000204, 0x1CC00311, 0x00000206, 0x19400327, 0x00000228, 0x1B80032D, 
        0x00001E18, 0x1B800330, 0x00001E1A, 0x1B800323, 0x00001EB8, 0x1CC00309, 0x00001EBA, 0x3CC00303, 0x00001EBC, 0x00000046, 
        0x3CC00307, 0x00001E1E, 0x00000047, 0x1CC00302, 0x0000011C, 0x1CC00306, 0x0000011E, 0x1CC00307, 0x00000120, 0x19400327, 
        0x00000122, 0x1CC0030C, 0x000001E6, 0x1CC00301, 0x000001F4, 0x3CC00304, 0x00001E20, 0x00000048, 0x1CC00302, 0x00000124, 
        0x1CC0030C, 0x0000021E, 0x1CC00307, 0x00001E22, 0x1B800323, 0x00001E24, 0x1CC00308, 0x00001E26, 0x19400327, 0x00001E28, 
        0x3B80032E, 0x00001E2A, 0x00000049, 0x1CC00300, 0x000000CC, 0x1CC00301, 0x000000CD, 0x1CC00302, 0x000000CE, 0x1CC00308, 
        0x000000CF, 0x1CC00303, 0x00000128, 0x1CC00304, 0x0000012A, 0x1CC00306, 0x0000012C, 0x19400328, 0x0000012E, 0x1CC00307, 
        0x00000130, 0x1CC0030C, 0x000001CF, 0x1CC0030F, 0x00000208, 0x1CC00311, 0x0000020A, 0x1B800330, 0x00001E2C, 0x1CC00309, 
        0x00001EC8, 0x3B800323, 0x00001ECA, 0x0000004A, 0x3CC00302, 0x00000134, 0x0000004B, 0x19400327, 0x00000136, 0x1CC0030C, 
        0x000001E8, 0x1CC00301, 0x00001E30, 0x1B800323, 0x00001E32, 0x3B800331, 0x00001E34, 0x0000004C, 0x1CC00301, 0x00000139, 
        0x19400327, 0x0000013B, 0x1CC0030C, 0x0000013D, 0x1B800323, 0x00001E36, 0x1B800331, 0x00001E3A, 0x3B80032D, 0x00001E3C, 
        0x0000004D, 0x1CC00301, 0x00001E3E, 0x1CC00307, 0x00001E40, 0x3B800323, 0x00001E42, 0x0000004E, 0x1CC00303, 0x000000D1, 
        0x1CC00301, 0x00000143, 0x19400327, 0x00000145, 0x1CC0030C, 0x00000147, 0x1CC00300, 0x000001F8, 0x1CC00307, 0x00001E44, 
        0x1B800323, 0x00001E46, 0x1B800331, 0x00001E48, 0x3B80032D, 0x00001E4A, 0x0000004F, 0x1CC00300, 0x000000D2, 0x1CC00301, 
        0x000000D3, 0x1CC00302, 0x000000D4, 0x1CC00303, 0x000000D5, 0x1CC00308, 0x000000D6, 0x1CC00304, 0x0000014C, 0x1CC00306, 
        0x0000014E, 0x1CC0030B, 0x00000150, 0x1B00031B, 0x000001A0, 0x1CC0030C, 0x000001D1, 0x19400328, 0x000001EA, 0x1CC0030F, 
        0x0000020C, 0x1CC00311, 0x0000020E, 0x1CC00307, 0x0000022E, 0x1B800323, 0x00001ECC, 0x3CC00309, 0x00001ECE, 0x00000050, 
        0x1CC00301, 0x00001E54, 0x3CC00307, 0x00001E56, 0x00000052, 0x1CC00301, 0x00000154, 0x19400327, 0x00000156, 0x1CC0030C, 
        0x00000158, 0x1CC0030F, 0x00000210, 0x1CC00311, 0x00000212, 0x1CC00307, 0x00001E58, 0x1B800323, 0x00001E5A, 0x3B800331, 
        0x00001E5E, 0x00000053, 0x1CC00301, 0x0000015A, 0x1CC00302, 0x0000015C, 0x19400327, 0x0000015E, 0x1CC0030C, 0x00000160, 
        0x1B800326, 0x00000218, 0x1CC00307, 0x00001E60, 0x3B800323, 0x00001E62, 0x00000054, 0x19400327, 0x00000162, 0x1CC0030C, 
        0x00000164, 0x1B800326, 0x0000021A, 0x1CC00307, 0x00001E6A, 0x1B800323, 0x00001E6C, 0x1B800331, 0x00001E6E, 0x3B80032D, 
        0x00001E70, 0x00000055, 0x1CC00300, 0x000000D9, 0x1CC00301, 0x000000DA, 0x1CC00302, 0x000000DB, 0x1CC00308, 0x000000DC, 
        0x1CC00303, 0x00000168, 0x1CC00304, 0x0000016A, 0x1CC00306, 0x0000016C, 0x1CC0030A, 0x0000016E, 0x1CC0030B, 0x00000170, 
        0x19400328, 0x00000172, 0x1B00031B, 0x000001AF, 0x1CC0030C, 0x000001D3, 0x1CC0030F, 0x00000214, 0x1CC00311, 0x00000216, 
        0x1B800324, 0x00001E72, 0x1B800330, 0x00001E74, 0x1B80032D, 0x00001E76, 0x1B800323, 0x00001EE4, 0x3CC00309, 0x00001EE6, 
        0x00000056, 0x1CC00303, 0x00001E7C, 0x3B800323, 0x00001E7E, 0x00000057, 0x1CC00302, 0x00000174, 0x1CC00300, 0x00001E80, 
        0x1CC00301, 0x00001E82, 0x1CC00308, 0x00001E84, 0x1CC00307, 0x00001E86, 0x3B800323, 0x00001E88, 0x00000058, 0x1CC00307, 
        0x00001E8A, 0x3CC00308, 0x00001E8C, 0x00000059, 0x1CC00301, 0x000000DD, 0x1CC00302, 0x00000176, 0x1CC00308, 0x00000178, 
        0x1CC00304, 0x00000232, 0x1CC00307, 0x00001E8E, 0x1CC00300, 0x00001EF2, 0x1B800323, 0x00001EF4, 0x1CC00309, 0x00001EF6, 
        0x3CC00303, 0x00001EF8, 0x0000005A, 0x1CC00301, 0x00000179, 0x1CC00307, 0x0000017B, 0x1CC0030C, 0x0000017D, 0x1CC00302, 
        0x00001E90, 0x1B800323, 0x00001E92, 0x3B800331, 0x00001E94, 0x20000060, 0x00000061, 0x1CC00300, 0x000000E0, 0x1CC00301, 
        0x000000E1, 0x1CC00302, 0x000000E2, 0x1CC00303, 0x000000E3, 0x1CC00308, 0x000000E4, 0x1CC0030A, 0x000000E5, 0x1CC00304, 
        0x00000101, 0x1CC00306, 0x00000103, 0x19400328, 0x00000105, 0x1CC0030C, 0x000001CE, 0x1CC0030F, 0x00000201, 0x1CC00311, 
        0x00000203, 0x1CC00307, 0x00000227, 0x1B800325, 0x00001E01, 0x1B800323, 0x00001EA1, 0x3CC00309, 0x00001EA3, 0x00000062, 
        0x1CC00307, 0x00001E03, 0x1B800323, 0x00001E05, 0x3B800331, 0x00001E07, 0x00000063, 0x19400327, 0x000000E7, 0x1CC00301, 
        0x00000107, 0x1CC00302, 0x00000109, 0x1CC00307, 0x0000010B, 0x3CC0030C, 0x0000010D, 0x00000064, 0x1CC0030C, 0x0000010F, 
        0x1CC00307, 0x00001E0B, 0x1B800323, 0x00001E0D, 0x1B800331, 0x00001E0F, 0x19400327, 0x00001E11, 0x3B80032D, 0x00001E13, 
        0x00000065, 0x1CC00300, 0x000000E8, 0x1CC00301, 0x000000E9, 0x1CC00302, 0x000000EA, 0x1CC00308, 0x000000EB, 0x1CC00304, 
        0x00000113, 0x1CC00306, 0x00000115, 0x1CC00307, 0x00000117, 0x19400328, 0x00000119, 0x1CC0030C, 0x0000011B, 0x1CC0030F, 
        0x00000205, 0x1CC00311, 0x00000207, 0x19400327, 0x00000229, 0x1B80032D, 0x00001E19, 0x1B800330, 0x00001E1B, 0x1B800323, 
        0x00001EB9, 0x1CC00309, 0x00001EBB, 0x3CC00303, 0x00001EBD, 0x00000066, 0x3CC00307, 0x00001E1F, 0x00000067, 0x1CC00302, 
        0x0000011D, 0x1CC00306, 0x0000011F, 0x1CC00307, 0x00000121, 0x19400327, 0x00000123, 0x1CC0030C, 0x000001E7, 0x1CC00301, 
        0x000001F5, 0x3CC00304, 0x00001E21, 0x00000068, 0x1CC00302, 0x00000125, 0x1CC0030C, 0x0000021F, 0x1CC00307, 0x00001E23, 
        0x1B800323, 0x00001E25, 0x1CC00308, 0x00001E27, 0x19400327, 0x00001E29, 0x1B80032E, 0x00001E2B, 0x3B800331, 0x00001E96, 
        0x00000069, 0x1CC00300, 0x000000EC, 0x1CC00301, 0x000000ED, 0x1CC00302, 0x000000EE, 0x1CC00308, 0x000000EF, 0x1CC00303, 
        0x00000129, 0x1CC00304, 0x0000012B, 0x1CC00306, 0x0000012D, 0x19400328, 0x0000012F, 0x1CC0030C, 0x000001D0, 0x1CC0030F, 
        0x00000209, 0x1CC00311, 0x0000020B, 0x1B800330, 0x00001E2D, 0x1CC00309, 0x00001EC9, 0x3B800323, 0x00001ECB, 0x0000006A, 
        0x1CC00302, 0x00000135, 0x3CC0030C, 0x000001F0, 0x0000006B, 0x19400327, 0x00000137, 0x1CC0030C, 0x000001E9, 0x1CC00301, 
        0x00001E31, 0x1B800323, 0x00001E33, 0x3B800331, 0x00001E35, 0x0000006C, 0x1CC00301, 0x0000013A, 0x19400327, 0x0000013C, 
        0x1CC0030C, 0x0000013E, 0x1B800323, 0x00001E37, 0x1B800331, 0x00001E3B, 0x3B80032D, 0x00001E3D, 0x0000006D, 0x1CC00301, 
        0x00001E3F, 0x1CC00307, 0x00001E41, 0x3B800323, 0x00001E43, 0x0000006E, 0x1CC00303, 0x000000F1, 0x1CC00301, 0x00000144, 
        0x19400327, 0x00000146, 0x1CC0030C, 0x00000148, 0x1CC00300, 0x000001F9, 0x1CC00307, 0x00001E45, 0x1B800323, 0x00001E47, 
        0x1B800331, 0x00001E49, 0x3B80032D, 0x00001E4B, 0x0000006F, 0x1CC00300, 0x000000F2, 0x1CC00301, 0x000000F3, 0x1CC00302, 
        0x000000F4, 0x1CC00303, 0x000000F5, 0x1CC00308, 0x000000F6, 0x1CC00304, 0x0000014D, 0x1CC00306, 0x0000014F, 0x1CC0030B, 
        0x00000151, 0x1B00031B, 0x000001A1, 0x1CC0030C, 0x000001D2, 0x19400328, 0x000001EB, 0x1CC0030F, 0x0000020D, 0x1CC00311, 
        0x0000020F, 0x1CC00307, 0x0000022F, 0x1B800323, 0x00001ECD, 0x3CC00309, 0x00001ECF, 0x00000070, 0x1CC00301, 0x00001E55, 
        0x3CC00307, 0x00001E57, 0x00000072, 0x1CC00301, 0x00000155, 0x19400327, 0x00000157, 0x1CC0030C, 0x00000159, 0x1CC0030F, 
        0x00000211, 0x1CC00311, 0x00000213, 0x1CC00307, 0x00001E59, 0x1B800323, 0x00001E5B, 0x3B800331, 0x00001E5F, 0x00000073, 
        0x1CC00301, 0x0000015B, 0x1CC00302, 0x0000015D, 0x19400327, 0x0000015F, 0x1CC0030C, 0x00000161, 0x1B800326, 0x00000219, 
        0x1CC00307, 0x00001E61, 0x3B800323, 0x00001E63, 0x00000074, 0x19400327, 0x00000163, 0x1CC0030C, 0x00000165, 0x1B800326, 
        0x0000021B, 0x1CC00307, 0x00001E6B, 0x1B800323, 0x00001E6D, 0x1B800331, 0x00001E6F, 0x1B80032D, 0x00001E71, 0x3CC00308, 
        0x00001E97, 0x00000075, 0x1CC00300, 0x000000F9, 0x1CC00301, 0x000000FA, 0x1CC00302, 0x000000FB, 0x1CC00308, 0x000000FC, 
        0x1CC00303, 0x00000169, 0x1CC00304, 0x0000016B, 0x1CC00306, 0x0000016D, 0x1CC0030A, 0x0000016F, 0x1CC0030B, 0x00000171, 
        0x19400328, 0x00000173, 0x1B00031B, 0x000001B0, 0x1CC0030C, 0x000001D4, 0x1CC0030F, 0x00000215, 0x1CC00311, 0x00000217, 
        0x1B800324, 0x00001E73, 0x1B800330, 0x00001E75, 0x1B80032D, 0x00001E77, 0x1B800323, 0x00001EE5, 0x3CC00309, 0x00001EE7, 
        0x00000076, 0x1CC00303, 0x00001E7D, 0x3B800323, 0x00001E7F, 0x00000077, 0x1CC00302, 0x00000175, 0x1CC00300, 0x00001E81, 
        0x1CC00301, 0x00001E83, 0x1CC00308, 0x00001E85, 0x1CC00307, 0x00001E87, 0x1B800323, 0x00001E89, 0x3CC0030A, 0x00001E98, 
        0x00000078, 0x1CC00307, 0x00001E8B, 0x3CC00308, 0x00001E8D, 0x00000079, 0x1CC00301, 0x000000FD, 0x1CC00308, 0x000000FF, 
        0x1CC00302, 0x00000177, 0x1CC00304, 0x00000233, 0x1CC00307, 0x00001E8F, 0x1CC0030A, 0x00001E99, 0x1CC00300, 0x00001EF3, 
        0x1B800323, 0x00001EF5, 0x1CC00309, 0x00001EF7, 0x3CC00303, 0x00001EF9, 0x0000007A, 0x1CC00301, 0x0000017A, 0x1CC00307, 
        0x0000017C, 0x1CC0030C, 0x0000017E, 0x1CC00302, 0x00001E91, 0x1B800323, 0x00001E93, 0x3B800331, 0x00001E95, 0x000000A8, 
        0x1CC00301, 0x00000385, 0x1CC00342, 0x00001FC1, 0x3CC00300, 0x00001FED, 0x200000B4, 0x200000B7, 0x000000C2, 0x1CC00301, 
        0x00001EA4, 0x1CC00300, 0x00001EA6, 0x1CC00309, 0x00001EA8, 0x3CC00303, 0x00001EAA, 0x000000C4, 0x3CC00304, 0x000001DE, 
        0x000000C5, 0x3CC00301, 0x000001FA, 0x000000C6, 0x1CC00304, 0x000001E2, 0x3CC00301, 0x000001FC, 0x000000C7, 0x3CC00301, 
        0x00001E08, 0x000000CA, 0x1CC00301, 0x00001EBE, 0x1CC00300, 0x00001EC0, 0x1CC00309, 0x00001EC2, 0x3CC00303, 0x00001EC4, 
        0x000000CF, 0x3CC00301, 0x00001E2E, 0x000000D4, 0x1CC00301, 0x00001ED0, 0x1CC00300, 0x00001ED2, 0x1CC00309, 0x00001ED4, 
        0x3CC00303, 0x00001ED6, 0x000000D5, 0x1CC00304, 0x0000022C, 0x1CC00301, 0x00001E4C, 0x3CC00308, 0x00001E4E, 0x000000D6, 
        0x3CC00304, 0x0000022A, 0x000000D8, 0x3CC00301, 0x000001FE, 0x000000DC, 0x1CC00304, 0x000001D5, 0x1CC00301, 0x000001D7, 
        0x1CC0030C, 0x000001D9, 0x3CC00300, 0x000001DB, 0x000000E2, 0x1CC00301, 0x00001EA5, 0x1CC00300, 0x00001EA7, 0x1CC00309, 
        0x00001EA9, 0x3CC00303, 0x00001EAB, 0x000000E4, 0x3CC00304, 0x000001DF, 0x000000E5, 0x3CC00301, 0x000001FB, 0x000000E6, 
        0x1CC00304, 0x000001E3, 0x3CC00301, 0x000001FD, 0x000000E7, 0x3CC00301, 0x00001E09, 0x000000EA, 0x1CC00301, 0x00001EBF, 
        0x1CC00300, 0x00001EC1, 0x1CC00309, 0x00001EC3, 0x3CC00303, 0x00001EC5, 0x000000EF, 0x3CC00301, 0x00001E2F, 0x000000F4, 
        0x1CC00301, 0x00001ED1, 0x1CC00300, 0x00001ED3, 0x1CC00309, 0x00001ED5, 0x3CC00303, 0x00001ED7, 0x000000F5, 0x1CC00304, 
        0x0000022D, 0x1CC00301, 0x00001E4D, 0x3CC00308, 0x00001E4F, 0x000000F6, 0x3CC00304, 0x0000022B, 0x000000F8, 0x3CC00301, 
        0x000001FF, 0x000000FC, 0x1CC00304, 0x000001D6, 0x1CC00301, 0x000001D8, 0x1CC0030C, 0x000001DA, 0x3CC00300, 0x000001DC, 
        0x00000102, 0x1CC00301, 0x00001EAE, 0x1CC00300, 0x00001EB0, 0x1CC00309, 0x00001EB2, 0x3CC00303, 0x00001EB4, 0x00000103, 
        0x1CC00301, 0x00001EAF, 0x1CC00300, 0x00001EB1, 0x1CC00309, 0x00001EB3, 0x3CC00303, 0x00001EB5, 0x00000112, 0x1CC00300, 
        0x00001E14, 0x3CC00301, 0x00001E16, 0x00000113, 0x1CC00300, 0x00001E15, 0x3CC00301, 0x00001E17, 0x0000014C, 0x1CC00300, 
        0x00001E50, 0x3CC00301, 0x00001E52, 0x0000014D, 0x1CC00300, 0x00001E51, 0x3CC00301, 0x00001E53, 0x0000015A, 0x3CC00307, 
        0x00001E64, 0x0000015B, 0x3CC00307, 0x00001E65, 0x00000160, 0x3CC00307, 0x00001E66, 0x00000161, 0x3CC00307, 0x00001E67, 
        0x00000168, 0x3CC00301, 0x00001E78, 0x00000169, 0x3CC00301, 0x00001E79, 0x0000016A, 0x3CC00308, 0x00001E7A, 0x0000016B, 
        0x3CC00308, 0x00001E7B, 0x0000017F, 0x3CC00307, 0x00001E9B, 0x000001A0, 0x1CC00301, 0x00001EDA, 0x1CC00300, 0x00001EDC, 
        0x1CC00309, 0x00001EDE, 0x1CC00303, 0x00001EE0, 0x3B800323, 0x00001EE2, 0x000001A1, 0x1CC00301, 0x00001EDB, 0x1CC00300, 
        0x00001EDD, 0x1CC00309, 0x00001EDF, 0x1CC00303, 0x00001EE1, 0x3B800323, 0x00001EE3, 0x000001AF, 0x1CC00301, 0x00001EE8, 
        0x1CC00300, 0x00001EEA, 0x1CC00309, 0x00001EEC, 0x1CC00303, 0x00001EEE, 0x3B800323, 0x00001EF0, 0x000001B0, 0x1CC00301, 
        0x00001EE9, 0x1CC00300, 0x00001EEB, 0x1CC00309, 0x00001EED, 0x1CC00303, 0x00001EEF, 0x3B800323, 0x00001EF1, 0x000001B7, 
        0x3CC0030C, 0x000001EE, 0x000001EA, 0x3CC00304, 0x000001EC, 0x000001EB, 0x3CC00304, 0x000001ED, 0x00000226, 0x3CC00304, 
        0x000001E0, 0x00000227, 0x3CC00304, 0x000001E1, 0x00000228, 0x3CC00306, 0x00001E1C, 0x00000229, 0x3CC00306, 0x00001E1D, 
        0x0000022E, 0x3CC00304, 0x00000230, 0x0000022F, 0x3CC00304, 0x00000231, 0x00000292, 0x3CC0030C, 0x000001EF, 0x200002B9, 
        0x3CC00300, 0x3CC00301, 0x1CC00308, 0x3CC00301, 0x1CC00344, 0x3CC00313, 0x20000385, 0x20000386, 0x20000388, 0x20000389, 
        0x2000038A, 0x2000038C, 0x2000038E, 0x2000038F, 0x20000390, 0x00000391, 0x1CC00301, 0x00000386, 0x1CC00313, 0x00001F08, 
        0x1CC00314, 0x00001F09, 0x1CC00306, 0x00001FB8, 0x1CC00304, 0x00001FB9, 0x1CC00300, 0x00001FBA, 0x3E000345, 0x00001FBC, 
        0x00000395, 0x1CC00301, 0x00000388, 0x1CC00313, 0x00001F18, 0x1CC00314, 0x00001F19, 0x3CC00300, 0x00001FC8, 0x00000397, 
        0x1CC00301, 0x00000389, 0x1CC00313, 0x00001F28, 0x1CC00314, 0x00001F29, 0x1CC00300, 0x00001FCA, 0x3E000345, 0x00001FCC, 
        0x00000399, 0x1CC00301, 0x0000038A, 0x1CC00308, 0x000003AA, 0x1CC00313, 0x00001F38, 0x1CC00314, 0x00001F39, 0x1CC00306, 
        0x00001FD8, 0x1CC00304, 0x00001FD9, 0x3CC00300, 0x00001FDA, 0x0000039F, 0x1CC00301, 0x0000038C, 0x1CC00313, 0x00001F48, 
        0x1CC00314, 0x00001F49, 0x3CC00300, 0x00001FF8, 0x000003A1, 0x3CC00314, 0x00001FEC, 0x000003A5, 0x1CC00301, 0x0000038E, 
        0x1CC00308, 0x000003AB, 0x1CC00314, 0x00001F59, 0x1CC00306, 0x00001FE8, 0x1CC00304, 0x00001FE9, 0x3CC00300, 0x00001FEA, 
        0x000003A9, 0x1CC00301, 0x0000038F, 0x1CC00313, 0x00001F68, 0x1CC00314, 0x00001F69, 0x1CC00300, 0x00001FFA, 0x3E000345, 
        0x00001FFC, 0x000003AC, 0x3E000345, 0x00001FB4, 0x200003AD, 0x000003AE, 0x3E000345, 0x00001FC4, 0x200003AF, 0x200003B0, 
        0x000003B1, 0x1CC00301, 0x000003AC, 0x1CC00313, 0x00001F00, 0x1CC00314, 0x00001F01, 0x1CC00300, 0x00001F70, 0x1CC00306, 
        0x00001FB0, 0x1CC00304, 0x00001FB1, 0x1E000345, 0x00001FB3, 0x3CC00342, 0x00001FB6, 0x000003B5, 0x1CC00301, 0x000003AD, 
        0x1CC00313, 0x00001F10, 0x1CC00314, 0x00001F11, 0x3CC00300, 0x00001F72, 0x000003B7, 0x1CC00301, 0x000003AE, 0x1CC00313, 
        0x00001F20, 0x1CC00314, 0x00001F21, 0x1CC00300, 0x00001F74, 0x1E000345, 0x00001FC3, 0x3CC00342, 0x00001FC6, 0x000003B9, 
        0x1CC00301, 0x000003AF, 0x1CC00308, 0x000003CA, 0x1CC00313, 0x00001F30, 0x1CC00314, 0x00001F31, 0x1CC00300, 0x00001F76, 
        0x1CC00306, 0x00001FD0, 0x1CC00304, 0x00001FD1, 0x3CC00342, 0x00001FD6, 0x000003BF, 0x1CC00301, 0x000003CC, 0x1CC00313, 
        0x00001F40, 0x1CC00314, 0x00001F41, 0x3CC00300, 0x00001F78, 0x000003C1, 0x1CC00313, 0x00001FE4, 0x3CC00314, 0x00001FE5, 
        0x000003C5, 0x1CC00308, 0x000003CB, 0x1CC00301, 0x000003CD, 0x1CC00313, 0x00001F50, 0x1CC00314, 0x00001F51, 0x1CC00300, 
        0x00001F7A, 0x1CC00306, 0x00001FE0, 0x1CC00304, 0x00001FE1, 0x3CC00342, 0x00001FE6, 0x000003C9, 0x1CC00301, 0x000003CE, 
        0x1CC00313, 0x00001F60, 0x1CC00314, 0x00001F61, 0x1CC00300, 0x00001F7C, 0x1E000345, 0x00001FF3, 0x3CC00342, 0x00001FF6, 
        0x000003CA, 0x1CC00301, 0x00000390, 0x1CC00300, 0x00001FD2, 0x3CC00342, 0x00001FD7, 0x000003CB, 0x1CC00301, 0x000003B0, 
        0x1CC00300, 0x00001FE2, 0x3CC00342, 0x00001FE7, 0x200003CC, 0x200003CD, 0x000003CE, 0x3E000345, 0x00001FF4, 0x000003D2, 
        0x1CC00301, 0x000003D3, 0x3CC00308, 0x000003D4, 0x00000406, 0x3CC00308, 0x00000407, 0x00000410, 0x1CC00306, 0x000004D0, 
        0x3CC00308, 0x000004D2, 0x00000413, 0x3CC00301, 0x00000403, 0x00000415, 0x1CC00300, 0x00000400, 0x1CC00308, 0x00000401, 
        0x3CC00306, 0x000004D6, 0x00000416, 0x1CC00306, 0x000004C1, 0x3CC00308, 0x000004DC, 0x00000417, 0x3CC00308, 0x000004DE, 
        0x00000418, 0x1CC00300, 0x0000040D, 0x1CC00306, 0x00000419, 0x1CC00304, 0x000004E2, 0x3CC00308, 0x000004E4, 0x0000041A, 
        0x3CC00301, 0x0000040C, 0x0000041E, 0x3CC00308, 0x000004E6, 0x00000423, 0x1CC00306, 0x0000040E, 0x1CC00304, 0x000004EE, 
        0x1CC00308, 0x000004F0, 0x3CC0030B, 0x000004F2, 0x00000427, 0x3CC00308, 0x000004F4, 0x0000042B, 0x3CC00308, 0x000004F8, 
        0x0000042D, 0x3CC00308, 0x000004EC, 0x00000430, 0x1CC00306, 0x000004D1, 0x3CC00308, 0x000004D3, 0x00000433, 0x3CC00301, 
        0x00000453, 0x00000435, 0x1CC00300, 0x00000450, 0x1CC00308, 0x00000451, 0x3CC00306, 0x000004D7, 0x00000436, 0x1CC00306, 
        0x000004C2, 0x3CC00308, 0x000004DD, 0x00000437, 0x3CC00308, 0x000004DF, 0x00000438, 0x1CC00306, 0x00000439, 0x1CC00300, 
        0x0000045D, 0x1CC00304, 0x000004E3, 0x3CC00308, 0x000004E5, 0x0000043A, 0x3CC00301, 0x0000045C, 0x0000043E, 0x3CC00308, 
        0x000004E7, 0x00000443, 0x1CC00306, 0x0000045E, 0x1CC00304, 0x000004EF, 0x1CC00308, 0x000004F1, 0x3CC0030B, 0x000004F3, 
        0x00000447, 0x3CC00308, 0x000004F5, 0x0000044B, 0x3CC00308, 0x000004F9, 0x0000044D, 0x3CC00308, 0x000004ED, 0x00000456, 
        0x3CC00308, 0x00000457, 0x00000474, 0x3CC0030F, 0x00000476, 0x00000475, 0x3CC0030F, 0x00000477, 0x000004D8, 0x3CC00308, 
        0x000004DA, 0x000004D9, 0x3CC00308, 0x000004DB, 0x000004E8, 0x3CC00308, 0x000004EA, 0x000004E9, 0x3CC00308, 0x000004EB, 
        0x000005D0, 0x022005B7, 0x00000000, 0x024005B8, 0x00000000, 0x22A005BC, 0x00000000, 0x000005D1, 0x02A005BC, 0x00000000, 
        0x22E005BF, 0x00000000, 0x000005D2, 0x22A005BC, 0x00000000, 0x000005D3, 0x22A005BC, 0x00000000, 0x000005D4, 0x22A005BC, 
        0x00000000, 0x000005D5, 0x02A005BC, 0x00000000, 0x226005B9, 0x00000000, 0x000005D6, 0x22A005BC, 0x00000000, 0x000005D8, 
        0x22A005BC, 0x00000000, 0x000005D9, 0x01C005B4, 0x00000000, 0x22A005BC, 0x00000000, 0x000005DA, 0x22A005BC, 0x00000000, 
        0x000005DB, 0x02A005BC, 0x00000000, 0x22E005BF, 0x00000000, 0x000005DC, 0x22A005BC, 0x00000000, 0x000005DE, 0x22A005BC, 
        0x00000000, 0x000005E0, 0x22A005BC, 0x00000000, 0x000005E1, 0x22A005BC, 0x00000000, 0x000005E3, 0x22A005BC, 0x00000000, 
        0x000005E4, 0x02A005BC, 0x00000000, 0x22E005BF, 0x00000000, 0x000005E6, 0x22A005BC, 0x00000000, 0x000005E7, 0x22A005BC, 
        0x00000000, 0x000005E8, 0x22A005BC, 0x00000000, 0x000005E9, 0x030005C1, 0x00000000, 0x032005C2, 0x00000000, 0x22A005BC, 
        0x00000000, 0x000005EA, 0x22A005BC, 0x00000000, 0x000005F2, 0x222005B7, 0x00000000, 0x00000627, 0x1CC00653, 0x00000622, 
        0x1CC00654, 0x00000623, 0x3B800655, 0x00000625, 0x00000648, 0x3CC00654, 0x00000624, 0x0000064A, 0x3CC00654, 0x00000626, 
        0x000006C1, 0x3CC00654, 0x000006C2, 0x000006D2, 0x3CC00654, 0x000006D3, 0x000006D5, 0x3CC00654, 0x000006C0, 0x00000915, 
        0x20E0093C, 0x00000000, 0x00000916, 0x20E0093C, 0x00000000, 0x00000917, 0x20E0093C, 0x00000000, 0x0000091C, 0x20E0093C, 
        0x00000000, 0x00000921, 0x20E0093C, 0x00000000, 0x00000922, 0x20E0093C, 0x00000000, 0x00000928, 0x20E0093C, 0x00000929, 
        0x0000092B, 0x20E0093C, 0x00000000, 0x0000092F, 0x20E0093C, 0x00000000, 0x00000930, 0x20E0093C, 0x00000931, 0x00000933, 
        0x20E0093C, 0x00000934, 0x000009A1, 0x20E009BC, 0x00000000, 0x000009A2, 0x20E009BC, 0x00000000, 0x000009AF, 0x20E009BC, 
        0x00000000, 0x000009C7, 0x000009BE, 0x000009CB, 0x200009D7, 0x000009CC, 0x00000A16, 0x20E00A3C, 0x00000000, 0x00000A17, 
        0x20E00A3C, 0x00000000, 0x00000A1C, 0x20E00A3C, 0x00000000, 0x00000A2B, 0x20E00A3C, 0x00000000, 0x00000A32, 0x20E00A3C, 
        0x00000000, 0x00000A38, 0x20E00A3C, 0x00000000, 0x00000B21, 0x20E00B3C, 0x00000000, 0x00000B22, 0x20E00B3C, 0x00000000, 
        0x00000B47, 0x00000B56, 0x00000B48, 0x00000B3E, 0x00000B4B, 0x20000B57, 0x00000B4C, 0x00000B92, 0x20000BD7, 0x00000B94, 
        0x00000BC6, 0x00000BBE, 0x00000BCA, 0x20000BD7, 0x00000BCC, 0x00000BC7, 0x20000BBE, 0x00000BCB, 0x00000C46, 0x2B600C56, 
        0x00000C48, 0x00000CBF, 0x20000CD5, 0x00000CC0, 0x00000CC6, 0x00000CD5, 0x00000CC7, 0x00000CD6, 0x00000CC8, 0x20000CC2, 
        0x00000CCA, 0x00000CCA, 0x20000CD5, 0x00000CCB, 0x00000D46, 0x00000D3E, 0x00000D4A, 0x20000D57, 0x00000D4C, 0x00000D47, 
        0x20000D3E, 0x00000D4B, 0x00000DD9, 0x01200DCA, 0x00000DDA, 0x00000DCF, 0x00000DDC, 0x20000DDF, 0x00000DDE, 0x00000DDC, 
        0x21200DCA, 0x00000DDD, 0x00000F40, 0x20000FB5, 0x00000000, 0x00000F42, 0x20000FB7, 0x00000000, 0x00000F4C, 0x20000FB7, 
        0x00000000, 0x00000F51, 0x20000FB7, 0x00000000, 0x00000F56, 0x20000FB7, 0x00000000, 0x00000F5B, 0x20000FB7, 0x00000000, 
        0x10200F71, 0x10400F72, 0x00000F73, 0x10800F74, 0x00000F75, 0x30400F80, 0x00000F81, 0x00000F90, 0x20000FB5, 0x00000000, 
        0x00000F92, 0x20000FB7, 0x00000000, 0x00000F9C, 0x20000FB7, 0x00000000, 0x00000FA1, 0x20000FB7, 0x00000000, 0x00000FA6, 
        0x20000FB7, 0x00000000, 0x00000FAB, 0x20000FB7, 0x00000000, 0x00000FB2, 0x30400F80, 0x00000000, 0x00000FB3, 0x30400F80, 
        0x00000000, 0x00001025, 0x2000102E, 0x00001026, 0x00001B05, 0x20001B35, 0x00001B06, 0x00001B07, 0x20001B35, 0x00001B08, 
        0x00001B09, 0x20001B35, 0x00001B0A, 0x00001B0B, 0x20001B35, 0x00001B0C, 0x00001B0D, 0x20001B35, 0x00001B0E, 0x00001B11, 
        0x20001B35, 0x00001B12, 0x00001B3A, 0x20001B35, 0x00001B3B, 0x00001B3C, 0x20001B35, 0x00001B3D, 0x00001B3E, 0x20001B35, 
        0x00001B40, 0x00001B3F, 0x20001B35, 0x00001B41, 0x00001B42, 0x20001B35, 0x00001B43, 0x00001E36, 0x3CC00304, 0x00001E38, 
        0x00001E37, 0x3CC00304, 0x00001E39, 0x00001E5A, 0x3CC00304, 0x00001E5C, 0x00001E5B, 0x3CC00304, 0x00001E5D, 0x00001E62, 
        0x3CC00307, 0x00001E68, 0x00001E63, 0x3CC00307, 0x00001E69, 0x00001EA0, 0x1CC00302, 0x00001EAC, 0x3CC00306, 0x00001EB6, 
        0x00001EA1, 0x1CC00302, 0x00001EAD, 0x3CC00306, 0x00001EB7, 0x00001EB8, 0x3CC00302, 0x00001EC6, 0x00001EB9, 0x3CC00302, 
        0x00001EC7, 0x00001ECC, 0x3CC00302, 0x00001ED8, 0x00001ECD, 0x3CC00302, 0x00001ED9, 0x00001F00, 0x1CC00300, 0x00001F02, 
        0x1CC00301, 0x00001F04, 0x1CC00342, 0x00001F06, 0x3E000345, 0x00001F80, 0x00001F01, 0x1CC00300, 0x00001F03, 0x1CC00301, 
        0x00001F05, 0x1CC00342, 0x00001F07, 0x3E000345, 0x00001F81, 0x00001F02, 0x3E000345, 0x00001F82, 0x00001F03, 0x3E000345, 
        0x00001F83, 0x00001F04, 0x3E000345, 0x00001F84, 0x00001F05, 0x3E000345, 0x00001F85, 0x00001F06, 0x3E000345, 0x00001F86, 
        0x00001F07, 0x3E000345, 0x00001F87, 0x00001F08, 0x1CC00300, 0x00001F0A, 0x1CC00301, 0x00001F0C, 0x1CC00342, 0x00001F0E, 
        0x3E000345, 0x00001F88, 0x00001F09, 0x1CC00300, 0x00001F0B, 0x1CC00301, 0x00001F0D, 0x1CC00342, 0x00001F0F, 0x3E000345, 
        0x00001F89, 0x00001F0A, 0x3E000345, 0x00001F8A, 0x00001F0B, 0x3E000345, 0x00001F8B, 0x00001F0C, 0x3E000345, 0x00001F8C, 
        0x00001F0D, 0x3E000345, 0x00001F8D, 0x00001F0E, 0x3E000345, 0x00001F8E, 0x00001F0F, 0x3E000345, 0x00001F8F, 0x00001F10, 
        0x1CC00300, 0x00001F12, 0x3CC00301, 0x00001F14, 0x00001F11, 0x1CC00300, 0x00001F13, 0x3CC00301, 0x00001F15, 0x00001F18, 
        0x1CC00300, 0x00001F1A, 0x3CC00301, 0x00001F1C, 0x00001F19, 0x1CC00300, 0x00001F1B, 0x3CC00301, 0x00001F1D, 0x00001F20, 
        0x1CC00300, 0x00001F22, 0x1CC00301, 0x00001F24, 0x1CC00342, 0x00001F26, 0x3E000345, 0x00001F90, 0x00001F21, 0x1CC00300, 
        0x00001F23, 0x1CC00301, 0x00001F25, 0x1CC00342, 0x00001F27, 0x3E000345, 0x00001F91, 0x00001F22, 0x3E000345, 0x00001F92, 
        0x00001F23, 0x3E000345, 0x00001F93, 0x00001F24, 0x3E000345, 0x00001F94, 0x00001F25, 0x3E000345, 0x00001F95, 0x00001F26, 
        0x3E000345, 0x00001F96, 0x00001F27, 0x3E000345, 0x00001F97, 0x00001F28, 0x1CC00300, 0x00001F2A, 0x1CC00301, 0x00001F2C, 
        0x1CC00342, 0x00001F2E, 0x3E000345, 0x00001F98, 0x00001F29, 0x1CC00300, 0x00001F2B, 0x1CC00301, 0x00001F2D, 0x1CC00342, 
        0x00001F2F, 0x3E000345, 0x00001F99, 0x00001F2A, 0x3E000345, 0x00001F9A, 0x00001F2B, 0x3E000345, 0x00001F9B, 0x00001F2C, 
        0x3E000345, 0x00001F9C, 0x00001F2D, 0x3E000345, 0x00001F9D, 0x00001F2E, 0x3E000345, 0x00001F9E, 0x00001F2F, 0x3E000345, 
        0x00001F9F, 0x00001F30, 0x1CC00300, 0x00001F32, 0x1CC00301, 0x00001F34, 0x3CC00342, 0x00001F36, 0x00001F31, 0x1CC00300, 
        0x00001F33, 0x1CC00301, 0x00001F35, 0x3CC00342, 0x00001F37, 0x00001F38, 0x1CC00300, 0x00001F3A, 0x1CC00301, 0x00001F3C, 
        0x3CC00342, 0x00001F3E, 0x00001F39, 0x1CC00300, 0x00001F3B, 0x1CC00301, 0x00001F3D, 0x3CC00342, 0x00001F3F, 0x00001F40, 
        0x1CC00300, 0x00001F42, 0x3CC00301, 0x00001F44, 0x00001F41, 0x1CC00300, 0x00001F43, 0x3CC00301, 0x00001F45, 0x00001F48, 
        0x1CC00300, 0x00001F4A, 0x3CC00301, 0x00001F4C, 0x00001F49, 0x1CC00300, 0x00001F4B, 0x3CC00301, 0x00001F4D, 0x00001F50, 
        0x1CC00300, 0x00001F52, 0x1CC00301, 0x00001F54, 0x3CC00342, 0x00001F56, 0x00001F51, 0x1CC00300, 0x00001F53, 0x1CC00301, 
        0x00001F55, 0x3CC00342, 0x00001F57, 0x00001F59, 0x1CC00300, 0x00001F5B, 0x1CC00301, 0x00001F5D, 0x3CC00342, 0x00001F5F, 
        0x00001F60, 0x1CC00300, 0x00001F62, 0x1CC00301, 0x00001F64, 0x1CC00342, 0x00001F66, 0x3E000345, 0x00001FA0, 0x00001F61, 
        0x1CC00300, 0x00001F63, 0x1CC00301, 0x00001F65, 0x1CC00342, 0x00001F67, 0x3E000345, 0x00001FA1, 0x00001F62, 0x3E000345, 
        0x00001FA2, 0x00001F63, 0x3E000345, 0x00001FA3, 0x00001F64, 0x3E000345, 0x00001FA4, 0x00001F65, 0x3E000345, 0x00001FA5, 
        0x00001F66, 0x3E000345, 0x00001FA6, 0x00001F67, 0x3E000345, 0x00001FA7, 0x00001F68, 0x1CC00300, 0x00001F6A, 0x1CC00301, 
        0x00001F6C, 0x1CC00342, 0x00001F6E, 0x3E000345, 0x00001FA8, 0x00001F69, 0x1CC00300, 0x00001F6B, 0x1CC00301, 0x00001F6D, 
        0x1CC00342, 0x00001F6F, 0x3E000345, 0x00001FA9, 0x00001F6A, 0x3E000345, 0x00001FAA, 0x00001F6B, 0x3E000345, 0x00001FAB, 
        0x00001F6C, 0x3E000345, 0x00001FAC, 0x00001F6D, 0x3E000345, 0x00001FAD, 0x00001F6E, 0x3E000345, 0x00001FAE, 0x00001F6F, 
        0x3E000345, 0x00001FAF, 0x00001F70, 0x3E000345, 0x00001FB2, 0x00001F74, 0x3E000345, 0x00001FC2, 0x00001F7C, 0x3E000345, 
        0x00001FF2, 0x00001FB6, 0x3E000345, 0x00001FB7, 0x00001FBF, 0x1CC00300, 0x00001FCD, 0x1CC00301, 0x00001FCE, 0x3CC00342, 
        0x00001FCF, 0x00001FC6, 0x3E000345, 0x00001FC7, 0x00001FF6, 0x3E000345, 0x00001FF7, 0x00001FFE, 0x1CC00300, 0x00001FDD, 
        0x1CC00301, 0x00001FDE, 0x3CC00342, 0x00001FDF, 0x20002002, 0x20002003, 0x00002190, 0x20200338, 0x0000219A, 0x00002192, 
        0x20200338, 0x0000219B, 0x00002194, 0x20200338, 0x000021AE, 0x000021D0, 0x20200338, 0x000021CD, 0x000021D2, 0x20200338, 
        0x000021CF, 0x000021D4, 0x20200338, 0x000021CE, 0x00002203, 0x20200338, 0x00002204, 0x00002208, 0x20200338, 0x00002209, 
        0x0000220B, 0x20200338, 0x0000220C, 0x00002223, 0x20200338, 0x00002224, 0x00002225, 0x20200338, 0x00002226, 0x0000223C, 
        0x20200338, 0x00002241, 0x00002243, 0x20200338, 0x00002244, 0x00002245, 0x20200338, 0x00002247, 0x00002248, 0x20200338, 
        0x00002249, 0x0000224D, 0x20200338, 0x0000226D, 0x00002261, 0x20200338, 0x00002262, 0x00002264, 0x20200338, 0x00002270, 
        0x00002265, 0x20200338, 0x00002271, 0x00002272, 0x20200338, 0x00002274, 0x00002273, 0x20200338, 0x00002275, 0x00002276, 
        0x20200338, 0x00002278, 0x00002277, 0x20200338, 0x00002279, 0x0000227A, 0x20200338, 0x00002280, 0x0000227B, 0x20200338, 
        0x00002281, 0x0000227C, 0x20200338, 0x000022E0, 0x0000227D, 0x20200338, 0x000022E1, 0x00002282, 0x20200338, 0x00002284, 
        0x00002283, 0x20200338, 0x00002285, 0x00002286, 0x20200338, 0x00002288, 0x00002287, 0x20200338, 0x00002289, 0x00002291, 
        0x20200338, 0x000022E2, 0x00002292, 0x20200338, 0x000022E3, 0x000022A2, 0x20200338, 0x000022AC, 0x000022A8, 0x20200338, 
        0x000022AD, 0x000022A9, 0x20200338, 0x000022AE, 0x000022AB, 0x20200338, 0x000022AF, 0x000022B2, 0x20200338, 0x000022EA, 
        0x000022B3, 0x20200338, 0x000022EB, 0x000022B4, 0x20200338, 0x000022EC, 0x000022B5, 0x20200338, 0x000022ED, 0x00002ADD, 
        0x20200338, 0x00000000, 0x20003008, 0x20003009, 0x00003046, 0x21003099, 0x00003094, 0x0000304B, 0x21003099, 0x0000304C, 
        0x0000304D, 0x21003099, 0x0000304E, 0x0000304F, 0x21003099, 0x00003050, 0x00003051, 0x21003099, 0x00003052, 0x00003053, 
        0x21003099, 0x00003054, 0x00003055, 0x21003099, 0x00003056, 0x00003057, 0x21003099, 0x00003058, 0x00003059, 0x21003099, 
        0x0000305A, 0x0000305B, 0x21003099, 0x0000305C, 0x0000305D, 0x21003099, 0x0000305E, 0x0000305F, 0x21003099, 0x00003060, 
        0x00003061, 0x21003099, 0x00003062, 0x00003064, 0x21003099, 0x00003065, 0x00003066, 0x21003099, 0x00003067, 0x00003068, 
        0x21003099, 0x00003069, 0x0000306F, 0x01003099, 0x00003070, 0x2100309A, 0x00003071, 0x00003072, 0x01003099, 0x00003073, 
        0x2100309A, 0x00003074, 0x00003075, 0x01003099, 0x00003076, 0x2100309A, 0x00003077, 0x00003078, 0x01003099, 0x00003079, 
        0x2100309A, 0x0000307A, 0x0000307B, 0x01003099, 0x0000307C, 0x2100309A, 0x0000307D, 0x0000309D, 0x21003099, 0x0000309E, 
        0x000030A6, 0x21003099, 0x000030F4, 0x000030AB, 0x21003099, 0x000030AC, 0x000030AD, 0x21003099, 0x000030AE, 0x000030AF, 
        0x21003099, 0x000030B0, 0x000030B1, 0x21003099, 0x000030B2, 0x000030B3, 0x21003099, 0x000030B4, 0x000030B5, 0x21003099, 
        0x000030B6, 0x000030B7, 0x21003099, 0x000030B8, 0x000030B9, 0x21003099, 0x000030BA, 0x000030BB, 0x21003099, 0x000030BC, 
        0x000030BD, 0x21003099, 0x000030BE, 0x000030BF, 0x21003099, 0x000030C0, 0x000030C1, 0x21003099, 0x000030C2, 0x000030C4, 
        0x21003099, 0x000030C5, 0x000030C6, 0x21003099, 0x000030C7, 0x000030C8, 0x21003099, 0x000030C9, 0x000030CF, 0x01003099, 
        0x000030D0, 0x2100309A, 0x000030D1, 0x000030D2, 0x01003099, 0x000030D3, 0x2100309A, 0x000030D4, 0x000030D5, 0x01003099, 
        0x000030D6, 0x2100309A, 0x000030D7, 0x000030D8, 0x01003099, 0x000030D9, 0x2100309A, 0x000030DA, 0x000030DB, 0x01003099, 
        0x000030DC, 0x2100309A, 0x000030DD, 0x000030EF, 0x21003099, 0x000030F7, 0x000030F0, 0x21003099, 0x000030F8, 0x000030F1, 
        0x21003099, 0x000030F9, 0x000030F2, 0x21003099, 0x000030FA, 0x000030FD, 0x21003099, 0x000030FE, 0x2000349E, 0x200034B9, 
        0x200034BB, 0x200034DF, 0x20003515, 0x200036EE, 0x200036FC, 0x20003781, 0x2000382F, 0x20003862, 0x2000387C, 0x200038C7, 
        0x200038E3, 0x2000391C, 0x2000393A, 0x20003A2E, 0x20003A6C, 0x20003AE4, 0x20003B08, 0x20003B19, 0x20003B49, 0x20003B9D, 
        0x20003C18, 0x20003C4E, 0x20003D33, 0x20003D96, 0x20003EAC, 0x20003EB8, 0x20003F1B, 0x20003FFC, 0x20004008, 0x20004018, 
        0x20004039, 0x20004046, 0x20004096, 0x200040E3, 0x2000412F, 0x20004202, 0x20004227, 0x200042A0, 0x20004301, 0x20004334, 
        0x20004359, 0x200043D5, 0x200043D9, 0x2000440B, 0x2000446B, 0x2000452B, 0x2000455D, 0x20004561, 0x2000456B, 0x200045D7, 
        0x200045F9, 0x20004635, 0x200046BE, 0x200046C7, 0x20004995, 0x200049E6, 0x20004A6E, 0x20004A76, 0x20004AB2, 0x20004B33, 
        0x20004BCE, 0x20004CCE, 0x20004CED, 0x20004CF8, 0x20004D56, 0x20004E0D, 0x20004E26, 0x20004E32, 0x20004E38, 0x20004E39, 
        0x20004E3D, 0x20004E41, 0x20004E82, 0x20004E86, 0x20004EAE, 0x20004EC0, 0x20004ECC, 0x20004EE4, 0x20004F60, 0x20004F80, 
        0x20004F86, 0x20004F8B, 0x20004FAE, 0x20004FBB, 0x20004FBF, 0x20005002, 0x2000502B, 0x2000507A, 0x20005099, 0x200050CF, 
        0x200050DA, 0x200050E7, 0x20005140, 0x20005145, 0x2000514D, 0x20005154, 0x20005164, 0x20005167, 0x20005168, 0x20005169, 
        0x2000516D, 0x20005177, 0x20005180, 0x2000518D, 0x20005192, 0x20005195, 0x20005197, 0x200051A4, 0x200051AC, 0x200051B5, 
        0x200051B7, 0x200051C9, 0x200051CC, 0x200051DC, 0x200051DE, 0x200051F5, 0x20005203, 0x20005207, 0x20005217, 0x20005229, 
        0x2000523A, 0x2000523B, 0x20005246, 0x20005272, 0x20005277, 0x20005289, 0x2000529B, 0x200052A3, 0x200052B3, 0x200052C7, 
        0x200052C9, 0x200052D2, 0x200052DE, 0x200052E4, 0x200052F5, 0x200052FA, 0x20005305, 0x20005306, 0x20005317, 0x2000533F, 
        0x20005349, 0x20005351, 0x2000535A, 0x20005373, 0x20005375, 0x2000537D, 0x2000537F, 0x200053C3, 0x200053CA, 0x200053DF, 
        0x200053E5, 0x200053EB, 0x200053F1, 0x20005406, 0x2000540F, 0x2000541D, 0x20005438, 0x20005442, 0x20005448, 0x20005468, 
        0x2000549E, 0x200054A2, 0x200054BD, 0x200054F6, 0x20005510, 0x20005553, 0x20005555, 0x20005563, 0x20005584, 0x20005587, 
        0x20005599, 0x2000559D, 0x200055AB, 0x200055B3, 0x200055C0, 0x200055C2, 0x200055E2, 0x20005606, 0x20005651, 0x20005668, 
        0x20005674, 0x200056F9, 0x20005716, 0x20005717, 0x2000578B, 0x200057CE, 0x200057F4, 0x2000580D, 0x20005831, 0x20005832, 
        0x20005840, 0x2000585A, 0x2000585E, 0x200058A8, 0x200058AC, 0x200058B3, 0x200058D8, 0x200058DF, 0x200058EE, 0x200058F2, 
        0x200058F7, 0x20005906, 0x2000591A, 0x20005922, 0x20005944, 0x20005948, 0x20005951, 0x20005954, 0x20005962, 0x20005973, 
        0x200059D8, 0x200059EC, 0x20005A1B, 0x20005A27, 0x20005A62, 0x20005A66, 0x20005AB5, 0x20005B08, 0x20005B28, 0x20005B3E, 
        0x20005B85, 0x20005BC3, 0x20005BD8, 0x20005BE7, 0x20005BEE, 0x20005BF3, 0x20005BFF, 0x20005C06, 0x20005C22, 0x20005C3F, 
        0x20005C60, 0x20005C62, 0x20005C64, 0x20005C65, 0x20005C6E, 0x20005C8D, 0x20005CC0, 0x20005D19, 0x20005D43, 0x20005D50, 
        0x20005D6B, 0x20005D6E, 0x20005D7C, 0x20005DB2, 0x20005DBA, 0x20005DE1, 0x20005DE2, 0x20005DFD, 0x20005E28, 0x20005E3D, 
        0x20005E69, 0x20005E74, 0x20005EA6, 0x20005EB0, 0x20005EB3, 0x20005EB6, 0x20005EC9, 0x20005ECA, 0x20005ED2, 0x20005ED3, 
        0x20005ED9, 0x20005EEC, 0x20005EFE, 0x20005F04, 0x20005F22, 0x20005F53, 0x20005F62, 0x20005F69, 0x20005F6B, 0x20005F8B, 
        0x20005F9A, 0x20005FA9, 0x20005FAD, 0x20005FCD, 0x20005FD7, 0x20005FF5, 0x20005FF9, 0x20006012, 0x2000601C, 0x20006075, 
        0x20006081, 0x20006094, 0x200060C7, 0x200060D8, 0x200060E1, 0x20006108, 0x20006144, 0x20006148, 0x2000614C, 0x2000614E, 
        0x20006160, 0x20006168, 0x2000617A, 0x2000618E, 0x20006190, 0x200061A4, 0x200061AF, 0x200061B2, 0x200061DE, 0x200061F2, 
        0x200061F6, 0x20006200, 0x20006210, 0x2000621B, 0x2000622E, 0x20006234, 0x2000625D, 0x200062B1, 0x200062C9, 0x200062CF, 
        0x200062D3, 0x200062D4, 0x200062FC, 0x200062FE, 0x2000633D, 0x20006350, 0x20006368, 0x2000637B, 0x20006383, 0x200063A0, 
        0x200063A9, 0x200063C4, 0x200063C5, 0x200063E4, 0x2000641C, 0x20006422, 0x20006452, 0x20006469, 0x20006477, 0x2000647E, 
        0x2000649A, 0x2000649D, 0x200064C4, 0x2000654F, 0x20006556, 0x2000656C, 0x20006578, 0x20006599, 0x200065C5, 0x200065E2, 
        0x200065E3, 0x20006613, 0x20006649, 0x20006674, 0x20006688, 0x20006691, 0x2000669C, 0x200066B4, 0x200066C6, 0x200066F4, 
        0x200066F8, 0x20006700, 0x20006717, 0x2000671B, 0x20006721, 0x2000674E, 0x20006753, 0x20006756, 0x2000675E, 0x2000677B, 
        0x20006785, 0x20006797, 0x200067F3, 0x200067FA, 0x20006817, 0x2000681F, 0x20006852, 0x20006881, 0x20006885, 0x2000688E, 
        0x200068A8, 0x20006914, 0x20006942, 0x200069A3, 0x200069EA, 0x20006A02, 0x20006A13, 0x20006AA8, 0x20006AD3, 0x20006ADB, 
        0x20006B04, 0x20006B21, 0x20006B54, 0x20006B72, 0x20006B77, 0x20006B79, 0x20006B9F, 0x20006BAE, 0x20006BBA, 0x20006BBB, 
        0x20006C4E, 0x20006C67, 0x20006C88, 0x20006CBF, 0x20006CCC, 0x20006CCD, 0x20006CE5, 0x20006D16, 0x20006D1B, 0x20006D1E, 
        0x20006D34, 0x20006D3E, 0x20006D41, 0x20006D69, 0x20006D6A, 0x20006D77, 0x20006D78, 0x20006D85, 0x20006DCB, 0x20006DDA, 
        0x20006DEA, 0x20006DF9, 0x20006E1A, 0x20006E2F, 0x20006E6E, 0x20006E9C, 0x20006EBA, 0x20006EC7, 0x20006ECB, 0x20006ED1, 
        0x20006EDB, 0x20006F0F, 0x20006F22, 0x20006F23, 0x20006F6E, 0x20006FC6, 0x20006FEB, 0x20006FFE, 0x2000701B, 0x2000701E, 
        0x20007039, 0x2000704A, 0x20007070, 0x20007077, 0x2000707D, 0x20007099, 0x200070AD, 0x200070C8, 0x200070D9, 0x20007145, 
        0x20007149, 0x2000716E, 0x2000719C, 0x200071CE, 0x200071D0, 0x20007210, 0x2000721B, 0x20007228, 0x2000722B, 0x20007235, 
        0x20007250, 0x20007262, 0x20007280, 0x20007295, 0x200072AF, 0x200072C0, 0x200072FC, 0x2000732A, 0x20007375, 0x2000737A, 
        0x20007387, 0x2000738B, 0x200073A5, 0x200073B2, 0x200073DE, 0x20007406, 0x20007409, 0x20007422, 0x20007447, 0x2000745C, 
        0x20007469, 0x20007471, 0x20007485, 0x20007489, 0x20007498, 0x200074CA, 0x20007506, 0x20007524, 0x2000753B, 0x2000753E, 
        0x20007559, 0x20007565, 0x20007570, 0x200075E2, 0x20007610, 0x2000761D, 0x2000761F, 0x20007642, 0x20007669, 0x200076CA, 
        0x200076DB, 0x200076E7, 0x200076F4, 0x20007701, 0x2000771E, 0x2000771F, 0x20007740, 0x2000774A, 0x2000778B, 0x200077A7, 
        0x2000784E, 0x2000786B, 0x2000788C, 0x20007891, 0x200078CA, 0x200078CC, 0x200078FB, 0x2000792A, 0x2000793C, 0x2000793E, 
        0x20007948, 0x20007949, 0x20007950, 0x20007956, 0x2000795D, 0x2000795E, 0x20007965, 0x2000797F, 0x2000798D, 0x2000798E, 
        0x2000798F, 0x200079AE, 0x200079CA, 0x200079EB, 0x20007A1C, 0x20007A40, 0x20007A4A, 0x20007A4F, 0x20007A81, 0x20007AB1, 
        0x20007ACB, 0x20007AEE, 0x20007B20, 0x20007BC0, 0x20007BC6, 0x20007BC9, 0x20007C3E, 0x20007C60, 0x20007C7B, 0x20007C92, 
        0x20007CBE, 0x20007CD2, 0x20007CD6, 0x20007CE3, 0x20007CE7, 0x20007CE8, 0x20007D00, 0x20007D10, 0x20007D22, 0x20007D2F, 
        0x20007D5B, 0x20007D63, 0x20007DA0, 0x20007DBE, 0x20007DC7, 0x20007DF4, 0x20007E02, 0x20007E09, 0x20007E37, 0x20007E41, 
        0x20007E45, 0x20007F3E, 0x20007F72, 0x20007F79, 0x20007F7A, 0x20007F85, 0x20007F95, 0x20007F9A, 0x20007FBD, 0x20007FFA, 
        0x20008001, 0x20008005, 0x20008046, 0x20008060, 0x2000806F, 0x20008070, 0x2000807E, 0x2000808B, 0x200080AD, 0x200080B2, 
        0x20008103, 0x2000813E, 0x200081D8, 0x200081E8, 0x200081ED, 0x20008201, 0x20008204, 0x20008218, 0x2000826F, 0x20008279, 
        0x2000828B, 0x20008291, 0x2000829D, 0x200082B1, 0x200082B3, 0x200082BD, 0x200082E5, 0x200082E6, 0x2000831D, 0x20008323, 
        0x20008336, 0x20008352, 0x20008353, 0x20008363, 0x200083AD, 0x200083BD, 0x200083C9, 0x200083CA, 0x200083CC, 0x200083DC, 
        0x200083E7, 0x200083EF, 0x200083F1, 0x2000843D, 0x20008449, 0x20008457, 0x200084EE, 0x200084F1, 0x200084F3, 0x200084FC, 
        0x20008516, 0x20008564, 0x200085CD, 0x200085FA, 0x20008606, 0x20008612, 0x2000862D, 0x2000863F, 0x20008650, 0x2000865C, 
        0x20008667, 0x20008669, 0x20008688, 0x200086A9, 0x200086E2, 0x2000870E, 0x20008728, 0x2000876B, 0x20008779, 0x20008786, 
        0x200087BA, 0x200087E1, 0x20008801, 0x2000881F, 0x2000884C, 0x20008860, 0x20008863, 0x200088C2, 0x200088CF, 0x200088D7, 
        0x200088DE, 0x200088E1, 0x200088F8, 0x200088FA, 0x20008910, 0x20008941, 0x20008964, 0x20008986, 0x2000898B, 0x20008996, 
        0x20008AA0, 0x20008AAA, 0x20008ABF, 0x20008ACB, 0x20008AD2, 0x20008AD6, 0x20008AED, 0x20008AF8, 0x20008AFE, 0x20008B01, 
        0x20008B39, 0x20008B58, 0x20008B80, 0x20008B8A, 0x20008C48, 0x20008C55, 0x20008CAB, 0x20008CC1, 0x20008CC2, 0x20008CC8, 
        0x20008CD3, 0x20008D08, 0x20008D1B, 0x20008D77, 0x20008DBC, 0x20008DCB, 0x20008DEF, 0x20008DF0, 0x20008ECA, 0x20008ED4, 
        0x20008F26, 0x20008F2A, 0x20008F38, 0x20008F3B, 0x20008F62, 0x20008F9E, 0x20008FB0, 0x20008FB6, 0x20009023, 0x20009038, 
        0x20009072, 0x2000907C, 0x2000908F, 0x20009094, 0x200090CE, 0x200090DE, 0x200090F1, 0x200090FD, 0x20009111, 0x2000911B, 
        0x2000916A, 0x20009199, 0x200091B4, 0x200091CC, 0x200091CF, 0x200091D1, 0x20009234, 0x20009238, 0x20009276, 0x2000927C, 
        0x200092D7, 0x200092D8, 0x20009304, 0x2000934A, 0x200093F9, 0x20009415, 0x2000958B, 0x200095AD, 0x200095B7, 0x2000962E, 
        0x2000964B, 0x2000964D, 0x20009675, 0x20009678, 0x2000967C, 0x20009686, 0x200096A3, 0x200096B7, 0x200096B8, 0x200096C3, 
        0x200096E2, 0x200096E3, 0x200096F6, 0x200096F7, 0x20009723, 0x20009732, 0x20009748, 0x20009756, 0x200097DB, 0x200097E0, 
        0x200097FF, 0x2000980B, 0x20009818, 0x20009829, 0x2000983B, 0x2000985E, 0x200098E2, 0x200098EF, 0x200098FC, 0x20009928, 
        0x20009929, 0x200099A7, 0x200099C2, 0x200099F1, 0x200099FE, 0x20009A6A, 0x20009B12, 0x20009B6F, 0x20009C40, 0x20009C57, 
        0x20009CFD, 0x20009D67, 0x20009DB4, 0x20009DFA, 0x20009E1E, 0x20009E7F, 0x20009E97, 0x20009E9F, 0x20009EBB, 0x20009ECE, 
        0x20009EF9, 0x20009EFE, 0x20009F05, 0x20009F0F, 0x20009F16, 0x20009F3B, 0x20009F43, 0x20009F8D, 0x20009F8E, 0x20009F9C, 
        0x0000FB49, 0x030005C1, 0x00000000, 0x232005C2, 0x00000000, 0x000105D2, 0x3CC00307, 0x000105C9, 0x000105DA, 0x3CC00307, 
        0x000105E4, 0x00011099, 0x20E110BA, 0x0001109A, 0x0001109B, 0x20E110BA, 0x0001109C, 0x000110A5, 0x20E110BA, 0x000110AB, 
        0x00011131, 0x20011127, 0x0001112E, 0x00011132, 0x20011127, 0x0001112F, 0x00011347, 0x0001133E, 0x0001134B, 0x20011357, 
        0x0001134C, 0x00011382, 0x200113C9, 0x00011383, 0x00011384, 0x200113BB, 0x00011385, 0x0001138B, 0x200113C2, 0x0001138E, 
        0x00011390, 0x200113C9, 0x00011391, 0x000113C2, 0x000113C2, 0x000113C5, 0x000113B8, 0x000113C7, 0x200113C9, 0x000113C8, 
        0x000114B9, 0x000114BA, 0x000114BB, 0x000114B0, 0x000114BC, 0x200114BD, 0x000114BE, 0x000115B8, 0x200115AF, 0x000115BA, 
        0x000115B9, 0x200115AF, 0x000115BB, 0x00011935, 0x20011930, 0x00011938, 0x0001611E, 0x0001611E, 0x00016121, 0x00016129, 
        0x00016122, 0x0001611F, 0x00016123, 0x20016120, 0x00016125, 0x00016121, 0x0001611F, 0x00016126, 0x20016120, 0x00016128, 
        0x00016122, 0x2001611F, 0x00016127, 0x00016129, 0x2001611F, 0x00016124, 0x00016D63, 0x20016D67, 0x00016D69, 0x00016D67, 
        0x20016D67, 0x00016D68, 0x00016D69, 0x20016D67, 0x00016D6A, 0x0001D157, 0x3B01D165, 0x00000000, 0x0001D158, 0x3B01D165, 
        0x00000000, 0x0001D15F, 0x1B01D16E, 0x00000000, 0x1B01D16F, 0x00000000, 0x1B01D170, 0x00000000, 0x1B01D171, 0x00000000, 
        0x3B01D172, 0x00000000, 0x0001D1B9, 0x3B01D165, 0x00000000, 0x0001D1BA, 0x3B01D165, 0x00000000, 0x0001D1BB, 0x1B01D16E, 
        0x00000000, 0x3B01D16F, 0x00000000, 0x0001D1BC, 0x1B01D16E, 0x00000000, 0x3B01D16F, 0x00000000, 0x20020122, 0x2002051C, 
        0x20020525, 0x2002054B, 0x2002063A, 0x20020804, 0x200208DE, 0x20020A2C, 0x20020B63, 0x200214E4, 0x200216A8, 0x200216EA, 
        0x200219C8, 0x20021B18, 0x20021D0B, 0x20021DE4, 0x20021DE6, 0x20022183, 0x2002219F, 0x20022331, 0x200226D4, 0x20022844, 
        0x2002284A, 0x20022B0C, 0x20022BF1, 0x2002300A, 0x200232B8, 0x2002335F, 0x20023393, 0x2002339C, 0x200233C3, 0x200233D5, 
        0x2002346D, 0x200236A3, 0x200238A7, 0x20023A8D, 0x20023AFA, 0x20023CBC, 0x20023D1E, 0x20023ED1, 0x20023F5E, 0x20023F8E, 
        0x20024263, 0x200242EE, 0x200243AB, 0x20024608, 0x20024735, 0x20024814, 0x20024C36, 0x20024C92, 0x20024FA1, 0x20024FB8, 
        0x20025044, 0x200250F2, 0x200250F3, 0x20025119, 0x20025133, 0x20025249, 0x2002541D, 0x20025626, 0x2002569A, 0x200256C5, 
        0x2002597C, 0x20025AA7, 0x20025BAB, 0x20025C80, 0x20025CD0, 0x20025F86, 0x200261DA, 0x20026228, 0x20026247, 0x200262D9, 
        0x2002633E, 0x200264DA, 0x20026523, 0x200265A8, 0x200267A7, 0x200267B5, 0x20026B3C, 0x20026C36, 0x20026CD5, 0x20026D6B, 
        0x20026F2C, 0x20026FB1, 0x200270D2, 0x200273CA, 0x20027667, 0x200278AE, 0x20027966, 0x20027CA8, 0x20027ED3, 0x20027F2F, 
        0x200285D2, 0x200285ED, 0x2002872E, 0x20028BFA, 0x20028D77, 0x20029145, 0x200291DF, 0x2002921A, 0x2002940A, 0x20029496, 
        0x200295B6, 0x20029B30, 0x2002A0CE, 0x2002A105, 0x2002A20E, 0x2002A291, 0x2002A392, 0x2002A600
    };
    
    inline constexpr uint32_t normalizer::values[] = {
        0x2100AFFF, 0x2200AFFF, 0x2300A332, 0x2400AFFF, 0x2500A33B, 0x2600A33E, 0x21032346, 0x2104AFFF, 0x2204AFFF, 0x2304A349, 
        0x2404AFFF, 0x2108EFFF, 0x2208EFFF, 0x2308EFFF, 0x2408E352, 0x210CFFFF, 0x210E2FFF, 0x220E2FFF, 0x230E2355, 0x240E235E, 
        0x250E2365, 0x21137FFF, 0x22137FFF, 0x23137FFF, 0x2413736B, 0x21175FFF, 0x21196FFF, 0x22196FFF, 0x23196374, 0x24196FFF, 
        0x2519637D, 0x26196380, 0x211BE388, 0x211D6FFF, 0x221D6FFF, 0x231D638B, 0x241D6FFF, 0x2121CFFF, 0x2221CFFF, 0x2321CFFF, 
        0x2421C394, 0x2125DFFF, 0x21270FFF, 0x22270FFF, 0x23270397, 0x242703A0, 0x252703A7, 0x212C7FFF, 0x222C7FFF, 0x232C7FFF, 
        0x242C73AD, 0x21307FFF, 0x22307FFF, 0x2700AFFF, 0x27196FFF, 0x2800A3B6, 0x281963BF, 0x2900AFFF, 0x29196FFF, 0x22032FFF, 
        0x221BEFFF, 0x23032FFF, 0x231BEFFF, 0x24032FFF, 0x241BEFFF, 0x25032FFF, 0x251BEFFF, 0x2103DFFF, 0x211C9FFF, 0x2504A3C8, 
        0x251D63CD, 0x2604AFFF, 0x261D6FFF, 0x2704AFFF, 0x271D6FFF, 0x2804AFFF, 0x281D6FFF, 0x2904AFFF, 0x291D6FFF, 0x21070FFF, 
        0x211FCFFF, 0x22070FFF, 0x221FCFFF, 0x23070FFF, 0x231FCFFF, 0x24070FFF, 0x241FCFFF, 0x2107FFFF, 0x2120BFFF, 0x2508EFFF, 
        0x2521CFFF, 0x2608EFFF, 0x2621CFFF, 0x2708EFFF, 0x2721CFFF, 0x2808EFFF, 0x2821CFFF, 0x2908EFFF, 0x210ADFFF, 0x21239FFF, 
        0x210B0FFF, 0x2123EFFF, 0x210BBFFF, 0x21249FFF, 0x220BBFFF, 0x22249FFF, 0x230BBFFF, 0x23249FFF, 0x220CFFFF, 0x2225DFFF, 
        0x230CFFFF, 0x2325DFFF, 0x240CFFFF, 0x2425DFFF, 0x260E23D2, 0x262703D7, 0x270E2FFF, 0x27270FFF, 0x280E2FFF, 0x28270FFF, 
        0x21108FFF, 0x21296FFF, 0x22108FFF, 0x22296FFF, 0x23108FFF, 0x23296FFF, 0x211193DC, 0x212A73DF, 0x22119FFF, 0x222A7FFF, 
        0x23119FFF, 0x232A7FFF, 0x241193E2, 0x242A73E5, 0x21128FFF, 0x212B6FFF, 0x22128FFF, 0x222B6FFF, 0x251373E8, 0x252C73EB, 
        0x261373EE, 0x262C73F1, 0x27137FFF, 0x272C7FFF, 0x28137FFF, 0x282C7FFF, 0x29137FFF, 0x292C7FFF, 0x2A137FFF, 0x2A2C7FFF, 
        0x21163FFF, 0x212F3FFF, 0x22175FFF, 0x23307FFF, 0x23175FFF, 0x21188FFF, 0x2131CFFF, 0x22188FFF, 0x2231CFFF, 0x23188FFF, 
        0x2331CFFF, 0x290E23F7, 0x29270402, 0x2B13740D, 0x2B2C7418, 0x2A00AFFF, 0x2A196FFF, 0x2A08EFFF, 0x2921CFFF, 0x2A0E2FFF, 
        0x2A270FFF, 0x2C137FFF, 0x2C2C7FFF, 0x0136BFFF, 0x013ADFFF, 0x0236BFFF, 0x023ADFFF, 0x0336BFFF, 0x033ADFFF, 0x0436BFFF, 
        0x043ADFFF, 0x0133BFFF, 0x0137DFFF, 0x0142CFFF, 0x0142FFFF, 0x21341FFF, 0x21383FFF, 0x25070FFF, 0x251FCFFF, 0x220B0FFF, 
        0x2223EFFF, 0x2B0E2426, 0x2B270429, 0x01426FFF, 0x01429FFF, 0x21423FFF, 0x2143EFFF, 0x22239FFF, 0x26070FFF, 0x261FCFFF, 
        0x250CFFFF, 0x2525DFFF, 0x0133EFFF, 0x01380FFF, 0x22341FFF, 0x22383FFF, 0x21368FFF, 0x213AAFFF, 0x2B00AFFF, 0x2B196FFF, 
        0x2C00AFFF, 0x2C196FFF, 0x2A04AFFF, 0x2A1D6FFF, 0x2B04AFFF, 0x2B1D6FFF, 0x2B08EFFF, 0x2A21CFFF, 0x2C08EFFF, 0x2B21CFFF, 
        0x2C0E2FFF, 0x2C270FFF, 0x2D0E2FFF, 0x2D270FFF, 0x24108FFF, 0x24296FFF, 0x25108FFF, 0x25296FFF, 0x2D137FFF, 0x2D2C7FFF, 
        0x2E137FFF, 0x2E2C7FFF, 0x25119FFF, 0x252A7FFF, 0x23128FFF, 0x232B6FFF, 0x2207FFFF, 0x2220BFFF, 0x2D00A42C, 0x2D19642F, 
        0x2C04A432, 0x2C1D6435, 0x01365FFF, 0x013A7FFF, 0x0135EFFF, 0x013A0FFF, 0x2E0E2438, 0x2E27043B, 0x01438FFF, 0x0143BFFF, 
        0x24175FFF, 0x24307FFF, 0xC00E6442, 0xC00E6443, 0xC00E6444, 0xC00E6447, 0x60442FFF, 0x60443FFF, 0x60447FFF, 0x61444FFF, 
        0x60441FFF, 0x60000FFF, 0x21329448, 0x21451449, 0x60331FFF, 0x2146044A, 0x2146944B, 0x2147444C, 0x2148344D, 0x2148F44E, 
        0x2149C44F, 0x01514450, 0x22474FFF, 0x2248FFFF, 0x214B04A7, 0x214C14AA, 0x214CA4AB, 0x214D74AE, 0x0151B4AF, 0x224D7514, 
        0x214F651B, 0x214E8522, 0x224F6523, 0x21507524, 0x21527FFF, 0x22527FFF, 0x21537FFF, 0x22537FFF, 0x21534FFF, 0x2152CFFF, 
        0x2154FFFF, 0x21546FFF, 0x21555FFF, 0x22546FFF, 0x2157EFFF, 0x2156FFFF, 0x2256FFFF, 0x2156CFFF, 0x2159FFFF, 0x21587FFF, 
        0x2257EFFF, 0x2158DFFF, 0x215A2FFF, 0x215A5FFF, 0x2153EFFF, 0x21576FFF, 0x2152FFFF, 0x21567FFF, 0x2252FFFF, 0x22567FFF, 
        0x23537FFF, 0x2356FFFF, 0x215A8FFF, 0x215ABFFF, 0x2253EFFF, 0x22576FFF, 0x21543FFF, 0x2157BFFF, 0x23546FFF, 0x2357EFFF, 
        0x24546FFF, 0x2457EFFF, 0x21552FFF, 0x2158AFFF, 0x215AEFFF, 0x215B1FFF, 0x21564FFF, 0x2159CFFF, 0x22555FFF, 0x2258DFFF, 
        0x23555FFF, 0x2358DFFF, 0x24555FFF, 0x2458DFFF, 0x2155EFFF, 0x21596FFF, 0x21561FFF, 0x21599FFF, 0x2160BFFF, 0x2260BFFF, 
        0x21612FFF, 0x2360BFFF, 0x21615FFF, 0x2161EFFF, 0x21618FFF, 0x2161BFFF, 0x21633FFF, 0x2163CFFF, 0x2163FFFF, 0x61621FFF, 
        0x61624FFF, 0x61627FFF, 0x6162AFFF, 0x6162DFFF, 0x61630FFF, 0x61636FFF, 0x61639FFF, 0x2164BFFF, 0x2264BFFF, 0x61642FFF, 
        0x61645FFF, 0x61648FFF, 0x6165CFFF, 0x6165FFFF, 0x61650FFF, 0x61653FFF, 0x61656FFF, 0x61659FFF, 0x21668FFF, 0x22668FFF, 
        0x23668FFF, 0x61662FFF, 0x61665FFF, 0x2166FFFF, 0x21672FFF, 0x21677FFF, 0x22672FFF, 0x2167AFFF, 0x2167DFFF, 0x21680FFF, 
        0x22680FFF, 0x23680687, 0x01687FFF, 0x2168AFFF, 0x2168FFFF, 0x2268AFFF, 0x21692FFF, 0x22692699, 0x01699FFF, 0x23692FFF, 
        0x6169FFFF, 0x616A2FFF, 0x616A5FFF, 0x616A8FFF, 0x616ABFFF, 0x6169CFFF, 0x800816AE, 0x616AEFFF, 0x626AEFFF, 0x616C7FFF, 
        0x616CAFFF, 0x636AEFFF, 0x616B8FFF, 0x616BBFFF, 0x616BEFFF, 0x616C1FFF, 0x616C4FFF, 0x616B5FFF, 0x216CDFFF, 0x216D0FFF, 
        0x216D3FFF, 0x216D6FFF, 0x216D9FFF, 0x216DCFFF, 0x216DFFFF, 0x216E2FFF, 0x216E5FFF, 0x216E8FFF, 0x216EBFFF, 0x216EEFFF, 
        0x2E00AFFF, 0x2E196FFF, 0x2102BFFF, 0x211B7FFF, 0x2202BFFF, 0x221B7FFF, 0x2302BFFF, 0x231B7FFF, 0x01346FFF, 0x01388FFF, 
        0x2203DFFF, 0x221C9FFF, 0x2303DFFF, 0x231C9FFF, 0x2403DFFF, 0x241C9FFF, 0x2503DFFF, 0x251C9FFF, 0x2603DFFF, 0x261C9FFF, 
        0x013C8FFF, 0x013CDFFF, 0x023C8FFF, 0x023CDFFF, 0x2D04AFFF, 0x2D1D6FFF, 0x2E04AFFF, 0x2E1D6FFF, 0x01432FFF, 0x01435FFF, 
        0x2106DFFF, 0x211F9FFF, 0x27070FFF, 0x271FCFFF, 0x2307FFFF, 0x2320BFFF, 0x2407FFFF, 0x2420BFFF, 0x2507FFFF, 0x2520BFFF, 
        0x2607FFFF, 0x2620BFFF, 0x2707FFFF, 0x2720BFFF, 0x2D08EFFF, 0x2C21CFFF, 0x01352FFF, 0x01394FFF, 0x230B0FFF, 0x2323EFFF, 
        0x240B0FFF, 0x2423EFFF, 0x250B0FFF, 0x2523EFFF, 0x240BB6F1, 0x242496F4, 0x016F1FFF, 0x016F4FFF, 0x250BBFFF, 0x25249FFF, 
        0x260BBFFF, 0x26249FFF, 0x210C8FFF, 0x21256FFF, 0x220C8FFF, 0x22256FFF, 0x230C8FFF, 0x23256FFF, 0x260CFFFF, 0x2625DFFF, 
        0x270CFFFF, 0x2725DFFF, 0x280CFFFF, 0x2825DFFF, 0x290CFFFF, 0x2925DFFF, 0x0235EFFF, 0x023A0FFF, 0x0335EFFF, 0x033A0FFF, 
        0x013D2FFF, 0x013D7FFF, 0x023D2FFF, 0x023D7FFF, 0x21103FFF, 0x21291FFF, 0x22103FFF, 0x22291FFF, 0x26108FFF, 0x26296FFF, 
        0x271086F7, 0x272966FA, 0x016F7FFF, 0x016FAFFF, 0x28108FFF, 0x28296FFF, 0x26119FFF, 0x262A7FFF, 0x271196FD, 0x272A7700, 
        0x013DCFFF, 0x013DFFFF, 0x013E2FFF, 0x013E5FFF, 0x016FDFFF, 0x01700FFF, 0x24128FFF, 0x242B6FFF, 0x25128FFF, 0x252B6FFF, 
        0x26128FFF, 0x262B6FFF, 0x27128FFF, 0x272B6FFF, 0x2F137FFF, 0x2F2C7FFF, 0x30137FFF, 0x302C7FFF, 0x31137FFF, 0x312C7FFF, 
        0x013E8FFF, 0x013EBFFF, 0x013EEFFF, 0x013F1FFF, 0x2115EFFF, 0x212EEFFF, 0x2215EFFF, 0x222EEFFF, 0x22163FFF, 0x222F3FFF, 
        0x23163FFF, 0x232F3FFF, 0x24163FFF, 0x242F3FFF, 0x25163FFF, 0x252F3FFF, 0x26163FFF, 0x262F3FFF, 0x21170FFF, 0x21302FFF, 
        0x22170FFF, 0x22302FFF, 0x25175FFF, 0x25307FFF, 0x24188FFF, 0x2431CFFF, 0x25188FFF, 0x2531CFFF, 0x26188FFF, 0x2631CFFF, 
        0x2820BFFF, 0x282B6FFF, 0x272F3FFF, 0x26307FFF, 0x213F4FFF, 0x2F00A703, 0x2F196708, 0x3000AFFF, 0x30196FFF, 0x01332FFF, 
        0x01374FFF, 0x02332FFF, 0x02374FFF, 0x03332FFF, 0x03374FFF, 0x04332FFF, 0x04374FFF, 0x01703FFF, 0x01708FFF, 0x013B6FFF, 
        0x013BFFFF, 0x023B6FFF, 0x023BFFFF, 0x033B6FFF, 0x033BFFFF, 0x043B6FFF, 0x043BFFFF, 0x02703FFF, 0x02708FFF, 0x2F04A70D, 
        0x2F1D6710, 0x3004AFFF, 0x301D6FFF, 0x3104AFFF, 0x311D6FFF, 0x01349FFF, 0x0138BFFF, 0x02349FFF, 0x0238BFFF, 0x03349FFF, 
        0x0338BFFF, 0x04349FFF, 0x0438BFFF, 0x0170DFFF, 0x01710FFF, 0x2E08EFFF, 0x2D21CFFF, 0x2F08EFFF, 0x2E21CFFF, 0x2F0E2713, 
        0x2F270716, 0x300E2FFF, 0x30270FFF, 0x01355FFF, 0x01397FFF, 0x02355FFF, 0x02397FFF, 0x03355FFF, 0x03397FFF, 0x04355FFF, 
        0x04397FFF, 0x01713FFF, 0x01716FFF, 0x013F7FFF, 0x01402FFF, 0x023F7FFF, 0x02402FFF, 0x033F7FFF, 0x03402FFF, 0x043F7FFF, 
        0x04402FFF, 0x053F7FFF, 0x05402FFF, 0x32137FFF, 0x322C7FFF, 0x33137FFF, 0x332C7FFF, 0x0140DFFF, 0x01418FFF, 0x0240DFFF, 
        0x02418FFF, 0x0340DFFF, 0x03418FFF, 0x0440DFFF, 0x04418FFF, 0x0540DFFF, 0x05418FFF, 0x26175FFF, 0x27307FFF, 0x27175FFF, 
        0x28307FFF, 0x28175FFF, 0x29307FFF, 0x29175FFF, 0x2A307FFF, 0x224B0719, 0x234B0722, 0x0171972B, 0x0172272E, 0x02719731, 
        0x02722734, 0x03719737, 0x0372273A, 0x2245173D, 0x23451746, 0x0173D74F, 0x01746752, 0x0273D755, 0x02746758, 0x0373D75B, 
        0x0374675E, 0x224C1761, 0x234C1766, 0x01761FFF, 0x01766FFF, 0x02761FFF, 0x02766FFF, 0x2246076B, 0x23460770, 0x0176BFFF, 
        0x01770FFF, 0x0276BFFF, 0x02770FFF, 0x224CA775, 0x234CA77E, 0x01775787, 0x0177E78A, 0x0277578D, 0x0277E790, 0x03775793, 
        0x0377E796, 0x22469799, 0x234697A2, 0x017997AB, 0x017A27AE, 0x027997B1, 0x027A27B4, 0x037997B7, 0x037A27BA, 0x234D77BD, 
        0x244D77C4, 0x017BDFFF, 0x017C4FFF, 0x027BDFFF, 0x027C4FFF, 0x037BDFFF, 0x037C4FFF, 0x234747CB, 0x244747D2, 0x017CBFFF, 
        0x017D2FFF, 0x027CBFFF, 0x027D2FFF, 0x037CBFFF, 0x037D2FFF, 0x224E87D9, 0x234E87DE, 0x017D9FFF, 0x017DEFFF, 0x027D9FFF, 
        0x027DEFFF, 0x224837E3, 0x234837E8, 0x017E3FFF, 0x017E8FFF, 0x027E3FFF, 0x027E8FFF, 0x234F67ED, 0x244F67F4, 0x017EDFFF, 
        0x017F4FFF, 0x027EDFFF, 0x027F4FFF, 0x037EDFFF, 0x037F4FFF, 0x2348F7FB, 0x017FBFFF, 0x027FBFFF, 0x037FBFFF, 0x22507802, 
        0x2350780B, 0x01802814, 0x0180B817, 0x0280281A, 0x0280B81D, 0x03802820, 0x0380B823, 0x2249C826, 0x2349C82F, 0x01826838, 
        0x0182F83B, 0x0282683E, 0x0282F841, 0x03826844, 0x0382F847, 0x244B084A, 0x404A7FFF, 0x244C1FFF, 0x404AAFFF, 0x244CA84D, 
        0x404ABFFF, 0x254D7FFF, 0x404AEFFF, 0x244E8FFF, 0x40522FFF, 0x254F6FFF, 0x40523FFF, 0x24507850, 0x40524FFF, 0x04719FFF, 
        0x04722FFF, 0x0172BFFF, 0x0172EFFF, 0x01731FFF, 0x01734FFF, 0x01737FFF, 0x0173AFFF, 0x0473DFFF, 0x04746FFF, 0x0174FFFF, 
        0x01752FFF, 0x01755FFF, 0x01758FFF, 0x0175BFFF, 0x0175EFFF, 0x04775FFF, 0x0477EFFF, 0x01787FFF, 0x0178AFFF, 0x0178DFFF, 
        0x01790FFF, 0x01793FFF, 0x01796FFF, 0x04799FFF, 0x047A2FFF, 0x017ABFFF, 0x017AEFFF, 0x017B1FFF, 0x017B4FFF, 0x017B7FFF, 
        0x017BAFFF, 0x04802FFF, 0x0480BFFF, 0x01814FFF, 0x01817FFF, 0x0181AFFF, 0x0181DFFF, 0x01820FFF, 0x01823FFF, 0x04826FFF, 
        0x0482FFFF, 0x01838FFF, 0x0183BFFF, 0x0183EFFF, 0x01841FFF, 0x01844FFF, 0x01847FFF, 0x254B0FFF, 0x264B0FFF, 0x0184AFFF, 
        0x274B0FFF, 0x014A7FFF, 0x284B0853, 0x01853FFF, 0x24451FFF, 0x25451FFF, 0x26451FFF, 0x40449FFF, 0x27451FFF, 0x604D7FFF, 
        0x22329FFF, 0x0184DFFF, 0x254CAFFF, 0x014ABFFF, 0x264CA85D, 0x0185DFFF, 0x24460FFF, 0x4044AFFF, 0x24469FFF, 0x4044BFFF, 
        0x25469FFF, 0x21856FFF, 0x22856FFF, 0x23856FFF, 0x264D7FFF, 0x274D7FFF, 0x02514FFF, 0x40450FFF, 0x284D7FFF, 0x03514FFF, 
        0x25474FFF, 0x26474FFF, 0x27474FFF, 0x4044CFFF, 0x21863FFF, 0x22863FFF, 0x23863FFF, 0x264F6FFF, 0x274F6FFF, 0x0251BFFF, 
        0x404AFFFF, 0x214F1FFF, 0x224F1FFF, 0x284F6FFF, 0x0351BFFF, 0x2448FFFF, 0x2548FFFF, 0x2648FFFF, 0x4044EFFF, 0x2148CFFF, 
        0x23329FFF, 0x40448FFF, 0x60195FFF, 0x01850FFF, 0x25507FFF, 0x01524FFF, 0x26507860, 0x01860FFF, 0x24483FFF, 0x4044DFFF, 
        0x2449CFFF, 0x4044FFFF, 0x2549CFFF, 0x60330FFF, 0x6086AFFF, 0x6086BFFF, 0x6049CFFF, 0x600B0FFF, 0x4033EFFF, 0x2186CFFF, 
        0x2186FFFF, 0x21872FFF, 0x21875FFF, 0x2187BFFF, 0x21878FFF, 0x2187EFFF, 0x21881FFF, 0x21884FFF, 0x21887FFF, 0x2188AFFF, 
        0x2188DFFF, 0x21890FFF, 0x21893FFF, 0x21896FFF, 0x21004FFF, 0x2189CFFF, 0x21899FFF, 0x21001FFF, 0x21007FFF, 0x2189FFFF, 
        0x218A2FFF, 0x218A5FFF, 0x218A8FFF, 0x218ABFFF, 0x218AEFFF, 0x218B1FFF, 0x218B4FFF, 0x218BDFFF, 0x218C0FFF, 0x218C3FFF, 
        0x218C6FFF, 0x218CFFFF, 0x218D2FFF, 0x218D5FFF, 0x218D8FFF, 0x218B7FFF, 0x218BAFFF, 0x218C9FFF, 0x218CCFFF, 0x218DBFFF, 
        0x218DEFFF, 0x218E1FFF, 0x218E4FFF, 0x608EAFFF, 0x608EBFFF, 0x618E7FFF, 0x218EFFFF, 0x218F2FFF, 0x218F5FFF, 0x218F8FFF, 
        0x218FBFFF, 0x218FEFFF, 0x21901FFF, 0x21904FFF, 0x21907FFF, 0x2190AFFF, 0x2190DFFF, 0x21910FFF, 0x21913FFF, 0x21916FFF, 
        0x21919FFF, 0x2191CFFF, 0x2291CFFF, 0x21921FFF, 0x22921FFF, 0x21926FFF, 0x22926FFF, 0x2192BFFF, 0x2292BFFF, 0x21930FFF, 
        0x22930FFF, 0x218ECFFF, 0x21935FFF, 0x2193BFFF, 0x2193EFFF, 0x21941FFF, 0x21944FFF, 0x21947FFF, 0x2194AFFF, 0x2194DFFF, 
        0x21950FFF, 0x21953FFF, 0x21956FFF, 0x21959FFF, 0x2195CFFF, 0x2195FFFF, 0x21962FFF, 0x21965FFF, 0x21968FFF, 0x22968FFF, 
        0x2196DFFF, 0x2296DFFF, 0x21972FFF, 0x22972FFF, 0x21977FFF, 0x22977FFF, 0x2197CFFF, 0x2297CFFF, 0x21938FFF, 0x21981FFF, 
        0x21984FFF, 0x21987FFF, 0x2198AFFF, 0x2198DFFF, 0x60C34FFF, 0x60AEFFFF, 0x60C42FFF, 0x60C39FFF, 0x60B35FFF, 0x609D5FFF, 
        0x60A28FFF, 0x60CA7FFF, 0x60CA7FFF, 0x60A60FFF, 0x60C5DFFF, 0x60A3BFFF, 0x60A5FFFF, 0x60ABEFFF, 0x60B7AFFF, 0x60BC7FFF, 
        0x60C05FFF, 0x60C12FFF, 0x60C1EFFF, 0x60C50FFF, 0x60B09FFF, 0x60B20FFF, 0x60B48FFF, 0x60B62FFF, 0x60BF7FFF, 0x60C58FFF, 
        0x60C8DFFF, 0x609DAFFF, 0x60A22FFF, 0x60B0EFFF, 0x60B50FFF, 0x60C04FFF, 0x60C98FFF, 0x60A81FFF, 0x60B3CFFF, 0x60C00FFF, 
        0x60C22FFF, 0x60AC6FFF, 0x60BD8FFF, 0x60C15FFF, 0x60A93FFF, 0x60AF2FFF, 0x60B26FFF, 0x60B5AFFF, 0x60C52FFF, 0x609E2FFF, 
        0x60A00FFF, 0x60A16FFF, 0x60ADEFFF, 0x60B0CFFF, 0x60B4FFFF, 0x60B7DFFF, 0x60BCCFFF, 0x60C02FFF, 0x60C07FFF, 0x60C40FFF, 
        0x60C7BFFF, 0x60C91FFF, 0x60C97FFF, 0x60B88FFF, 0x60B97FFF, 0x60BBAFFF, 0x60BF0FFF, 0x60C64FFF, 0x60C99FFF, 0x60C2BFFF, 
        0x60A57FFF, 0x60A99FFF, 0x60BABFFF, 0x60BD2FFF, 0x60B55FFF, 0x60B8AFFF, 0x60C38FFF, 0x60C79FFF, 0x60A56FFF, 0x60A79FFF, 
        0x60B0AFFF, 0x60B2BFFF, 0x60B37FFF, 0x60BB7FFF, 0x60BC0FFF, 0x60C6CFFF, 0x60A15FFF, 0x60BD3FFF, 0x60A03FFF, 0x60A02FFF, 
        0x60B9EFFF, 0x60BBBFFF, 0x60BF6FFF, 0x60C6EFFF, 0x60C32FFF, 0x60AC7FFF, 0x60B09FFF, 0x60C2EFFF, 0x609D7FFF, 0x60A71FFF, 
        0x60AA7FFF, 0x60B5EFFF, 0x60B74FFF, 0x60A1CFFF, 0x60B8CFFF, 0x609E6FFF, 0x60AA1FFF, 0x609D3FFF, 0x60B1CFFF, 0x60AE2FFF, 
        0x60BB6FFF, 0x60A25FFF, 0x60A52FFF, 0x60B7FFFF, 0x60BF8FFF, 0x60C27FFF, 0x60B16FFF, 0x60C4AFFF, 0x60B1AFFF, 0x60ACBFFF, 
        0x60BE6FFF, 0x60AD1FFF, 0x60B73FFF, 0x609DCFFF, 0x609F5FFF, 0x60A01FFF, 0x60B01FFF, 0x60BB2FFF, 0x60BDEFFF, 0x60C2AFFF, 
        0x60C5CFFF, 0x60A18FFF, 0x60A2FFFF, 0x60A63FFF, 0x60A97FFF, 0x60AE4FFF, 0x60B3DFFF, 0x60B8DFFF, 0x60C69FFF, 0x60C8FFFF, 
        0x60C9AFFF, 0x60C9DFFF, 0x60A10FFF, 0x60AEEFFF, 0x60B12FFF, 0x60C48FFF, 0x60A8DFFF, 0x60AB8FFF, 0x60ABFFFF, 0x60ADCFFF, 
        0x60B39FFF, 0x60B4AFFF, 0x60B6BFFF, 0x60B9CFFF, 0x60BBDFFF, 0x60BD0FFF, 0x60C44FFF, 0x60BFAFFF, 0x60C4CFFF, 0x60C65FFF, 
        0x60A08FFF, 0x60A11FFF, 0x60A34FFF, 0x60B47FFF, 0x60C19FFF, 0x60C27FFF, 0x60A92FFF, 0x60AA5FFF, 0x60ACFFFF, 0x60B15FFF, 
        0x60BAAFFF, 0x60B5CFFF, 0x609DFFFF, 0x60A47FFF, 0x60A71FFF, 0x60A86FFF, 0x60AA8FFF, 0x60B61FFF, 0x60B68FFF, 0x60BC9FFF, 
        0x60BCEFFF, 0x60C5EFFF, 0x60C78FFF, 0x60C7CFFF, 0x60C82FFF, 0x609E3FFF, 0x60B9BFFF, 0x60C5AFFF, 0x60C74FFF, 0x60AAEFFF, 
        0x609DBFFF, 0x609ECFFF, 0x60A72FFF, 0x60A77FFF, 0x60AE3FFF, 0x60B09FFF, 0x60B4DFFF, 0x60B79FFF, 0x60BFDFFF, 0x60C4FFFF, 
        0x60CA5FFF, 0x60AEAFFF, 0x60C6BFFF, 0x60A0FFFF, 0x60AF9FFF, 0x60AFCFFF, 0x60B24FFF, 0x60B31FFF, 0x60B64FFF, 0x60B72FFF, 
        0x60B87FFF, 0x60BB5FFF, 0x60C85FFF, 0x609F6FFF, 0x60AC2FFF, 0x60C6FFFF, 0x609E8FFF, 0x60A7FFFF, 0x60B2CFFF, 0x60C45FFF, 
        0x60A9FFFF, 0x60AB0FFF, 0x60AFEFFF, 0x60B5EFFF, 0x60C71FFF, 0x60A09FFF, 0x60A2CFFF, 0x60A7BFFF, 0x60AE7FFF, 0x60AF5FFF, 
        0x60B04FFF, 0x60B1EFFF, 0x60B63FFF, 0x60B75FFF, 0x60BC5FFF, 0x60C1AFFF, 0x60C1DFFF, 0x60C5BFFF, 0x60C76FFF, 0x60A1DFFF, 
        0x60B32FFF, 0x60A2DFFF, 0x60B4EFFF, 0x60B6CFFF, 0x60C01FFF, 0x60C72FFF, 0x60C93FFF, 0x60C9BFFF, 0x60AFBFFF, 0x60B2AFFF, 
        0x60BD9FFF, 0x60BA4FFF, 0x60BA6FFF, 0x60BADFFF, 0x60B59FFF, 0x60B45FFF, 0x60C31FFF, 0x609DDFFF, 0x60BEAFFF, 0x60A0AFFF, 
        0x60A07FFF, 0x60A8EFFF, 0x60AC8FFF, 0x60BB0FFF, 0x60A6EFFF, 0x60B21FFF, 0x60AEDFFF, 0x60C47FFF, 0x60C16FFF, 0x60C6DFFF, 
        0x60C24FFF, 0x60A95FFF, 0x609EEFFF, 0x60A40FFF, 0x60A51FFF, 0x60AE9FFF, 0x60A04FFF, 0x60B5BFFF, 0x60B7BFFF, 0x60B8EFFF, 
        0x60B95FFF, 0x60B96FFF, 0x60B9AFFF, 0x60C7DFFF, 0x60BAEFFF, 0x60BCAFFF, 0x60C03FFF, 0x60C2DFFF, 0x60C4DFFF, 0x60C55FFF, 
        0x60C87FFF, 0x60C88FFF, 0x60C89FFF, 0x60C96FFF, 0x60C53FFF, 0x60C73FFF, 0x609E4FFF, 0x609EDFFF, 0x609F0FFF, 0x60A14FFF, 
        0x60A17FFF, 0x60A1FFFF, 0x60A3DFFF, 0x60A43FFF, 0x60A45FFF, 0x60A50FFF, 0x60A53FFF, 0x60A7AFFF, 0x60A7CFFF, 0x60AABFFF, 
        0x60AB5FFF, 0x60AB7FFF, 0x60ABDFFF, 0x60ADFFFF, 0x60AE5FFF, 0x60AEBFFF, 0x60B02FFF, 0x60B27FFF, 0x60B2EFFF, 0x60B38FFF, 
        0x60B4BFFF, 0x60B52FFF, 0x60B65FFF, 0x60B89FFF, 0x60B8FFFF, 0x60B91FFF, 0x60B90FFF, 0x60B92FFF, 0x60B93FFF, 0x60B94FFF, 
        0x60B98FFF, 0x60B99FFF, 0x60B9FFFF, 0x60BA2FFF, 0x60BA7FFF, 0x60BBDFFF, 0x60BBFFFF, 0x60BC1FFF, 0x60BC4FFF, 0x60BCDFFF, 
        0x60BDAFFF, 0x60BDFFFF, 0x60BDFFFF, 0x60BF9FFF, 0x60C20FFF, 0x60C25FFF, 0x60C2FFFF, 0x60C30FFF, 0x60C3AFFF, 0x60C3BFFF, 
        0x60C4BFFF, 0x60C4DFFF, 0x60C77FFF, 0x60C80FFF, 0x60C84FFF, 0x60AA9FFF, 0x60D53FFF, 0x60BDDFFF, 0x609D4FFF, 0x609FFFFF, 
        0x609F4FFF, 0x609E1FFF, 0x609EFFFF, 0x609F8FFF, 0x60A13FFF, 0x60A19FFF, 0x60A3DFFF, 0x60A38FFF, 0x60A3CFFF, 0x60A42FFF, 
        0x60A51FFF, 0x60A55FFF, 0x60A5EFFF, 0x60A61FFF, 0x60A68FFF, 0x60A6CFFF, 0x60A94FFF, 0x60A96FFF, 0x60A9DFFF, 0x60AA2FFF, 
        0x60AADFFF, 0x60AB3FFF, 0x60AAFFFF, 0x60AB7FFF, 0x60AB4FFF, 0x60ABDFFF, 0x60AC3FFF, 0x60AD3FFF, 0x60AD6FFF, 0x60AD8FFF, 
        0x60AE0FFF, 0x60AE9FFF, 0x60AF2FFF, 0x60AF3FFF, 0x60AF7FFF, 0x60B13FFF, 0x60B16FFF, 0x60B24FFF, 0x60B36FFF, 0x60B34FFF, 
        0x60B38FFF, 0x60B3FFFF, 0x60B4BFFF, 0x60B85FFF, 0x60B53FFF, 0x60B58FFF, 0x60B5BFFF, 0x60B69FFF, 0x60B6EFFF, 0x60B70FFF, 
        0x60B77FFF, 0x60B78FFF, 0x60B7BFFF, 0x60B7CFFF, 0x60B7EFFF, 0x60B83FFF, 0x60B82FFF, 0x60B8BFFF, 0x60BA3FFF, 0x60BA7FFF, 
        0x60BACFFF, 0x60BB8FFF, 0x60BBDFFF, 0x60BC3FFF, 0x60BCDFFF, 0x60BEBFFF, 0x60BF5FFF, 0x60C10FFF, 0x60C21FFF, 0x60C23FFF, 
        0x60C25FFF, 0x60C28FFF, 0x60C2DFFF, 0x60C29FFF, 0x60C2FFFF, 0x60C2EFFF, 0x60C2CFFF, 0x60C30FFF, 0x60C33FFF, 0x60C3BFFF, 
        0x60C46FFF, 0x60C4EFFF, 0x60C59FFF, 0x60C60FFF, 0x60C70FFF, 0x60C77FFF, 0x60C7DFFF, 0x60C7EFFF, 0x60C80FFF, 0x60C81FFF, 
        0x60C84FFF, 0x60C90FFF, 0x60CA7FFF, 0x60D3EFFF, 0x60D3DFFF, 0x60D47FFF, 0x609A5FFF, 0x609AFFFF, 0x609B0FFF, 0x60D61FFF, 
        0x60D6AFFF, 0x60D82FFF, 0x60CA4FFF, 0x60CA6FFF, 0x615D4FFF, 0x61608FFF, 0x615FEFFF, 0x625FEFFF, 0x41CA8FFF, 0x42CA8FFF, 
        0x615B4FFF, 0x625B4FFF, 0x635B4FFF, 0x615BBFFF, 0x615C0FFF, 0x615C3FFF, 0x615C6FFF, 0x615C9FFF, 0x615CEFFF, 0x615D1FFF, 
        0x625D4FFF, 0x615D9FFF, 0x615DCFFF, 0x615E1FFF, 0x615E4FFF, 0x615E7FFF, 0x615EAFFF, 0x615EDFFF, 0x615F0FFF, 0x615F5FFF, 
        0x615F8FFF, 0x615FBFFF, 0x635FECA8, 0x61605FFF, 0x625C9FFF, 0x625BBFFF, 0x625DCFFF, 0x625F0FFF, 0x21CADFFF, 0x21CB0FFF, 
        0x21CB3FFF, 0x21CB6FFF, 0x21CB9FFF, 0x21CBCFFF, 0x21CBFFFF, 0x21CC2FFF, 0x22CC2FFF, 0x21CC7FFF, 0x21CCAFFF, 0x21CCDFFF, 
        0x21CD0FFF, 0xC0000CD3, 0x61CD3FFF, 0x62CD3FFF, 0x63CD3FFF, 0x21CDAFFF, 0x22CDAFFF, 0x23CDAFFF, 0x21CE1FFF, 0x21CE4FFF, 
        0x21CE7FFF, 0xC0000CEA, 0x61CEACF3, 0x62CEACF8, 0x63CEAFFF, 0x61CFBFFF, 0x64CEAFFF, 0x41CF3FFF, 0x41CF8FFF, 0x42CF3FFF, 
        0xC0000CFB, 0xC0000D01, 0x61D01FFF, 0x21CFED04, 0x01D04FFF, 0x61D07FFF, 0x61D0AD0D, 0x41D0DFFF, 0x42D0DFFF, 0x43D0DFFF, 
        0x44D0DFFF, 0x45D0DFFF, 0x61D18D1E, 0x61D1BD23, 0x41D1EFFF, 0x41D23FFF, 0x42D1EFFF, 0x42D23FFF, 0x609D8FFF, 0x609D6FFF, 
        0x609D9FFF, 0x60D28FFF, 0x609E0FFF, 0x609E4FFF, 0x609E5FFF, 0x609E7FFF, 0x609E9FFF, 0x609EAFFF, 0x609EDFFF, 0x609EBFFF, 
        0x60990FFF, 0x60D2CFFF, 0x609F0FFF, 0x609F1FFF, 0x609F2FFF, 0x609F7FFF, 0x60D29FFF, 0x60991FFF, 0x609F3FFF, 0x609F9FFF, 
        0x60D2BFFF, 0x609FCFFF, 0x609FDFFF, 0x609DEFFF, 0x609FEFFF, 0x609FFFFF, 0x60D8AFFF, 0x60A05FFF, 0x60A06FFF, 0x60993FFF, 
        0x60A0BFFF, 0x60A0CFFF, 0x60A0DFFF, 0x60A0EFFF, 0x60994FFF, 0x60A13FFF, 0x60A14FFF, 0x60A17FFF, 0x60A19FFF, 0x60A1AFFF, 
        0x60A1BFFF, 0x60A1CFFF, 0x60A1EFFF, 0x60A1FFFF, 0x60A20FFF, 0x60A21FFF, 0x60A23FFF, 0x60A24FFF, 0x60A24FFF, 0x60A24FFF, 
        0x60D2FFFF, 0x60B42FFF, 0x60A26FFF, 0x60A27FFF, 0x60D30FFF, 0x60A29FFF, 0x60A2AFFF, 0x60A2BFFF, 0x60A32FFF, 0x60A2EFFF, 
        0x60A30FFF, 0x60A31FFF, 0x60A33FFF, 0x60A35FFF, 0x60A36FFF, 0x60A37FFF, 0x60A39FFF, 0x60A3AFFF, 0x60A3AFFF, 0x60A3CFFF, 
        0x60A3EFFF, 0x60A3FFFF, 0x60A41FFF, 0x60A48FFF, 0x60A43FFF, 0x60A49FFF, 0x60A44FFF, 0x60A46FFF, 0x60A07FFF, 0x60A58FFF, 
        0x60A4BFFF, 0x60A4CFFF, 0x60A4DFFF, 0x60A4AFFF, 0x60A4FFFF, 0x60A4EFFF, 0x60A54FFF, 0x60D31FFF, 0x60A59FFF, 0x60A5AFFF, 
        0x60A5BFFF, 0x60A5CFFF, 0x60A5DFFF, 0x60A62FFF, 0x60D32FFF, 0x60D33FFF, 0x60A65FFF, 0x60A66FFF, 0x60A67FFF, 0x60A64FFF, 
        0x60A69FFF, 0x60995FFF, 0x60996FFF, 0x60A6BFFF, 0x60A6DFFF, 0x60A6DFFF, 0x60D34FFF, 0x60A6FFFF, 0x60A70FFF, 0x60A71FFF, 
        0x60A73FFF, 0x60D35FFF, 0x60A74FFF, 0x60A75FFF, 0x60A9BFFF, 0x60A76FFF, 0x60997FFF, 0x60A78FFF, 0x60A7CFFF, 0x60A7EFFF, 
        0x60A7DFFF, 0x60D37FFF, 0x60A80FFF, 0x60D38FFF, 0x60A83FFF, 0x60A82FFF, 0x60A84FFF, 0x60A87FFF, 0x60A88FFF, 0x60998FFF, 
        0x60A89FFF, 0x60A8AFFF, 0x60A8BFFF, 0x60A8CFFF, 0x60999FFF, 0x60D39FFF, 0x6099AFFF, 0x60A8FFFF, 0x60A90FFF, 0x60A91FFF, 
        0x60A93FFF, 0x60D94FFF, 0x60A98FFF, 0x60D3BFFF, 0x60D3BFFF, 0x60BDBFFF, 0x60A9AFFF, 0x60A9AFFF, 0x6099BFFF, 0x60D42FFF, 
        0x60D6CFFF, 0x60A9CFFF, 0x60A9EFFF, 0x6099CFFF, 0x60AA0FFF, 0x60AA3FFF, 0x60AA4FFF, 0x60AA6FFF, 0x60AAAFFF, 0x6099EFFF, 
        0x6099DFFF, 0x60AABFFF, 0x60D3CFFF, 0x60AACFFF, 0x60AB1FFF, 0x60AB2FFF, 0x60AB3FFF, 0x60AB2FFF, 0x60AB6FFF, 0x60AB7FFF, 
        0x60ABBFFF, 0x60AB9FFF, 0x60ABAFFF, 0x60ABCFFF, 0x60ABDFFF, 0x60ABEFFF, 0x60AC0FFF, 0x60AC1FFF, 0x60AC4FFF, 0x60AC5FFF, 
        0x60AC9FFF, 0x60ACDFFF, 0x60D3FFFF, 0x60ACCFFF, 0x60ACAFFF, 0x60ACEFFF, 0x60AD0FFF, 0x60AD5FFF, 0x60D40FFF, 0x60AD7FFF, 
        0x60AD4FFF, 0x60AD2FFF, 0x6099FFFF, 0x60AD9FFF, 0x60ADBFFF, 0x60ADDFFF, 0x60ADAFFF, 0x609A0FFF, 0x60ADFFFF, 0x60AE1FFF, 
        0x60D41FFF, 0x60AE6FFF, 0x60AF0FFF, 0x60AE8FFF, 0x609A3FFF, 0x60AEBFFF, 0x609A2FFF, 0x609A1FFF, 0x609FAFFF, 0x609FBFFF, 
        0x60AF1FFF, 0x60AECFFF, 0x60BD4FFF, 0x609BCFFF, 0x60AF2FFF, 0x60AF3FFF, 0x60AF4FFF, 0x60AF8FFF, 0x60AF6FFF, 0x60D46FFF, 
        0x609A4FFF, 0x60AFDFFF, 0x60AFAFFF, 0x60B00FFF, 0x60B02FFF, 0x60D48FFF, 0x60B03FFF, 0x60AFFFFF, 0x60B05FFF, 0x609A5FFF, 
        0x60B06FFF, 0x60B07FFF, 0x60B08FFF, 0x60B0BFFF, 0x60D49FFF, 0x60B0DFFF, 0x609A6FFF, 0x60B0FFFF, 0x60D4AFFF, 0x60B10FFF, 
        0x609A7FFF, 0x60B11FFF, 0x60B14FFF, 0x60B16FFF, 0x60B17FFF, 0x60D4BFFF, 0x60D36FFF, 0x60D4CFFF, 0x60B18FFF, 0x60D4DFFF, 
        0x60B1BFFF, 0x60B1DFFF, 0x60B19FFF, 0x60B1FFFF, 0x60B23FFF, 0x60B27FFF, 0x60B24FFF, 0x60B25FFF, 0x60B28FFF, 0x60B29FFF, 
        0x60D4EFFF, 0x60B22FFF, 0x60B2FFFF, 0x60B30FFF, 0x609A8FFF, 0x60B34FFF, 0x60B33FFF, 0x60D4FFFF, 0x60B2DFFF, 0x60B3AFFF, 
        0x60D50FFF, 0x60D51FFF, 0x60B3BFFF, 0x60B40FFF, 0x60B3FFFF, 0x60B3EFFF, 0x609A9FFF, 0x60B41FFF, 0x60B44FFF, 0x60B43FFF, 
        0x60B46FFF, 0x60D2AFFF, 0x60B49FFF, 0x60D52FFF, 0x60B4CFFF, 0x60D54FFF, 0x60B51FFF, 0x60B53FFF, 0x60B54FFF, 0x60D55FFF, 
        0x60B56FFF, 0x60B57FFF, 0x60D56FFF, 0x60D57FFF, 0x60B5DFFF, 0x60B5FFFF, 0x609AAFFF, 0x60B60FFF, 0x609ABFFF, 0x609ABFFF, 
        0x60B66FFF, 0x60B67FFF, 0x60B69FFF, 0x60B6AFFF, 0x60B6DFFF, 0x609ACFFF, 0x60B6FFFF, 0x60D58FFF, 0x60B71FFF, 0x60D59FFF, 
        0x60B74FFF, 0x60D3AFFF, 0x60B76FFF, 0x60D5AFFF, 0x60D5BFFF, 0x60D5CFFF, 0x609ADFFF, 0x609AEFFF, 0x60B7EFFF, 0x60D5EFFF, 
        0x60D5DFFF, 0x60D5FFFF, 0x60D60FFF, 0x60B80FFF, 0x60B81FFF, 0x60B81FFF, 0x60B83FFF, 0x609B0FFF, 0x60B84FFF, 0x609B1FFF, 
        0x609B2FFF, 0x60D62FFF, 0x60B86FFF, 0x60B88FFF, 0x60B8BFFF, 0x609B3FFF, 0x60D63FFF, 0x60B93FFF, 0x60D64FFF, 0x60D65FFF, 
        0x60B9AFFF, 0x60B9DFFF, 0x609B4FFF, 0x60B9FFFF, 0x60BA0FFF, 0x60BA1FFF, 0x60D66FFF, 0x60D67FFF, 0x60D67FFF, 0x60BA5FFF, 
        0x609B5FFF, 0x60D68FFF, 0x60BA8FFF, 0x60BA9FFF, 0x609B6FFF, 0x60D69FFF, 0x60BAFFFF, 0x609B7FFF, 0x60BB3FFF, 0x60BB1FFF, 
        0x60BB4FFF, 0x60D6BFFF, 0x60BB9FFF, 0x609B8FFF, 0x60BBCFFF, 0x60BBEFFF, 0x60BC2FFF, 0x609B9FFF, 0x60D6DFFF, 0x60D6EFFF, 
        0x609BAFFF, 0x60D6FFFF, 0x60BC6FFF, 0x60D70FFF, 0x60BC8FFF, 0x60BCBFFF, 0x60BCDFFF, 0x60D71FFF, 0x60D72FFF, 0x60BCFFFF, 
        0x60D73FFF, 0x60BD1FFF, 0x60D43FFF, 0x609BBFFF, 0x60BD5FFF, 0x60BD6FFF, 0x609BDFFF, 0x60BD7FFF, 0x60A6AFFF, 0x60D74FFF, 
        0x60D75FFF, 0x60D44FFF, 0x60D45FFF, 0x60BDBFFF, 0x60BDCFFF, 0x60C49FFF, 0x609BEFFF, 0x60BE1FFF, 0x60BE0FFF, 0x60BE2FFF, 
        0x60A12FFF, 0x60BE3FFF, 0x60BE4FFF, 0x60BE5FFF, 0x60BE7FFF, 0x60D76FFF, 0x60BE6FFF, 0x60BE8FFF, 0x60BEDFFF, 0x60BEEFFF, 
        0x60BE9FFF, 0x60BEFFFF, 0x60BF4FFF, 0x60BF9FFF, 0x60BECFFF, 0x60BF1FFF, 0x60BF2FFF, 0x60BF3FFF, 0x60D77FFF, 0x60D79FFF, 
        0x60D78FFF, 0x609BFFFF, 0x60BFBFFF, 0x60BFCFFF, 0x60BFEFFF, 0x60D7DFFF, 0x60BFFFFF, 0x60D7AFFF, 0x609C0FFF, 0x609C1FFF, 
        0x60D7BFFF, 0x60D7CFFF, 0x609C2FFF, 0x60C06FFF, 0x60C07FFF, 0x60C08FFF, 0x60C09FFF, 0x60C0BFFF, 0x60C0AFFF, 0x60C0DFFF, 
        0x60C0CFFF, 0x60C10FFF, 0x60C0EFFF, 0x60C0FFFF, 0x60C11FFF, 0x609C3FFF, 0x60C13FFF, 0x60C14FFF, 0x609C4FFF, 0x60C17FFF, 
        0x60C18FFF, 0x60D7EFFF, 0x60C1BFFF, 0x60C1CFFF, 0x609C5FFF, 0x60C1FFFF, 0x60992FFF, 0x60D7FFFF, 0x60D80FFF, 0x609C6FFF, 
        0x609C7FFF, 0x60C26FFF, 0x60C2CFFF, 0x60C33FFF, 0x60C35FFF, 0x60D81FFF, 0x60C36FFF, 0x60C37FFF, 0x60C3CFFF, 0x60C3DFFF, 
        0x60D83FFF, 0x60D2DFFF, 0x60C3FFFF, 0x60C3EFFF, 0x60C41FFF, 0x60D2EFFF, 0x60C43FFF, 0x60C46FFF, 0x60D84FFF, 0x60D85FFF, 
        0x60C51FFF, 0x60C54FFF, 0x60C56FFF, 0x60D86FFF, 0x60C57FFF, 0x60C5FFFF, 0x60C62FFF, 0x60C63FFF, 0x60C61FFF, 0x60C66FFF, 
        0x60C67FFF, 0x60D87FFF, 0x60C68FFF, 0x609C8FFF, 0x60C6AFFF, 0x60D88FFF, 0x609C9FFF, 0x60C75FFF, 0x60A85FFF, 0x60C7AFFF, 
        0x60D89FFF, 0x60D8BFFF, 0x609CAFFF, 0x609CBFFF, 0x60C7FFFF, 0x60D8CFFF, 0x609CCFFF, 0x60D8DFFF, 0x60C81FFF, 0x60C81FFF, 
        0x60C83FFF, 0x60D8EFFF, 0x60C86FFF, 0x609CDFFF, 0x60C8AFFF, 0x60C8BFFF, 0x60C8CFFF, 0x60C8EFFF, 0x609CEFFF, 0x60D8FFFF, 
        0x60C90FFF, 0x60C92FFF, 0x60C94FFF, 0x609CFFFF, 0x609D0FFF, 0x60C95FFF, 0x60D90FFF, 0x609D1FFF, 0x60D91FFF, 0x60D92FFF, 
        0x60D93FFF, 0x60C9CFFF, 0x609D2FFF, 0x60C9EFFF, 0x60C9FFFF, 0x60CA0FFF, 0x60CA1FFF, 0x60CA2FFF, 0x60CA3FFF, 0x60D95FFF, 
        0x80000001, 0x80000004, 0x80000007, 0x8000000A, 0x8000002B, 0x80000032, 0x8000003D, 0x8000004A, 0x8000006D, 0x80000070, 
        0x8000007F, 0x8000008E, 0x800000AD, 0x800000B0, 0x800000BB, 0x800000C8, 0x800000CF, 0x800000E2, 0x80000103, 0x80000108, 
        0x80000119, 0x80000128, 0x80000137, 0x8000015E, 0x80000163, 0x80000170, 0x80000175, 0x80000188, 0x80000196, 0x800001B7, 
        0x800001BE, 0x800001C9, 0x800001D6, 0x800001F9, 0x800001FC, 0x8000020B, 0x8000021C, 0x80000239, 0x8000023E, 0x80000249, 
        0x80000256, 0x8000025D, 0x80000270, 0x80000291, 0x80000296, 0x800002A7, 0x800002B6, 0x800002C7, 0x800002EE, 0x800002F3, 
        0x80000302, 0x80000307, 0x8000031C, 0x80000329, 0x80000341, 0x80000368, 0x80000383, 0x800003AA, 0x800003F4, 0x80000423, 
        0x8000043E, 0x80000451, 0x80000460, 0x80000469, 0x80000474, 0x80000483, 0x8000048C, 0x8000048F, 0x8000049C, 0x800004B0, 
        0x800004C1, 0x800004CA, 0x800004D7, 0x800004E8, 0x800004F1, 0x800004F6, 0x80000507, 0x80000527, 0x8000052C, 0x8000052F, 
        0x80000534, 0x80000537, 0x8000053E, 0x80000543, 0x80000546, 0x8000054F, 0x80000552, 0x80000555, 0x8000055E, 0x80000561, 
        0x80000564, 0x80000567, 0x8000056C, 0x8000056F, 0x80000576, 0x8000057B, 0x8000057E, 0x80000587, 0x8000058A, 0x8000058D, 
        0x80000596, 0x80000599, 0x8000059C, 0x8000059F, 0x800005A2, 0x800005A5, 0x800005A8, 0x800005AB, 0x800005AE, 0x800005B1, 
        0x800005B4, 0x800005BB, 0x800005C0, 0x800005C3, 0x800005C6, 0x800005C9, 0x800005CE, 0x800005D1, 0x800005D4, 0x800005D9, 
        0x800005DC, 0x800005E1, 0x800005E4, 0x800005E7, 0x800005EA, 0x800005ED, 0x800005F0, 0x800005F5, 0x800005F8, 0x800005FB, 
        0x800005FE, 0x80000605, 0x80000608, 0x8000060B, 0x80000612, 0x80000615, 0x80000618, 0x8000061B, 0x8000061E, 0x80000621, 
        0x80000624, 0x80000627, 0x8000062A, 0x8000062D, 0x80000630, 0x80000633, 0x80000636, 0x80000639, 0x8000063C, 0x8000063F, 
        0x80000642, 0x80000645, 0x80000648, 0x8000064B, 0x80000650, 0x80000653, 0x80000656, 0x80000659, 0x8000065C, 0x8000065F, 
        0x80000662, 0x80000665, 0x80000668, 0x8000066F, 0x80000672, 0x80000677, 0x8000067A, 0x8000067D, 0x80000680, 0x8000068A, 
        0x8000068F, 0x80000692, 0x8000069C, 0x8000069F, 0x800006A2, 0x800006A5, 0x800006A8, 0x800006AB, 0x800006B5, 0x800006B8, 
        0x800006BB, 0x800006BE, 0x800006C1, 0x800006C4, 0x800006C7, 0x800006CA, 0x800006CD, 0x800006D0, 0x800006D3, 0x800006D6, 
        0x800006D9, 0x800006DC, 0x800006DF, 0x800006E2, 0x800006E5, 0x800006E8, 0x800006EB, 0x800006EE, 0x80000856, 0x80000863, 
        0x8000086C, 0x8000086F, 0x80000872, 0x80000875, 0x80000878, 0x8000087B, 0x8000087E, 0x80000881, 0x80000884, 0x80000887, 
        0x8000088A, 0x8000088D, 0x80000890, 0x80000893, 0x80000896, 0x80000899, 0x8000089C, 0x8000089F, 0x800008A2, 0x800008A5, 
        0x800008A8, 0x800008AB, 0x800008AE, 0x800008B1, 0x800008B4, 0x800008B7, 0x800008BA, 0x800008BD, 0x800008C0, 0x800008C3, 
        0x800008C6, 0x800008C9, 0x800008CC, 0x800008CF, 0x800008D2, 0x800008D5, 0x800008D8, 0x800008DB, 0x800008DE, 0x800008E1, 
        0x800008E4, 0x800008E7, 0x800008EC, 0x800008EF, 0x800008F2, 0x800008F5, 0x800008F8, 0x800008FB, 0x800008FE, 0x80000901, 
        0x80000904, 0x80000907, 0x8000090A, 0x8000090D, 0x80000910, 0x80000913, 0x80000916, 0x80000919, 0x8000091C, 0x80000921, 
        0x80000926, 0x8000092B, 0x80000930, 0x80000935, 0x80000938, 0x8000093B, 0x8000093E, 0x80000941, 0x80000944, 0x80000947, 
        0x8000094A, 0x8000094D, 0x80000950, 0x80000953, 0x80000956, 0x80000959, 0x8000095C, 0x8000095F, 0x80000962, 0x80000965, 
        0x80000968, 0x8000096D, 0x80000972, 0x80000977, 0x8000097C, 0x80000981, 0x80000984, 0x80000987, 0x8000098A, 0x8000098D, 
        0x80000CAD, 0x80000CB0, 0x80000CB3, 0x80000CB6, 0x80000CB9, 0x80000CBC, 0x80000CBF, 0x80000CC2, 0x80000CC7, 0x80000CCA, 
        0x80000CCD, 0x80000CD0, 0x80000CDA, 0x80000CE1, 0x80000CE4, 0x80000CE7, 0x80000CFE, 0x80000D07, 0x80000D0A, 0x80000D18, 
        0x80000D1B
    };
    
    inline constexpr std::array<uint16_t, 128> normalizer::lookup::ascii({{
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0812, 0x0812, 0x0812, 0x0000, 0x0000, 0x0814, 0x0814, 0x0814, 0x0814, 0x0814, 0x0814, 0x0814, 0x0814, 0x0814, 0x0814, 
        0x0814, 0x0814, 0x0814, 0x0814, 0x0814, 0x0814, 0x0000, 0x0815, 0x0815, 0x0815, 0x0815, 0x0815, 0x0815, 0x0815, 0x0815, 
        0x0815, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x081B, 0x081B, 0x081B, 0x081B, 0x081B, 0x081B, 0x081B, 0x081B, 
        0x081B, 0x081B, 0x081B, 0x081B, 0x081B, 0x081B, 0x081B, 0x081B, 0x0000, 0x081C, 0x081C, 0x081C, 0x081C, 0x081C, 0x081C, 
        0x081C, 0x081C, 0x081C, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
    }});
    
    inline constexpr std::array<uint8_t, 2048> normalizer::lookup::stage1({{
        22, 46, 49, 42, 41, 40, 30, 32, 37,  5,  0, 10, 21, 39, 33, 51, 52, 45,  0,  0,  0, 16, 13,  0, 36,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24, 44, 18,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0, 11, 50,  0, 38,  1, 15,  3,  0,  0, 43,  8, 20, 25, 34, 26, 29, 31, 35, 28,  7,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  0,  0,  0, 
         0, 12,  4,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 23,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6, 27,  0,  0,  0,  0,  0,  0, 
        53, 19, 14,  0, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 47,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
         0,  0,  0,  0,  0,  0,  0,  0
    }});
    
    inline constexpr std::array<std::array<uint16_t, 32>, 54> normalizer::lookup::stage2({{
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  16}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 254}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  48, 140, 108,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 120,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  19,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  70, 221, 265, 268,   0,  39,  44, 224,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 167,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 116,   0,   0,   0,  56,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   5, 303,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 114,   0, 113,   0,   0,   0,   0,   0,   0,   0,   0,   0, 145,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 253,   0,   0,   0, 292,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   3, 276,   0,   0,   0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0, 297, 297}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  27,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   9,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 266,   0,   0,   0,   0,   0,   0,   0,   0,  94,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 269,   0,   0,   0,   0,   0,   0, 117,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 274, 171,   0,   0,  89,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  15,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   0,   0,   0,  26,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  34,   0,   0,   0,   0,  18,   0,   0,   0,   0,  92, 266,   0,   0, 149,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  55,   0,   0,   0,   0,   0,   0,   0,   0,  54,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  57,   0, 176, 152, 177, 153, 178, 128, 179, 180, 119, 181, 182, 183,   0,   0, 184, 185,  12, 186, 187, 188}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   7,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   3, 118,   0,  10,   0,   0,   0,   0,   0, 276,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,  89,   0,   0, 257,   0,  43, 101,  52,   0,   0,   0,   0, 284,   0,  78, 160,   0,   0,   0, 125,   0,   0,   0,   0, 258,   0,   0,   0}},
        {{  0,   0,   0,   0, 146,   8,   0,   0,   0,   0,   0, 301, 147,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4,  58, 255,   0,   0,   0}},
        {{  0,   0,   0,   0, 150,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 144,   0,   0,   0,   0,  74,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   0,  90,   0,   0,   0,   0,   0,   0,  30,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,  22,  83, 103,   0,   0,   0,   0,   0,  23, 220, 107,   0,   0,   0,   0,   0,  31,  84,  77,   0,   0,   0,   0,   0,   0,  41,  42,   0,   0}},
        {{  0,   0,   0,  51,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 302, 125,   0,   0,   0,   0,   0,   0,   0,   0,   0, 256,   0}},
        {{  0,   0,   0,  60,  61,   0,   0,   0,   0,   0,   0,  62,  63,   0,   0,   0,   0,  64,   0, 104, 213, 158,  46, 165, 263, 214, 159, 133,  91,   0,   0,   0}},
        {{  0,   0,   0,  72,   0,   0,   0,   0,   0,   0,   0,   0,   0, 285, 273, 115,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 283, 264, 297, 298}},
        {{  0,   0,   0, 102,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  49,   0,   0,   0,   0,  29,  71,  77,  95, 290, 137, 219,   0,  66, 142, 256,   0,   0}},
        {{  0,   0,   0, 113,  74,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,  38,   0,  85, 241, 242, 243,   0, 111,  86, 244, 245, 246,   1, 215,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0, 100,  73,   0,   0,   0,   0,  16,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 174, 305,   0,   0,  67, 306, 304,   0,   0,   0}},
        {{  0,   0, 294,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,  75,   0,   0,   0,   0, 256, 106,   0,   0,   0, 291, 271,   0,   0,   0,  98, 155,   0, 112, 218,   0,  33, 288,   0,   0,  37,   0,   0,   0,  88, 148}},
        {{  0,  81,  28, 129,  15,  45,   0,   0,   0,   0,   0,  22,  15,   0,   0,   0,   0,   0, 156,  24,  69,  96,   0,   0,   0, 130,   0,  11,  82,  77,   0,   0}},
        {{  0,  93, 289,   0,   0,  53,   0,   0,   0,  76,   0,   0,  40, 270, 277, 260,   0,  97, 151, 175,  15, 173,   0,   0,   0,   0, 154,  24,  68,  79,   0,   8}},
        {{  0, 295, 132,   0,  59, 261,   0, 262,   0,   0,   0,   0, 210, 143, 287,   0,   0, 169,   0, 275, 282,   0,   0,   0,   0,   0,   0,   0,   0,   0,  35, 281}},
        {{ 17,   0,   0,  65,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 105,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{ 89,   0,  25,   0,   0,   0,   0,   0,   0,   0,   0,   0, 113,   0, 297, 276,   0,   0,  32,   0,   0, 126,   0,   0,   0,   0,   0, 125, 256,   0,   0,   0}},
        {{121,   0, 122,  21, 166,   0, 249, 250, 251, 161, 134, 135,   0,   0, 252,   0,   0,   0,  47,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{190, 189,  80, 191,   0,   0,   0,   0,   0, 136,   0,   0,   0,   0,   0,   0, 194, 299, 267, 307, 195, 286, 300, 109,  99, 196, 162, 197, 163, 138,   0,   0}},
        {{193, 192,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193}},
        {{198, 222, 123, 223, 124, 199,   0, 110, 127,   0,   0,   0, 157, 200, 131, 201,   0,   0,   0,   0,   0,   0,   0,   0,   0, 170, 280, 259, 168, 247, 248, 141}},
        {{202, 204, 205, 207, 207, 207, 206, 209, 209, 209, 209, 209, 209, 208,   0,   0,   0,  13,  36, 211, 212,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{226, 226, 226, 226, 226, 226, 226, 226, 226, 225, 228, 228, 228, 228, 228, 227, 230, 229, 232, 232, 231, 233, 235, 234, 237, 237, 237, 236, 164, 238, 239, 139}},
        {{240,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 279, 172, 272,   0,   0,  87,   0,   0,   0,   0,   0,   0, 216,   6,   0,  14, 217,   0,   0}},
        {{293, 296, 278,   0,   0,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 292,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}}
    }});
    
    inline constexpr std::array<std::array<uint16_t, 16>, 308> normalizer::lookup::stage3({{
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 179}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 439}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 483}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 487}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 252, 487}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 407,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 436,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 483,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 483, 478}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 483, 483}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 487,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  31,  31,  31}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 106, 456, 107}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 408, 408, 408}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 439,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 478,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 478,   0, 483}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 478, 478, 478}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 483, 483, 483}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 133,  43,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 211,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 438,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 438,   0,   0,  91}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 438,   0, 487,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 439,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 483, 483, 483, 483}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 484, 484, 478, 483}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  40,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 438, 438,   0, 487,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 439,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 439, 439,   0, 487,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 478, 478, 478,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 483, 478, 483, 483, 483}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 483, 483,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 483, 483, 483, 483, 483}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 110, 110, 110, 110, 110, 110}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 439, 439,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 477, 482, 484, 479, 480, 480}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 483, 483, 483, 483,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 483, 483, 483, 483, 483, 478}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 491,   0,   0,   0,   0, 487}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,  96, 296,   0, 297, 297, 297, 487}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0, 154, 247, 155, 248,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0, 188, 188, 286, 286, 286, 286, 286}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0, 203, 203, 203,   0,   0, 204,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0, 391,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0, 432, 432,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0, 435,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0, 438, 439,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0, 439,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0, 439, 438,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0, 439, 489,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0, 478, 478, 478,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0, 479, 483, 478,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0, 482,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0, 483, 483, 483, 483, 483,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0, 187,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0, 300, 300, 433, 433,   0,   0,   0, 439}},
        {{  0,   0,   0,   0,   0,   0,   0,   0, 377,   0, 378, 457, 458, 459, 460, 461}},
        {{  0,   0,   0,   0,   0,   0,   0,   0, 468, 468, 439,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0, 469, 469, 469, 469,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0, 470, 470, 439,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0, 471, 471, 471, 471,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0, 478, 478,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0, 483, 436, 478,   0,   0,   0,   0, 439}},
        {{  0,   0,   0,   0,   0,   0,   0,   0, 487,   0,   0, 487,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0, 487, 487, 487, 487, 487, 487, 487, 487}},
        {{  0,   0,   0,   0,   0,   0,   0,  29,   0,   0,   0, 189, 189, 439,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,  63, 236,   0,   0, 237, 237, 439,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0, 185, 185,   0,   0,   0,   0,   0, 269, 269}},
        {{  0,   0,   0,   0,   0,   0,   0, 239,   0,   0,   0, 375, 375, 439,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0, 438,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0, 438,   0, 439, 439,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0, 439,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0, 483, 478,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0, 483, 483, 478, 478, 478, 483, 483, 483, 483}},
        {{  0,   0,   0,   0,   0,   0,   0, 487,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0, 487,   0,   0,   0,   0,   0,   0, 265, 265}},
        {{  0,   0,   0,   0,   0,   0,   0, 487,   0,   0,   0,   0, 190, 190,   0, 191}},
        {{  0,   0,   0,   0,   0,   0,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37}},
        {{  0,   0,   0,   0,   0,   0,  38,  38,   0,   0,   0,   0,  39,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,  76,  76,   0,   0, 246, 246, 246, 439,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,  83,   0, 250,   0,   0,   0,   0, 439,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,  94,  94,   0,   0, 292, 292, 292, 439,   0,   0}},
        {{  0,   0,   0,   0,   0,   0, 132,   0,   0,   0,   0, 134, 323, 135, 324, 136}},
        {{  0,   0,   0,   0,   0,   0, 158,   0,   0,   0,   0, 159, 345, 160, 346, 161}},
        {{  0,   0,   0,   0,   0,   0, 381,   0,   0,   0, 382, 382,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0, 438,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0, 439,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0, 439, 438,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0, 478,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0, 478, 478, 483, 483, 483, 478, 483, 478, 478, 478}},
        {{  0,   0,   0,   0,   0,   0, 483, 483, 483, 483,   0, 483, 483, 483, 483, 483}},
        {{  0,   0,   0,   0,   0,   0, 483, 483, 483, 483, 483,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0, 483, 483, 483, 483, 483, 483, 483,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0, 487, 487,   0,   0,   0,   0, 238, 238,   0,   0}},
        {{  0,   0,   0,   0,   0,   2,   2,   2,   0,   0,   0,   0,   3,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,  48, 219,  49, 220,  50, 221,  51, 222,  52, 223,   0}},
        {{  0,   0,   0,   0,   0,  64,  64,  64,  64,  64,  64,   0,  65,   0,  66,  66}},
        {{  0,   0,   0,   0,   0, 149, 412,   0,   0,   0,   0,   0,   0,   0, 487,   0}},
        {{  0,   0,   0,   0,   0, 156,   0,   0,   0,   0,   0, 249,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0, 439, 438,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0, 467, 492,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0, 478,   0, 478,   0, 476,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0, 483, 478,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0, 483, 483, 483, 483, 483, 483, 483, 483,   0,   0, 478}},
        {{  0,   0,   0,   0,   0, 487, 487,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,  61,   0,   0,   0,   0,   0,   0,   0,   0,   0,  62,   0}},
        {{  0,   0,   0,   0, 293, 293,  82,  82,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0, 343,   0,   0,   0,   0, 490, 490,   0,   0, 157, 344,   0}},
        {{  0,   0,   0,   0, 438, 487,   0,   0,   0,   0,  56, 232,  57, 233,  58,  58}},
        {{  0,   0,   0,   0, 439,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0, 439, 439,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0, 483,   0,   0,   0, 483, 483,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0, 483, 483, 483, 483,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0, 483, 483, 483, 483, 483, 483, 438,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483,   0,   0}},
        {{  0,   0,   0,  22,  22,  22,  22,  22,  22,   0,   0,   0,  23,  23,  23,  23}},
        {{  0,   0,   0,  93,   0,   0,   0, 157, 157, 157, 157,   0,   0,   0,   0,   0}},
        {{  0,   0,   0, 205, 409,   0,   0,   0, 206, 410,   0, 207, 411,   0,   0,   0}},
        {{  0,   0,   0, 209, 413, 210, 414,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0, 277,   0,   0,   0, 278,   0,   0,   0, 279,   0, 280,   0,   0}},
        {{  0,   0,   0, 287,   0,   0,   0, 288,   0,   0,   0, 289,   0, 290,   0,   0}},
        {{  0,   0,   0, 438,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0, 439,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0, 483, 483, 483, 483, 483,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18}},
        {{  0,   0,  41, 198,   0,   0, 199,   0,  42,   0,   0,   0, 438,   0,   0,   0}},
        {{  0,   0,  70,   0, 240,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,  88,  88,  88,  88,  88,  88, 295, 295,  89,  89,  89,  89,  89,  89}},
        {{  0,   0,  95,  95,  95,  95,  95, 376,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0, 130, 130,   0,   0,   0,   0,   0, 406,   0,   0,   0,   0,   0,   0}},
        {{  0,   0, 228,   0,   0,   0,   0,   0, 229, 229,   0, 230, 429, 429, 429, 429}},
        {{  0,   0, 231, 231, 231, 231,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0, 241,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0, 242, 379, 243, 380,   0,   0,   0,   0,   0, 244,   0,   0, 382,   0}},
        {{  0,   0, 268,  69,  69,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0, 318, 318, 318,   0, 319, 319, 319, 319, 319, 319, 319, 319, 131,   0}},
        {{  0,   0, 321,   0,   0,   0,   0,   0,   0,   0, 322,   0,   0,   0,   0,   0}},
        {{  0,   0, 374,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0, 389,   0,   0, 390,   0, 391, 391, 487,   0,   0,   0,   0, 439, 439}},
        {{  0,   0, 404, 116,   0, 405, 483, 483, 483, 483, 483, 483, 483,   0,   0, 483}},
        {{  0,   0, 438,   0, 439, 439,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0, 439,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 483,   0,   0}},
        {{  0,   0, 439,   0,   0,   0, 438,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0, 439, 438,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0, 439, 439,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0, 483, 478, 483, 478,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0, 483, 483, 483,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   4,   4,   0,   0,   0,   0,   0,   5, 181,   0,   6,   0,   0,   0,   7}},
        {{  0,  11,  11,  11,  11,  11,  11,   0, 192,  12,  12,  12,  12,  12,   0,   0}},
        {{  0,  15,  15,  15,  15,  15,  15,   0, 194,  16,  16,  16,  16,  16,   0,  17}},
        {{  0,  26,  26,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  27}},
        {{  0,  53, 224,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,  60,  60,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,  84,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0, 120, 386,   0,   0,   0, 121, 387,   0,   0,   0, 122, 388,   0,   0,   0}},
        {{  0, 126, 399,   0,   0,   0, 127, 401,   0,   0,   0, 128, 402,   0,   0,   0}},
        {{  0, 184, 184, 439, 439,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0, 227, 227,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0, 256,   0,   0,   0, 257,   0,   0,   0, 258,  67,  67,  67,  67,  67,  67}},
        {{  0, 264,   0,   0,   0, 266,   0,   0,   0, 267,  68,  68,  68,  68,  68,   0}},
        {{  0, 314, 314, 314, 314,   0, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315}},
        {{  0, 392, 472, 393, 473, 394, 394,   0, 395,   0, 472, 472, 472, 472,   0,   0}},
        {{  0, 415,   0, 212, 416, 213,   0, 417, 214, 418,   0,   0,   0, 215,   0,   0}},
        {{  0, 439, 439,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0, 454, 455,   0, 483, 478,   0, 448,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0, 466,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0, 478, 483, 483, 483, 483, 478, 483, 483, 483, 479, 478, 483, 483, 483, 483}},
        {{  0, 483,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0, 483,   0,   0,   0, 436, 436, 483, 478, 483, 436, 436, 478, 478, 478, 478}},
        {{  0, 483, 478, 483, 483,   0,   0,   0, 186, 186, 186, 186, 186, 186, 186, 186}},
        {{  0, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487}},
        {{  7, 182,   0,   8, 183,   0,   0,   0,   0,   0,   0,   0, 489,   0,   0,   0}},
        {{  9,   9,   9,   9,   9,   9, 189,  10,  10,  10,  10,  10,  10,  10,  10,  10}},
        {{ 13,  13,  13,  13,  13,  13, 193,  14,  14,  14,  14,  14,  14,  14,  14,  14}},
        {{ 17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17}},
        {{ 18,  18,  18,  18,  18,  18,   0,   0,  19,  19,  19,  19,  19,  19,  19,  19}},
        {{ 19,   0,   0,   0,  20,  20,  20,  20,   0,  21,  21,  21,  21,  21,  21,   0}},
        {{ 23,  23,   0,   0,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24}},
        {{ 24,  24,  24,  24,  24,  24,   0,   0,  25,  25,  25,  25,  25,  25,  25,  25}},
        {{ 25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25, 208}},
        {{ 28,  28,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  30}},
        {{ 30,   0,   0,   0,   0,   0,   0, 216,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{ 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,   0,  32,  32}},
        {{ 32,  32,  32,  32,   0,   0,  33,  33,  33,  33,  33,  33,  33,  33,  33,  33}},
        {{ 33,   0,   0,   0,  34,  34,   0,   0,  35,  35,  35,  35,  35,  35,  35,  35}},
        {{ 35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,   0,   0,  36,  36}},
        {{ 35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35}},
        {{ 37,  37,  37,  37,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{ 44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,   0,   0}},
        {{ 44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44}},
        {{ 45,  45, 496, 496, 496, 483, 496, 496,  46, 496, 496, 496, 496, 483, 483, 496}},
        {{ 54,  54, 496,  55,  55, 497, 483, 478, 478, 478, 483, 483, 483, 478, 478,   0}},
        {{ 66, 251,   0,   0,   0, 252,   0, 253,   0, 254,   0,   0,   0,   0,   0, 255}},
        {{ 67, 259,   0,   0,   0, 260,   0, 261,   0, 262,   0,   0,   0,   0,   0, 263}},
        {{ 71,  71,   0,  72,   0,   0, 271,  73,   0,   0,   0,   0,  74,  74,  74,   0}},
        {{ 78,  78,   0,  79,   0,   0, 291,  80,   0,   0,   0,   0,  81,  81,  81,   0}},
        {{ 85,  85,  85,  85,   0,   0,  86,  86, 294, 294,  87,  87,  87,  87,  87,  87}},
        {{ 89,  89,  89,  89,  89,  89,   0,   0,  90,  90,   0,   0,   0,   0,   0,   0}},
        {{ 97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,   0,   0}},
        {{ 97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97}},
        {{ 98,   0,  99,   0,   0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,   0}},
        {{101,   0, 102,   0,   0, 103, 103,   0,   0,   0, 104, 104, 104, 104, 104, 104}},
        {{104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104,   0,   0}},
        {{104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104}},
        {{105, 105, 105, 105, 105, 105, 105, 105, 105, 105,   0,   0,   0,   0,   0,   0}},
        {{105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105}},
        {{108, 400, 109,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{110, 110, 110, 110, 110, 110, 110,   0, 111, 111, 111, 111, 111,   0, 112,   0}},
        {{113, 113,   0, 114, 114,   0, 115, 115, 115, 115, 115, 115, 115, 115, 115,   0}},
        {{117,   0, 118, 384,   0,   0,   0,   0,   0,   0,   0,   0, 119, 385,   0,   0}},
        {{123,   0, 124, 397,   0,   0,   0,   0,   0,   0,   0,   0, 125, 398,   0,   0}},
        {{179, 179, 179,   0, 365,   0,   0, 366, 366, 366, 366,   0,   0, 180, 367,   0}},
        {{195,   0, 196,   0, 197,   0,   0,   0,   0,   0, 403, 403,   0,   0,   0,   0}},
        {{200,   0, 201,   0, 202,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{234, 234,  59, 235, 439,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{245, 383,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{261,   0, 487,   0,   0,   0,  92, 263, 263,   0, 264, 264,   0, 439,   0,   0}},
        {{269, 269, 269, 269, 269, 476, 476, 436, 436, 436,   0,   0,   0, 481, 476, 476}},
        {{272,   0,   0, 273,   0, 274, 274, 274, 274,  75, 275,   0,   0,   0, 276,   0}},
        {{281,   0,   0, 282,   0, 283, 283, 283, 283,  77, 284,   0,   0,   0, 285,   0}},
        {{286,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{298, 298, 298, 298, 298, 298, 298, 298, 298, 298,   0, 299,   0,   0,   0,   0}},
        {{298, 298, 298, 298, 298, 298, 298, 298, 298, 298, 298, 298, 298, 298, 298, 298}},
        {{301, 301, 301, 301, 301, 301, 301, 301, 301, 301,   0,   0,   0,   0,   0,   0}},
        {{301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301}},
        {{302, 302, 302, 302, 302, 302,   0,   0, 303, 303, 303, 303, 303, 303,   0,   0}},
        {{302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302}},
        {{304, 304, 304, 304, 304, 304,   0,   0, 305, 305, 305, 305, 305, 305,   0,   0}},
        {{304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304}},
        {{306, 306, 306, 306, 306, 306, 306, 306,   0, 307,   0, 308,   0, 309,   0, 310}},
        {{310, 310, 310, 310, 310, 310, 310, 310, 310, 310, 310, 310, 310, 310,   0,   0}},
        {{310, 310, 310, 310, 310, 310, 310, 310, 310, 310, 310, 310, 310, 310, 310, 310}},
        {{311, 311, 311, 311, 311,   0, 312, 312, 312, 312, 312, 312, 312,   0, 313, 129}},
        {{311, 311, 311, 311, 311, 311, 311, 311, 311, 311, 311, 311, 311, 311, 311, 311}},
        {{315, 315, 315, 315,   0,   0, 316, 316, 316, 316, 316, 316,   0, 317, 317, 317}},
        {{317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317}},
        {{320, 320,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{325, 137, 326, 138, 327, 139, 328, 140, 329, 141, 330, 142, 331, 143, 332, 144}},
        {{333, 145, 334,   0, 146, 335, 147, 336, 148, 337,   0,   0,   0,   0,   0, 150}},
        {{338, 338, 151, 339, 339, 152, 340, 340, 153, 341, 341, 154, 342, 342,   0,   0}},
        {{347, 162, 348, 163, 349, 164, 350, 165, 351, 166, 352, 167, 353, 168, 354, 169}},
        {{355, 170, 356,   0, 171, 357, 172, 358, 173, 359,   0,   0,   0,   0,   0, 174}},
        {{360, 360, 175, 361, 361, 176, 362, 362, 177, 363, 363, 178, 364, 364,   0,   0}},
        {{368, 368, 368, 368, 368, 368, 368,   0, 369, 369, 369, 369, 369,   0, 370,   0}},
        {{371, 371,   0, 372, 372,   0, 373, 373, 373, 373, 373,   0,   0,   0,   0,   0}},
        {{419, 217, 420,   0, 218, 218,   0,   0,   0,   0,   0,   0,   0, 423, 423, 423}},
        {{423, 423, 219, 219, 424, 424, 221, 221, 425, 425, 223, 223, 223, 223,   0,   0}},
        {{426, 426, 225, 225, 427, 427, 226, 226, 428, 428,   0,   0,   0,   0,   0,   0}},
        {{430, 430, 430, 430,   0,   0,   0,   0,   0,   0, 431, 431, 431, 431,   0,   0}},
        {{436, 436, 436, 436, 436,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{437, 437,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{438,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{439,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{439,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 439}},
        {{439,   0,   0,   0,   0,   0,   0,   0,   0,   0, 438,   0,   0,   0,   0,   0}},
        {{440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 449, 450, 451, 452,   0, 453}},
        {{457, 458, 459, 483, 483, 483, 478, 483, 483, 478, 478, 483, 483, 483, 483, 483}},
        {{462, 463, 464, 496, 496, 495, 478, 483, 483, 483, 483, 483, 478, 483, 483, 478}},
        {{465,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{472, 396, 483, 483, 439,   0, 483, 483,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{474, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483}},
        {{476, 476, 476,   0,   0,   0,   0,   0,   0,   0,   0, 478, 478, 478, 478, 478}},
        {{478,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{478, 474, 474, 495, 495, 495, 495, 493, 493, 478, 478, 478, 478, 495, 495, 478}},
        {{478, 478, 478,   0,   0, 483, 483, 483, 483, 483, 478, 478,   0,   0,   0,   0}},
        {{478, 478, 478, 478, 478, 478, 478,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{478, 478, 478, 478, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483}},
        {{478, 483, 483, 478, 478, 483, 483, 483, 483, 483, 478, 483, 483, 483, 483,   0}},
        {{483,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{483,   0, 436, 436, 436, 436, 436, 436, 436,   0,   0,   0,   0, 478,   0,   0}},
        {{483,   0, 483, 483, 478,   0,   0, 483, 483,   0,   0,   0,   0,   0, 483, 483}},
        {{483, 478, 483, 483, 478, 483, 483, 478, 478, 478, 483, 478, 478, 483, 478, 483}},
        {{483, 483,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{483, 483,   0, 478, 483, 483, 478, 483, 483, 478, 483, 483, 483, 478, 478, 478}},
        {{483, 483,   0, 483, 483,   0, 483, 483, 483, 483, 483,   0,   0,   0,   0,   0}},
        {{483, 483, 436, 436, 483, 483, 483, 483, 436, 436, 436, 483, 483,   0,   0,   0}},
        {{483, 483, 478, 478, 478, 478, 478, 478, 483, 483, 478, 483, 483, 479, 482, 483}},
        {{483, 483, 478, 483,   0,   0,   0,   0,   0,   0,   0,   0,   0, 478,   0,   0}},
        {{483, 483, 478, 483, 478, 483, 478, 483, 478, 483, 483,   0,   0,   0,   0,   0}},
        {{483, 483, 478, 483, 483, 483, 483, 483, 483, 483, 478, 483, 483, 486, 475, 478}},
        {{483, 483, 483,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{483, 483, 483,   0, 436, 478, 478, 478, 478, 478, 483, 483, 478, 478, 478, 478}},
        {{483, 483, 483, 478, 478, 478, 478, 483, 484, 478, 478, 483, 485, 486, 486, 485}},
        {{483, 483, 483, 478, 483,   0,   0, 483, 483,   0, 478, 483, 483, 478,   0,   0}},
        {{483, 483, 483, 483,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{483, 483, 483, 483,   0, 483, 483, 483,   0, 483, 483, 483, 483, 483,   0,   0}},
        {{483, 483, 483, 483, 483,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{483, 483, 483, 483, 483, 478, 478, 478, 478, 478, 478, 483, 483, 478,   0, 478}},
        {{483, 483, 483, 483, 483, 483, 483,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{483, 483, 483, 483, 483, 483, 483,   0, 483, 483, 483, 483, 483, 483, 483, 483}},
        {{483, 483, 483, 483, 483, 483, 483, 478, 478, 478, 478, 478, 478, 478, 483, 483}},
        {{483, 483, 483, 483, 483, 483, 483, 483, 460, 461, 462,   0,   0,   0,   0,   0}},
        {{483, 483, 483, 483, 483, 483, 483, 483, 483,   0,   0, 483, 483, 483, 483, 483}},
        {{483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483}},
        {{483, 483, 483, 483, 483, 483, 484, 482, 482, 478, 477, 483, 485, 478, 483, 478}},
        {{483, 496, 483,  47, 496, 484, 478, 478, 478, 478, 484, 494, 478, 478, 478, 478}},
        {{486, 486, 485, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483}},
        {{487,   0,   0,   0,   0,   0,   0,   0,   0, 270, 487, 421, 421, 487, 422,   0}},
        {{487,   0,   0,   0,   0, 434,   0,   0,  82,   0,   0,   0,   0, 439, 439,   0}},
        {{487, 254, 254, 254, 254, 254, 254, 254, 254, 254,   0,   0,   0,   0,   0, 439}},
        {{487, 487, 487,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{487, 487, 487, 487, 487, 487,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487}},
        {{495, 495, 478, 478, 436, 436, 436, 436, 488, 478, 478, 478, 478, 483, 483, 483}}
    }});
    
    inline constexpr std::array<uint16_t, 498> normalizer::lookup::stage4({{
        0x0000, 0x0009, 0x0060, 0x0064, 0x0068, 0x006D, 0x006F, 0x0072, 0x0074, 0x00C0, 0x00C1, 0x00C2, 0x00C4, 0x00C6, 0x00C7, 
        0x00C8, 0x00CA, 0x00CB, 0x00CD, 0x00CF, 0x00D2, 0x00D3, 0x00D7, 0x00DA, 0x00DC, 0x00DE, 0x00E1, 0x00ED, 0x00FF, 0x0104, 
        0x010C, 0x0128, 0x0129, 0x012B, 0x012E, 0x0130, 0x0132, 0x0138, 0x0152, 0x0156, 0x0164, 0x016A, 0x016F, 0x01C2, 0x01F4, 
        0x0204, 0x020A, 0x0214, 0x0220, 0x0221, 0x0222, 0x0223, 0x0224, 0x0227, 0x0240, 0x0241, 0x024F, 0x0250, 0x0251, 0x0253, 
        0x0257, 0x0270, 0x0279, 0x027B, 0x027F, 0x0280, 0x0281, 0x029A, 0x02B3, 0x02B7, 0x02C5, 0x02E2, 0x02E3, 0x02E6, 0x02EA, 
        0x02F4, 0x02F8, 0x0313, 0x0329, 0x032A, 0x032D, 0x0331, 0x0348, 0x0376, 0x0391, 0x039E, 0x03A0, 0x03A2, 0x03A4, 0x03A6, 
        0x03A8, 0x03EE, 0x03F4, 0x0407, 0x0473, 0x04D0, 0x0504, 0x0514, 0x0516, 0x0517, 0x0519, 0x051A, 0x051B, 0x051D, 0x0520, 
        0x0522, 0x0565, 0x0566, 0x0569, 0x056A, 0x0570, 0x0571, 0x0572, 0x0573, 0x0574, 0x0575, 0x057A, 0x066A, 0x066B, 0x0674, 
        0x0678, 0x067C, 0x0680, 0x06B4, 0x06B5, 0x06BE, 0x06C2, 0x06C6, 0x06CA, 0x06CF, 0x06D0, 0x070D, 0x072A, 0x072B, 0x072E, 
        0x072F, 0x0730, 0x0731, 0x0732, 0x0733, 0x0734, 0x0735, 0x0736, 0x0737, 0x0738, 0x0739, 0x073B, 0x073C, 0x073D, 0x0741, 
        0x0743, 0x0745, 0x0747, 0x0749, 0x074B, 0x074C, 0x0755, 0x076C, 0x0774, 0x0778, 0x0779, 0x077A, 0x077B, 0x077C, 0x077D, 
        0x077E, 0x077F, 0x0780, 0x0781, 0x0782, 0x0783, 0x0785, 0x0786, 0x0787, 0x078D, 0x078F, 0x0791, 0x0793, 0x0795, 0x07A8, 
        0x07B2, 0x07CF, 0x07D6, 0x07D8, 0x07E0, 0x07FA, 0x07FB, 0x0849, 0x085A, 0x0866, 0x0875, 0x0876, 0x0877, 0x0884, 0x0895, 
        0x089E, 0x089F, 0x08A0, 0x08C9, 0x08CB, 0x08DB, 0x08DC, 0x08DD, 0x08ED, 0x08EF, 0x090B, 0x090F, 0x0911, 0x091B, 0x0928, 
        0x0929, 0x093F, 0x0945, 0x0946, 0x0948, 0x094C, 0x0952, 0x095F, 0x0961, 0x096D, 0x096E, 0x096F, 0x0970, 0x0971, 0x0974, 
        0x0975, 0x0977, 0x0980, 0x098F, 0x0994, 0x0995, 0x099B, 0x099C, 0x099D, 0x099F, 0x09A0, 0x09D8, 0x09DA, 0x09E9, 0x09F4, 
        0x0A1F, 0x0A2C, 0x0A2E, 0x0A2F, 0x0A35, 0x0A39, 0x0A54, 0x0ABE, 0x0ABF, 0x0ACD, 0x0ACF, 0x0B2A, 0x0B2D, 0x0B2E, 0x0B2F, 
        0x0B34, 0x0B35, 0x0B38, 0x0B3B, 0x0B42, 0x0B45, 0x0B46, 0x0B47, 0x0B4C, 0x0B4D, 0x0B4F, 0x0B50, 0x0B53, 0x0B5B, 0x0B5F, 
        0x0B61, 0x0B8E, 0x0B97, 0x0B99, 0x0B9A, 0x0B9B, 0x0B9E, 0x0BA2, 0x0BA5, 0x0BA8, 0x0BA9, 0x0BAB, 0x0BAD, 0x0BAE, 0x0BAF, 
        0x0BB2, 0x0BB5, 0x0BB6, 0x0BB9, 0x0BBC, 0x0BBD, 0x0BC5, 0x0BCB, 0x0BE2, 0x0C44, 0x0C52, 0x0C58, 0x0C59, 0x0C5C, 0x0C5D, 
        0x0C5F, 0x0C61, 0x0C67, 0x0C69, 0x0C6B, 0x0C6D, 0x0C6F, 0x0C70, 0x0C71, 0x0C72, 0x0C73, 0x0C75, 0x0C76, 0x0C77, 0x0C79, 
        0x0C7A, 0x0C7C, 0x0C7D, 0x0C7F, 0x0C80, 0x0C82, 0x0C86, 0x0C8D, 0x0C9A, 0x0C9B, 0x0C9C, 0x0C9D, 0x0C9E, 0x0C9F, 0x0CA0, 
        0x0CA1, 0x0CA2, 0x0CA3, 0x0CA4, 0x0CA5, 0x0CA7, 0x0CA8, 0x0CA9, 0x0CAF, 0x0CB0, 0x0CB1, 0x0CB2, 0x0CB3, 0x0CC9, 0x0CD2, 
        0x0CDF, 0x0CE0, 0x0CE1, 0x0CE2, 0x0CE3, 0x0CE4, 0x0CE5, 0x0CE6, 0x0CE7, 0x0CE8, 0x0CE9, 0x0CEA, 0x0CEC, 0x0CED, 0x0CEE, 
        0x0CF4, 0x0CF5, 0x0CF6, 0x0CF7, 0x0CF8, 0x0D0E, 0x0D10, 0x0D13, 0x0D38, 0x0D39, 0x0D3A, 0x0D3B, 0x0D3C, 0x0D3D, 0x0D44, 
        0x0D6A, 0x0D78, 0x0D98, 0x0D99, 0x0DA0, 0x0DA1, 0x0DA6, 0x0DA9, 0x0DAB, 0x0DBD, 0x0DC6, 0x0DCA, 0x0DCE, 0x0DD2, 0x0DDB, 
        0x0DDD, 0x0DDE, 0x0DE5, 0x0DE6, 0x0DE7, 0x0DE8, 0x0DF0, 0x0E01, 0x0E0A, 0x0E0E, 0x0E0F, 0x0E12, 0x0E16, 0x0E17, 0x0E1F, 
        0x0E21, 0x0E22, 0x0E29, 0x0E47, 0x0E7B, 0x0E7F, 0x0E81, 0x0E8E, 0x0E98, 0x0E99, 0x0EB3, 0x0EB5, 0x0EB7, 0x0EB8, 0x0ECE, 
        0x0ECF, 0x0ED0, 0x0ED1, 0x0ED9, 0x0EDB, 0x0EDD, 0x0EE3, 0x0EE5, 0x0EE7, 0x0F09, 0x0F39, 0x0F3F, 0x0F7A, 0x0FCC, 0x0FDA, 
        0x0FEF, 0x1001, 0x1006, 0x1007, 0x1009, 0x100A, 0x100B, 0x100C, 0x100D, 0x100E, 0x100F, 0x1010, 0x1011, 0x1012, 0x1013, 
        0x1014, 0x1015, 0x1016, 0x1017, 0x1018, 0x1019, 0x101A, 0x101B, 0x101C, 0x101D, 0x101E, 0x101F, 0x1020, 0x1021, 0x1022, 
        0x1023, 0x1024, 0x1054, 0x1067, 0x106B, 0x1076, 0x107A, 0x1082, 0x1084, 0x10CA, 0x10D6, 0x10D8, 0x10DA, 0x10DC, 0x10DE, 
        0x10E0, 0x10E2, 0x10E4, 0x10E6, 0x10E8, 0x10E9, 0x10EA, 0x1100, 0x1101, 0x1107, 0x1108, 0x1109, 0x115B, 0x11CA, 0x11D8, 
        0x11DC, 0x11E6, 0x11F0
    }});
    

    inline constexpr std::array<uint8_t, 128> grapheme_cluster_break_prop::ascii({{
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
    }});
    
    inline constexpr std::array<uint8_t, 2048> grapheme_cluster_break_prop::stage1({{
        27,  14,  26,  53,  42,  48,  70,  31,  35,   4,   0,  11,  45,  41,  40,   0,  44,  10,  21,  63,   6,  12,  17,   0, 
         38,  25,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  28,  46,  39,  56,  61,  59,  57,  62,  60,  58,  56,  61,  59, 
         57,  62,  60,  58,  56,  61,  59,  57,  62,  60,  58,  55,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   8,   0,  72,   1,  19,   0,   0,   0,  49,   7,  23,  54,  36,  30,  33,  37,  50,  34,  15, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  32,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   9,   0,   0,   0,   0,  16,   3,   5,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,  24,   0,   0,   0,   0,   0,   0,   0,   0,  13,   2,  29,   0,   0,   0,  73,   0,   0, 
         71,  22,  18,   0,  20,   0,   0,   0,  66,  47,  67,  64,  43,  65,  69,  68,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  52,  51,  51,  51,  51,  51,  51,  51, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0
    }});
    
    inline constexpr std::array<std::array<uint16_t, 32>, 74> grapheme_cluster_break_prop::stage2({{
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  12}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  89, 255, 252,   0,  39,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 108,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  15,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   5, 131, 163, 163, 162, 251,   0,   0,   0,   0,  98, 240}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  95,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 101,   0,   0,   0,  49,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10, 278,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  35,  52,   0,   0,   0,   0,   0,  52,   0,   0,   0,   4,   0,  44, 202}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128,   0, 127,   0, 126,   0, 126,   0,   0,   0, 100, 164, 259,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81,  26,   0,   0,   0, 198,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 285, 285, 282, 285, 268,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 285, 285, 285, 285, 285, 285, 285,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 115, 245,   0,   0,  93, 251,  37,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 263,   0,   0,   0, 268,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   5, 240,   0,   0,   0,   0,   0,   0,   0,   5,   0,   0,   0,   0,   0,   0, 285, 285}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  23,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 220,   0,   0,   0,   0,   0,   0,   0,   0,  73,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 268,   0,   0,   0,   0,   0,   0, 102,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 120,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  35,  68, 195,   0,   0,  27}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   0,   0,   0,  23,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  29,   0,   0,   0,   0,  23,   0,   0,   0,   0,  76, 220,   0,   0, 128,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,  14, 154,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0,   0,  60,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0, 117,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 153, 285, 283, 144,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   0,   0, 156, 156,  42,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0,   0,   0,   5, 253,   0,  10,   0,   0,   0,   0,   0, 240,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0,  78, 173,   8,   0,   0,   0,   0,   0, 234, 236,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   5, 174, 220,  22,   0,   0}},
        {{  0,   0,   0, 140,  65,   0,   0,   0,   0,   0,   0, 141,  58,   0,   0,   0,   0,  56,   0,  85,   0,   0,   0, 152, 265, 275, 285, 281,  70,   0,   0,   0}},
        {{  0,   0,   0, 156, 221, 266,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   0, 168, 220,   0,   0,   0,   0,   0,  28, 273,   0,   0,   0,   0,   0,  13, 129,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,   3, 270,   0,   0,   0,   0,   0, 130, 274, 237,   0,   0,   0,   0,   0,   0,   0, 148, 267,   0,   0,   0,  33, 243,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,  15, 238,   0,  67, 220, 147, 125,  12,   0,   0,   0,   0,   0,   0, 183, 183, 183, 183, 183, 183, 185, 185, 185, 185, 186, 188, 188, 188, 188, 188}},
        {{  0,   0,  16, 249, 139,   0,   0,   0,   0,   0,   0,   0,   0,   5, 167,   0, 248,   0,   0,  30, 230,  61, 119, 263,   0,   0,   0,  55, 225, 228, 143,   0}},
        {{  0,   0,  16, 276,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 232, 160,   0,   0,   0,   0,   0,   0,   0,   0, 132, 222,   0}},
        {{  0,   0,  40, 197,   0,   0,   0,   0,   0,  47,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{  0,   0,  50, 177, 113,   0,   0,  19,   0,   0,   0, 227, 139,   0,  25,  79,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 106,   0}},
        {{  0,   0,  92, 262,   0,   0,   0,   0,   0,   0,   0,   0,   0, 254, 239,  99,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 285, 285, 285, 285}},
        {{  0,  63,   0,   0,   0,  80, 226, 169,   0,   0,   0, 285, 284,   0,   0,   0, 261,   0,   0, 103, 171,   0,  32, 258, 247,   0, 133,   0,   0,   0,  71, 258}},
        {{  0,  72, 260,   0,   0,  48,   0,   0,   0, 158,   0,   0,  40, 285, 246, 285, 256,  83, 217, 219, 181, 150, 126,  54, 142,  83, 215, 211, 178,  62, 126, 214}},
        {{ 18,   0,   0,   0,  53,  36,   0,   0,  53,   0,   7, 209, 209, 209, 209, 209,  18, 209, 209, 207, 205, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209}},
        {{ 24,   0,  51,  17,  41,   0, 156,   0,   0,   0,   0,   0,   0, 285, 285, 220,   0,   0, 120,  41,   0,   0,   0,   0,   0,  96,  43,   0,   0,   0,   0,   0}},
        {{ 31,   0,   0,   0,   0,   0,   0,   0,  86,   0,  46,   0,   0,   0,   0,   0,   0,   0, 257, 170,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{124,   0, 107,   0,   0,   0,   0,   0, 161,   0,   0,  91, 165,   0, 285, 241,   0,   0,  75,   0,  66, 250, 183, 182, 256,   0,   0, 114, 220,   0,  84,   0}},
        {{137,  34,   4, 121,  44, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 210}},
        {{145,   0,   0,  20, 176, 139,   0, 242, 145,  83, 215, 212, 180,   0, 126,  45, 142,  83, 215, 213, 178,  88, 126, 138, 122,   0,   0,   9, 229,  61,   0,   0}},
        {{146,   0,   0,  57,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  86,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{151,   0,   0, 116,  61, 149,   0,   0,  90, 272,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156}},
        {{156, 156, 285, 285, 285, 285, 285, 285, 156, 156, 156, 156, 156, 156, 156, 156, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 156}},
        {{159, 280,   0,   0,  32, 285,   0, 220,   0,   0,   0,   0,   0,  74, 264,   0,   2, 139,   0, 285, 279,   0,   0,   0,   0,   0,  76, 220,   0,   0,  32, 259}},
        {{175,   0,   0,  59, 268,   0,   0, 223, 247,   0,   0, 166, 123,   0,   0,   0, 251,   0,  64, 263,  77,   0,   0, 112, 247,   0,   0, 105, 224,   0,   0,   0}},
        {{189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 190, 185, 184, 188, 188, 187}},
        {{189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192}},
        {{191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193}},
        {{192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194}},
        {{193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194}},
        {{194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194}},
        {{194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189}},
        {{194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191, 194, 189, 193, 194, 192, 194, 191}},
        {{205, 201, 209, 209, 209, 209, 209, 209, 203, 209, 209, 209, 209, 209, 209, 209, 204, 200, 135, 109,  94, 110, 111,   0,   0,  81, 134, 196,   0,   0,   0,   0}},
        {{209, 209, 209, 209, 209,   0,   0,   0, 209, 209, 209, 209, 209, 209, 209, 209,   0,   0,   0,   0,   0,   0,   0,  97,   0,   0,   0,   0,   0,  82, 209, 209}},
        {{209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209,  11,   0,   4,   0,   0,   0,  18, 199,   6, 136,  11, 209, 209, 209, 206, 157}},
        {{209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 208,  69, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209}},
        {{209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 208}},
        {{209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209}},
        {{233,  83, 215, 216, 231,  87, 126,   0, 142,   0,   0,  21, 235,  86, 126, 104, 248,  83, 217, 218, 179,  61, 126,   0, 142,   0,   0,   0,  38, 172,   0, 118}},
        {{271, 277, 244,   0,   0,   0,   0,   0,   5,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 268,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}},
        {{285,   0, 285,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,   0,   0,   0,   0,   0, 155}},
        {{285, 285, 285, 269, 285, 285, 281,  84,  98,  32, 153,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}}
    }});
    
    inline constexpr std::array<std::array<uint8_t, 16>, 286> grapheme_cluster_break_prop::stage3({{
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11, 11}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  5}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14, 14}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11, 11, 11}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  5, 14}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14, 14,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14, 14, 14}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,  5,  5, 14}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11, 11, 11, 11}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  0,  5,  5}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  0,  5, 14}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14, 14,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14, 14, 14, 14}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2, 13,  1,  1}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5, 14, 14,  5,  5}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11, 11,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11, 11, 11, 11,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  5, 14,  5,  5}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14, 14,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14, 14,  0, 14,  5}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14, 14, 14,  1, 14}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14, 14, 14, 14, 14}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,  5,  5,  5,  5,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11, 11,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11, 11, 11, 11, 11, 11}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  0,  0,  0,  0, 14}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14, 14, 14, 14,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14, 14, 14, 14, 14, 14}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0, 11,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0, 11,  0,  0,  0,  1, 11,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0, 11, 11,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0, 11, 11, 11, 11, 11, 11, 11}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 14, 14, 14, 14, 14, 14}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0, 14, 14,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0, 14, 14, 14,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0, 14, 14, 14, 14, 14,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  0, 14, 14, 14, 14, 14, 14,  5}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0, 11,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0, 11, 11, 11, 11, 11, 11, 11, 11}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 12, 12, 12, 12, 12}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0, 14,  5,  5, 14, 14, 14, 14, 14}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0, 14, 14,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0, 14, 14, 14,  0,  0,  0,  0, 14}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0, 14, 14, 14, 14, 14, 14, 14,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0,  0, 14, 14, 14, 14, 14, 14, 14, 14}},
        {{ 0,  0,  0,  0,  0,  0,  0, 11,  0, 11,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0, 14,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0, 14,  0,  0,  0,  0, 12, 12,  0, 12}},
        {{ 0,  0,  0,  0,  0,  0,  0, 14, 14,  5,  5, 14,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0, 14, 14, 14, 14, 14,  5, 14, 14, 14}},
        {{ 0,  0,  0,  0,  0,  0,  0, 14, 14, 14, 14, 14, 14, 14, 14,  0}},
        {{ 0,  0,  0,  0,  0,  0,  0, 14, 14, 14, 14, 14, 14, 14, 14, 14}},
        {{ 0,  0,  0,  0,  0,  0,  5,  5, 14, 14,  0,  0,  0,  0, 14, 14}},
        {{ 0,  0,  0,  0,  0,  0, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11}},
        {{ 0,  0,  0,  0,  0,  0, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0, 14,  5, 14, 14,  5,  5,  5, 14,  5, 14}},
        {{ 0,  0,  0,  0,  0,  0, 14, 14, 14, 14,  0, 14, 14, 14, 14, 14}},
        {{ 0,  0,  0,  0,  0,  0, 14, 14, 14, 14, 14,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0, 14, 14, 14, 14, 14, 14, 14,  4,  0, 14}},
        {{ 0,  0,  0,  0,  0,  0, 14, 14, 14, 14, 14, 14, 14, 14,  0,  0}},
        {{ 0,  0,  0,  0,  0,  0, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14}},
        {{ 0,  0,  0,  0,  0,  5,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  5,  5,  5, 14, 14, 14, 14, 14, 14, 14, 14}},
        {{ 0,  0,  0,  0,  0,  5, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0,  5, 14,  5, 14, 14, 14, 14, 14, 14, 14,  0}},
        {{ 0,  0,  0,  0,  0, 11, 11, 11,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11}},
        {{ 0,  0,  0,  0,  0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12}},
        {{ 0,  0,  0,  0,  0, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0, 14,  0, 14,  0, 14,  0,  0,  0,  0,  5,  5}},
        {{ 0,  0,  0,  0,  0, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0, 14, 14,  0, 12, 12, 12,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0,  0, 14, 14, 14,  0,  0,  0,  0, 12, 12,  0, 12}},
        {{ 0,  0,  0,  0,  0, 14, 14, 14, 14, 14,  0,  0,  0, 14, 14, 14}},
        {{ 0,  0,  0,  0,  4,  4,  4,  4,  4,  4, 14, 14, 14, 14, 14, 14}},
        {{ 0,  0,  0,  0,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5}},
        {{ 0,  0,  0,  0,  5,  5,  5,  5,  5,  5,  5,  5, 14, 14, 14, 14}},
        {{ 0,  0,  0,  0,  5,  5, 14, 14, 14, 14, 14,  0,  0,  0,  5,  5}},
        {{ 0,  0,  0,  0, 11,  0,  0, 11,  0,  0,  0,  0, 11,  0, 11,  0}},
        {{ 0,  0,  0,  0, 11, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0, 11, 11, 11, 11, 11, 11,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11}},
        {{ 0,  0,  0,  0, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0, 14,  0,  0,  5, 14, 14,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0, 14, 14,  5, 14, 14, 14, 14, 14, 14, 14,  5,  5}},
        {{ 0,  0,  0,  0, 14, 14, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0, 14, 14, 14, 14, 14, 14, 14,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  0, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  5,  5}},
        {{ 0,  0,  0,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0,  5,  5,  5, 14, 14, 14, 14, 14, 14, 14, 14, 14,  5}},
        {{ 0,  0,  0,  5,  5, 14,  5,  5, 14,  5,  5,  0,  5, 14,  0,  0}},
        {{ 0,  0,  0,  5,  5, 14, 14,  5,  0,  0,  0,  0, 14,  0,  0,  0}},
        {{ 0,  0,  0,  7,  0,  0,  0,  7,  7,  7,  7,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0, 11, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0, 11, 11, 11,  0, 11,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0, 11, 11, 11, 11, 11,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0, 14,  0,  0,  0,  0,  0,  0,  0,  0, 14,  5,  0,  0}},
        {{ 0,  0,  0, 14,  5,  5, 14, 14, 14, 14,  5,  5, 14, 14,  5,  5}},
        {{ 0,  0,  0, 14, 14,  5,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  0, 14, 14, 14, 14, 14, 14,  5,  4, 14, 14, 14, 14,  0}},
        {{ 0,  0,  0, 14, 14, 14, 14, 14, 14, 14,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  5,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0,  5,  5,  0,  0, 14, 14, 14, 14, 14, 14, 14,  0,  0,  0}},
        {{ 0,  0, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0, 11, 11, 11, 11, 11, 11, 11, 11, 11,  0, 11, 11, 11, 11}},
        {{ 0,  0, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  0,  0}},
        {{ 0,  0, 14,  0,  0,  0, 14,  0,  0,  0,  0, 14,  0,  0,  0,  0}},
        {{ 0,  0, 14,  0,  5, 14, 14,  0,  0,  0,  0,  0,  0, 14,  0,  0}},
        {{ 0,  0, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0, 14, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0, 14, 14, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0,  0, 14, 14, 14, 14,  5, 14, 14, 14, 14, 14,  0,  0,  0,  0}},
        {{ 0,  0, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14}},
        {{ 0,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5}},
        {{ 0,  5,  5,  5, 14, 14, 14, 14,  0,  0, 14, 14,  5,  5,  5,  5}},
        {{ 0,  5, 14, 14, 14, 14,  5,  5, 14, 14, 14, 14, 14, 14,  0,  0}},
        {{ 0, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0, 11,  0,  0,  0,  0,  0,  0, 11,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,  0,  0,  0,  0,  0}},
        {{ 0, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11}},
        {{ 0, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0, 14,  0,  5, 14, 14, 14, 14, 14, 14, 14,  0,  0,  0,  0,  0}},
        {{ 0, 14,  0,  5, 14, 14, 14, 14, 14, 14, 14, 14, 14,  0,  0,  0}},
        {{ 0, 14,  5,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0, 14, 14,  0, 14, 14,  0, 14,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0, 14, 14,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0, 14, 14, 14,  0, 14, 14,  0,  0,  0,  0,  0, 14, 14, 14, 14}},
        {{ 0, 14, 14, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 0, 14, 14, 14, 14, 14, 14,  0,  0,  0, 14,  0, 14, 14,  0, 14}},
        {{ 0, 14, 14, 14, 14, 14, 14,  5,  5, 14, 14, 14,  0,  0,  0,  0}},
        {{ 0, 14, 14, 14, 14, 14, 14, 14, 12, 12, 12, 12, 12, 12, 12, 12}},
        {{ 0, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  0,  0,  0,  0,  0}},
        {{ 0, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  5}},
        {{ 0, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14}},
        {{ 1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0}},
        {{ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1}},
        {{ 3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3}},
        {{ 4,  4,  0,  0,  0,  0,  0, 14, 14, 14, 14, 14, 14, 14, 14, 14}},
        {{ 4,  4,  4,  4,  4,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 5,  4,  5, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 5,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 5,  5,  5,  5,  5,  5,  5,  5,  0,  0,  0,  0,  0,  0,  0, 14}},
        {{ 5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5}},
        {{ 5,  5,  5,  5,  5,  5, 14,  5,  5, 14, 14, 14, 14, 14, 14, 14}},
        {{ 5,  5,  5,  5, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 5,  5,  5, 14, 14, 14, 14,  5,  5, 14, 14,  0,  0,  4,  0,  0}},
        {{ 5,  5,  5, 14, 14, 14, 14, 14, 14, 14, 14,  0,  0,  0,  0,  0}},
        {{ 5,  5,  5, 14, 14, 14, 14, 14, 14, 14, 14,  5,  5, 14,  5, 14}},
        {{ 5,  5,  5, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  0,  0, 14}},
        {{ 5,  5, 14,  5,  5,  5,  5,  5,  5, 14, 14, 14,  0,  0,  0,  0}},
        {{ 5,  5, 14, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 5,  5, 14, 14, 14,  0, 14,  0,  5,  5,  5,  5,  5,  5,  5, 14}},
        {{ 5,  5, 14, 14, 14,  5, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 5,  5, 14, 14, 14, 14,  0,  0,  5,  5,  5,  5, 14, 14,  5, 14}},
        {{ 5, 14,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 5, 14, 14,  0,  0,  0,  0, 14, 14,  0,  0, 14, 14, 14,  0,  0}},
        {{ 5, 14, 14,  5,  5, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{ 5, 14, 14, 14, 14,  0,  0,  5,  5,  0,  0,  5,  5, 15,  0,  0}},
        {{ 5, 14, 14, 14, 14,  0,  5,  5,  5,  0,  5,  5,  5, 15,  4,  0}},
        {{ 5, 14, 14, 14, 14, 14,  0, 14, 14,  5,  0,  5,  5, 15,  0,  0}},
        {{ 5, 14, 14, 14, 14, 14, 14, 14, 14,  5,  5,  5,  5, 15,  5,  5}},
        {{ 6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  0,  0,  0}},
        {{ 6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6}},
        {{ 7,  7,  7,  7,  7,  7,  7,  0,  0,  0,  0,  8,  8,  8,  8,  8}},
        {{ 7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7}},
        {{ 7,  7,  7,  7,  7,  7,  7,  7,  8,  8,  8,  8,  8,  8,  8,  8}},
        {{ 8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  0}},
        {{ 8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8}},
        {{ 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}},
        {{10, 10, 10, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{10, 10, 10, 10,  9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}},
        {{10, 10, 10, 10, 10, 10, 10, 10,  9, 10, 10, 10, 10, 10, 10, 10}},
        {{10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  9, 10, 10, 10}},
        {{10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}},
        {{11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11}},
        {{11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11,  0,  0}},
        {{11,  0,  0,  0,  0, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{11, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11, 11}},
        {{11, 11, 11,  0, 11,  0, 11,  0,  0,  0,  0,  0,  0, 11,  0,  0}},
        {{11, 11, 11,  0, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11}},
        {{11, 11, 11, 11,  0,  0,  0,  0, 11, 11, 11,  0,  0,  0,  0,  0}},
        {{11, 11, 11, 11, 11, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{11, 11, 11, 11, 11, 11,  0,  0, 11, 11, 11, 11, 11, 11, 11, 11}},
        {{11, 11, 11, 11, 11, 11,  0, 11, 11, 11, 11, 11, 11, 11, 11, 11}},
        {{11, 11, 11, 11, 11, 11,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3}},
        {{11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,  0, 11, 11, 11, 11}},
        {{11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,  0,  0}},
        {{11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11}},
        {{11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 14, 14, 14, 14, 14}},
        {{12,  0, 12,  0,  0,  0, 12, 12, 12, 12,  0,  0, 14,  0, 14,  5}},
        {{12,  0, 12, 12,  0, 12, 12, 12, 12, 12,  0,  0, 14,  0,  5,  5}},
        {{12,  0, 12, 12,  0, 12, 12, 12, 12, 12,  0,  0, 14,  0, 14, 14}},
        {{12, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  0}},
        {{12, 12, 12, 12, 12, 12, 12, 12, 12,  0, 12, 12, 12, 12, 12, 12}},
        {{12, 12, 12, 12, 12, 12, 12, 12, 12, 12,  0,  0, 14,  0, 14, 14}},
        {{12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12}},
        {{12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 14, 14,  0, 14,  5}},
        {{12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 14,  5, 14,  0,  5,  5}},
        {{14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14,  0,  0,  0,  0,  0,  0, 14, 14, 14, 14, 14, 14, 14, 14, 14}},
        {{14,  0,  0,  0,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14,  0,  0, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14}},
        {{14,  0,  4,  4,  0,  0,  0,  0,  0, 14, 14, 14, 14,  0,  5, 14}},
        {{14,  0, 14,  0,  0, 14,  0, 14, 14, 14,  5,  0,  5,  5, 14, 14}},
        {{14,  0, 14,  0,  0, 14, 14, 14, 14, 14, 14, 14, 14,  5,  5,  5}},
        {{14,  0, 14, 14, 14,  0,  0, 14, 14,  0,  0,  0,  0,  0, 14, 14}},
        {{14,  4, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14,  5,  5,  0,  0,  0,  5,  5,  5,  0,  5,  5,  5, 14,  0,  0}},
        {{14,  5,  5,  5,  5,  0,  0,  5,  5,  0,  0,  5,  5, 14,  0,  0}},
        {{14,  5,  5,  5,  5,  0, 14, 14, 14,  0, 14, 14, 14, 15,  0,  0}},
        {{14,  5,  5,  5,  5,  5,  0,  5,  5,  0,  0, 14, 14, 14, 14,  4}},
        {{14,  5,  5,  5, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14,  5,  5, 14, 14, 14, 14, 14, 14,  5, 14,  5,  5, 14,  5, 14}},
        {{14,  5, 14,  5,  5,  0, 14, 14, 14,  0, 14, 14, 14, 14,  0,  0}},
        {{14,  5, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14,  5, 14, 14,  5, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14,  5, 14, 14, 14, 14, 14, 14,  0, 14, 14,  5,  5, 14, 14,  0}},
        {{14,  5, 14, 14, 14, 14, 14, 14, 14,  0,  0,  0,  0, 14,  0,  0}},
        {{14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14}},
        {{14, 14,  0,  0,  0, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14,  0,  5,  5, 14,  5, 14,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14,  0, 14, 14,  0, 14, 14, 14, 14, 14,  0,  0,  0,  0,  0}},
        {{14, 14,  4,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14,  4, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14}},
        {{14, 14,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14,  5,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14,  5,  5, 14, 14, 14, 14,  0,  0,  0,  0,  0,  0, 14,  0}},
        {{14, 14,  5, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14, 14,  0,  0, 14, 14, 14, 14, 14, 14, 14,  0,  0,  0,  0}},
        {{14, 14, 14,  0, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  0,  0}},
        {{14, 14, 14,  0, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14}},
        {{14, 14, 14,  1,  1,  1,  1,  1,  1,  1,  1, 14, 14, 14, 14, 14}},
        {{14, 14, 14,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14, 14,  5,  5,  5,  5, 14, 14,  5,  5,  5,  0,  0,  0,  0}},
        {{14, 14, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  0,  0}},
        {{14, 14, 14, 14,  0, 14, 14, 14,  0, 14, 14, 14, 14, 14,  0,  0}},
        {{14, 14, 14, 14,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14, 14, 14,  5,  5, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14, 14, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14, 14, 14, 14,  0,  0, 14, 14,  0, 14, 14, 14, 14,  0,  0}},
        {{14, 14, 14, 14, 14,  0, 14, 14,  0,  0,  0,  0,  0, 14, 14, 14}},
        {{14, 14, 14, 14, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14, 14, 14, 14, 14,  4, 14,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14, 14, 14, 14, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14, 14, 14, 14, 14, 14,  0,  0,  0,  0, 14, 14, 14, 14, 14}},
        {{14, 14, 14, 14, 14, 14, 14,  0, 14, 14, 14, 14, 14, 14,  5, 14}},
        {{14, 14, 14, 14, 14, 14, 14,  0, 14, 14, 14, 14, 14, 14, 14, 14}},
        {{14, 14, 14, 14, 14, 14, 14,  5, 14, 14,  0,  0,  0,  0,  0,  0}},
        {{14, 14, 14, 14, 14, 14, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0}},
        {{14, 14, 14, 14, 14, 14, 14, 14,  0,  5, 14, 14, 14, 14, 14, 14}},
        {{14, 14, 14, 14, 14, 14, 14, 14,  0, 14, 14, 14, 14, 14, 14, 14}},
        {{14, 14, 14, 14, 14, 14, 14, 14,  5, 14, 14,  0,  0,  0,  0,  0}},
        {{14, 14, 14, 14, 14, 14, 14, 14, 14,  0,  0, 14, 14, 14, 14, 14}},
        {{14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  5,  5,  5, 14, 14, 14}},
        {{14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  0,  0,  0,  0,  0}},
        {{14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  0,  1,  0,  0,  0}},
        {{14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  0,  0,  0}},
        {{14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  0,  0}},
        {{14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  0, 14}},
        {{14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  0}},
        {{14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14}}
    }});
    
    inline constexpr std::array<uint8_t, 16> grapheme_cluster_break_prop::stage4({{
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x10, 0x20, 0x22, 0x32
    }});

    
    constexpr auto total_data_size = 
        is_whitespace::data_size +
        case_mapper::data_size +
        normalizer::data_size +
        grapheme_cluster_break_prop::data_size;
    static_assert(total_data_size == 80724);
}

#endif

