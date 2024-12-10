//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_BITMASK_ENUM_H_INCLUDED
#define HEADER_SYS_STRING_BITMASK_ENUM_H_INCLUDED

#include <sys_string/config.h>

#include <type_traits>

namespace sysstr
{
    template<class T>
    requires(std::is_enum_v<T>)
    inline constexpr bool bitmask_enum = false;


    template<class E>
    requires(bitmask_enum<E>)
    inline constexpr E operator|(E lhs, E rhs) noexcept
        { return E(std::underlying_type_t<E>(lhs) | std::underlying_type_t<E>(rhs)); }
    template<class E>
    requires(bitmask_enum<E>)
    inline E operator&(E lhs, E rhs) noexcept
        { return E(std::underlying_type_t<E>(lhs) & std::underlying_type_t<E>(rhs)); }
    template<class E>
    requires(bitmask_enum<E>)
    inline E operator^(E lhs, E rhs) noexcept
        { return E(std::underlying_type_t<E>(lhs) ^ std::underlying_type_t<E>(rhs)); }
    template<class E>
    requires(bitmask_enum<E>)
    inline E operator~(E val) noexcept
        { return E(~std::underlying_type_t<E>(val)); }

}

#endif



