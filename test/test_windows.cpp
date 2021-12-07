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

#if SYS_STRING_WIN_BSTR
    TEST_CASE("Windows Empty String", "[windows]")
    {
        CHECK(sys_string().b_str() == nullptr);
        CHECK(wcscmp(sys_string().w_str(), L"") == 0);
        CHECK(sys_string(BSTR(nullptr), copy_content) == sys_string());
        CHECK(sys_string(BSTR(nullptr), copy_content) == S(""));
        CHECK(sys_string(BSTR(nullptr), attach_pointer) == sys_string());
        CHECK(sys_string(BSTR(nullptr), attach_pointer) == S(""));
    }

    TEST_CASE("Windows Attach", "[windows]")
    {
        BSTR bstr = SysAllocString(L"a水𐀀𝄞bcå🤢");
        sys_string attached(bstr, attach_pointer);

        CHECK(attached == S("a水𐀀𝄞bcå🤢"));
        CHECK(attached.b_str() == bstr);
        CHECK(wcscmp(attached.w_str(), L"a水𐀀𝄞bcå🤢") == 0);

        CHECK(sys_string(attached) == S("a水𐀀𝄞bcå🤢"));
        CHECK(sys_string(attached).b_str() != bstr);
        CHECK(wcscmp(sys_string(attached).w_str(), L"a水𐀀𝄞bcå🤢") == 0);

        BSTR bstr1 = SysAllocString(L"a水𐀀𝄞bcå🤢");
        attached = sys_string(bstr1, attach_pointer);
        BSTR released = attached.release();
        CHECK(released == bstr1);
        CHECK(attached == S(""));
        SysFreeString(released);
    }

    TEST_CASE("Windows Copy", "[windows]")
    {
        BSTR bstr = S("a水𐀀𝄞bcå🤢").b_str();
        CHECK(bstr);
        CHECK(CompareStringOrdinal(bstr, -1, L"a水𐀀𝄞bcå🤢", -1, FALSE) == CSTR_EQUAL);

        BSTR bstr1 = SysAllocString(L"a水𐀀𝄞bcå🤢");

        sys_string copied(bstr1, copy_content);

        CHECK(copied == S("a水𐀀𝄞bcå🤢"));
        CHECK(copied.b_str() != bstr1);
        CHECK(wcscmp(copied.w_str(), L"a水𐀀𝄞bcå🤢") == 0);

        SysFreeString(bstr1);
    }

    TEST_CASE("Windows Static and embedded", "[windows]")
    {

        CHECK(SysStringLen(S("").b_str()) == 0);
        CHECK(SysStringLen(S("a").b_str()) == 1);
        CHECK(SysStringLen(S("abcdefghijklmnopqrstuvwxyz").b_str()) == 26);

        sys_string s;
        BSTR bstr4 = s.release();
        CHECK(bstr4 == nullptr);

        s = S("");
        bstr4 = s.release();
        CHECK(bstr4 != nullptr);
        CHECK(SysStringLen(bstr4) == 0);
        CHECK(s == S(""));
        SysFreeString(bstr4);

        s = S("a");
        bstr4 = s.release();
        CHECK(bstr4 != nullptr);
        CHECK(SysStringLen(bstr4) == 1);
        CHECK(s == S(""));
        SysFreeString(bstr4); 
    }
#elif SYS_STRING_WIN_HSTRING

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

#else

    TEST_CASE( "Windows Conversions", "[windows]") {

        REQUIRE(sys_string().w_str());
        CHECK(wcscmp(sys_string().w_str(), L"") == 0);

        REQUIRE(S("").w_str());
        CHECK(wcscmp(S("").w_str(), L"") == 0);

        REQUIRE(sys_string("").w_str());
        CHECK(wcscmp(sys_string("").w_str(), L"") == 0);

        REQUIRE(sys_string((const wchar_t *)nullptr).w_str());
        CHECK(wcscmp(sys_string((const wchar_t *)nullptr).w_str(), L"") == 0);

        CHECK(wcscmp(sys_string(L"a水𐀀𝄞bcå🤢").w_str(), L"a水𐀀𝄞bcå🤢") == 0);
    }

#endif