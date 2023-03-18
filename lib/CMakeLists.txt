#
# Copyright 2010 Eugene Gershnik
#
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE.txt
#

cmake_minimum_required(VERSION 3.16)

project(sys_string)

find_package (Python3 COMPONENTS Interpreter)

set(SRCDIR ${CMAKE_CURRENT_LIST_DIR})
set(LIBNAME sys_string${SYS_STRING_LIBRARY_SUFFIX})

add_library(${LIBNAME} STATIC)

set_target_properties(${LIBNAME} PROPERTIES
    CXX_EXTENSIONS OFF
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

target_compile_features(${LIBNAME} PUBLIC 
    cxx_std_17
)

target_compile_options(${LIBNAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4;/WX>
    $<$<CXX_COMPILER_ID:Clang>:-Wall;-Wextra;-pedantic>
    $<$<CXX_COMPILER_ID:AppleClang>:-Wall;-Wextra;-pedantic>
    $<$<CXX_COMPILER_ID:GNU>:-Wall;-Wextra;-pedantic>
)

target_link_libraries(${LIBNAME} PUBLIC
    "$<$<PLATFORM_ID:Darwin>:-framework CoreFoundation>"
    "$<$<PLATFORM_ID:Windows>:runtimeobject.lib>"
    "$<$<PLATFORM_ID:Emscripten>:embind>"
)

target_compile_definitions(${LIBNAME} PUBLIC
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)

target_include_directories(${LIBNAME}
PUBLIC
    ${SRCDIR}/inc
)

set(MAIN_CODE 
    ${SRCDIR}/inc/sys_string/config.h
    ${SRCDIR}/inc/sys_string/utf_view.h
    ${SRCDIR}/inc/sys_string/sys_string.h
    ${SRCDIR}/inc/sys_string/impl/compare.h
    ${SRCDIR}/inc/sys_string/impl/hash.h
    ${SRCDIR}/inc/sys_string/impl/addition.h
    ${SRCDIR}/inc/sys_string/impl/builder.h
    ${SRCDIR}/inc/sys_string/impl/misc.h
    ${SRCDIR}/inc/sys_string/impl/format.h
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
    ${SRCDIR}/inc/sys_string/impl/unicode/mappings.h
    ${SRCDIR}/src/unicode_mappings.cpp
)
source_group("Unicode" FILES ${UNICODE_FILES})

set(UTIL_FILES

    ${SRCDIR}/inc/sys_string/impl/util/util.h
    ${SRCDIR}/inc/sys_string/impl/util/char_buffer.h
    ${SRCDIR}/inc/sys_string/impl/util/generic_buffer.h
    ${SRCDIR}/inc/sys_string/impl/util/cursor.h
    
)
source_group("Utils" FILES ${UTIL_FILES})

target_sources(${LIBNAME} PRIVATE 
    ${MAIN_CODE}

    ${PLATFORM_FILES}

    ${UNICODE_FILES}

    ${UTIL_FILES}
)

file(GLOB UNICODE_DATA ${SRCDIR}/res/*.txt)
file(GLOB UNICODE_SCRIPTS ${SRCDIR}/scripts/*.py)

if(${Python3_Interpreter_FOUND})

    if (NOT SYS_STRING_MAPPINGS_COMMAND_ADDED)
        add_custom_command(
            COMMENT "Generating Unicoode mappings"
            OUTPUT ${SRCDIR}/src/unicode_mappings.cpp
            COMMAND ${Python3_EXECUTABLE} ${SRCDIR}/scripts/genmappings.py ${SRCDIR}/res ${SRCDIR}/src/unicode_mappings.cpp 
            DEPENDS 
                ${UNICODE_DATA} 
                ${UNICODE_SCRIPTS}
        )
        set(SYS_STRING_MAPPINGS_COMMAND_ADDED TRUE PARENT_SCOPE)
    endif()

endif()





