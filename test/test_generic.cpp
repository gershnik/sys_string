//
// Copyright 2023 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
// _🪦🐮🐌_

#include <sys_string/sys_string.h>


#include <doctest/doctest.h>

using namespace sysstr;

#if SYS_STRING_USE_GENERIC

TEST_SUITE("generic") {

TEST_CASE( "Generic Conversions") {

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

}

#endif