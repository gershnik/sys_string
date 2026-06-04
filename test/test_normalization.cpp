//
// Copyright 2025 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
// _🪦🐮🐌_

#include <sys_string/config.h>
#include <sys_string/impl/unicode/algorithms.h>

#include <doctest/doctest.h>

#include <string_view>
#include <vector>
#include <string>

using namespace sysstr;
using namespace std::literals;

#define FILINE __FILE__, __LINE__

#if !SYS_STRING_USE_ICU

namespace {

#if defined(__GNUC__)
    [[gnu::noinline]]
#elif defined(__MSVC__)
    [[msvc::noinline]]
#endif
    void _check_nfd(std::u32string_view src, const std::u32string_view & expected, const char * file, int line)
    {
        std::vector<char32_t> result;
        normalize::nfd<utf32> normalizer;
        normalizer(src, std::back_inserter(result));
        INFO("source: ", std::string(file), std::string(":"), line);
        bool res = std::ranges::equal(result, expected);
        CHECK(res);
    }

    #define check_nfd(x, y) _check_nfd(x, y, FILINE)

#if defined(__GNUC__)
    [[gnu::noinline]]
#elif defined(__MSVC__)
    [[msvc::noinline]]
#endif
    void _check_nfc(std::u32string_view src, const std::u32string_view & expected, const char * file, int line)
    {
        std::vector<char32_t> result;
        normalize::nfc<utf32> normalizer;
        normalizer(src, std::back_inserter(result));
        INFO("source: ", std::string(file), std::string(":"), line);
        bool res = std::ranges::equal(result, expected);
        REQUIRE(res);
    }

    #define check_nfc(x, y) _check_nfc(x, y, FILINE)

#if defined(__GNUC__)
    [[gnu::noinline]]
#elif defined(__MSVC__)
    [[msvc::noinline]]
#endif
    void check_generated(size_t idx,
                         std::u32string_view src, 
                         std::u32string_view nfc, 
                         std::u32string_view nfd, 
                         std::u32string_view nfkc, 
                         std::u32string_view nfkd)
    {
        INFO("item " << idx);
        check_nfd(src, nfd);
        check_nfd(nfc, nfd);
        check_nfd(nfd, nfd);
        check_nfd(nfkc, nfkd);
        check_nfd(nfkd, nfkd);
        
        check_nfc(src, nfc);
        check_nfc(nfc, nfc);
        check_nfc(nfd, nfc);
        check_nfc(nfkc, nfkc);
        check_nfc(nfkd, nfkc);
    }

}

TEST_SUITE("normalization") {

TEST_CASE("boundary") {
    check_nfd(U"", {});
    check_nfd(std::u32string_view(U"\0\0", 2), std::u32string_view(U"\0\0", 2));
    check_nfd(U"q\u0307\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323"
               U"\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323b", 
              U"q\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323"
               U"\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0323\u0307b");

    check_nfc(U"", {});
    check_nfc(std::u32string_view(U"\0\0", 2), std::u32string_view(U"\0\0", 2));
}

TEST_CASE("basics") {
    check_nfd(U"Ç", U"C"s + U"̧"s);
    check_nfd(U"q\u0307\u0323", U"q\u0323\u0307");
    check_nfd(U"가", U"ᄀ"s + U"ᅡ"s);
    check_nfd(U"Ω", U"Ω");

    check_nfc(U"C"s + U"̧"s, U"Ç");
    check_nfc(U"s\u0323\u0307", U"\u1e69");
    check_nfc(U"ᄀ"s + U"ᅡ"s, U"가");
    check_nfc(U"Ω", U"Ω");
}

TEST_CASE("generated") {

    #include "test_normalization_data.h"
}

}

#endif

