//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_CONFIG_H_INCLUDED
#define HEADER_SYS_STRING_CONFIG_H_INCLUDED

#define SYS_STRING_USE_SPACESHIP_OPERATOR (__cpp_impl_three_way_comparison >= 201907)
#define SYS_STRING_USE_CHAR8              (__cpp_char8_t >= 201811)
#define SYS_STRING_USE_SPAN               (__cpp_lib_span >= 202002)
#define SYS_STRING_USE_STD_ENDIAN         (__cpp_lib_endian >= 201907)

#if __cpp_lib_constexpr_algorithms >= 201806
    #define SYS_STRING_CONSTEXPR_ALGO  constexpr
#else
    #define SYS_STRING_CONSTEXPR_ALGO
#endif

#if defined(_WIN32)
    #define SYS_STRING_WCHAR_T_IS_UTF16 1
#endif

#if __SIZEOF_POINTER__ == 8 || (defined(_MSC_VER) && _WIN64)
    #define SYS_STRING_SIZEOF_POINTER 8
#elif __SIZEOF_POINTER__ == 4 || (defined(_MSC_VER) && _WIN32)
    #define SYS_STRING_SIZEOF_POINTER 4
#else
    #error Please define pointer size for your platform
#endif

#if defined(_MSC_VER)
    #define SYS_STRING_FORCE_INLINE __forceinline
#elif defined(__GNUC__)
    #define SYS_STRING_FORCE_INLINE [[gnu::always_inline]] inline
#else
    #error Please define how to force inline for your compiler
#endif

#if SYS_STRING_USE_STD_ENDIAN

    #include <bit>
    namespace sysstr
    {
        using endian = std::endian; 
    }

#else

    namespace sysstr
    {
        enum class endian
        {
        #ifdef _MSC_VER
            little = 0,
            big    = 1,
            native = little
        #elif defined(__BYTE_ORDER__)
            little = __ORDER_LITTLE_ENDIAN__,
            big    = __ORDER_BIG_ENDIAN__,
            native = __BYTE_ORDER__
        #else 
            #error Please define byte order values for your compiler
        #endif
        };
    }

#endif


#endif
