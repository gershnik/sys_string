//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
// _🪦🐮🐌_

#include <sys_string/sys_string.h>

#include <sstream>
#include <iomanip>

#include <doctest/doctest.h>

#include <list>

using namespace sysstr;

static_assert(std::is_nothrow_default_constructible_v<sys_string>);
static_assert(std::is_nothrow_copy_constructible_v<sys_string>);
static_assert(std::is_nothrow_move_constructible_v<sys_string>);
static_assert(std::is_nothrow_copy_assignable_v<sys_string>);
static_assert(std::is_nothrow_move_assignable_v<sys_string>);
static_assert(std::is_nothrow_destructible_v<sys_string>);
static_assert(std::is_nothrow_swappable_v<sys_string>);
static_assert(std::is_standard_layout_v<sys_string>);

static_assert(std::ranges::random_access_range<sys_string::char_access>);
static_assert(!std::ranges::view<sys_string::char_access>);
static_assert(!std::ranges::borrowed_range<sys_string::char_access>);
              
static_assert(std::ranges::forward_range<sys_string::utf32_access>);
static_assert(!std::ranges::bidirectional_range<sys_string::utf32_access>);
static_assert(!std::ranges::common_range<sys_string::utf32_access>);
static_assert(!std::ranges::view<sys_string::utf32_access>);
static_assert(!std::ranges::borrowed_range<sys_string::utf32_access>);
static_assert(ranges::custom_reverse_traversable_range<sys_string::utf32_access>);

static_assert(std::ranges::forward_range<sys_string::utf16_access>);
static_assert(!std::ranges::bidirectional_range<sys_string::utf16_access>);
static_assert(!std::ranges::common_range<sys_string::utf16_access>);
static_assert(!std::ranges::view<sys_string::utf16_access>);
static_assert(!std::ranges::borrowed_range<sys_string::utf16_access>);
static_assert(ranges::custom_reverse_traversable_range<sys_string::utf16_access>);

static_assert(std::ranges::forward_range<sys_string::utf8_access>);
static_assert(!std::ranges::bidirectional_range<sys_string::utf8_access>);
static_assert(!std::ranges::common_range<sys_string::utf8_access>);
static_assert(!std::ranges::view<sys_string::utf8_access>);
static_assert(!std::ranges::borrowed_range<sys_string::utf8_access>);
static_assert(ranges::custom_reverse_traversable_range<sys_string::utf8_access>);

template<class T>
bool is_eq(T val)
    { return std::partial_ordering(val) == 0; }
template<class T>
bool is_lt(T val)
    { return std::partial_ordering(val) != 0; }

template<class CharT> const CharT * select([[maybe_unused]] const char * s8, 
                                            [[maybe_unused]] const char16_t * s16, 
                                            [[maybe_unused]] const char32_t * s32)
{
    if constexpr (std::is_same_v<CharT, char32_t>)
        return s32;
    else if constexpr (std::is_same_v<CharT, char16_t>)
        return s16;
    else
        return s8;
}

TEST_SUITE("general") {

TEST_CASE( "Creation" ) {
    
    sys_string from_char("abc", 3);
    CHECK(from_char == S("abc"));

    sys_string from_utf16(u"abc", 3);
    CHECK(from_utf16 == S("abc"));

    sys_string from_utf32(U"abc", 3);
    CHECK(from_utf32 == S("abc"));
    
    CHECK(sys_string(S("abc")) == S("abc"));
    
    sys_string temp = S("abc");
    sys_string new_temp(std::move(temp));
    CHECK(new_temp == S("abc"));

    temp = S("abc");
    new_temp = S("xyz");
    swap(temp, new_temp);
    CHECK(temp == S("xyz"));
    CHECK(new_temp == S("abc"));

    sys_string from_long_char("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 52);
    CHECK(from_long_char == S("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"));
}

TEST_CASE( "Slicing" ) {
    {
        sys_string str = S("a水𐀀𝄞");
        sys_string::utf32_access access(str);

        auto first = std::begin(access);
        ++first;
        CHECK(sys_string(first, std::end(access)) == S("水𐀀𝄞"));
        auto last = first;
        ++last;
        CHECK(sys_string(first, last) == S("水"));
        CHECK(sys_string(std::ranges::subrange(first, last)) == S("水"));

        auto rfirst = ranges::make_reverse_iterator(access, last);
        CHECK(sys_string(rfirst, std::rend(access)) == S("a水"));
        auto rlast = ranges::make_reverse_iterator(access, first);
        CHECK(sys_string(rfirst, rlast) == S("水"));
        CHECK(sys_string(std::ranges::subrange(rfirst, rlast)) == S("水"));
    }

    {
        sys_string str = S("abc");
        sys_string::char_access access(str);

        auto first = std::begin(access);
        ++first;
        CHECK(sys_string(first, std::end(access)) == S("bc"));
        auto last = first;
        ++last;
        CHECK(sys_string(first, last) == S("b"));
        CHECK(sys_string(std::ranges::subrange(first, last)) == S("b"));

        auto rfirst = ranges::make_reverse_iterator(access, last);
        CHECK(sys_string(rfirst, std::rend(access)) == S("ab"));
        auto rlast = ranges::make_reverse_iterator(access, first);
        CHECK(sys_string(rfirst, rlast) == S("b"));
        CHECK(sys_string(std::ranges::subrange(rfirst, rlast)) == S("b"));
    }
}

TEST_CASE( "Iteration" ) {
    sys_string str = S("a水𐀀𝄞");
    sys_string empty = S("");
    sys_string long_str = sys_string(std::string(4096, 'a'));
    
    SUBCASE("utf8") {
        std::string converted;
        for (char c: sys_string::utf8_access(str))
        {
            converted.push_back(c);
        }
        std::string expected = "a水𐀀𝄞";
        CHECK(converted == expected);
        
        converted.clear();
        sys_string::utf8_access view(str);
        std::ranges::copy(view.rbegin(), view.rend(), std::back_inserter(converted));
        CHECK(converted == std::string{expected.rbegin(), expected.rend()});

        converted.clear();
        std::ranges::copy(sys_string::utf8_access(str), std::back_inserter(converted));
        CHECK(converted == expected);

        converted.clear();
        sys_string::char_access access(str);
        std::ranges::copy(as_utf8(access), std::back_inserter(converted));
        CHECK(converted == expected);

        static_assert(std::is_same_v<decltype(as_utf8(access))::iterator,
                                     sys_string::utf8_access::iterator>);

        bool res = std::ranges::equal(as_utf8(access) | std::views::take(1), std::array{'a'});
        CHECK(res);

        res = std::ranges::equal(std::views::all(view) | std::views::take(1), std::array{'a'});
        CHECK(res);

        converted.clear();
        for (char c: sys_string::utf8_access(empty))
        {
            converted.push_back(c);
        }
        CHECK(converted.empty());

        size_t acc = 0;
        for(char c: sys_string::utf8_access(long_str))
            acc += c;
        CHECK(acc == size_t(4096) * 'a');
    }

    SUBCASE("utf16") {
        std::u16string converted;
        for (char16_t c: sys_string::utf16_access(str))
        {
            converted.push_back(c);
        }
        std::u16string expected = u"a水𐀀𝄞";
        CHECK(converted == expected);
        
        converted.clear();
        sys_string::utf16_access view(str);
        std::ranges::copy(view.rbegin(), view.rend(), std::back_inserter(converted));
        CHECK(converted == std::u16string{expected.rbegin(), expected.rend()});

        converted.clear();
        std::ranges::copy(sys_string::utf16_access(str), std::back_inserter(converted));
        CHECK(converted == expected);

        converted.clear();
        sys_string::char_access access(str);
        std::ranges::copy(as_utf16(access), std::back_inserter(converted));
        CHECK(converted == expected);

        static_assert(std::is_same_v<decltype(as_utf16(access))::iterator,
                                     sys_string::utf16_access::iterator>);

        bool res = std::ranges::equal(as_utf16(access) | std::views::take(1), std::array{'a'});
        CHECK(res);

        res = std::ranges::equal(std::views::all(view) | std::views::take(1), std::array{'a'});
        CHECK(res);

        converted.clear();
        for (char16_t c: sys_string::utf16_access(empty))
        {
            converted.push_back(c);
        }
        CHECK(converted.empty());

        size_t acc = 0;
        for(char16_t c: sys_string::utf16_access(long_str))
            acc += c;
        CHECK(acc == size_t(4096) * u'a');
    }

    SUBCASE("utf32") {
        std::u32string converted;
        for (char32_t c: sys_string::utf32_access(str))
        {
            converted.push_back(c);
        }
        std::u32string expected = U"a水𐀀𝄞";
        CHECK(converted == expected);

        
        converted.clear();
        sys_string::utf32_access view(str);
        std::ranges::copy(view.rbegin(), view.rend(), std::back_inserter(converted));
        CHECK(converted == std::u32string{expected.rbegin(), expected.rend()});

        converted.clear();
        std::ranges::copy(sys_string::utf32_access(str), std::back_inserter(converted));
        CHECK(converted == expected);

        converted.clear();
        sys_string::char_access access(str);
        std::ranges::copy(as_utf32(access), std::back_inserter(converted));
        CHECK(converted == expected);

        static_assert(std::is_same_v<decltype(as_utf32(access))::iterator,
                                     sys_string::utf32_access::iterator>);

        bool res = std::ranges::equal(as_utf32(access) | std::views::take(1), std::array{'a'});
        CHECK(res);

        res = std::ranges::equal(std::views::all(view) | std::views::take(1), std::array{'a'});
        CHECK(res);
        
        converted.clear();
        for (char32_t c: sys_string::utf16_access(empty))
        {
            converted.push_back(c);
        }
        CHECK(converted.empty());
        
        sys_string part(++(++view.begin()), view.end());
        CHECK(part == S("𐀀𝄞"));

        size_t acc = 0;
        for(char32_t c: sys_string::utf32_access(long_str))
            acc += c;
        CHECK(acc == size_t(4096) * U'a');
    }
    
    SUBCASE("storage") {
        
        std::basic_string<sys_string::storage_type> converted;
        for (sys_string::storage_type c: sys_string::char_access(str))
        {
            converted.push_back(c);
        }
        std::basic_string<sys_string::storage_type> expected = select<sys_string::storage_type>("a水𐀀𝄞", u"a水𐀀𝄞", U"a水𐀀𝄞");
        CHECK(converted == expected);
        
        converted.clear();
        sys_string::char_access access(str);
        std::ranges::copy(access.rbegin(), access.rend(), std::back_inserter(converted));
        CHECK(converted == std::basic_string<sys_string::storage_type>{expected.rbegin(), expected.rend()});
        
        converted.clear();
        for (sys_string::storage_type c: sys_string::char_access(empty))
        {
            converted.push_back(c);
        }
        CHECK(converted.empty());
        
        sys_string part(access.begin() + 1, access.end());
        CHECK(part == S("水𐀀𝄞"));

        size_t acc = 0;
        for(sys_string::storage_type c: sys_string::char_access(long_str))
            acc += c;
        CHECK(acc == size_t(4096) * 'a');

        sys_string::char_access long_acc(long_str);
        CHECK(long_acc[1024] == sys_string::storage_type('a'));
    }
}

TEST_CASE( "Comparsion" ) {
    
    CHECK(sys_string() == sys_string());
    CHECK(sys_string() == S(""));
    CHECK(S("") == S(""));
    CHECK(S("abc") != sys_string());
    CHECK(compare(S(""), sys_string()) == std::strong_ordering::equal);
    CHECK(compare(sys_string(), S("abc")) == std::strong_ordering::less);
    
    
    CHECK(is_eq(compare_no_case(sys_string(), sys_string())));
    CHECK(is_eq(compare_no_case(sys_string(), S(""))));
    CHECK(is_eq(compare_no_case(S(""), S(""))));
    
    sys_string base = S("азбука");
    sys_string greater = S("алфавит");
    sys_string ucase = S("аЗбука");
    
    CHECK(base < greater);
    CHECK(base <= greater);
    CHECK(base <= base);
    CHECK(greater > base);
    CHECK(greater >= base);
    CHECK(greater >= greater);
    
    CHECK(base == base);
    CHECK(base != greater);
    
    CHECK(is_eq(compare_no_case(base, ucase)));
    
    CHECK(is_eq(compare_no_case(S("maße"), S("MASSE"))));

    CHECK(is_eq(compare_no_case(S("ßßßßßßßßßßßßßßßßßßßß"), S("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS"))));
    
    CHECK(is_lt(compare_no_case(S("SS"), S("ßßßßßß"))));
    
    CHECK(is_lt(compare_no_case(S("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS"), S("ßßßßßßßßßßßßßßßßß"))));
    
    CHECK(is_lt(compare_no_case(S("maße"), S("MASSF"))));
    
    sys_string bad(u"a\xD800", 2);
    CHECK(is_lt(compare_no_case(S("ab"), bad)));

}

TEST_CASE( "Hash" ) {
    std::hash<sys_string> hasher;

    CHECK(hasher(sys_string()) == hasher(sys_string()));
    CHECK(hasher(S("")) == hasher(S("")));
    CHECK(hasher(S("")) == hasher(sys_string()));
    CHECK(hasher(S("abc")) == hasher(S("abc")));
    //technically this could fail but this means the hash function is atrocious...
    CHECK(hasher(S("a")) != hasher(S("b")));
}

TEST_CASE( "Case conversion" ) {
    
    CHECK(S("maße").to_upper() == S("MASSE"));
    CHECK(S("MAẞE").to_lower() == S("maße"));
    
    CHECK(S("ΒΟΥΣ").to_lower() == S("βους"));
    CHECK(S("ΒΟΥΣ AA").to_lower() == S("βους aa"));
    CHECK(S("ΒΟΥΣAA").to_lower() == S("βουσaa"));
    CHECK(S("ͺΣ").to_lower() == S("ͺσ"));
    CHECK(S("ΑͺΣ").to_lower() == S("αͺς"));
    CHECK(S("ΑΣͺ").to_lower() == S("αςͺ"));
    CHECK(S("ΑΣͺΑ").to_lower() == S("ασͺα"));
    
    CHECK(S("βους").to_upper() == S("ΒΟΥΣ"));

    CHECK(S("\U00010400").to_lower() == S("\U00010428"));

    CHECK(S("𐐀").to_lower() == S("𐐨"));

    CHECK(S("İ").to_lower() == S("i̇"));

    sys_string upper(u"\u0041\U000118A9\U00016E5F\U0001D7CA\U0001E921");
    sys_string lower(u"\u0061\U000118C9\U00016E7F\U0001D7CA\U0001E943");
    CHECK(upper.to_lower() == lower);
    CHECK(lower.to_upper() == upper);
}

TEST_CASE( "Normalization" ) {

    CHECK(S("\u00C5").normalize(normalization::nfd) == S("\u0041\u030A"));
    CHECK(S("\u0041\u030A").normalize(normalization::nfc) == S("\u00C5"));
    
    CHECK(S("nörmalization").normalize(normalization::nfc) == S("nörmalization"));
    CHECK(S("No\u0308rmalization").normalize(normalization::nfc) == S("Nörmalization"));
}

TEST_CASE( "Trim" ) {
    CHECK(S("").trim() == S(""));
    CHECK(S("  \t\n   ").trim() == S(""));
    CHECK(S("   a").trim() == S("a"));
    CHECK(S("a   ").trim() == S("a"));
    CHECK(S("   a   ").trim() == S("a"));
    CHECK(S("   a    b    ").trim() == S("a    b"));
    CHECK(S(" '  a    b    ").trim() == S("'  a    b"));
    
    CHECK(S("").ltrim() == S(""));
    CHECK(S("  \t\n   ").ltrim() == S(""));
    CHECK(S("   a").ltrim() == S("a"));
    CHECK(S("a   ").ltrim() == S("a   "));
    CHECK(S("   a   ").ltrim() == S("a   "));
    CHECK(S("   a    b    ").ltrim() == S("a    b    "));
    CHECK(S(" '  a    b    ").ltrim() == S("'  a    b    "));
    
    CHECK(S("").rtrim() == S(""));
    CHECK(S("  \t\n   ").rtrim() == S(""));
    CHECK(S("   a").rtrim() == S("   a"));
    CHECK(S("a   ").rtrim() == S("a"));
    CHECK(S("   a   ").rtrim() == S("   a"));
    CHECK(S("   a    b    ").rtrim() == S("   a    b"));
    CHECK(S("   a    b  ' ").rtrim() == S("   a    b  '"));
}

namespace
{
    struct
    {
        template<class Sep>
        auto operator()(const sys_string & str, Sep sep) {
          
            std::vector<sys_string> ret;
            str.split(std::back_inserter(ret), sep);
            return ret;
        }
        
        template<class Sep>
        auto operator()(const sys_string & str, Sep sep, size_t max_split) {
          
            std::vector<sys_string> ret;
            str.split(std::back_inserter(ret), sep, max_split);
            return ret;
        }
        
    } splitter;
}

TEST_CASE( "Split" ) {
    
    CHECK(splitter(S(""), U'q') == std::vector({S("")}));
    CHECK(splitter(S("q"), U'q') == std::vector({S(""), S("")}));
    CHECK(splitter(S("qa"), U'q') == std::vector({S(""), S("a")}));
    CHECK(splitter(S("aq"), U'q') == std::vector({S("a"), S("")}));
    CHECK(splitter(S("q"), U'q', 0) == std::vector({S("q")}));
    CHECK(splitter(S("q"), U'q', 1) == std::vector({S(""), S("")}));
    CHECK(splitter(S("q"), U'q', 2) == std::vector({S(""), S("")}));
    CHECK(splitter(S("q"), U'q', 3) == std::vector({S(""), S("")}));
    CHECK(splitter(S("qq"), U'q', 1) == std::vector({S(""), S("q")}));
    CHECK(splitter(S("a;b;c"), U';', 0) == std::vector({S("a;b;c")}));
    CHECK(splitter(S("a;b;c"), U';', 1) == std::vector({S("a"), S("b;c")}));
    CHECK(splitter(S("a;b;c"), U';', 2) == std::vector({S("a"), S("b"), S("c")}));
    
    CHECK(splitter(S(""), S("q")) == std::vector({S("")}));
    CHECK(splitter(S("q"), S("q")) == std::vector({S(""), S("")}));
    CHECK(splitter(S("qa"), S("q")) == std::vector({S(""), S("a")}));
    CHECK(splitter(S("aq"), S("q")) == std::vector({S("a"), S("")}));
    CHECK(splitter(S("q"), S("q"), 0) == std::vector({S("q")}));
    CHECK(splitter(S("q"), S("q"), 1) == std::vector({S(""), S("")}));
    CHECK(splitter(S("q"), S("q"), 2) == std::vector({S(""), S("")}));
    CHECK(splitter(S("q"), S("q"), 3) == std::vector({S(""), S("")}));
    CHECK(splitter(S("qq"), S("q"), 1) == std::vector({S(""), S("q")}));
    CHECK(splitter(S("a;b;c"), S(";"), 0) == std::vector({S("a;b;c")}));
    CHECK(splitter(S("a;b;c"), S(";"), 1) == std::vector({S("a"), S("b;c")}));
    CHECK(splitter(S("a;b;c"), S(";"), 2) == std::vector({S("a"), S("b"), S("c")}));
    
    CHECK(splitter(S(""), S("")) == std::vector({S("")}));
    CHECK(splitter(S("a"), S("")) == std::vector({S("a")}));
    CHECK(splitter(S("ab"), S("")) == std::vector({S("ab")}));
    
    auto searcher = [] (sys_string::utf32_access::iterator str_start, std::default_sentinel_t str_end) noexcept {
        
        constexpr char32_t seps[] = U"xyz";
        
        auto found_it = std::ranges::find_first_of(str_start, str_end, std::begin(seps), std::end(seps));
        if (found_it == str_end)
            return std::pair(found_it, found_it);
        auto found_end = found_it;
        ++found_end;
        return std::pair(found_it, found_end);
    };
    
    CHECK(splitter(S(""), searcher) == std::vector({S("")}));
    CHECK(splitter(S("x"), searcher) == std::vector({S(""), S("")}));
    CHECK(splitter(S("ya"), searcher) == std::vector({S(""), S("a")}));
    CHECK(splitter(S("az"), searcher) == std::vector({S("a"), S("")}));
}

TEST_CASE( "Join" ) {

    std::vector<sys_string> empty;
    std::vector<sys_string> one = {S("Q")};
    std::vector<sys_string> two = {S("Q"), S("R")};
    
    CHECK(S("").join(empty.begin(), empty.end()) == S(""));
    CHECK(S("").join(empty) == S(""));
    CHECK(S("").join(one.begin(), one.end()) == S("Q"));
    CHECK(S("").join(one) == S("Q"));
    CHECK(S("").join(two.begin(), two.end()) == S("QR"));
    CHECK(S("").join(two) == S("QR"));
    
    CHECK(S("A").join(empty.begin(), empty.end()) == S(""));
    CHECK(S("A").join(empty) == S(""));
    CHECK(S("A").join(one.begin(), one.end()) == S("Q"));
    CHECK(S("A").join(one) == S("Q"));
    CHECK(S("A").join(two.begin(), two.end()) == S("QAR"));
    CHECK(S("A").join(two) == S("QAR"));
}

TEST_CASE( "Prefix" ) {
    
    CHECK(S("").starts_with(S("")));
    CHECK(!S("").starts_with(S("a")));
    CHECK(S("a").starts_with(S("")));
    CHECK(S("a").starts_with(S("a")));
    CHECK(!S("a").starts_with(S("b")));
    CHECK(S("ab").starts_with(S("a")));
    
    CHECK(!S("").starts_with(U'a'));
    CHECK(S("a").starts_with(U'a'));
    CHECK(!S("a").starts_with(U'b'));
    CHECK(S("ab").starts_with(U'a'));
    CHECK(!S("\uFFFDb").starts_with(char32_t(0x110000)));
    
    std::array prefixes = {S("a"), S("b"), S("c")};
    CHECK(S("").find_prefix(prefixes.begin(), prefixes.end()) == prefixes.end());
    CHECK(S("").find_prefix(prefixes) == prefixes.end());
    CHECK(S("cd").find_prefix(prefixes.begin(), prefixes.end()) == prefixes.end() - 1);
    CHECK(S("cd").find_prefix(prefixes) == prefixes.end() - 1);

    static_assert(std::is_same_v<decltype(S("cd").find_prefix(std::array{S("a"), S("b"), S("c")})), std::ranges::dangling>);
    
    CHECK(S("").remove_prefix(S("")) == S(""));
    CHECK(S("").remove_prefix(S("a")) == S(""));
    CHECK(S("a").remove_prefix(S("")) == S("a"));
    CHECK(S("a").remove_prefix(S("a")) == S(""));
    CHECK(S("a").remove_prefix(S("b")) == S("a"));
    CHECK(S("ab").remove_prefix(S("a")) == S("b"));
    
    CHECK(S("").remove_prefix(U'a') == S(""));
    CHECK(S("a").remove_prefix(U'a') == S(""));
    CHECK(S("a").remove_prefix(U'b') == S("a"));
    CHECK(S("ab").remove_prefix(U'a') == S("b"));
}

TEST_CASE( "Suffix" ) {
    
    CHECK(S("").ends_with(S("")));
    CHECK(!S("").ends_with(S("a")));
    CHECK(S("a").ends_with(S("")));
    CHECK(S("a").ends_with(S("a")));
    CHECK(!S("a").ends_with(S("b")));
    CHECK(S("ab").ends_with(S("b")));
    
    CHECK(!S("").ends_with(U'a'));
    CHECK(S("a").ends_with(U'a'));
    CHECK(!S("a").ends_with(U'b'));
    CHECK(S("ab").ends_with(U'b'));
    CHECK(!S("b\uFFFD").ends_with(char32_t(0x110000)));
    
    std::array suffixes = {S("a"), S("b"), S("c")};
    CHECK(S("").find_suffix(suffixes.begin(), suffixes.end()) == suffixes.end());
    CHECK(S("").find_suffix(suffixes) == suffixes.end());
    CHECK(S("dc").find_suffix(suffixes.begin(), suffixes.end()) == suffixes.end() - 1);
    CHECK(S("dc").find_suffix(suffixes) == suffixes.end() - 1);

    static_assert(std::is_same_v<decltype(S("cd").find_suffix(std::array{S("a"), S("b"), S("c")})), std::ranges::dangling>);
    
    CHECK(S("").remove_suffix(S("")) == S(""));
    CHECK(S("").remove_suffix(S("a")) == S(""));
    CHECK(S("a").remove_suffix(S("")) == S("a"));
    CHECK(S("a").remove_suffix(S("a")) == S(""));
    CHECK(S("a").remove_suffix(S("b")) == S("a"));
    CHECK(S("ab").remove_suffix(S("b")) == S("a"));
    
    CHECK(S("").remove_suffix(U'a') == S(""));
    CHECK(S("a").remove_suffix(U'a') == S(""));
    CHECK(S("a").remove_suffix(U'b') == S("a"));
    CHECK(S("ab").remove_suffix(U'b') == S("a"));
}

TEST_CASE( "Contains" ) {
    
    CHECK(S("").contains(S("")));
    CHECK(!S("").contains(S("a")));
    CHECK(S("a").contains(S("")));
    CHECK(S("a").contains(S("a")));
    CHECK(!S("a").contains(S("b")));
    CHECK(S("ab").contains(S("b")));
    CHECK(S("abc").contains(S("b")));
    
    CHECK(!S("").contains(U'a'));
    CHECK(S("a").contains(U'a'));
    CHECK(!S("a").contains(U'b'));
    CHECK(S("ab").contains(U'b'));
    CHECK(S("aba").contains(U'b'));
    CHECK(!S("b\uFFFDc").contains(char32_t(0x110000)));
    
    std::array infixes = {S("a"), S("b"), S("c")};
    CHECK(S("").find_contained(infixes.begin(), infixes.end()) == infixes.end());
    CHECK(S("").find_contained(infixes) == infixes.end());
    CHECK(S("dc").find_contained(infixes.begin(), infixes.end()) == infixes.end() - 1);
    CHECK(S("dc").find_contained(infixes) == infixes.end() - 1);
    CHECK(S("dbn").find_contained(infixes.begin(), infixes.end()) == infixes.end() - 2);
    CHECK(S("dbn").find_contained(infixes) == infixes.end() - 2);

    static_assert(std::is_same_v<decltype(S("cd").find_contained(std::array{S("a"), S("b"), S("c")})), std::ranges::dangling>);
}

TEST_CASE( "Replace" ) {
    
    CHECK(S("").replace(S(""), S("")) == S(""));
    CHECK(S("").replace(S(""), S("a")) == S(""));
    CHECK(S("").replace(S("a"), S("")) == S(""));
    CHECK(S("😏").replace(S(""), S("")) == S("😏"));
    CHECK(S("😏").replace(S(""), S("😫")) == S("😏"));
    CHECK(S("😏").replace(S("😫"), S("")) == S("😏"));
    CHECK(S("😏").replace(S("😏"), S("")) == S(""));
    CHECK(S("😏").replace(S("😏"), S("😫")) == S("😫"));
    CHECK(S("🥳😏🥸").replace(S("😏"), S("😫")) == S("🥳😫🥸"));
    CHECK(S("🥳😏🥸😏").replace(S("😏"), S("😫")) == S("🥳😫🥸😫"));
    
    CHECK(S("").replace(S(""), U'a') == S(""));
    CHECK(S("").replace(U'a', S("")) == S(""));
    CHECK(S("😏").replace(S(""), U'😫') == S("😏"));
    CHECK(S("😏").replace(U'😫', S("")) == S("😏"));
    CHECK(S("😏").replace(U'😏', S("")) == S(""));
    CHECK(S("😏").replace(U'😏', U'😫') == S("😫"));
    CHECK(S("🥳😏🥸").replace(U'😏', U'😫') == S("🥳😫🥸"));
    CHECK(S("🥳😏🥸😏").replace(U'😏', U'😫') == S("🥳😫🥸😫"));
}

TEST_CASE( "HeadTail" ) {

    CHECK(S("").suffix_after_first(S("")) == S(""));
    CHECK(S("").suffix_after_first(U'a') == std::nullopt);
    CHECK(S("").prefix_before_first(S("")) == S(""));
    CHECK(S("").prefix_before_first(U'a') == std::nullopt);
    CHECK(S("").suffix_after_last(S("")) == S(""));
    CHECK(S("").suffix_after_last(U'a') == std::nullopt);
    CHECK(S("").prefix_before_last(S("")) == S(""));
    CHECK(S("").prefix_before_last(U'a') == std::nullopt);

    CHECK(S("ab").suffix_after_first(S("c")) == std::nullopt);
    CHECK(S("ab").prefix_before_first(S("c")) == std::nullopt);
    CHECK(S("ab").suffix_after_last(S("c")) == std::nullopt);
    CHECK(S("ab").prefix_before_last(S("c")) == std::nullopt);

    CHECK(S("ab").suffix_after_first(S("a")) == S("b"));
    CHECK(S("ab").prefix_before_first(S("a")) == S(""));
    CHECK(S("ab").suffix_after_last(S("b")) == S(""));
    CHECK(S("ab").prefix_before_last(S("b")) == S("a"));

    CHECK(S("a🥸b").suffix_after_first(S("🥸")) == S("b"));
    CHECK(S("a🥸b").prefix_before_first(S("🥸")) == S("a"));
    CHECK(S("a🥸b").suffix_after_last(S("🥸")) == S("b"));
    CHECK(S("a🥸b").prefix_before_last(S("🥸")) == S("a"));

    CHECK(S("a🥸b🥸c").suffix_after_first(S("🥸")) == S("b🥸c"));
    CHECK(S("a🥸b🥸c").prefix_before_first(S("🥸")) == S("a"));
    CHECK(S("a🥸b🥸c").suffix_after_last(S("🥸")) == S("c"));
    CHECK(S("a🥸b🥸c").prefix_before_last(S("🥸")) == S("a🥸b"));
}

TEST_CASE( "Partition" ) {
    CHECK(S("").partition_at_first(S("")) == std::pair(S(""), S("")));
    CHECK(S("").partition_at_first(U'a') == std::nullopt);
    CHECK(S("").partition_at_last(S("")) == std::pair(S(""), S("")));
    CHECK(S("").partition_at_last(U'a') == std::nullopt);

    CHECK(S("ab").partition_at_first(S("c")) == std::nullopt);
    CHECK(S("ab").partition_at_last(S("c")) == std::nullopt);
    CHECK(S("ab").partition_at_first(S("a")) == std::pair(S(""), S("b")));
    CHECK(S("ab").partition_at_last(S("b")) == std::pair(S("a"), S("")));

    CHECK(S("a🥸b").partition_at_first(S("🥸")) == std::pair(S("a"), S("b")));
    CHECK(S("a🥸b").partition_at_last(S("🥸")) == std::pair(S("a"), S("b")));
    CHECK(S("a🥸b🥸c").partition_at_first(S("🥸")) == std::pair(S("a"), S("b🥸c")));
    CHECK(S("a🥸b🥸c").partition_at_last(S("🥸")) == std::pair(S("a🥸b"), S("c")));
}

TEST_CASE( "Addition" ) {

    using namespace std::literals;
    
    CHECK(S("") + S("") == S(""));
    
    CHECK(S("") + S("🤢") == S("🤢"));
    CHECK(S("") + U'🤢' == S("🤢"));
    CHECK(S("🤢") + S("") == S("🤢"));
    CHECK(U'🤢' + S("") == S("🤢"));
    
    CHECK(S("💜") + S("🧡") == S("💜🧡"));
    CHECK(U'💜' + S("🧡") == S("💜🧡"));
    CHECK(S("💜") + U'🧡' == S("💜🧡"));
    
    CHECK(S("💾") + S("💿") + S("⏰") == S("💾💿⏰"));
    CHECK(S("💾") + U'💿' + S("⏰") == S("💾💿⏰"));
    CHECK(S("💾") + (S("💿") + S("⏰")) == S("💾💿⏰"));
    CHECK(S("💾") + (U'💿' + S("⏰")) == S("💾💿⏰"));
    CHECK((S("💾") + S("💿")) + S("⏰") == S("💾💿⏰"));
    CHECK((S("💾") + U'💿') + S("⏰") == S("💾💿⏰"));
    
    CHECK((S("💾") + U'💿') + (U'🜇' + S("⏰")) == S("💾💿🜇⏰"));
    CHECK((S("💾") + U'💿') + S("🧡") + (U'🜇' + S("⏰")) == S("💾💿🧡🜇⏰"));
    CHECK(((U'a' + S("b")) + (S("💾") + U'💿')) + (U'🜇' + S("⏰")) == S("ab💾💿🜇⏰"));

    CHECK(S("💜") + "🧡" == S("💜🧡"));
    CHECK("💜" + S("🧡") == S("💜🧡"));
    CHECK(S("💜") + "🧡"s == S("💜🧡"));
    CHECK("💜"s + S("🧡") == S("💜🧡"));

    std::list<char16_t> list{u'Ю'};
    CHECK(S("💜") + list == S("💜Ю"));

    CHECK((("a"sv + S("b")) + (S("💾") + "💿"s)) + ("🜇"s + S("⏰")) == S("ab💾💿🜇⏰"));
    sys_string result = S("a") + U'b' + "cd" + "ef"s + u"gh"sv + U"ij" + std::vector{'k', 'l'};
    CHECK(result == S("abcdefghijkl"));
}

TEST_CASE( "c_str" ) {
    const sys_string & str = S("a🧡bc");
    sys_string::char_access access(str);
    const char * cstr = access.c_str();
    REQUIRE(cstr);
    CHECK(strcmp(cstr, "a🧡bc") == 0);
    CHECK(access.c_str() == cstr);
}

TEST_CASE( "data" ) {
    const sys_string & str = S("a🧡bc");

    auto * data = str.data();
    if (data)
    {
        if constexpr (std::is_same_v<sys_string::storage_type, char>)
        {
            REQUIRE(str.storage_size() == sys_string::size_type(strlen("a🧡bc")));
            CHECK(memcmp(data, "a🧡bc", str.storage_size()) == 0);
        }
        else if constexpr (std::is_same_v<sys_string::storage_type, char16_t>)
        {
            REQUIRE(str.storage_size() == sys_string::size_type(std::char_traits<char16_t>::length(u"a🧡bc")));
            CHECK(memcmp(data, u"a🧡bc", str.storage_size() * sizeof(sys_string::storage_type)) == 0);
        }
        else 
        {
            REQUIRE(str.storage_size() == sys_string::size_type(std::char_traits<char32_t>::length(U"a🧡bc")));
            CHECK(memcmp(data, U"a🧡bc", str.storage_size() * sizeof(sys_string::storage_type)) == 0);
        }
    }
    std::vector<sys_string::storage_type> buf(str.storage_size() + 5);
    auto res = str.copy_data(0, buf.data(), sys_string::size_type(buf.size()));
    REQUIRE(res == str.storage_size());

    if constexpr (std::is_same_v<sys_string::storage_type, char>)
        CHECK(memcmp(buf.data(), "a🧡bc", str.storage_size()) == 0);
    else if constexpr (std::is_same_v<sys_string::storage_type, char16_t>)
        CHECK(memcmp(buf.data(), u"a🧡bc", str.storage_size() * sizeof(sys_string::storage_type)) == 0);
    else 
        CHECK(memcmp(buf.data(), U"a🧡bc", str.storage_size() * sizeof(sys_string::storage_type)) == 0);

    res = str.copy_data(1, buf.data(), sys_string::size_type(buf.size()));
    REQUIRE(res == str.storage_size() - 1);

    if constexpr (std::is_same_v<sys_string::storage_type, char>)
        CHECK(memcmp(buf.data(), ("a🧡bc") + 1, str.storage_size() - 1) == 0);
    else if constexpr (std::is_same_v<sys_string::storage_type, char16_t>)
        CHECK(memcmp(buf.data(), (u"a🧡bc") + 1, (str.storage_size() - 1) * sizeof(sys_string::storage_type)) == 0);
    else 
        CHECK(memcmp(buf.data(), (U"a🧡bc") + 1, (str.storage_size() - 1) * sizeof(sys_string::storage_type)) == 0);

    res = sys_string().copy_data(0, buf.data(), sys_string::size_type(buf.size()));
    REQUIRE(res == 0);
}

TEST_CASE( "ostream" ) {
    
    {
        std::ostringstream stream;
        stream << sys_string();
        CHECK(stream.str() == "");
    }
    
    {
        std::ostringstream stream;
        stream << S("");
        CHECK(stream.str() == "");
    }
    
    {
        std::ostringstream stream;
        stream << S("a🧡bc");
        CHECK(stream.str() == "a🧡bc");
    }

    {
        std::ostringstream stream;
        stream << std::setw(15) << std::setfill(' ') << S("a🧡bc");
        CHECK(stream.str() == "a🧡bc");
    }
    
#if SYS_STRING_WCHAR_T_IS_UTF16 || SYS_STRING_WCHAR_T_IS_UTF32
    {
        std::wostringstream stream;
        stream << sys_string();
        CHECK(stream.str() == L"");
    }
    
    {
        std::wostringstream stream;
        stream << S("");
        CHECK(stream.str() == L"");
    }
    
    {
        std::wostringstream stream;
        stream << S("a🧡bc");
        CHECK(stream.str() == L"a🧡bc");
    }

    {
        std::wostringstream stream;
        stream << std::setw(15) << std::setfill(L' ') << sys_string(L"a🧡bc");
        CHECK(stream.str() == L"a🧡bc");
    }
#endif
}



TEST_CASE( "format" ) {

    CHECK(sys_string::format("%d", 5) == S("5"));

#if SYS_STRING_SUPPORTS_STD_FORMAT
    CHECK(sys_string::std_format("{} a水𐀀𝄞", 5) == S("5 a水𐀀𝄞"));
    int i = 5;
    CHECK(sys_string::std_vformat("{} a水𐀀𝄞", std::make_format_args(i)) == S("5 a水𐀀𝄞"));

    CHECK(std::format("{0}", S("a🧡bc")) == "a🧡bc");
#if SYS_STRING_WCHAR_T_IS_UTF16 || SYS_STRING_WCHAR_T_IS_UTF32
    CHECK(std::format(L"{0}", S("a🧡bc")) == L"a🧡bc");
#endif

#endif

}

}

