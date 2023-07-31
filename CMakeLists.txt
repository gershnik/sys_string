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


add_subdirectory(lib)

if (PROJECT_IS_TOP_LEVEL)
    
    include(lib/cmake/install.cmake)
    add_subdirectory(test)

endif()
