//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#include <sys_string/sys_string.h>


#include "catch.hpp"

using namespace sysstr;

#if !SYS_STRING_USE_GENERIC

TEST_CASE( "Apple Conversions", "[apple]") {

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

#else

TEST_CASE( "Apple Conversions", "[apple]") {

    REQUIRE(sys_string().c_str());
    CHECK(strcmp(sys_string().c_str(), "") == 0);

    REQUIRE(S("").c_str());
    CHECK(strcmp(S("").c_str(), "") == 0);

    REQUIRE(sys_string("").c_str());
    CHECK(strcmp(sys_string("").c_str(), "") == 0);

    REQUIRE(sys_string((const char*)nullptr).c_str());
    CHECK(strcmp(sys_string((const char*)nullptr).c_str(), "") == 0);

    CHECK(strcmp(sys_string("a水𐀀𝄞bcå🤢").c_str(), "a水𐀀𝄞bcå🤢") == 0);
}

#endif
