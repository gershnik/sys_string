//
// Copyright 2021 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_H_INSIDE
#error This header must not be included directly. Please include sys_string.h
#endif

#include <sys_string/impl/platforms/unix_generic.h>

#if SYS_STRING_ENABLE_PYTHON
    #include <sys_string/impl/platforms/python_any.h>
#endif

#if SYS_STRING_USE_PYTHON
    namespace sysstr
    {
        using sys_string = sys_string_pystr;
        using sys_string_builder = sys_string_pystr_builder;
    }

    #define SYS_STRING_STATIC SYS_STRING_STATIC_PYSTR

#elif SYS_STRING_USE_GENERIC

    namespace sysstr
    {
        using sys_string = sys_string_generic;
        using sys_string_builder = sys_string_generic_builder;
    }

    #define SYS_STRING_STATIC SYS_STRING_STATIC_GENERIC

#endif
   
#if (defined(__APPLE__) && defined(__MACH__))
    #include <sys_string/impl/platforms/apple_cfstr.h>

    #if !SYS_STRING_USE_PYTHON && !SYS_STRING_USE_GENERIC
        namespace sysstr
        {
            using sys_string = sys_string_cfstr;
            using sys_string_builder = sys_string_cfstr_builder;
        }

        #define SYS_STRING_STATIC SYS_STRING_STATIC_CFSTR
    #endif

#elif defined(__ANDROID__)

    #include <sys_string/impl/platforms/android_java.h>

    #if !SYS_STRING_USE_PYTHON && !SYS_STRING_USE_GENERIC

        namespace sysstr
        {
            using sys_string = sys_string_android;
            using sys_string_builder = sys_string_android_builder;
        }

        #define SYS_STRING_STATIC SYS_STRING_STATIC_ANDROID

    #endif

#elif defined(_WIN32)
    #include <sys_string/impl/platforms/windows_bstr.h>
    #include <sys_string/impl/platforms/windows_hstring.h>
    #include <sys_string/impl/platforms/windows_generic.h>

    #if !SYS_STRING_USE_PYTHON && !SYS_STRING_USE_GENERIC

        #if SYS_STRING_WIN_BSTR

            namespace sysstr 
            { 
                using sys_string = sys_string_bstr; 
                using sys_string_builder = sys_string_bstr_builder; 
            }

            #define SYS_STRING_STATIC SYS_STRING_STATIC_BSTR
            
        #elif SYS_STRING_WIN_HSTRING

            namespace sysstr 
            { 
                using sys_string = sys_string_hstring; 
                using sys_string_builder = sys_string_hstring_builder; 
            }

            #define SYS_STRING_STATIC SYS_STRING_STATIC_HSTRING

        #else

            namespace sysstr
            {
                using sys_string = sys_string_win_generic;
                using sys_string_builder = sys_string_win_generic_builder;
            }

            #define SYS_STRING_STATIC SYS_STRING_STATIC_WIN_GENERIC

        #endif

    #endif


#elif defined(__EMSCRIPTEN__)

    #include <sys_string/impl/platforms/emscripten_javascript.h>

    #if !SYS_STRING_USE_PYTHON && !SYS_STRING_USE_GENERIC

        namespace sysstr
        {
            using sys_string = sys_string_emscripten;
            using sys_string_builder = sys_string_emscripten_builder;
        }

        #define SYS_STRING_STATIC SYS_STRING_STATIC_EMSCRIPTEN

    #endif
    
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__unix__) || defined(__HAIKU__)
    
    
    #if !SYS_STRING_USE_PYTHON && !SYS_STRING_USE_GENERIC

        namespace sysstr
        {
            using sys_string = sys_string_generic;
            using sys_string_builder = sys_string_generic_builder;
        }

        #define SYS_STRING_STATIC SYS_STRING_STATIC_GENERIC

    #endif

#else
    #error Unsupported platform
#endif
