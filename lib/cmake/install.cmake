#
# Copyright 2010 Eugene Gershnik
#
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE.txt
#

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS sys_string EXPORT sys_string FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT sys_string NAMESPACE sys_string:: FILE sys_string-exports.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/sys_string)

configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/sys_string-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/sys_string-config.cmake
    INSTALL_DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/sys_string
)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/sys_string-config-version.cmake
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/sys_string-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/sys_string-config-version.cmake
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/sys_string
)

file(RELATIVE_PATH FROM_PCFILEDIR_TO_PREFIX ${CMAKE_INSTALL_FULL_DATAROOTDIR}/sys_string ${CMAKE_INSTALL_PREFIX})
string(REGEX REPLACE "/+$" "" FROM_PCFILEDIR_TO_PREFIX "${FROM_PCFILEDIR_TO_PREFIX}") 

configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/sys_string.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/sys_string.pc
    @ONLY
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/sys_string.pc
    DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)