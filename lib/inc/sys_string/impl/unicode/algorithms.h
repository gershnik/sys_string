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

#include <vector>

namespace sysstr
{
    namespace util
    {
        template<class T, size_t StackLimit>
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
                    m_data.heap.~vector();
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
        
        template<class OutIt>
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
            template<class Cursor>
            auto operator()(Cursor cursor) const noexcept -> char32_t
            {
                if (any_non_cased_then_cased(cursor.reverse()))
                {
                    ++cursor;
                    if (!any_non_cased_then_cased(cursor))
                    {
                        return U'\u03C2'; //replace with ς
                    }
                }
                return U'\u03C3'; // replace with σ
            }
        private:
            template<class Cursor>
            auto any_non_cased_then_cased(Cursor cursor) const noexcept -> bool
            {
                for( ; cursor; ++cursor)
                {
                    auto c = *cursor;
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
        template<class Cursor, class OutIt>
        inline auto operator()(Cursor cursor, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<OutEnc>())) -> OutIt
        {
            using namespace util::unicode;
            
            for( ; cursor; ++cursor)
            {
                auto c = *cursor;
                if (c != U'\u03A3') // not Σ
                {
                    dest = case_mapper::to_lower::map_char<OutEnc>(c, dest);
                }
                else
                {
                    char32_t lc = util::sigma_tolower()(cursor);
                    auto writer = utf32_output<OutEnc>::make_writer([&](auto value) {
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
        template<class Cursor, class OutIt>
        inline auto operator()(Cursor cursor, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<OutEnc>())) -> OutIt
        {
            using namespace util::unicode;
            
            for( ; cursor; ++cursor)
            {
                auto c = *cursor;
                dest = case_mapper::to_upper::map_char<OutEnc>(c, dest);
            }

            return dest;
        }
    };

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
            template<class InIt, class OutIt>
            inline auto operator()(InIt first, InIt last, OutIt dest) -> OutIt
            {
                using namespace util;
                using namespace util::unicode;

                stack_or_heap_buffer<uint32_t, 32> buffer;
                auto inserter = std::back_inserter(buffer);

                for ( ; first != last; ++first)
                {
                    char32_t c = *first;
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
            template<class OutIt>
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

            template<class RanIt>
            static void sort(RanIt first, RanIt last)
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
            template<class FwdIt, class OutIt>
            inline auto operator()(FwdIt first, FwdIt last, OutIt dest) -> OutIt
            {
                using namespace util;
                
                struct sink
                {
                    OutIt dest;
                    
                    SYS_STRING_FORCE_INLINE
                    void copy(FwdIt it)
                        { dest = write_unsafe<OutEnc>(*it, this->dest); }
                    SYS_STRING_FORCE_INLINE
                    void copy(FwdIt first, FwdIt last)
                    {
                        for ( ; first != last; ++first)
                            dest = write_unsafe<OutEnc>(*first, this->dest);
                    }
                    SYS_STRING_FORCE_INLINE
                    void write(char32_t c)
                        { dest = write_unsafe<OutEnc>(c, dest); }
                } s {dest};
                
                this->call_with_sink(first, last, s);
                return s.dest;
            }
            
            
            template<class FwdIt, class Sink>
            inline void call_with_sink(FwdIt first, FwdIt last, Sink & sink)
            {
                using namespace util;
                using namespace util::unicode;
                
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
            template<class FwdIt, class Sink>
            static void convert_slow(util::stack_or_heap_buffer<char32_t, 32> & buffer, FwdIt first, FwdIt last, Sink & sink)
            {
                nfd<utf32>()(first, last, std::back_inserter(buffer));
                convert_from_nfd(buffer.begin(), buffer.end(), sink);
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

            template<class It, class Sink>
            static inline void convert_from_nfd(It first, It last, Sink & sink)
            {
                using namespace util;
                using namespace util::unicode;

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

            template<class FwdIt>
            static auto find_replacements(char32_t & c, FwdIt & next, FwdIt last, size_t skips[32]) -> size_t
            {
                using namespace util;
                using namespace util::unicode;

                auto * compositions = normalizer::get_compositions(c);
                if (!compositions)
                    return 0;

                size_t skips_size = 0;
                size_t it_idx = 0;
                int last_ccc = -1;
                FwdIt it = next;
                
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

            template<class FwdIt>
            static bool find_hangul_replacement(char32_t & c, FwdIt & next, FwdIt last)
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

}

#endif
