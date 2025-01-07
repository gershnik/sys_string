//
// Copyright 2024 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
// _🪦🐮🐌_

#include <sys_string/grapheme_view.h>
#include <sys_string/utf_view.h>

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
    
    template<std::ranges::forward_range Range>
    void check_graphemes_reverse_range(Range && range, const std::vector<std::u32string> & expected,
                                       std::source_location loc = std::source_location::current())
    {
        std::vector<std::u32string> rexpected;
        for(auto & ex: expected | std::views::reverse)
            rexpected.emplace_back(ex.rbegin(), ex.rend());
        auto gr = graphemes(std::forward<Range>(range));
        std::vector<std::u32string> result;
        std::ranges::transform(std::ranges::subrange(gr.rbegin(), gr.rend()), std::back_inserter(result), [](auto range) {
            return std::u32string(range.begin(), range.end());
        });
        INFO("source: ", std::string(loc.file_name()), std::string(":"), loc.line());
        bool res = std::ranges::equal(result, rexpected);
        CHECK(res);
    }
    
    template<std::ranges::forward_range Range>
    void check_graphemes_iter(const Range & range, const std::vector<std::u32string> & expected, 
                              std::source_location loc = std::source_location::current())
    {
        util::grapheme_iterator<std::ranges::iterator_t<const Range>, 
                                std::ranges::sentinel_t<const Range>,
                                util::iter_direction::forward> it(
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
    void check_graphemes_reverse_iter(const Range & range, const std::vector<std::u32string> & expected,
                                      std::source_location loc = std::source_location::current())
    {
        util::grapheme_iterator<ranges::reverse_iterator_t<const Range>,
                                ranges::reverse_sentinel_t<const Range>,
                                util::iter_direction::backward> rit(
                                    std::ranges::rbegin(range),
                                    std::ranges::rend(range)
                                );
        std::vector<std::u32string> result;
        std::ranges::transform(rit, std::default_sentinel, std::back_inserter(result), [](auto range) {
            return std::u32string(range.begin(), range.end());
        });
        INFO("source: ", std::string(loc.file_name()), std::string(":"), loc.line());
        bool res = std::ranges::equal(result, expected | std::views::reverse);
        CHECK(res);
    }

#if defined(__GNUC__)
    [[gnu::noinline]]
#elif defined(__MSVC__)
    [[msvc::noinline]]
#endif
    void check_graphemes(std::u32string_view src, const std::vector<std::u32string> & expected, std::source_location loc = std::source_location::current())
    {
        check_graphemes_range(src, expected, loc);
        check_graphemes_reverse_range(src, expected, loc);
    }
    
}

TEST_SUITE("grapheme") {

TEST_CASE("boundary") {
    check_graphemes(U""sv, {});
    check_graphemes(std::u32string_view(U"\0\0", 2), {std::u32string(U"\0", 1), std::u32string(U"\0", 1)});
}

TEST_CASE("generated") {

#include "test_grapheme_data.h"
}

TEST_CASE("iterators") {

    check_graphemes_iter(""s, {});
    check_graphemes_reverse_iter(""s, {});
    check_graphemes_iter("ab"s, {U"a", U"b"});
    check_graphemes_reverse_iter("ab"s, {U"a", U"b"});
}

TEST_CASE("ranges") {

    check_graphemes_range("ab"s, {U"a", U"b"});
    check_graphemes_reverse_range("ab"s, {U"a", U"b"});
    check_graphemes_range(u"ab"s, {U"a", U"b"});
    check_graphemes_reverse_range(u"ab"s, {U"a", U"b"});
    check_graphemes_range(U"ab"s, {U"a", U"b"});
    check_graphemes_reverse_range(U"ab"s, {U"a", U"b"});
    check_graphemes_range(as_utf32("ab"s), {U"a", U"b"});
    check_graphemes_reverse_range(as_utf32("ab"s), {U"a", U"b"});
}

TEST_CASE("view interface") {

    auto view = graphemes("abc"s);

    CHECK(view);
    CHECK(!view.empty());
    auto fr = view.front();
    CHECK(fr.size() == 1);
    CHECK(fr.front() == 'a');
    auto bc = view.back();
    CHECK(bc.size() == 1);
    CHECK(bc.front() == 'c');

    CHECK(view.begin() == view.cbegin());
    static_assert(std::is_same_v<decltype(view.end()), std::default_sentinel_t>);
    static_assert(std::is_same_v<decltype(view.cend()), std::default_sentinel_t>);
    CHECK(view.rbegin() == view.crbegin());
    static_assert(std::is_same_v<decltype(view.rend()), std::default_sentinel_t>);
    static_assert(std::is_same_v<decltype(view.crend()), std::default_sentinel_t>);

    auto empty_view = graphemes(""s);
    CHECK(!empty_view);
    CHECK(empty_view.empty());
}

}
