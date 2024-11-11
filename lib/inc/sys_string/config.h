//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_CONFIG_H_INCLUDED
#define HEADER_SYS_STRING_CONFIG_H_INCLUDED

#if __cplusplus < 202002L || (defined(_MSVC_LANG) && _MSVC_LANG < 202002L)
    #error This library requires C++20 or above
#endif

#if __cpp_impl_three_way_comparison < 201907
    #error This library requires your compiler to support three-way comparison
#endif

#if __cpp_char8_t < 201811
    #error This library requires your compiler to support char8_t
#endif



#if (defined(__APPLE__) && defined(__MACH__))

    #include <CoreFoundation/CoreFoundation.h>

#elif defined(__ANDROID__)
    
    #include <jni.h>

#elif defined(_WIN32)
    #define SYS_STRING_WCHAR_T_IS_UTF16 1

    #ifndef NOMINMAX
    #define NOMINMAX
    #endif

    #include <wtypes.h>
    #include <winstring.h>
    #include <oleauto.h>

#elif defined(__EMSCRIPTEN__)

    #include <emscripten/emscripten.h>
    #include <emscripten/val.h>

#endif

#if SYS_STRING_USE_PYTHON
    #if defined(SYS_STRING_ENABLE_PYTHON)
        #undef SYS_STRING_ENABLE_PYTHON
    #endif

    #define SYS_STRING_ENABLE_PYTHON 1
#endif

#if SYS_STRING_ENABLE_PYTHON
    #include <Python.h>
    #if PY_MAJOR_VERSION < 3 || (PY_MAJOR_VERSION == 3 && PY_MINOR_VERSION < 7)
        #error This code requires Python 3.7 or newer
    #endif
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

//GCC up to 11.3 has a weird constexpr bug in some palces
#if __GNUC__ > 11 || (__GNUC__ == 11 && __GNUC_MINOR__ > 2)
    #define BUGGY_CONSTEXPR constexpr
#else 
    #define BUGGY_CONSTEXPR 
#endif

#if __has_include(<version>)
    #include <version>
#endif

#if __cpp_lib_endian < 201907
    #error Your standard library does not support std::endian
#endif

#if __cpp_lib_ranges < 201911
    #error Your standard library does not support ranges
#endif



#endif
