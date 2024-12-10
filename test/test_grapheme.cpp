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

static_assert(std::invocable<const sysstr::graphemes_func &, std::string>);

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

    template<std::ranges::forward_range Range>
    void check_graphemes_iter(const Range & range, const std::vector<std::u32string> & expected, 
                              std::source_location loc = std::source_location::current())
    {
        util::grapheme_iterator<std::ranges::iterator_t<const Range>, 
                                std::ranges::sentinel_t<const Range>> it(
                                    std::ranges::begin(range),
                                    std::ranges::end(range)
                                );
        std::vector<std::u32string> result;
        std::ranges::transform(it, std::default_sentinel, std::back_inserter(result), [](auto range) {
            return std::u32string(range.begin(), range.end());
        });
        INFO("source: ", std::string(loc.file_name()), std::string(":"), loc.line());
        bool res = std::ranges::equal(result, expected);
        CHECK(res);
    }

    template<std::ranges::forward_range Range>
    void check_graphemes_range(Range && range, const std::vector<std::u32string> & expected, 
                               std::source_location loc = std::source_location::current())
    {
        std::vector<std::u32string> result;
        #if __cpp_lib_ranges >= 202202L
            std::ranges::transform(std::forward<Range>(range) | graphemes, std::back_inserter(result), [](auto range) {
                return std::u32string(range.begin(), range.end());
            });
        #else
            std::ranges::transform(graphemes(std::forward<Range>(range)), std::back_inserter(result), [](auto range) {
                return std::u32string(range.begin(), range.end());
            });
        #endif
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

    check_graphemes_iter(""s, {});
    check_graphemes_iter("ab"s, {U"a", U"b"});
}

TEST_CASE("ranges") {

    check_graphemes_range(""s, {});
    check_graphemes_range("ab"s, {U"a", U"b"});
}

}
