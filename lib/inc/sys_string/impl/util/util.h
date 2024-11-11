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
#include <compare>

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

    template<class Char>
    inline constexpr auto compare_3way(const Char * lhs, size_t lhs_size, const Char * rhs, size_t rhs_size) noexcept
    {
        auto shortest_length = std::min(lhs_size, rhs_size);
        int res = std::char_traits<Char>::compare(lhs, rhs, shortest_length);
        if (auto ord = (res <=> 0); ord != 0)
            return ord;
        return lhs_size <=> rhs_size;
    }

    template<class OutT, std::forward_iterator It, std::sentinel_for<It> Sentinel, std::output_iterator<OutT> OutIt, class Pred>
    auto split(It str_start, Sentinel str_end, OutIt dest, Pred pred) -> OutIt
    requires(std::is_constructible_v<OutT, std::tuple_element_t<0, decltype(pred(str_start, str_end))>, std::tuple_element_t<1, decltype(pred(str_start, str_end))>>)
    {
        for( ; ; )
        {
            auto [found_start, found_end] = pred(str_start, str_end);
            *dest++ = OutT(str_start, found_start);
            if (found_start == found_end)
                break;
            str_start = found_end;
        }
        return dest;
    }

}

#endif
