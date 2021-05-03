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

TEST_CASE( "Windows Conversions", "[windows]") {

    CHECK(sys_string().h_str() == nullptr);
    CHECK(wcscmp(sys_string().w_str(), L"") == 0);
    CHECK(sys_string(HSTRING(nullptr)) == sys_string());
    CHECK(sys_string(HSTRING(nullptr)) == S(""));

    HSTRING hstr; 
    WindowsCreateString(L"a水𐀀𝄞bcå🤢", UINT32(std::size(L"a水𐀀𝄞bcå🤢") - 1), &hstr);

    CHECK(sys_string(hstr) == S("a水𐀀𝄞bcå🤢"));
    CHECK(sys_string(hstr).h_str() == hstr);
    CHECK(wcscmp(sys_string(hstr).w_str(), L"a水𐀀𝄞bcå🤢") == 0);

    HSTRING hstr1 = S("a水𐀀𝄞bcå🤢").h_str();
    CHECK(hstr1);
    int res;
    WindowsCompareStringOrdinal(hstr, hstr1, &res);
    CHECK(res == 0);

    WindowsDeleteString(hstr);
}