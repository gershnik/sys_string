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

extern JavaVM * g_vm;
extern JNIEnv * g_env;

#if !SYS_STRING_USE_GENERIC

    TEST_CASE( "Android Conversions", "[android]") {

        CHECK(sys_string().make_jstring(g_env) == nullptr);
        CHECK(sys_string(g_env, nullptr) == S(""));

        jstring jstr = S("a水𐀀𝄞bcå🤢").make_jstring(g_env);
        REQUIRE(jstr);
        CHECK(g_env->GetStringLength(jstr) == S("a水𐀀𝄞bcå🤢").storage_size());

        jstr = g_env->NewString((const jchar *)u"a水𐀀𝄞bcå🤢", std::size(u"a水𐀀𝄞bcå🤢") - 1);
        sys_string str(g_env, jstr);
        CHECK(str == S("a水𐀀𝄞bcå🤢"));
    }

    TEST_CASE( "Android error handling", "[android]") {
        jclass integer_cls = g_env->FindClass("java/lang/Integer");
        REQUIRE(integer_cls);
        jmethodID integer_ctor = g_env->GetMethodID(integer_cls, "<init>", "()V");
        REQUIRE(integer_ctor);
        
        jobject an_integer = g_env->NewObject(integer_cls, integer_ctor);
        REQUIRE(an_integer);

        sys_string str(g_env, (jstring)an_integer);
        CHECK(str == sys_string());

        sys_string str1(g_env, (jstring)integer_cls);
        CHECK(str1 == sys_string());

        jclass exception_cls = g_env->FindClass("java/lang/Exception");
        REQUIRE(exception_cls);
        
        jstring jstr = g_env->NewString((const jchar *)u"a水𐀀𝄞bcå🤢", std::size(u"a水𐀀𝄞bcå🤢") - 1);
        REQUIRE(jstr);
        REQUIRE(g_env->ThrowNew(exception_cls, "haha") == 0);

        CHECK(sys_string(g_env, jstr) == S("a水𐀀𝄞bcå🤢"));
        CHECK(S("haha").make_jstring(g_env));

        g_env->ExceptionClear();
    }

#else

    TEST_CASE( "Android Conversions", "[android]") {

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
