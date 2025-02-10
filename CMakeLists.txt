#
# Copyright 2010 Eugene Gershnik
#
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE.txt
#

cmake_minimum_required(VERSION 3.25)

file(READ lib/VERSION SYSSTR_VERSION)
if (NOT SYSSTR_VERSION)
    message(FATAL_ERROR "Cannot determine library version (lib/VERSION file not found)")
endif()
string(STRIP ${SYSSTR_VERSION} SYSSTR_VERSION)

project(sys_string VERSION ${SYSSTR_VERSION} LANGUAGES CXX)

find_package (Python3 COMPONENTS Interpreter Development)


add_subdirectory(lib)

if(${Python3_Interpreter_FOUND})

    file(GLOB UNICODE_DATA ${CMAKE_CURRENT_LIST_DIR}/unicode/data/*.txt)
    file(GLOB UNICODE_SCRIPTS ${CMAKE_CURRENT_LIST_DIR}/unicode/scripts/*.py)

    set(UNICODE_GENERATED_FILES
        ${CMAKE_CURRENT_LIST_DIR}/lib/src/unicode_mappings.cpp 
        ${CMAKE_CURRENT_LIST_DIR}/lib/inc/sys_string/impl/unicode/mappings.h
        ${CMAKE_CURRENT_LIST_DIR}/test/test_normalization_data.h
    )

    add_custom_command(
        COMMENT "Generating Unicoode mappings"
        OUTPUT ${UNICODE_GENERATED_FILES}
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/unicode/scripts/genmappings.py 
                        ${CMAKE_CURRENT_LIST_DIR}/unicode/data 
                        ${UNICODE_GENERATED_FILES}
        DEPENDS 
            ${UNICODE_DATA} 
            ${UNICODE_SCRIPTS}
    )

    add_custom_target(generate_unicode_mappings
        DEPENDS 
            ${UNICODE_GENERATED_FILES}
    )

    add_dependencies(sys_string generate_unicode_mappings)

endif()


if (PROJECT_IS_TOP_LEVEL)
    
    include(lib/cmake/install.cmake)
    add_subdirectory(test)

endif()
