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
#include <sys_string/impl/unicode/utf_util.h>
#include <sys_string/impl/util/iter_util.h>

#include <ranges>
#include <iterator>

namespace sysstr
{

    namespace util
    {
        template<std::forward_iterator It, std::sentinel_for<It> EndIt, iter_direction Direction = iter_direction::forward>
        requires has_utf_encoding<std::iter_value_t<It>> 
        struct utf32_stream
        {
            It next; 
            const EndIt & end;
            
            char32_t read()
            { 
                if constexpr (Direction == iter_direction::forward)
                    return utf32_input<utf_encoding_of<std::iter_value_t<It>>>::read(next, end); 
                else
                    return utf32_input<utf_encoding_of<std::iter_value_t<It>>>::read_reversed(next, end);
            }
            bool eof() const
                { return next == end; }
        };

        template<std::forward_iterator It, std::sentinel_for<It> EndIt, iter_direction Direction>
        requires(has_utf_encoding<std::iter_value_t<It>> &&
                 utf_encoding_of<std::iter_value_t<It>> == utf32)
        struct utf32_stream<It, EndIt, Direction>
        {
            It next; 
            const EndIt & end;
            
            char32_t read()
            {
                char32_t ret = *next;
                ++next;
                return ret;
            }
            bool eof() const
                { return next == end; }
        };


    }
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
        requires(utf_encoding_of<std::ranges::range_value_t<Range>> == utf32)
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
        requires(utf_encoding_of<std::ranges::range_value_t<Range>> == utf32)
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

    class grapheme_cluster_break_common
    {
    protected:
        using table = util::unicode::grapheme_cluster_break_prop;

        static constexpr uint16_t s_lf_offset = 6;
        static constexpr uint16_t s_cr_offset = 7;
        static constexpr uint16_t s_zwj_offset = 8;
        static constexpr uint16_t s_special_mask = 0b0000'0001'1100'0000;

        enum class special : uint16_t
        {
            lf = uint16_t(1 << s_lf_offset),
            cr = uint16_t(1 << s_cr_offset),
            zwj = uint16_t(1 << s_zwj_offset)
        };

        static constexpr special get_special(char32_t c)
        {
            return special(
                   (uint16_t(c == U'\u000A') << s_lf_offset) |
                   (uint16_t(c == U'\u000D') << s_cr_offset) |
                   (uint16_t(c == U'\u200D') << s_zwj_offset)
            );
        }

        static constexpr table::value basic_props(table::value props)
            { return table::value(props & table::basic_mask); }
        static constexpr table::value in_cb_props(table::value  props)
            { return table::value(props & table::in_cb_mask); }
    };

    class grapheme_cluster_break_finder : private grapheme_cluster_break_common
    {
    private:
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
        static constexpr uint16_t s_state_mask = 0b000'1110'0000'0000;


        static constexpr uint16_t pack(table::value props, special sp, state st)
            { return props | uint16_t(sp) | (uint16_t(st) << s_state_offset); }

        template<std::forward_iterator It, std::sentinel_for<It> EndIt>
        using reader = util::utf32_stream<It, EndIt, util::iter_direction::forward>; 
        

        uint16_t m_data;

    public:
        template<std::forward_iterator It, std::sentinel_for<It> EndIt>
        requires has_utf_encoding<std::iter_value_t<It>>
        grapheme_cluster_break_finder(It & it, const EndIt & end)
        {
            if (it == end)
            {
                m_data = pack(table::none, special::lf, state::none);
                return;
            }

            reader<It, EndIt> src{it, end};
            this->init(src.read());
            for (it = src.next; !src.eof(); it = src.next)
            {
                if (this->next(src.read()))
                    break;
            }
        }

        template<std::forward_iterator It, std::sentinel_for<It> EndIt>
        requires has_utf_encoding<std::iter_value_t<It>>
        void operator()(It & it, const EndIt & end)
        {
            if (it == end)
                return;

            reader<It, EndIt> src{it, end};
            src.read();
            for (it = src.next; !src.eof(); it = src.next)
            {
                if (this->next(src.read()))
                    break;
            }
        }

    private:

        void init(char32_t first)
        {
            auto props = table::get(first);
            auto sp = get_special(first);
            state st = (basic_props(props) == table::regional_indicator ? state::after_ri_indicator : (
                        in_cb_props(props) == table::in_cb_consonant ? state::conjunct_cluster_start : (
                        basic_props(props) == table::extended_pictographic ? state::extended_picto_start :
                        state::none)));
            m_data = pack(props, sp, st);
        }

        bool next(char32_t c)
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
                        prev_basic_props == table::prepend)            //GB9b
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

    class grapheme_cluster_break_reverse_finder : private grapheme_cluster_break_common
    {
    private:
        enum class state : uint16_t
        {
            none,
            ri_prefix_even,
            ri_prefix_odd
        };

        static constexpr uint16_t s_state_offset = 10;
        static constexpr uint16_t s_state_mask = 0b000'0110'0000'0000;


        static constexpr uint16_t pack(table::value props, special sp, state st)
            { return props | uint16_t(sp) | (uint16_t(st) << s_state_offset); }

        template<std::forward_iterator It, std::sentinel_for<It> EndIt>
        using reader = util::utf32_stream<It, EndIt, util::iter_direction::backward>; 

        
        uint16_t m_data;

    public:
        template<std::forward_iterator It, std::sentinel_for<It> EndIt>
        requires has_utf_encoding<std::iter_value_t<It>>
        grapheme_cluster_break_reverse_finder(It & it, const EndIt & end)
        {
            if (it == end)
            {
                m_data = pack(table::none, special::cr, state::none);
                return;
            }

            reader<It, EndIt> src{it, end};
            auto c = src.read();
            auto current_props = table::get(c);
            auto current_special = get_special(c);
            m_data = pack(current_props, current_special, state::none);
            find_break(it, src);
        }

        template<std::forward_iterator It, std::sentinel_for<It> EndIt>
        requires has_utf_encoding<std::iter_value_t<It>>
        void operator()(It & it, const EndIt & end)
        {
            if (it == end)
                return;

            reader<It, EndIt> src{it, end};
            src.read();
            find_break(it, src);
        }
        
    private:

        template<std::forward_iterator It, std::sentinel_for<It> EndIt>
        void find_break(It & it, reader<It, EndIt> & src)
        {
            for (it = src.next; !src.eof(); it = src.next)
            {
                if (this->find_break(src))
                    break;
            }
        }

        template<std::forward_iterator It, std::sentinel_for<It> EndIt>
        bool find_break(reader<It, EndIt> & src)
        {
            auto c = src.read();
            
            auto current_props = table::get(c);
            auto current_special = get_special(c);
            const auto current_basic_props = basic_props(current_props);
            const auto current_in_cb_props = in_cb_props(current_props);
            
            auto current_state = state(m_data >> s_state_offset);
            switch(current_state)
            {
                case state::none:
                {
                    const auto prev_props = table::value(m_data);
                    const auto prev_basic_props = basic_props(prev_props);
                    const auto prev_in_cb_props = in_cb_props(prev_props);
                    const auto prev_special = special(m_data & s_special_mask);

                    //GB3
                    if (current_special == special::cr && prev_special == special::lf)
                    {
                        m_data = pack(current_props, current_special, state::none);
                        return false;
                    }

                    //GB4
                    if (current_special == special::cr || current_special == special::lf || current_basic_props == table::control)
                    {
                        m_data = pack(current_props, current_special, state::none);
                        return true;
                    }

                    //GB5
                    if (prev_special == special::cr || prev_special == special::lf || prev_basic_props == table::control)
                    {
                        m_data = pack(current_props, current_special, state::none);
                        return true;
                    }

                    //GB6
                    if (current_basic_props == table::hangul_l && (
                            prev_basic_props == table::hangul_l ||
                            prev_basic_props == table::hangul_v ||
                            prev_basic_props == table::hangul_lv ||
                            prev_basic_props == table::hangul_lvt))
                    {
                        m_data = pack(current_props, current_special, state::none);
                        return false;
                    }

                    //GB7
                    if ((current_basic_props == table::hangul_lv ||
                            current_basic_props == table::hangul_v) && (
                            prev_basic_props == table::hangul_v ||
                            prev_basic_props == table::hangul_t))
                    {
                        m_data = pack(current_props, current_special, state::none);
                        return false;
                    }

                    //GB8
                    if ((current_basic_props == table::hangul_lvt ||
                            current_basic_props == table::hangul_t) && (
                            prev_basic_props == table::hangul_t))
                    {
                        m_data = pack(current_props, current_special, state::none);
                        return false;
                    }

                    if (prev_basic_props == table::extend || prev_special == special::zwj || //GB9
                        prev_basic_props == table::spacing_mark ||        //GB9a
                        current_basic_props == table::prepend)            //GB9b
                    {
                        m_data = pack(current_props, current_special, state::none);
                        return false;
                    }

                    //GB9c start
                    if (prev_in_cb_props == table::in_cb_consonant &&
                            (current_in_cb_props == table::in_cb_extend ||
                             current_in_cb_props == table::in_cb_linker))
                    {
                        reader<It, EndIt> test_reader(src);
                        bool has_linker = (current_in_cb_props == table::in_cb_linker);
                        while(!test_reader.eof())
                        {
                            auto test_c = test_reader.read();
                            auto test_props = table::get(test_c);
                            auto test_in_cb_props = in_cb_props(test_props);
                            if (test_in_cb_props == table::in_cb_extend)
                                continue;
                            if (test_in_cb_props == table::in_cb_linker)
                            {
                                has_linker = true;
                                continue;
                            }
                            if (test_in_cb_props == table::in_cb_consonant && has_linker)
                            {
                                src.next = test_reader.next;
                                m_data = pack(test_props, get_special(test_c), state::none);
                                return false;
                            }
                            break;
                        }
                        m_data = pack(current_props, current_special, state::none);
                        return true;
                    }

                    //GB11 start
                    if (prev_basic_props == table::extended_pictographic &&
                        current_special == special::zwj)
                    {
                        reader<It, EndIt> test_reader(src);
                        while(!test_reader.eof())
                        {
                            auto test_c = test_reader.read();
                            auto test_props = table::get(test_c);
                            auto test_basic_props = basic_props(test_props);
                            if (test_basic_props == table::extend)
                                continue;
                            if (test_basic_props == table::extended_pictographic)
                            {
                                src.next = test_reader.next;
                                m_data = pack(test_props, get_special(test_c), state::none);
                                return false;
                            }
                        }
                        m_data = pack(current_props, current_special, state::none);
                        return true;
                    }

                    //GB12 and GB13 start
                    if (prev_basic_props == table::regional_indicator &&
                        current_basic_props == table::regional_indicator)
                    {
                        reader<It, EndIt> test_reader(src);
                        bool even = true;
                        while(!test_reader.eof())
                        {
                            auto test_c = test_reader.read();
                            auto test_props = table::get(test_c);
                            if (test_props != table::regional_indicator)
                                break;
                            even = !even;
                        }
                        if (even)
                        {
                            m_data = pack(current_props, current_special, state::ri_prefix_even);
                            return false;
                        }
                        m_data = pack(current_props, current_special, state::ri_prefix_odd);
                        return true;
                    }
                    
                    
                    //GB999
                    m_data = pack(current_props, current_special, state::none);
                    return true;
                }

                case state::ri_prefix_even:

                    if (current_basic_props == table::regional_indicator)
                        m_data = pack(current_props, current_special, state::ri_prefix_odd);
                    else
                        m_data = pack(current_props, current_special, state::none);
                    

                    return true;

                case state::ri_prefix_odd:

                    if (current_basic_props == table::regional_indicator)
                    {
                        m_data = pack(current_props, current_special, state::ri_prefix_even);
                        return false;
                    }
                        
                    m_data = pack(current_props, current_special, state::none);
                    return true;

            }

            SYS_STRING_UNREACHABLE;
            
        }

    };

}

#endif
