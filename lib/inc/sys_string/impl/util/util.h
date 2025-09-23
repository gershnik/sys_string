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
#include <ranges>
#include <algorithm>
#include <tuple>
#include <utility>
#include <cstdarg>
#include <cassert>
#include <cstdio>

namespace sysstr::util
{
    template<bool Val, class... Args>
    static constexpr bool dependent_bool = Val;

    template<class Char, size_t N>
    struct ct_string {
        using char_type = Char;

        Char chars[N];

        constexpr ct_string(const Char (&src)[N]) noexcept
        {
            std::ranges::copy(src, chars);
        }

        constexpr auto size() const -> size_t { return N; }

        constexpr auto operator[](size_t i) const -> Char { return chars[i]; }

        friend constexpr bool operator==(const ct_string & lhs, const ct_string & rhs) {
            return std::ranges::equal(lhs.chars, rhs.chars);
        }

        constexpr auto begin() const { return chars; }
        constexpr auto end() const { return chars + N; }
    };

    template<class Char, size_t N>
    ct_string(const Char (&src)[N]) -> ct_string<Char, N>;

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

    template<std::ranges::contiguous_range Range>
    requires(
        std::is_same_v<std::ranges::range_value_t<Range>, char> &&
        requires(Range & range) { range.resize(size_t(0)); }
    )
    int vprintf_to(Range & range, const char * format, va_list vl)
    {
        va_list try_vl;
        va_copy(try_vl, vl);
        const int size = vsnprintf(0, 0, format, try_vl);
        va_end(try_vl);
        if (size == -1)
            return size;
        range.resize(size + 1);
        const int ret = vsnprintf(std::ranges::data(range), std::ranges::size(range), format, vl);
        va_end(vl);
        if (ret >= 0)
            range.resize(ret);
        return ret;
    }

    #if __cpp_lib_unreachable >= 202202L

        #define SYS_STRING_UNREACHABLE assert(false); ::std::unreachable()

    #else

        [[noreturn]] inline void unreachable()
        {
            // Uses compiler specific extensions if possible.
            // Even if no extension is used, undefined behavior is still raised by
            // an empty function body and the noreturn attribute.
        #if defined(_MSC_VER) && !defined(__clang__) // MSVC
            __assume(false);
        #else // GCC, Clang
            __builtin_unreachable();
        #endif
        }

        #define SYS_STRING_UNREACHABLE assert(false); ::sysstr::util::unreachable()

    #endif

    template<std::unsigned_integral T>
    constexpr T saturated_mul_div(T val, std::convertible_to<T> auto numerator, std::convertible_to<T> auto denominator) {
        T num = T(numerator);
        T denom = T(denominator);
        T whole = num / denom;
        num = num % denom;

        T quot = val / denom;
        T rem = val % denom;
        T increment = quot * num;

        T extra = rem * num;
        quot = extra / denom;
        rem = extra % denom;
        increment += quot;
        increment += (2 * rem >= denom);

        if (whole && (std::numeric_limits<T>::max() - increment) / whole < val)
            return std::numeric_limits<T>::max();
        return val * whole + increment;
    }

}

#endif
