//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
// _🪦🐮🐌_

#include <sys_string/sys_string.h>


#include <doctest/doctest.h>

using namespace sysstr;

#if !SYS_STRING_USE_GENERIC && !SYS_STRING_USE_PYTHON

TEST_SUITE("apple") {

TEST_CASE( "Apple Conversions" ) {

    CHECK(sys_string((NSString *)nullptr) == sys_string());
    CHECK(sys_string().cf_str() == nullptr);
    CHECK(sys_string().ns_str() == nullptr);
    
    auto str = CFSTR("abc");
    auto nsstr = @"xyz";
    
    CHECK(sys_string(str) == S("abc"));
    CHECK(sys_string(str).cf_str() == str);
    
    CHECK(sys_string(nsstr) == S("xyz"));
    CHECK(sys_string(nsstr).ns_str() == nsstr);

}

}

#endif
