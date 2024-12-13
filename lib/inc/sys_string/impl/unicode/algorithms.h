//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_UNICODE_ALGORITHMS_H_INCLUDED
#define HEADER_SYS_STRING_UNICODE_ALGORITHMS_H_INCLUDED

#include <sys_string/impl/unicode/mappings.h>
#include <sys_string/impl/util/iter_util.h>

#include <ranges>
#include <iterator>

namespace sysstr
{
    struct isspace
    {
        auto operator()(char32_t c) const noexcept -> bool
        {
            return util::unicode::isspace(c);
        }
    };

    template<utf_encoding OutEnc>
    struct casefold
    {
        static constexpr auto max_output_length = util::unicode::mapper::max_mapped_length;
        
        template<std::output_iterator<utf_char_of<OutEnc>> OutIt>
        auto operator()(char32_t c, OutIt dest) const noexcept(noexcept(*dest++ = utf_char_of<OutEnc>())) -> OutIt
        {
            return util::unicode::mapper::case_fold.map_char<OutEnc>(c, dest);
        }
    };

    namespace util
    {
        class sigma_tolower
        {
        public:
            template<ranges::reversible_range Range>
            auto operator()(const Range & range, 
                            std::ranges::iterator_t<Range> where) const noexcept -> char32_t
            {
                auto reversed = ranges::make_reverse_iterator(range, where);
                if (any_non_cased_then_cased(reversed, std::ranges::rend(range)))
                {
                    ++where;
                    if (!any_non_cased_then_cased(where, std::ranges::end(range)))
                    {
                        return U'\u03C2'; //replace with ς
                    }
                }
                return U'\u03C3'; // replace with σ
            }
        private:
            template<std::input_iterator It, std::sentinel_for<It> Sentinel>
            auto any_non_cased_then_cased(It first, Sentinel last) const noexcept -> bool
            {
                for( ; first != last; ++first)
                {
                    auto c = *first;
                    auto props = unicode::case_prop::get(c);
                    if (props & unicode::case_prop::case_ignorable)
                        continue;
                    return (props & unicode::case_prop::cased);
                }
                return false;
            }
        };
    }

    template<utf_encoding OutEnc>
    struct tolower
    {
        template<ranges::reversible_range Range, std::output_iterator<utf_char_of<OutEnc>> OutIt>
        inline auto operator()(const Range & range, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<OutEnc>())) -> OutIt
        {
            using namespace util::unicode;

            auto first = std::ranges::begin(range);
            auto last = std::ranges::end(range);
            
            for( ; first != last; ++first)
            {
                auto c = *first;
                if (c != U'\u03A3') // not Σ
                {
                    dest = mapper::to_lower_case.map_char<OutEnc>(c, dest);
                }
                else
                {
                    char32_t lc = util::sigma_tolower()(range, first);
                    auto writer = make_utf32_encoder<OutEnc>([&](auto value) {
                        *dest++ = value;
                    });
                    writer(lc);
                }
            }
            return dest;
        }
    };

    template<utf_encoding OutEnc>
    struct toupper
    {
        template<ranges::reversible_range Range, std::output_iterator<utf_char_of<OutEnc>> OutIt>
        inline auto operator()(const Range & range, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<OutEnc>())) -> OutIt
        {
            using namespace util::unicode;
            
            auto first = std::ranges::begin(range);
            auto last = std::ranges::end(range);
            for( ; first != last; ++first)
            {
                auto c = *first;
                dest = mapper::to_upper_case.map_char<OutEnc>(c, dest);
            }

            return dest;
        }
    };

    class grapheme_cluster_break_finder
    {
    private:
        using table = util::unicode::grapheme_cluster_break_prop;

        static constexpr uint16_t s_lf_offset = 7;
        static constexpr uint16_t s_cr_offset = 8;
        static constexpr uint16_t s_zwj_offset = 9;
        static constexpr uint16_t s_special_mask = 0b0000'0011'1000'0000;

        enum class special : uint16_t
        {
            lf = uint16_t(1 << s_lf_offset),
            cr = uint16_t(1 << s_cr_offset),
            zwj = uint16_t(1 << s_zwj_offset)
        };

        enum class state : uint16_t
        {
            none,
            after_ri_indicator,
            conjunct_cluster_start,
            conjunct_cluster_has_linker,
            extended_picto_start,
            extended_picto_has_zwj
        };

        static constexpr uint16_t s_state_offset = 10;
        static constexpr uint16_t s_state_mask = 0b0001'1100'0000'0000;

        static constexpr special get_special(char32_t c)
        {
            return special(
                   (uint16_t(c == U'\u000A') << s_lf_offset) |
                   (uint16_t(c == U'\u000D') << s_cr_offset) |
                   (uint16_t(c == U'\u200D') << s_zwj_offset)
            );
        }

        static constexpr uint16_t pack(table::value props, special sp, state st)
        {
            return props | uint16_t(sp) | (uint16_t(st) << s_state_offset);
        }

        static constexpr table::value basic_props(table::value props)
            { return table::value(props & table::basic_mask); }
        static constexpr table::value in_cb_props(table::value  props)
            { return table::value(props & table::in_cb_mask); }

        
        uint16_t m_data;

    public:
        grapheme_cluster_break_finder():
            m_data(pack(table::none, special::lf, state::none))
        {}
        
        grapheme_cluster_break_finder(char32_t first)
        {
            reset(first);
        }

        void reset(char32_t first)
        {
            auto props = table::get(first);
            auto sp = get_special(first);
            state st = (basic_props(props) == table::regional_indicator ? state::after_ri_indicator : (
                        in_cb_props(props) == table::in_cb_consonant ? state::conjunct_cluster_start : (
                        basic_props(props) == table::extended_pictographic ? state::extended_picto_start :
                        state::none)));
            m_data = pack(props, sp, st);
        }

        bool operator()(char32_t c)
        {
            auto current_props = table::get(c);
            auto current_basic_props = basic_props(current_props);
            auto current_in_cb_props = in_cb_props(current_props);
            auto current_special = get_special(c);
            auto current_state = state(m_data >> s_state_offset);

            bool ret = false;
            state next_state = current_state;
            switch (current_state)
            {
                case state::none:
                case state::after_ri_indicator:
                main_case:
                {
                    auto prev_basic_props = basic_props(table::value(m_data));
                    auto prev_special = special(m_data & s_special_mask);

                    if (current_basic_props == table::regional_indicator) //GB12 and GB13
                    {
                        next_state = state((int(current_state) + 1) % (int(state::after_ri_indicator) + 1));
                        if (current_state == state::after_ri_indicator)
                            break;
                    }
                    else if (current_in_cb_props == table::in_cb_consonant) //GB9c start
                    {
                        next_state = state::conjunct_cluster_start;
                    }
                    else if (current_basic_props == table::extended_pictographic) //GB11 start
                    {
                        next_state = state::extended_picto_start;
                    }
                    else
                    {
                        next_state = state::none;
                    }

                    //GB3
                    if (current_special == special::lf && prev_special == special::cr)
                        break;

                    //GB4
                    if (prev_special == special::cr || prev_special == special::lf || prev_basic_props == table::control)
                    {
                        ret = true;
                        break;
                    }

                    //GB5
                    if (current_special == special::cr || current_special == special::lf || current_basic_props == table::control)
                    {
                        ret = true;
                        break;
                    }

                    //GB6
                    if (prev_basic_props == table::hangul_l && (
                            current_basic_props == table::hangul_l ||
                            current_basic_props == table::hangul_v ||
                            current_basic_props == table::hangul_lv ||
                            current_basic_props == table::hangul_lvt))
                    {
                        break;
                    }

                    //GB7
                    if ((prev_basic_props == table::hangul_lv ||
                         prev_basic_props == table::hangul_v) && (
                            current_basic_props == table::hangul_v ||
                            current_basic_props == table::hangul_t))
                    {
                        break;
                    }

                    //GB8
                    if ((prev_basic_props == table::hangul_lvt ||
                         prev_basic_props == table::hangul_t) && (
                            current_basic_props == table::hangul_t))
                    {
                        break;
                    }

                    
                    if (current_basic_props == table::extend || current_special == special::zwj || //GB9
                        current_basic_props == table::spacing_mark ||  //GB9a
                        prev_basic_props == table::prepend)          //GB9b
                        break;

                    
                    //GB999
                    ret = true;
                    break;
                
                }
                case state::conjunct_cluster_has_linker:
                    
                    if (current_in_cb_props == table::in_cb_consonant)
                    {
                        next_state = state::conjunct_cluster_start;
                        break;
                    }
                    [[fallthrough]];

                case state::conjunct_cluster_start:

                    if (current_in_cb_props == table::in_cb_extend)
                        break;
                    if (current_in_cb_props == table::in_cb_linker)
                    {
                        next_state = state::conjunct_cluster_has_linker;
                        break;
                    }

                    current_state = state::none;
                    next_state = state::none;
                    goto main_case;

                case state::extended_picto_start:

                    if (current_basic_props == table::extend)
                        break;
                    if (current_special == special::zwj)
                    {
                        next_state = state::extended_picto_has_zwj;
                        break;
                    }

                    current_state = state::none;
                    next_state = state::none;
                    goto main_case;

                case state::extended_picto_has_zwj:

                    next_state = state::none;
                
                    if (current_basic_props == table::extended_pictographic)
                        break;
                    
                    current_state = state::none;
                    goto main_case;
            }

            m_data = pack(current_props, current_special, next_state);
            return ret;
        }
    };

}

#endif
