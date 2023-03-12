//
// Copyright 2023 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#include <sys_string/sys_string.h>


#include "catch.hpp"

using namespace sysstr;

#if SYS_STRING_USE_PYTHON

TEST_CASE( "Python Conversions", "[python]") {

    auto str = sys_string();
    REQUIRE(str.py_str());
    CHECK(strcmp(PyUnicode_AsUTF8(str.py_str()), "") == 0);
    
    str = S("");
    REQUIRE(str.py_str());
    CHECK(strcmp(PyUnicode_AsUTF8(str.py_str()), "") == 0);
    
    str = sys_string((PyObject*)nullptr);
    REQUIRE(str.py_str());
    CHECK(strcmp(PyUnicode_AsUTF8(str.py_str()), "") == 0);
    
    CHECK(strcmp(PyUnicode_AsUTF8(sys_string("a水𐀀𝄞bcå🤢").py_str()), "a水𐀀𝄞bcå🤢") == 0);

    auto raw = PyUnicode_FromString("\xEF\xBF\xBD");
    REQUIRE(raw);
    CHECK(sys_string(raw) == sys_string(u"�"));
}

#endif
