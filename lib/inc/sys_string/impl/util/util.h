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
#include <tuple>

namespace sysstr::util
{
    template<bool Val, class... Args>
    static constexpr bool dependent_bool = Val;

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
