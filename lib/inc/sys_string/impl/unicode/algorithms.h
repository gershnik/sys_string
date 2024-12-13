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

        struct char_data
        {
            char32_t c;
            table::value props;

            table::value basic_props() const
                { return table::value(props & table::basic_mask); }
            table::value in_cb_props() const
                { return table::value(props & table::in_cb_mask); }
        };

        enum class state
        {
            none,
            after_ri_indicator,
            conjunct_cluster_start,
            conjunct_cluster_has_linker,
            extended_picto_start,
            extended_picto_has_zwj
        };

        char_data m_prev;
        state m_state;

    public:
        grapheme_cluster_break_finder():
            m_prev{U'\u000A', table::none},
            m_state(state::none)
        {}
        
        grapheme_cluster_break_finder(char32_t first)
        {
            reset(first);
        }

        void reset(char32_t first)
        {
            m_prev = char_data{first, table::get(first)},
            m_state = (m_prev.basic_props() == table::regional_indicator ? state::after_ri_indicator : (
                        m_prev.in_cb_props() == table::in_cb_consonant ? state::conjunct_cluster_start : (
                        m_prev.props & table::extended_pictographic ? state::extended_picto_start :
                        state::none)));
        }

        bool operator()(char32_t c)
        {
            char_data current{c, table::get(c)};
            bool ret = false;
            state next_state = m_state;
            switch (m_state)
            {
                case state::none:
                case state::after_ri_indicator:
                main_case:

                    if (current.basic_props() == table::regional_indicator) //GB12 and GB13
                    {
                        next_state = state((int(m_state) + 1) % (int(state::after_ri_indicator) + 1));
                        if (m_state == state::after_ri_indicator)
                            break;
                    }
                    else if (current.in_cb_props() == table::in_cb_consonant) //GB9c start
                    {
                        next_state = state::conjunct_cluster_start;
                    }
                    else if (current.props & table::extended_pictographic) //GB11 start
                    {
                        next_state = state::extended_picto_start;
                    }
                    else
                    {
                        next_state = state::none;
                    }


                    //GB3
                    if (current.c == U'\u000A' && m_prev.c == U'\u000D')
                        break;

                    //GB4
                    if (m_prev.c == U'\u000D' || m_prev.c == U'\u000A' || m_prev.props == table::control)
                    {
                        ret = true;
                        break;
                    }

                    //GB5
                    if (current.c == U'\u000D' || current.c == U'\u000A' || current.props == table::control)
                    {
                        ret = true;
                        break;
                    }

                    //GB6
                    if (m_prev.basic_props() == table::hangul_l && (
                            current.basic_props() == table::hangul_l ||
                            current.basic_props() == table::hangul_v ||
                            current.basic_props() == table::hangul_lv ||
                            current.basic_props() == table::hangul_lvt))
                    {
                        break;
                    }

                    //GB7
                    if ((m_prev.basic_props() == table::hangul_lv ||
                         m_prev.basic_props() == table::hangul_v) && (
                            current.basic_props() == table::hangul_v ||
                            current.basic_props() == table::hangul_t))
                    {
                        break;
                    }

                    //GB8
                    if ((m_prev.basic_props() == table::hangul_lvt ||
                         m_prev.basic_props() == table::hangul_t) && (
                            current.basic_props() == table::hangul_t))
                    {
                        break;
                    }

                    
                    if (current.basic_props() == table::extend || current.c == U'\u200D' || //GB9
                        current.basic_props() == table::spacing_mark ||  //GB9a
                        m_prev.basic_props() == table::prepend)          //GB9b
                        break;

                    
                    //GB999
                    ret = true;
                    break;
                
                
                case state::conjunct_cluster_has_linker:
                    
                    if (current.in_cb_props() == table::in_cb_consonant)
                    {
                        next_state = state::conjunct_cluster_start;
                        break;
                    }
                    [[fallthrough]];

                case state::conjunct_cluster_start:

                    if (current.in_cb_props() == table::in_cb_extend)
                        break;
                    if (current.in_cb_props() == table::in_cb_linker)
                    {
                        next_state = state::conjunct_cluster_has_linker;
                        break;
                    }

                    m_state = state::none;
                    next_state = state::none;
                    goto main_case;

                case state::extended_picto_start:

                    if (current.basic_props() == table::extend)
                        break;
                    if (current.c == U'\u200D')
                    {
                        next_state = state::extended_picto_has_zwj;
                        break;
                    }

                    m_state = state::none;
                    next_state = state::none;
                    goto main_case;

                case state::extended_picto_has_zwj:

                    next_state = state::none;
                
                    if (current.props & table::extended_pictographic)
                        break;
                    
                    m_state = state::none;
                    goto main_case;
            }

            m_state = next_state;
            m_prev = current;
            return ret;
        }
    };

}

#endif
