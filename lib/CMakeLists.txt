#
# Copyright 2010 Eugene Gershnik
#
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE.txt
#

cmake_minimum_required(VERSION 3.25)

if (NOT SYSSTR_VERSION)
    file(READ VERSION SYSSTR_VERSION)
    if (NOT SYSSTR_VERSION)
        message(FATAL_ERROR "Cannot determine library version (VERSION file not found)")
    endif()
    string(STRIP ${SYSSTR_VERSION} SYSSTR_VERSION)
endif()
project(sys_string VERSION ${SYSSTR_VERSION})

set(SRCDIR ${CMAKE_CURRENT_LIST_DIR})
set(LIBNAME sys_string)

set(MAIN_CODE 
    ${SRCDIR}/inc/sys_string/config.h
    ${SRCDIR}/inc/sys_string/utf_view.h
    ${SRCDIR}/inc/sys_string/grapheme_view.h
    ${SRCDIR}/inc/sys_string/sys_string.h
    ${SRCDIR}/inc/sys_string/impl/compare.h
    ${SRCDIR}/inc/sys_string/impl/hash.h
    ${SRCDIR}/inc/sys_string/impl/addition.h
    ${SRCDIR}/inc/sys_string/impl/builder.h
    ${SRCDIR}/inc/sys_string/impl/misc.h
    ${SRCDIR}/inc/sys_string/impl/format.h
    ${SRCDIR}/inc/sys_string/impl/print.h
    ${SRCDIR}/inc/sys_string/impl/platform.h
)
source_group("Main Code" FILES ${MAIN_CODE})

set(PLATFORM_FILES
    ${SRCDIR}/inc/sys_string/impl/platforms/apple_cfstr.h
    ${SRCDIR}/inc/sys_string/impl/platforms/android_java.h
    ${SRCDIR}/inc/sys_string/impl/platforms/windows_hstring.h
    ${SRCDIR}/inc/sys_string/impl/platforms/windows_bstr.h
    ${SRCDIR}/inc/sys_string/impl/platforms/windows_generic.h
    ${SRCDIR}/inc/sys_string/impl/platforms/emscripten_javascript.h
    ${SRCDIR}/inc/sys_string/impl/platforms/python_any.h
    ${SRCDIR}/inc/sys_string/impl/platforms/unix_generic.h
)
source_group("Platforms" FILES ${PLATFORM_FILES})


set(UNICODE_FILES

    ${SRCDIR}/inc/sys_string/impl/unicode/utf_encoding.h
    ${SRCDIR}/inc/sys_string/impl/unicode/utf_util.h
    ${SRCDIR}/inc/sys_string/impl/unicode/algorithms.h
    ${SRCDIR}/inc/sys_string/impl/unicode/mappings_common.h
    ${SRCDIR}/inc/sys_string/impl/unicode/mappings.h
    ${SRCDIR}/inc/sys_string/impl/unicode/mappings_icu.h
)
source_group("Unicode" FILES ${UNICODE_FILES})

set(UTIL_FILES

    ${SRCDIR}/inc/sys_string/impl/util/util.h
    ${SRCDIR}/inc/sys_string/impl/util/char_vector.h
    ${SRCDIR}/inc/sys_string/impl/util/generic_impl.h
    ${SRCDIR}/inc/sys_string/impl/util/iter_util.h
    
)
source_group("Utils" FILES ${UTIL_FILES})

set(HEADERS 
    ${MAIN_CODE}
    ${PLATFORM_FILES}
    ${UNICODE_FILES}
    ${UTIL_FILES}
)

add_library(${LIBNAME} INTERFACE)

target_compile_features(${LIBNAME}
INTERFACE 
    cxx_std_20
)

target_include_directories(${LIBNAME}
INTERFACE
    $<BUILD_INTERFACE:${SRCDIR}/inc>
    $<INSTALL_INTERFACE:include>  # <prefix>/include
)

target_sources(${LIBNAME}
INTERFACE 
    FILE_SET HEADERS BASE_DIRS ${SRCDIR}/inc FILES
        ${HEADERS}
PRIVATE 
    ${HEADERS}
)

add_library(${LIBNAME}::${LIBNAME} ALIAS ${LIBNAME})

if (PROJECT_IS_TOP_LEVEL OR SYS_STRING_CONFIGURE_INSTALL)
    
    include(cmake/install.cmake)

endif()


