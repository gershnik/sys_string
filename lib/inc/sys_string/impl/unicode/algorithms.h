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

    namespace util
    {

        template<std::forward_iterator It, std::sentinel_for<It> EndIt>
        requires(std::is_same_v<std::iter_value_t<It>, char32_t>)
        class grapheme_cluster_step
        {
        private:
            using table = util::unicode::grapheme_cluster_break_prop;

            class state
            {
            public:

                state(It & first, const EndIt & last, char32_t c_):
                    m_current(first), 
                    m_last(last), 
                    c(c_), 
                    props(table::get(c))
                {}

                state(It & first, const state & parent):
                    m_current(first), 
                    m_last(parent.m_last), 
                    c(parent.c), 
                    props(parent.props)
                {}

                state & operator=(const state & st)
                {
                    m_current = st.m_current;
                    this->c = st.c;
                    this->props = st.props;
                    return *this;
                }

                [[nodiscard]] bool advance()
                {
                    ++m_current;
                    if (m_current == m_last)
                        return false;
                    c = *m_current;
                    props = table::get(c);
                    return true;
                }

                bool done() const
                    { return m_current == m_last; }

                const It & current() const
                    { return m_current; }

                table::value basic_props() const
                    { return table::value(props & table::basic_mask); }
                table::value in_cb_props() const
                    { return table::value(props & table::in_cb_mask); }
            private:
                It & m_current;
                const EndIt & m_last;
            public:
                char32_t c;
                table::value props;
            };
        public:
            void operator()(It & first, const EndIt & last)
            {
                if (first == last) 
                    return;

                char32_t c = *first;

                if (c == U'\u000D')
                {
                    ++first;
                    if (first != last && *first == U'\u000A')
                        ++first;
                    return;
                }
                
                if (c == U'\u000A')
                {
                    ++first;
                    return;
                }

                state st{first, last, c};

                if (st.basic_props() == table::control)
                {
                    ++first;
                    return;
                }

                if (!detect_main(st))
                {
                    ++first;
                    return;
                }
            }
        private:
            bool detect_main(state & st)
            {
                if (st.basic_props() != table::prepend)
                {
                    if (detect_core(st))
                    {
                        if (!st.done())
                            skip_postcore(st);
                        return true;
                    }
                    return false;
                }
                if (!st.advance())
                    return true;

                It test_it = st.current();
                state test_st{test_it, st};
                do
                {
                    if (st.basic_props() != table::prepend)
                    {
                        if (detect_core(test_st))
                        {
                            st = test_st;
                            if (!st.done())
                                skip_postcore(st);
                        }
                        break;
                    }
                    else 
                    {
                        st = test_st;
                    }
                }
                while (st.advance());
                return true;
            }

            bool detect_core(state & st)
            {
                return detect_hangul_syllable(st) ||
                        detect_ri_sequence(st) ||
                        detect_xpicto_sequence(st) ||
                        detect_conjunct_cluster(st) ||
                        detect_general(st);
            }

            void skip_postcore(state & st)
            {
                while (st.basic_props() == table::extend ||
                       st.c == U'\u200D' ||
                       st.basic_props() == table::spacing_mark)
                {
                    if (!st.advance())
                        break;
                }
            }

            bool detect_hangul_syllable(state & st)
            {
                bool success = false;
                bool can_have_t = true;

                while (st.basic_props() == table::hangul_l)
                {
                    if (!st.advance())
                        return true;

                    success = true;
                    can_have_t = false;
                }

                if (st.basic_props() == table::hangul_lvt)
                {
                    if (!st.advance())
                        return true;

                    success = true;
                    can_have_t = true;
                } 
                else if (st.basic_props() == table::hangul_lv || st.basic_props() == table::hangul_v)
                {
                    if (!st.advance())
                        return true;
                    
                    while (st.basic_props() == table::hangul_v)
                    {
                        if (!st.advance())
                            return true;
                    }

                    success = true;
                    can_have_t = true;
                }

                if (can_have_t && st.basic_props() == table::hangul_t)
                {
                    if (!st.advance())
                        return true;

                    while(st.basic_props() == table::hangul_t)
                    {
                        if (!st.advance())
                            return true;
                    }
                    success = true;
                }

                return success;
            }

            bool detect_ri_sequence(state & st)
            {
                if (st.basic_props() != table::regional_indicator)
                    return false;

                //technically only RI RI matches but since single RI isn't going
                //to match anything else and fall back to default rule after trying
                //all the rest we might as well accept it as valid here and save some cycles

                if (st.advance() && st.basic_props() == table::regional_indicator)
                    (void)st.advance();

                return true;
            }

            bool detect_xpicto_sequence(state & st)
            {
                if (!(st.props & table::extended_pictographic))
                    return false;

                if (!st.advance())
                    return true;

                do
                {
                    It test_it = st.current();
                    state test_st{test_it, st};
                    
                    while (test_st.basic_props() == table::extend)
                    {
                        if (!test_st.advance())
                            return true;
                    }
                    if (test_st.c != U'\u200D' || !test_st.advance()) //ZWJ
                        return true;

                    if (!(test_st.props & table::extended_pictographic))
                        return true;

                    st = test_st;
                }
                while(st.advance());
                return true;
            }

            bool detect_conjunct_cluster(state & st)
            {
                if (st.in_cb_props() != table::in_cb_consonant)
                    return false;
                
                It test_it = st.current();
                state test_st{test_it, st};
                
                if (!test_st.advance())
                    return false;
                
                bool success = false;
                for( ; ; )
                {
                    bool has_linker = false;
                    table::value last_cb_props;
                    for( ; ; )
                    {
                        last_cb_props = test_st.in_cb_props();
                        if (last_cb_props == table::in_cb_extend)
                        {
                            if (!test_st.advance())
                                return success;
                            continue;
                        }
                        
                        if (last_cb_props == table::in_cb_linker)
                        {
                            if (!test_st.advance())
                                return success;
                            has_linker = true;
                            continue;
                        }
                        
                        if (has_linker && last_cb_props == table::in_cb_consonant)
                            break;
                        
                        return success;
                    }

                    success = true;
                    bool advance_res = test_st.advance();
                    st = test_st;
                    if (!advance_res)
                        return success;
                }
            }

            bool detect_general(state & st)
            {
                if (st.c == U'\u000D' || st.c == U'\u000A' || st.basic_props() == table::control)
                    return false;

                (void)st.advance();
                return true;
            }
        };
    }

    template<std::forward_iterator It, std::sentinel_for<It> EndIt>
    requires(std::is_same_v<std::iter_value_t<It>, char32_t>)
    void advance_to_grapheme_cluster_break(It & it, const EndIt & end)
    {
        util::grapheme_cluster_step<It, EndIt> step;
        step(it, end);
    }

    template<std::forward_iterator It, std::sentinel_for<It> EndIt>
    requires(std::is_same_v<std::iter_value_t<It>, char32_t>)
    It find_grapheme_cluster_break(It it, const EndIt & end)
    {
        advance_to_grapheme_cluster_break(it, end);
        return it;
    }

    template<std::ranges::forward_range Range>
    requires(std::is_same_v<std::ranges::range_value_t<Range>, char32_t>)
    std::ranges::borrowed_iterator_t<Range> find_grapheme_cluster_break(Range && range)
    {
        return find_grapheme_cluster_break(std::ranges::begin(std::forward<Range>(range)), std::ranges::end(std::forward<Range>(range)));
    }

}

#endif
