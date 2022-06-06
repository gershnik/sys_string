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

#if (defined(__APPLE__) && defined(__MACH__))
    #include <sys_string/impl/platforms/apple_cfstr.h>
    #include <sys_string/impl/platforms/unix_generic.h>

    #if defined(SYS_STRING_USE_GENERIC)

        namespace sysstr
        {
            using sys_string = sys_string_generic;
            using sys_string_builder = sys_string_generic_builder;
        }

        #define SYS_STRING_STATIC SYS_STRING_STATIC_GENERIC

    #else

        namespace sysstr
        {
            using sys_string = sys_string_cfstr;
            using sys_string_builder = sys_string_cfstr_builder;
        }

        #define SYS_STRING_STATIC SYS_STRING_STATIC_CFSTR
    #endif

#elif defined(__ANDROID__)

    #include <sys_string/impl/platforms/android_java.h>
    #include <sys_string/impl/platforms/unix_generic.h>

    #if defined(SYS_STRING_USE_GENERIC)

        namespace sysstr
        {
            using sys_string = sys_string_generic;
            using sys_string_builder = sys_string_generic_builder;
        }

        #define SYS_STRING_STATIC SYS_STRING_STATIC_GENERIC

    #else
        

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
    #include <sys_string/impl/platforms/unix_generic.h>

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

    #elif SYS_STRING_USE_GENERIC

        namespace sysstr
        {
            using sys_string = sys_string_generic;
            using sys_string_builder = sys_string_generic_builder;
        }

        #define SYS_STRING_STATIC SYS_STRING_STATIC_GENERIC

    #else

        namespace sysstr
        {
            using sys_string = sys_string_win_generic;
            using sys_string_builder = sys_string_win_generic_builder;
        }

        #define SYS_STRING_STATIC SYS_STRING_STATIC_WIN_GENERIC

    #endif
    
#elif defined(__linux__) || defined(__FreeBSD__)
    #include <sys_string/impl/platforms/unix_generic.h>


    namespace sysstr
    {
        using sys_string = sys_string_generic;
        using sys_string_builder = sys_string_generic_builder;
    }

    #define SYS_STRING_STATIC SYS_STRING_STATIC_GENERIC

#else
    #error Unsupported platform
#endif