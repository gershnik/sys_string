#
# Copyright 2010 Eugene Gershnik
#
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE.txt
#
cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0092 NEW)
cmake_policy(SET CMP0079 NEW)

if (WIN32)
set(CMAKE_SYSTEM_VERSION "10.0.19041.0" CACHE STRING "Windows SDK Version" FORCE)
endif()


project(test)

find_package (Python3 COMPONENTS Interpreter Development)

if(${Python3_Development_FOUND})
    include_directories(
    SYSTEM
        ${Python3_INCLUDE_DIRS}
    )

    link_libraries(
        ${Python3_LIBRARIES}
    )
endif()

set (CXX_STANDARDS
    17
    20
)

set(CXX_STANDARD_17 17)
set(CXX_STANDARD_20 20)

if (WIN32)

    set (STORAGE_TYPES
        bstr
        hstring
        win_gen
        unix_gen
    )

    if (Python3_Development_FOUND)

        list(APPEND STORAGE_TYPES python)

    endif()

elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")

    set (STORAGE_TYPES
        cfstr
        unix_gen
    )

    if (Python3_Development_FOUND)

        list(APPEND STORAGE_TYPES python)

    endif()

elseif (${CMAKE_SYSTEM_NAME} STREQUAL Android)

    set (STORAGE_TYPES
        andr
        unix_gen
    )

elseif (DEFINED EMSCRIPTEN)

    set (STORAGE_TYPES
        emscr
        unix_gen
    )

else()

    set (STORAGE_TYPES
        def
    )

    if (Python3_FOUND)

        list(APPEND STORAGE_TYPES python)

    endif()

endif()

set(STORAGE_FLAG_def "")
set(STORAGE_FLAG_bstr SYS_STRING_WIN_BSTR=1)
set(STORAGE_FLAG_hstring SYS_STRING_WIN_HSTRING=1)
set(STORAGE_FLAG_win_gen "")
set(STORAGE_FLAG_cfstr "")
set(STORAGE_FLAG_andr "")
set(STORAGE_FLAG_unix_gen SYS_STRING_USE_GENERIC=1)
set(STORAGE_FLAG_python SYS_STRING_USE_PYTHON=1)

set(TEST_COMMAND "")
set(TEST_DEPS "")

if (${CMAKE_SYSTEM_NAME} STREQUAL Android)
    set(ANDROID_TEST_DIR /data/local/tmp/sys_string_test)
    set(ANDROID_SDK_DIR ${CMAKE_ANDROID_NDK}/../..)
    set(ADB ${ANDROID_SDK_DIR}/platform-tools/adb)

    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        set(ANDROID_LD_LIBRARY_PATH /apex/com.android.art/lib:/apex/com.android.runtime/lib)
    else()
        set(ANDROID_LD_LIBRARY_PATH /apex/com.android.art/lib64:/apex/com.android.runtime/lib64)
    endif()

    list(APPEND TEST_COMMAND COMMAND ${ADB} shell mkdir -p ${ANDROID_TEST_DIR})
endif()
    
foreach(STANDARD_SUFFIX ${CXX_STANDARDS})

    set(CMAKE_CXX_STANDARD ${CXX_STANDARD_${STANDARD_SUFFIX}})
    set(SYS_STRING_LIBRARY_SUFFIX ${STANDARD_SUFFIX})
    add_subdirectory(../lib  lib${STANDARD_SUFFIX})
    unset(CMAKE_CXX_STANDARD)
endforeach()

foreach(STANDARD_SUFFIX ${CXX_STANDARDS})
foreach(STORAGE_SUFFIX ${STORAGE_TYPES})

    set(TEST_SUFFIX ${STANDARD_SUFFIX}${STORAGE_SUFFIX})

    if (SYS_STRING_TEST_SHARED)
        add_library(test-${TEST_SUFFIX} SHARED)
    else()
        add_executable(test-${TEST_SUFFIX})
    endif()

    set_target_properties(test-${TEST_SUFFIX} PROPERTIES
        CXX_STANDARD ${CXX_STANDARD_${STANDARD_SUFFIX}}
        CXX_STANDARD_REQUIRED OFF
        CXX_EXTENSIONS OFF
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
    )

    target_compile_options(test-${TEST_SUFFIX} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/utf-8;/W4;/WX>
        $<$<CXX_COMPILER_ID:Clang>:-Wall;-Wextra;-pedantic;-Wno-self-assign-overloaded;-Wno-self-move>
        $<$<CXX_COMPILER_ID:AppleClang>:-Wall;-Wextra;-pedantic;-Wno-self-assign-overloaded;-Wno-self-move>
        $<$<CXX_COMPILER_ID:GNU>:-Wall;-Wextra;-pedantic>
    )

    target_compile_definitions(test-${TEST_SUFFIX} PRIVATE
        ${STORAGE_FLAG_${STORAGE_SUFFIX}}
    )

    target_link_options(test-${TEST_SUFFIX} PRIVATE
    
        "$<$<PLATFORM_ID:Android>:-Wl,--export-dynamic>"
    )

    target_link_libraries(test-${TEST_SUFFIX} PRIVATE
        
        sys_string${STANDARD_SUFFIX}
                
        "$<$<PLATFORM_ID:Android>:log>"
        "$<$<PLATFORM_ID:Emscripten>:embind>" 
    )

    if (SYS_STRING_TEST_SHARED)

        target_compile_definitions(test-${TEST_SUFFIX} PUBLIC

            SYS_STRING_TEST_SHARED=1
        )
    
    endif()

    target_sources(test-${TEST_SUFFIX} PRIVATE

        test_main.cpp
        test_general.cpp
        test_builder.cpp
        test_utf_iteration.cpp
        test_utf_util.cpp
        "$<$<PLATFORM_ID:Darwin>:test_apple.mm>"
        "$<$<PLATFORM_ID:Android>:test_android.cpp>" 
        "$<$<PLATFORM_ID:Windows>:test_windows.cpp>" 
        "$<$<PLATFORM_ID:Linux>:test_linux.cpp>" 
        "$<$<PLATFORM_ID:Emscripten>:test_javascript.cpp>" 

        catch.hpp
    )

    if (${CMAKE_SYSTEM_NAME} STREQUAL Android)
        list(APPEND TEST_COMMAND COMMAND ${ADB} push test-${TEST_SUFFIX} ${ANDROID_TEST_DIR})
        list(APPEND TEST_COMMAND COMMAND ${ADB} shell LD_LIBRARY_PATH=${ANDROID_LD_LIBRARY_PATH} ${ANDROID_TEST_DIR}/test-${TEST_SUFFIX})
    elseif (DEFINED EMSCRIPTEN)
        list(APPEND TEST_COMMAND COMMAND node test-${TEST_SUFFIX})
    else()
        list(APPEND TEST_COMMAND COMMAND test-${TEST_SUFFIX})
    endif()

    list(APPEND TEST_DEPS test-${TEST_SUFFIX})

endforeach()
endforeach()

add_custom_target(run-test 
    DEPENDS ${TEST_DEPS}
    ${TEST_COMMAND}
)
