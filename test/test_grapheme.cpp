//
// Copyright 2024 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
// _🪦🐮🐌_

#include <sys_string/grapheme_view.h>

#include <doctest/doctest.h>

#include <ranges>
#include <vector>
#include <string>
#include <string_view>
#include <ostream>
#include <source_location>

using namespace sysstr;
using namespace std::literals;

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

    template<std::forward_iterator It, std::sentinel_for<It> EndIt>
    void check_graphemes(util::grapheme_iterator<It, EndIt> it, const std::vector<std::u32string> & expected, 
                         std::source_location loc = std::source_location::current())
    {
        std::vector<std::u32string> result;
        std::ranges::transform(it, std::default_sentinel, std::back_inserter(result), [](auto range) {
            return std::u32string(range.begin(), range.end());
        });
        INFO("source: ", std::string(loc.file_name()), std::string(":"), loc.line());
        bool res = std::ranges::equal(result, expected);
        CHECK(res);
    }
}

TEST_SUITE("grapheme") {

TEST_CASE("boundary") {
    check_graphemes(U"", {U""}, "empty");
    check_graphemes(std::u32string_view(U"\0\0", 2), {std::u32string(U"\0", 1), std::u32string(U"\0", 1)}, "null");
}

TEST_CASE("generated") {

#include "test_grapheme_data.h"
}

TEST_CASE("iterators") {

    std::string str("ab");
    util::grapheme_iterator it(str.begin(), str.end());
    check_graphemes(it, {U"a", U"b"});
}

}
