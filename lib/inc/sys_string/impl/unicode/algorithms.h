//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_UNICODE_ALGORITHMS_H_INCLUDED
#define HEADER_SYS_STRING_UNICODE_ALGORITHMS_H_INCLUDED

#if SYS_STRING_USE_ICU
    #include <sys_string/impl/unicode/mappings_icu.h>
#else
    #include <sys_string/impl/unicode/mappings.h>
#endif
#include <sys_string/impl/unicode/utf_util.h>
#include <sys_string/impl/util/iter_util.h>

#include <ranges>
#include <iterator>
#include <vector>
#include <cstring>

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

        template<std::integral T, size_t StackLimit>
        struct stack_or_heap_buffer
        {
        public:
            using value_type = T;

            stack_or_heap_buffer():
                m_first(m_data.stack),
                m_size(0)
            {}
            stack_or_heap_buffer(const stack_or_heap_buffer &) = delete;
            stack_or_heap_buffer & operator=(const stack_or_heap_buffer &) = delete;
            ~stack_or_heap_buffer()
            {
                if (m_first != m_data.stack)
                    m_data.heap.~vector<T>();
            }

            void push_back(T val)
            {
                if (m_size < StackLimit)
                {
                    m_first[m_size++] = val;
                }
                else if (m_size == StackLimit)
                {
                    std::vector<T> heap;
                    heap.reserve(m_size + 1);
                    heap.insert(heap.begin(), m_first, m_first + m_size);
                    heap.push_back(val);
                    new (&m_data.heap) std::vector<T>(std::move(heap));
                    m_first = m_data.heap.data();
                    ++m_size;
                }
                else
                {
                    m_data.heap.push_back(val);
                    m_first = m_data.heap.data();
                    ++m_size;
                }
            }

            void clear()
            {
                if (m_first != m_data.stack)
                    m_data.heap.clear();
                m_size = 0;
            }
            
            void reserve(size_t size)
            {
                if (size < StackLimit)
                    return;
                
                if (m_size == 0)
                {
                    new (&m_data.heap) std::vector<T>();
                    m_data.heap.reserve(size);
                    m_first = m_data.heap.data();
                }
                else if (size <= StackLimit)
                {
                    std::vector<T> heap;
                    heap.reserve(size);
                    heap.insert(heap.begin(), m_first, m_first + m_size);
                    new (&m_data.heap) std::vector<T>(std::move(heap));
                    m_first = m_data.heap.data();
                }
                else
                {
                    m_data.heap.reserve(size);
                    m_first = m_data.heap.data();
                }
            }

            void erase_front(size_t size)
            {
                if (m_first != m_data.stack)
                    m_data.heap.erase(m_data.heap.begin(), m_data.heap.begin() + size);
                else
                    memmove(m_first, m_first + size, (m_size - size) * sizeof(T));
                m_size -= size;
            }

            T * begin() { return m_first; }
            const T * begin() const { return m_first; }
            T * end() { return m_first + m_size; }
            const T * end() const { return m_first + m_size; }
            size_t size() const { return m_size; }
            bool empty() const { return m_size == 0; }

            T operator[](size_t idx) const { return m_first[idx]; }
        private:
            union data
            {
                data()
                {}
                ~data()
                {}
                T stack[StackLimit];
                std::vector<T> heap;
            } m_data;
            T * m_first;
            size_t m_size;
        };


    }
    struct isspace
    {
        auto operator()(char32_t c) const noexcept -> bool
        {
            return util::unicode::is_whitespace::test(c);
        }
    };

    template<utf_encoding OutEnc>
    struct casefold
    {
        static constexpr auto max_output_length = util::unicode::case_mapper::fold::max_mapped_length;
        
        template<std::output_iterator<utf_char_of<OutEnc>> OutIt>
        auto operator()(char32_t c, OutIt dest) const noexcept(noexcept(*dest++ = utf_char_of<OutEnc>())) -> OutIt
        {
            return util::unicode::case_mapper::fold::map_char<OutEnc>(c, dest);
        }
    };

    namespace util
    {
        class sigma_tolower
        {
        public:
            template<ranges::reversible_range Range>
            SYS_STRING_FORCE_INLINE
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
                    auto props = unicode::case_mapper::props::get(c);
                    if (props & unicode::case_mapper::props::case_ignorable)
                        continue;
                    return (props & unicode::case_mapper::props::cased);
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
                    dest = case_mapper::to_lower::map_char<OutEnc>(c, dest);
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
                dest = case_mapper::to_upper::map_char<OutEnc>(c, dest);
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

#if !SYS_STRING_USE_ICU

    class normalize
    {
    private:
        static constexpr int SBase = 0xAC00,
            LBase = 0x1100, VBase = 0x1161, TBase = 0x11A7,
            LCount = 19, VCount = 21, TCount = 28,
            NCount = VCount * TCount,   // 588
            SCount = LCount * NCount;   // 11172

    public:
        template<utf_encoding OutEnc>
        class nfd
        {
        public:
            template<std::ranges::input_range Range, std::output_iterator<utf_char_of<OutEnc>> OutIt>
            requires(utf_encoding_of<std::ranges::range_value_t<Range>> == utf32)
            inline auto operator()(const Range & range, OutIt dest) -> OutIt
            {
                using namespace util;
                using namespace util::unicode;

                stack_or_heap_buffer<uint32_t, 32> buffer;
                auto inserter = std::back_inserter(buffer);

                for (char32_t c: range)
                {
                    size_t insert_idx = buffer.size();
                    
                    if (insert_idx == 0 && c < 128)
                    {
                        *dest = utf_char_of<OutEnc>(c);
                        ++dest;
                        continue;
                    }
                    
                    if (c < SBase || c >= SBase + SCount)
                        normalizer::decompose<OutEnc>(c, inserter);
                    else
                        this->decompose_hangul(char16_t(c), inserter);

                    size_t new_size = buffer.size();

                    if (new_size == 1 && buffer[0] == c)
                    {
                        dest = write_unsafe<OutEnc>(c, dest);
                        buffer.clear();
                        continue;
                    }

                    if ((buffer[insert_idx] >> 21) == 0)
                    {
                        this->sort(buffer.begin(), buffer.begin() + insert_idx);
                        ++insert_idx;
                        size_t copied = 0;
                        for( ; copied < insert_idx; ++copied)
                        {
                            dest = write_unsafe<OutEnc>(char32_t(buffer[copied] & 0x1FFFFF), dest);
                        }
                        for( ; copied < new_size; ++copied)
                        {
                            auto val = buffer[copied]; 
                            if ((val >> 21) != 0)
                                break;
                            dest = write_unsafe<OutEnc>(char32_t(val & 0x1FFFFF), dest);
                        }
                        buffer.erase_front(copied);
                    }
                }
                if (!buffer.empty())
                {
                    this->sort(buffer.begin(), buffer.end());
                    for(size_t i = 0; i < buffer.size(); ++i)
                        dest = write_unsafe<OutEnc>(char32_t(buffer[i] & 0x1FFFFF), dest);
                }
                return dest;
            }
        private:
            //see https://www.unicode.org/versions/Unicode16.0.0/core-spec/chapter-3/#G61399
            template<std::output_iterator<uint32_t> OutIt>
            static auto decompose_hangul(char16_t s, OutIt dest) noexcept(noexcept(*dest++ = uint32_t())) -> OutIt
            {
                uint32_t SIndex = s - SBase;
                
                auto l = uint32_t(LBase + SIndex / NCount);
                *dest++ = l;
                auto v = uint32_t(VBase + (SIndex % NCount) / TCount);
                *dest++ = v;
                auto t = uint32_t(TBase + SIndex % TCount);
                if (t != TBase)
                    *dest++ = t;
                return dest;
            }

            template<std::random_access_iterator It, std::sized_sentinel_for<It> EndIt>
            requires(std::is_same_v<std::iter_value_t<It>, uint32_t>)
            static void sort(It first, EndIt last)
            {
                auto len = last - first;
                
                if (len < 2)
                    return;

                if (len == 2)
                {
                    auto & a = *first;
                    ++first;
                    auto & b = *first;
                    if ((a >> 21) > (b >> 21))
                        std::swap(a, b);
                    return;
                }
                
                if (len >= 8)
                {
                    std::sort(first, last, [] (uint32_t lhs, uint32_t rhs) {
                        return (lhs >> 21) < (rhs >> 21);
                    });
                    return;
                }

                for ( ; ; )
                {
                    bool swapped = false;
                    for (auto cur_it = first; ; )
                    {
                        auto & prev = *cur_it;
                        if (++cur_it == last)
                            break;
                        auto & cur = *cur_it;
                        if ((prev >> 21) > (cur >> 21))
                        {
                            std::swap(prev, cur);
                            swapped = true;
                        }
                    }
                    if (!swapped)
                        break;
                }
            }
        };

        template<utf_encoding OutEnc>
        class nfc
        {
        private:
            using nfc_qc_status = util::unicode::normalizer::nfc_qc_status;
        public:
            template<std::ranges::forward_range Range, std::output_iterator<utf_char_of<OutEnc>> OutIt>
            requires(utf_encoding_of<std::ranges::range_value_t<Range>> == utf32)
            inline auto operator()(const Range & range, OutIt dest) -> OutIt
            {
                using namespace util;
                
                struct sink
                {
                    OutIt dest;
                    
                    SYS_STRING_FORCE_INLINE
                    void copy(std::ranges::iterator_t<Range> it)
                        { dest = write_unsafe<OutEnc>(*it, this->dest); }
                    SYS_STRING_FORCE_INLINE
                    void copy(std::ranges::iterator_t<Range> first, std::ranges::iterator_t<Range> last)
                    {
                        for ( ; first != last; ++first)
                            dest = write_unsafe<OutEnc>(*first, this->dest);
                    }
                    SYS_STRING_FORCE_INLINE
                    void write(char32_t c)
                        { dest = write_unsafe<OutEnc>(c, dest); }
                } s {dest};
                
                (*this)(range, s);
                return s.dest;
            }
            
            
            template<std::ranges::forward_range Range, class Sink>
            requires(utf_encoding_of<std::ranges::range_value_t<Range>> == utf32)
            inline void operator()(const Range & range, Sink & sink)
            {
                using namespace util;
                using namespace util::unicode;
                
                auto first = std::ranges::begin(range);
                auto last = std::ranges::end(range);
                
                if (first == last)
                    return;
                
                stack_or_heap_buffer<char32_t, 32> buffer;

                auto cur = first;
                for ( ; ; )
                {
                    auto status = get_nfc_qc_status(*cur);
                    
                check_again:
                    if (status == nfc_qc_status::bad)
                    {
                        for (++cur; ; ++cur)
                        {
                            if (cur == last)
                            {
                                convert_slow(buffer, first, cur, sink);
                                return;
                            }
                            status = get_nfc_qc_status(*cur);
                            if (status == nfc_qc_status::stable)
                                break;
                        }
                        convert_slow(buffer, first, cur, sink);
                        buffer.clear();
                        first = cur;
                        if (++cur == last)
                        {
                            sink.copy(first);
                            return;
                        }
                    }
                    else if (status == nfc_qc_status::stable)
                    {
                        auto prev = cur;
                        for (++cur; ; ++cur)
                        {
                            if (cur == last)
                            {
                                sink.copy(first, cur);
                                return;
                            }
                            status = get_nfc_qc_status(*cur);
                            if (status != nfc_qc_status::stable)
                            {
                                sink.copy(first, prev);
                                first = prev;
                                goto check_again;
                            }
                            prev = cur;
                        }
                    }
                    else
                    {
                        if (++cur == last)
                        {
                            sink.copy(first, cur);
                            return;
                        }
                    }
                }
            }

        private:
            template<std::forward_iterator It, std::sentinel_for<It> EndIt, class Sink>
            requires(std::is_same_v<std::iter_value_t<It>, char32_t>)
            static void convert_slow(util::stack_or_heap_buffer<char32_t, 32> & buffer,It first, EndIt last, Sink & sink)
            {
                if constexpr (std::sized_sentinel_for<It, EndIt>)
                    buffer.reserve(last - first);
                nfd<utf32>()(std::ranges::subrange{first, last}, std::back_inserter(buffer));
                convert_from_nfd(buffer, sink);
            }
            
            
            SYS_STRING_FORCE_INLINE
            static auto get_nfc_qc_status(char32_t c) -> nfc_qc_status
            {
                using namespace util;
                using namespace util::unicode;
                
                if (c >= SBase && c < SBase + SCount)
                    return nfc_qc_status::good;
                    
                if ((c >= LBase && c < LBase + LCount) ||
                    (c >= VBase && c < VBase + VCount) ||
                    (c >= TBase && c < TBase + TCount))
                    return nfc_qc_status::bad;
                
                return normalizer::get_nfc_qc_status(c);
            }

            template<std::ranges::forward_range Range, class Sink>
            requires(utf_encoding_of<std::ranges::range_value_t<Range>> == utf32)
            static inline void convert_from_nfd(const Range & range, Sink & sink)
            {
                using namespace util;
                using namespace util::unicode;

                auto first = std::ranges::begin(range);
                auto last = std::ranges::end(range);

                while (first != last) 
                {
                    char32_t c = *first;
                    auto next = first;
                    if (++next == last)
                    {
                        sink.write(c);
                        break;
                    }

                    if (find_hangul_replacement(c, next, last))
                    {
                        sink.write(c);
                        first = next;
                        continue;
                    }

                    size_t skips[32];

                    size_t skips_len = find_replacements(c, next, last, skips);
                    sink.write(c);
                    ++first;

                    if (skips_len == 0)
                    {
                        for ( ; first != next; ++first)
                            sink.write(*first);
                    }
                    else
                    {
                        for(size_t idx = 0, skips_idx = 0; first != next; ++first, ++idx)
                        {
                            if (idx == skips[skips_idx])
                            {
                                ++skips_idx;
                                continue;
                            }
                            sink.write(*first);
                        }
                    }
                }
            }

            template<std::forward_iterator It, std::sentinel_for<It> EndIt>
            requires(std::is_same_v<std::iter_value_t<It>, char32_t>)
            static auto find_replacements(char32_t & c, It & next, EndIt last, size_t skips[32]) -> size_t
            {
                using namespace util;
                using namespace util::unicode;

                auto * compositions = normalizer::get_compositions(c);
                if (!compositions)
                    return 0;

                size_t skips_size = 0;
                size_t it_idx = 0;
                int last_ccc = -1;
                It it = next;
                
                for ( ; ; )
                {
                    char32_t nc = *it;
                    int ccc = normalizer::get_comb_class(nc);
                    if (ccc > last_ccc)
                    {
                        auto repl = find_composition(nc | (ccc << 21), compositions);
                        if (repl != 0)
                        {
                            auto repl_val = char32_t(repl & 0x1F'FFFF);
                            auto repl_ccc = repl >> 21;
                            c = repl_val;
                            skips[skips_size] = it_idx;
                            ++skips_size;
                            ++it;
                            ++it_idx;
                            next = it;
                            if (repl_ccc != 0 || next == last)
                                return skips_size;
                            compositions = normalizer::get_compositions(c);
                            if (!compositions)
                                return skips_size;
                            continue;
                        }
                    }
                    if (ccc == 0)
                        return skips_size;
                    if (++it == last)
                        return skips_size;
                    ++it_idx;
                    last_ccc = ccc;
                }
                
            }

            SYS_STRING_FORCE_INLINE
            static auto find_composition(uint32_t val , const uint32_t * compositions) -> uint32_t
            {
                for ( ; ; )
                {
                    uint32_t cur = compositions[0] & 0x1FFF'FFFF;
                    if (cur == val)
                        return compositions[1] & 0x1FFF'FFFF; //will be 0 if blocked
                    bool is_last = (compositions[0] >> 29);
                    if (is_last)
                        return 0;
                    compositions += 2;
                }
            }

            template<std::forward_iterator It, std::sentinel_for<It> EndIt>
            requires(std::is_same_v<std::iter_value_t<It>, char32_t>)
            static bool find_hangul_replacement(char32_t & c, It & next, EndIt last)
            {
                if (c < LBase || c >= LBase + LCount)
                    return false;
                
                int LIndex = c - LBase;

                char32_t second = *next;
                if (second < VBase || second >= VBase + VCount)
                    return true;
                int VIndex = second - VBase;
                
                c = char32_t(SBase + (LIndex * VCount + VIndex) * TCount);
                
                if (++next == last)
                    return true;

                char32_t third = *next;
                if (third < TBase || third >= TBase + TCount)
                    return true;

                int TIndex = third - TBase;
                c += TIndex;
                ++next;
                
                return true;
            }
        };
    };

#endif

}

#endif
