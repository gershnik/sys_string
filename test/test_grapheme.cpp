//
// Copyright 2024 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
// _🪦🐮🐌_

#include <sys_string/impl/unicode/algorithms.h>

#include <doctest/doctest.h>

#include <ranges>
#include <vector>
#include <string>

using namespace sysstr;

namespace {

    void check_graphemes(std::u32string_view src, const std::vector<std::u32string> & expected, const char * info)
    {
        auto first = std::ranges::begin(src);
        auto last = std::ranges::end(src);

        std::vector<std::u32string> result;
        do
        {
            auto next = first;
            sysstr::advance_to_grapheme_cluster_break(next, last);
            result.push_back(std::u32string(first, next));
            first = next;
        } 
        while (first != last);

        INFO("source: ", std::string_view(info));
        bool res = std::ranges::equal(result, expected);
        CHECK(res);
    }
}

TEST_SUITE("grapheme") {

TEST_CASE("boundary") {
    check_graphemes(U"", {U""}, "empty");
}

TEST_CASE("generated") {

#include "test_grapheme_data.h"
}

}
