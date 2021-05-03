//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_UTIL_H_INCLUDED
#define HEADER_SYS_STRING_UTIL_H_INCLUDED

#include <sys_string/config.h>

#include <iterator>
#include <algorithm>
#include <string>
#if SYS_STRING_USE_SPACESHIP_OPERATOR
    #include <compare>
#endif

namespace sysstr::util
{
    class count_output_iterator
    {
    public:
        using value_type = void;
        using difference_type = void;
        using pointer = void ;
        using reference = void;
        using iterator_category = std::output_iterator_tag;
    public:
        constexpr count_output_iterator(size_t & count) noexcept:
            m_count(&count)
        {}

        template<class Value>
        constexpr count_output_iterator & operator=(Value &&) noexcept
            { ++(*this->m_count); return *this;}
        
        constexpr count_output_iterator & operator*() noexcept     {return *this;}
        constexpr count_output_iterator & operator++() noexcept    {return *this;}
        constexpr count_output_iterator & operator++(int) noexcept {return *this;}

    private:
        size_t * const m_count;
    };

    template<bool Val, class... Args>
    static constexpr bool dependent_bool = Val;

    inline constexpr auto make_compare_result(int res)
    {
    #if SYS_STRING_USE_SPACESHIP_OPERATOR
        return res <=> 0;
    #else
        return res;
    #endif
    }

    template<class T>
    inline constexpr auto compare_3way(T a, T b)
    {
    #if SYS_STRING_USE_SPACESHIP_OPERATOR
        return a <=> b;
    #else
        return (a > b) - (a < b);
    #endif
    }

    template<class Char>
    inline constexpr auto compare_3way(const Char * lhs, size_t lhs_size, const Char * rhs, size_t rhs_size) noexcept
    {
        auto shortest_length = std::min(lhs_size, rhs_size);
        int res = std::char_traits<Char>::compare(lhs, rhs, shortest_length);
    #if SYS_STRING_USE_SPACESHIP_OPERATOR
        if (res != 0)
            return res <=> 0;
    #else
        if (res != 0)
            return res;
    #endif
        return compare_3way(lhs_size, rhs_size);
    }

    template<class InputIt1, class InputIt2>
    inline constexpr auto lexicographical_compare_3way(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
    {
        for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) 
        {
            if (auto res = compare_3way(*first1, *first2))
                return res;
        }
    #if SYS_STRING_USE_SPACESHIP_OPERATOR
        return (first2 == last2) <=> (first1 == last1);
    #else
        return (first2 == last2) - (first1 == last1);
    #endif
    }

    template<class InputIt1, class InputIt2, class Compare3Way>
    inline constexpr auto lexicographical_compare_3way(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, Compare3Way comp)
    {
        for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) 
        {
            if (auto res = comp(*first1, *first2); res != 0)
                return res;
        }
    #if SYS_STRING_USE_SPACESHIP_OPERATOR
        return (first2 == last2) <=> (first1 == last1);
    #else
        return (first2 == last2) - (first1 == last1);
    #endif
    }

    template<class OutT, class InIt, class Pred, class OutIt>
    auto split(InIt str_start, InIt str_end, OutIt dest, Pred pred) -> OutIt
    {
        for( ; ; )
        {
            auto [found_start, found_end] = pred(str_start, str_end);
            *dest++ = OutT(str_start, found_start);
            if (found_start == str_end)
                break;
            str_start = found_end;
        }
        return dest;
    }

}

#endif
