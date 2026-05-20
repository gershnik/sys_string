//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
// _🪦🐮🐌_

#include <sys_string/sys_string.h>

#include <list>


#include <doctest/doctest.h>

using namespace sysstr;

using namespace std::literals;

static_assert(std::is_nothrow_default_constructible_v<sys_string_builder>);
static_assert(!std::is_copy_constructible_v<sys_string_builder>);
static_assert(std::is_nothrow_move_constructible_v<sys_string_builder>);
static_assert(!std::is_copy_assignable_v<sys_string_builder>);
static_assert(std::is_nothrow_move_assignable_v<sys_string_builder>);
static_assert(std::is_nothrow_destructible_v<sys_string_builder>);
static_assert(std::is_nothrow_swappable_v<sys_string_builder>);

static_assert(std::ranges::forward_range<sys_string_builder>);
static_assert(!std::ranges::random_access_range<sys_string_builder>);
static_assert(!std::ranges::common_range<sys_string_builder>);
static_assert(!std::ranges::view<sys_string_builder>);
static_assert(!std::ranges::borrowed_range<sys_string_builder>);
static_assert(std::ranges::viewable_range<sys_string_builder>);
              
static_assert(std::ranges::forward_range<sys_string_builder::utf32_view>);
static_assert(!std::ranges::bidirectional_range<sys_string_builder::utf32_view>);
static_assert(!std::ranges::common_range<sys_string_builder::utf32_view>);
static_assert(std::ranges::view<sys_string_builder::utf32_view>);
static_assert(std::ranges::borrowed_range<sys_string_builder::utf32_view>);
static_assert(std::ranges::viewable_range<sys_string_builder::utf32_view>);
static_assert(ranges::custom_reverse_traversable_range<sys_string_builder::utf32_view>);

static_assert(std::ranges::forward_range<sys_string_builder::utf16_view>);
static_assert(!std::ranges::bidirectional_range<sys_string_builder::utf16_view>);
static_assert(!std::ranges::common_range<sys_string_builder::utf16_view>);
static_assert(std::ranges::view<sys_string_builder::utf16_view>);
static_assert(std::ranges::borrowed_range<sys_string_builder::utf16_view>);
static_assert(std::ranges::viewable_range<sys_string_builder::utf16_view>);
static_assert(ranges::custom_reverse_traversable_range<sys_string_builder::utf16_view>);

static_assert(std::ranges::forward_range<sys_string_builder::utf8_view>);
static_assert(!std::ranges::bidirectional_range<sys_string_builder::utf8_view>);
static_assert(!std::ranges::common_range<sys_string_builder::utf8_view>);
static_assert(std::ranges::view<sys_string_builder::utf8_view>);
static_assert(std::ranges::borrowed_range<sys_string_builder::utf8_view>);
static_assert(std::ranges::viewable_range<sys_string_builder::utf8_view>);
static_assert(ranges::custom_reverse_traversable_range<sys_string_builder::utf8_view>);

TEST_SUITE("builder") {

TEST_CASE( "Builder basics" ) {
    using namespace std::literals;

    CHECK(sys_string_builder().build() == S(""));
    CHECK(sys_string_builder().append(U'🟣').build() == S("🟣"));
    CHECK(sys_string_builder().append("￥").build() == S("￥"));
    CHECK(sys_string_builder().append((const char *)"￥").build() == S("￥"));
    CHECK(sys_string_builder().append("￥", 6).build() == S(""));
    CHECK(sys_string_builder().append("￥"sv).build() == S("￥"));

    CHECK(sys_string_builder().append(u8"￥").build() == S("￥"));
    CHECK(sys_string_builder().append((const char8_t*)u8"￥").build() == S("￥"));
    CHECK(sys_string_builder().append(u8"￥", 3).build() == S(""));
    CHECK(sys_string_builder().append(u8"￥"sv).build() == S("￥"));

    CHECK(sys_string_builder().append(u"￥").build() == S("￥"));
    CHECK(sys_string_builder().append((const char16_t *)u"￥").build() == S("￥"));
    CHECK(sys_string_builder().append(u"￥", 1).build() == S(""));
    CHECK(sys_string_builder().append(u"￥"sv).build() == S("￥"));

    CHECK(sys_string_builder().append(U"￥").build() == S("￥"));
    CHECK(sys_string_builder().append((const char32_t *)U"￥").build() == S("￥"));
    CHECK(sys_string_builder().append(U"￥", 2).build() == S(""));
    CHECK(sys_string_builder().append(U"￥"sv).build() == S("￥"));

    CHECK(sys_string_builder().append(S("�𐊃𐜃𐤈🀁")).build() == S("�𐊃𐜃𐤈🀁"));
    
    sys_string_builder builder;
    builder.push_back(U'再');
    CHECK(builder.build() == S("再"));
    
    builder.clear();
    CHECK(builder.build() == S(""));
    
    builder.push_back(U'🪀');
    builder.push_back(U'🩳');
    builder.pop_back();
    CHECK(builder.build() == S("🪀"));

    builder.clear();
    std::u32string expected;
    for(int i = 0; i < 4096; ++i)
    {
        builder.append('a');
        expected += U'a';
    }
    CHECK(builder.build() == sys_string(expected));

    sys_string_builder builder1;
    for(int i = 0; i < 4096; ++i)
    {
        builder.append('a');
        builder1.append('a');
    }
    builder = std::move(builder1);
    CHECK(builder1.build() == sys_string());
    CHECK(builder.build() == sys_string(expected));

    {
        sys_string_builder unused;
        for(int i = 0; i < 4096; ++i)
            builder.append('a');
    }
    {
        sys_string_builder unused;
        builder.append('a');
    }
}

TEST_CASE( "Builder iteration" ) {
    
    sys_string_builder builder;
    CHECK(builder.begin() == builder.end());
    CHECK(builder.rbegin() == builder.rend());
    
    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");
    
    CHECK(*builder.begin() == U'🪀');
    CHECK(*builder.rbegin() == U'🀁');
    CHECK(std::ranges::distance(builder.begin(), builder.end()) == 7);
    CHECK(std::ranges::distance(builder.rbegin(), builder.rend()) == 7);

    auto last = builder.begin();
    while(last != builder.end())
        ++last;

    auto rlast = builder.rbegin();
    while(rlast != builder.rend())
        ++rlast;
    
    CHECK(builder.reverse(builder.begin()) == rlast);
    CHECK(builder.reverse(last) == builder.rbegin());
    CHECK(builder.reverse(builder.rbegin()) == last);
    CHECK(builder.reverse(rlast) == builder.begin());
    CHECK(builder.begin() == builder.cbegin());
    //CHECK(builder.end() == builder.cend());  sentinels cannot be compared with each other
    CHECK(builder.rbegin() == builder.crbegin());
    //CHECK(builder.rend() == builder.crend()); sentinels cannot be compared with each other

}

TEST_CASE( "Builder UTF iteration" ) {
    sys_string_builder builder;
    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");

    {
        static_assert(std::is_same_v<decltype(as_utf8(builder)), decltype((sys_string_builder::utf8_view(builder)))>);

        std::string buf1;
        std::ranges::copy(as_utf8(builder), std::back_inserter(buf1));

        std::string buf2;
        std::ranges::copy(sys_string_builder::utf8_view(builder), std::back_inserter(buf2));

        CHECK(buf1 == buf2);
        CHECK(buf1 == (const char *)u8"🪀🩳�𐊃𐜃𐤈🀁");
    }
    {
        static_assert(std::is_same_v<decltype(as_utf16(builder)), decltype((sys_string_builder::utf16_view(builder)))>);

        std::u16string buf1;
        std::ranges::copy(as_utf16(builder), std::back_inserter(buf1));

        std::u16string buf2;
        std::ranges::copy(sys_string_builder::utf16_view(builder), std::back_inserter(buf2));

        CHECK(buf1 == buf2);
        CHECK(buf1 == u"🪀🩳�𐊃𐜃𐤈🀁");
    }
    {
        static_assert(std::is_same_v<decltype(as_utf32(builder)), decltype((sys_string_builder::utf32_view(builder)))>);

        std::u32string buf1;
        std::ranges::copy(as_utf32(builder), std::back_inserter(buf1));

        std::u32string buf2;
        std::ranges::copy(sys_string_builder::utf32_view(builder), std::back_inserter(buf2));

        CHECK(buf1 == buf2);
        CHECK(buf1 == U"🪀🩳�𐊃𐜃𐤈🀁");
    }
}

TEST_CASE( "Builder insert" ) {
    
    sys_string_builder builder;
    sys_string_builder::iterator it;
    
    it = builder.insert(builder.begin(), U'🟫');
    CHECK(it == builder.begin());
    CHECK(builder.build() == S("🟫"));
    it = builder.insert(builder.begin(), "🟫", std::size("🟫") - 1);
    CHECK(it == builder.begin());
    CHECK(*it == U'🟫');
    CHECK(builder.build() == S("🟫"));
    CHECK(builder.empty());

    it = builder.insert(builder.begin(), "🔺");
    CHECK(it == builder.begin());
    CHECK(builder.build() == S("🔺"));
    CHECK(builder.empty());

    it = builder.insert(builder.begin(), "🔺"s);
    CHECK(it == builder.begin());
    CHECK(builder.build() == S("🔺"));
    CHECK(builder.empty());
    
    it = builder.insert(builder.end(), U'🟦');
    CHECK(it == builder.begin());
    CHECK(builder.build() == S("🟦"));
    it = builder.insert(builder.end(), u"🟦", std::size(u"🟦") - 1);
    CHECK(it == builder.begin());
    CHECK(*it == U'🟦');
    CHECK(builder.build() == S("🟦"));

    it = builder.insert(builder.end(), "🔺");
    CHECK(it == builder.begin());
    CHECK(builder.build() == S("🔺"));
    CHECK(builder.empty());

    it = builder.insert(builder.end(), "🔺"s);
    CHECK(it == builder.begin());
    CHECK(builder.build() == S("🔺"));
    CHECK(builder.empty());
    
    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");
    it = builder.insert(builder.begin(), U'🟫');
    CHECK(it == builder.begin());
    CHECK(builder.build() == S("🟫🪀🩳�𐊃𐜃𐤈🀁"));
    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");
    it = builder.insert(builder.begin(), U"🟫", std::size(U"🟫") - 1);
    CHECK(it == builder.begin());
    CHECK(builder.build() == S("🟫🪀🩳�𐊃𐜃𐤈🀁"));
    
    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");
    it = builder.insert(builder.end(), U'🟦');
    CHECK(++it == builder.end());
    CHECK(builder.build() == S("🪀🩳�𐊃𐜃𐤈🀁🟦"));
    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");
    it = builder.insert(builder.end(), u8"🟦", std::size(u8"🟦") - 1);
    CHECK(++it == builder.end());
    CHECK(builder.build() == S("🪀🩳�𐊃𐜃𐤈🀁🟦"));

    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");
    it = builder.insert(++builder.begin(), U'🟫');
    CHECK(it == ++builder.begin());
    CHECK(builder.build() == S("🪀🟫🩳�𐊃𐜃𐤈🀁"));
    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");
    it = builder.insert(++builder.begin(), u"🟫", std::size(u"🟫") - 1);
    CHECK(it == ++builder.begin());
    CHECK(builder.build() == S("🪀🟫🩳�𐊃𐜃𐤈🀁"));

    builder.append(std::array{U'🪀', U'🩳'});
    CHECK(builder.build() == S("🪀🩳"));

    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");
    it = builder.insert(++builder.begin(), std::list<char32_t>{U'🟫'});
    CHECK(it == ++builder.begin());
    CHECK(builder.build() == S("🪀🟫🩳�𐊃𐜃𐤈🀁"));

}

TEST_CASE( "Builder erase" ) {
    
    sys_string_builder builder;
    sys_string_builder::iterator it;
    
    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");
    it = builder.erase(builder.begin());
    CHECK(it == builder.begin());
    CHECK(builder.build() == S("🩳�𐊃𐜃𐤈🀁"));
    
    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");
    it = builder.erase(builder.reverse(++builder.rbegin()));
    CHECK(it == builder.end());
    CHECK(builder.build() == S("🪀🩳�𐊃𐜃𐤈"));
    
    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");
    it = builder.erase(++builder.begin());
    CHECK(it == ++builder.begin());
    CHECK(builder.build() == S("🪀�𐊃𐜃𐤈🀁"));
    
    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");
    it = builder.erase(builder.begin(), builder.end());
    CHECK(it == builder.end());
    CHECK(builder.build() == S(""));
    
    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");
    it = builder.erase(++builder.begin(), ++(++builder.begin()));
    CHECK(it == ++builder.begin());
    CHECK(builder.build() == S("🪀�𐊃𐜃𐤈🀁"));
    
}

}
