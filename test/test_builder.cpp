//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#include <sys_string/sys_string.h>


#include <doctest/doctest.h>

using namespace sysstr;

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

    CHECK(sys_string_builder().build() == S(""));
    CHECK(sys_string_builder().append(U'🟣').build() == S("🟣"));
    CHECK(sys_string_builder().append("￥").build() == S("￥"));
    CHECK(sys_string_builder().append("￥", 6).build() == S(""));
    CHECK(sys_string_builder().append(u8"￥").build() == S("￥"));
    CHECK(sys_string_builder().append(u8"￥", 3).build() == S(""));
    CHECK(sys_string_builder().append(u"￥").build() == S("￥"));
    CHECK(sys_string_builder().append(u"￥", 1).build() == S(""));
    CHECK(sys_string_builder().append(U"￥").build() == S("￥"));
    CHECK(sys_string_builder().append(U"￥", 2).build() == S(""));
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
    //CHECK(builder.end() == builder.cend());
    CHECK(builder.rbegin() == builder.crbegin());
    //CHECK(builder.rend() == builder.crend());

}

TEST_CASE( "Builder insert" ) {
    
    sys_string_builder builder;
    sys_string_builder::iterator it;
    
    it = builder.insert(builder.begin(), U'🟫');
    CHECK(it == builder.begin());
    CHECK(builder.build() == S("🟫"));
    it = builder.insert(builder.begin(), "🟫", std::size("🟫") - 1);
    CHECK(it == builder.begin());
    CHECK(builder.build() == S("🟫"));
    CHECK(builder.empty());
    
    it = builder.insert(builder.end(), U'🟦');
    CHECK(it == builder.begin());
    CHECK(builder.build() == S("🟦"));
    it = builder.insert(builder.end(), u"🟦", std::size(u"🟦") - 1);
    CHECK(it == builder.begin());
    CHECK(builder.build() == S("🟦"));
    
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
