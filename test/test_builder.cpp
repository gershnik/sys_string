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

TEST_CASE( "Builder basics", "[builder]") {

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

TEST_CASE( "Builder iteration", "[builder]") {
    
    sys_string_builder builder;
    CHECK(builder.begin() == builder.end());
    CHECK(builder.rbegin() == builder.rend());
    
    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");
    
    CHECK(*builder.begin() == U'🪀');
    CHECK(*builder.rbegin() == U'🀁');
    CHECK(std::distance(builder.begin(), builder.end()) == 7);
    CHECK(std::distance(builder.rbegin(), builder.rend()) == 7);
    
    CHECK(builder.begin().reverse() == builder.rend());
    CHECK(builder.end().reverse() == builder.rbegin());
    CHECK(builder.rbegin().reverse() == builder.end());
    CHECK(builder.rend().reverse() == builder.begin());
    CHECK(builder.begin() == builder.cbegin());
    CHECK(builder.end() == builder.cend());
    CHECK(builder.rbegin() == builder.crbegin());
    CHECK(builder.rend() == builder.crend());

}

TEST_CASE( "Builder insert", "[builder]") {
    
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

TEST_CASE( "Builder erase", "[builder]") {
    
    sys_string_builder builder;
    sys_string_builder::iterator it;
    
    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");
    it = builder.erase(builder.begin());
    CHECK(it == builder.begin());
    CHECK(builder.build() == S("🩳�𐊃𐜃𐤈🀁"));
    
    builder.append(u"🪀🩳�𐊃𐜃𐤈🀁");
    it = builder.erase((++builder.rbegin()).reverse());
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
