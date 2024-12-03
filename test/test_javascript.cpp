//
// Copyright 2022 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
// _🪦🐮🐌_

#include <sys_string/sys_string.h>


#include <doctest/doctest.h>

using namespace sysstr;
using namespace emscripten;

#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"


#if !SYS_STRING_USE_GENERIC && !SYS_STRING_USE_PYTHON

    TEST_SUITE("javascript") {

    TEST_CASE( "Javascript Conversions" ) {

        EM_VAL handle = (EM_VAL)EM_ASM_PTR({
            return Emval.toHandle("");
        }, 0);
        CHECK(sys_string(handle) == S(""));
        emscripten::internal::_emval_decref(handle);

        handle = (EM_VAL)EM_ASM_PTR({
            return Emval.toHandle("a水𐀀𝄞bcå🤢");
        }, 0);
        CHECK(sys_string(handle) == S("a水𐀀𝄞bcå🤢"));
        emscripten::internal::_emval_decref(handle);

        handle = sys_string().make_js_string();
        auto result = EM_ASM_INT({
            let ret = (Emval.toValue($0) === "");
            __emval_decref($0);
            return ret;
        }, handle);
        CHECK(result);

        handle = S("a水𐀀𝄞bcå🤢").make_js_string();
        result = EM_ASM_INT({
            let ret = (Emval.toValue($0) === "a水𐀀𝄞bcå🤢");
            __emval_decref($0);
            return ret;
        }, handle);
        CHECK(result);
    }

    }

#endif