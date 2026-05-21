//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#include <sys_string/impl/unicode/utf_util.h>

#include <string>
#include <cstring>

#include <doctest/doctest.h>

using namespace sysstr;

//Adapted from https://www.w3.org/2001/06/utf-8-wrong/UTF-8-test.html


namespace
{
    #if __cpp_char8_t
        using u8string = std::u8string;
    #else
        using u8string = std::string;
    #endif

    std::u32string utf32_from_utf16(const char16_t * str)
    {
        std::u32string ret;
        utf_converter<utf16, utf32>::convert(str, str + std::char_traits<char16_t>::length(str), std::back_inserter(ret));
        return ret;
    }

    u8string utf8_from_utf16(const char16_t * str)
    {
        u8string ret;
        utf_converter<utf16, utf8>::convert(str, str + std::char_traits<char16_t>::length(str), std::back_inserter(ret));
        return ret;
    }

    std::u16string utf16_from_utf8(const char * str)
    {
        std::u16string ret;
        utf_converter<utf8, utf16>::convert(str, str + strlen(str), std::back_inserter(ret));
        return ret;
    }
}

TEST_SUITE_BEGIN( "utf_util" );

TEST_CASE( "UTF-16 to UTF-32" ) {

    CHECK(utf32_from_utf16(u"a水𐀀𝄞bcå®®") == U"a水𐀀𝄞bcå®®");
    CHECK(utf32_from_utf16(u"\xD800") == U"�");
    CHECK(utf32_from_utf16(u"\xDBFF") == U"�");
    CHECK(utf32_from_utf16(u"\xDC00") == U"�");
    CHECK(utf32_from_utf16(u"\xDC00") == U"�");
    CHECK(utf32_from_utf16(u"\xDFFF") == U"�");
    CHECK(utf32_from_utf16(u"\xD800\xDBFF") == U"��");
    CHECK(utf32_from_utf16(u"\xD800\x0061") == U"�a");
    CHECK(utf32_from_utf16(u"\xDBFF\xDBFF") == U"��");
    CHECK(utf32_from_utf16(u"\xDBFF\x0061") == U"�a");
    CHECK(utf32_from_utf16(u"\xD800\xDC00") == U"𐀀");
    CHECK(utf32_from_utf16(u"\xD800\xDFFF") == U"\U000103FF");
    CHECK(utf32_from_utf16(u"\xDBFF\xDC00") == U"\U0010FC00");
    CHECK(utf32_from_utf16(u"\xDBFF\xDFFF") == U"\U0010FFFF");
}

TEST_CASE( "UTF-16 to UTF-8" ) {
    CHECK(utf8_from_utf16(u"a水𐀀𝄞bcå®®") == u8"a水𐀀𝄞bcå®®");
    CHECK(utf8_from_utf16(u"\xD800") == u8"�");
    CHECK(utf8_from_utf16(u"\xDBFF") == u8"�");
    CHECK(utf8_from_utf16(u"\xDC00") == u8"�");
    CHECK(utf8_from_utf16(u"\xDC00") == u8"�");
    CHECK(utf8_from_utf16(u"\xDFFF") == u8"�");
    CHECK(utf8_from_utf16(u"\xD800\xDBFF") == u8"��");
    CHECK(utf8_from_utf16(u"\xD800\x0061") == u8"�a");
    CHECK(utf8_from_utf16(u"\xDBFF\xDBFF") == u8"��");
    CHECK(utf8_from_utf16(u"\xDBFF\x0061") == u8"�a");
    CHECK(utf8_from_utf16(u"\xD800\xDC00") == u8"𐀀");
    CHECK(utf8_from_utf16(u"\xD800\xDFFF") == u8"\U000103FF");
    CHECK(utf8_from_utf16(u"\xDBFF\xDC00") == u8"\U0010FC00");
    CHECK(utf8_from_utf16(u"\xDBFF\xDFFF") == u8"\U0010FFFF");
}

TEST_CASE( "UTF-8 to UTF-16" ) {

    //1  Some correct UTF-8 text
    CHECK(utf16_from_utf8("a水𐀀𝄞bcå®®") == u"a水𐀀𝄞bcå®®");
    
    //2  Boundary condition test cases
    
    //2.1  First possible sequence of a certain length
    CHECK(utf16_from_utf8("\xC2\x80") == u"\u0080");
    CHECK(utf16_from_utf8("\xE0\xA0\x80") == u"ࠀ");
    CHECK(utf16_from_utf8("\xF0\x90\x80\x80") == u"𐀀");
    CHECK(utf16_from_utf8("\xF8\x88\x80\x80\x80") == u"�����");
    CHECK(utf16_from_utf8("\xFC\x84\x80\x80\x80\x80") == u"������");
    
    //2.2  Last possible sequence of a certain length
    CHECK(utf16_from_utf8("\x7F") == u"\u007F");
    CHECK(utf16_from_utf8("\xDF\xBF") == u"\u07FF");
    CHECK(utf16_from_utf8("\xEF\xBF\xBF") == u"\uFFFF");
    CHECK(utf16_from_utf8("\xF7\xBF\xBF\xBF") == u"����");
    CHECK(utf16_from_utf8("\xFB\xBF\xBF\xBF\xBF") == u"�����");
    CHECK(utf16_from_utf8("\xFD\xBF\xBF\xBF\xBF\xBF") == u"������");
    
    //2.3  Other boundary conditions
    CHECK(utf16_from_utf8("\xED\x9F\xBF") == u"\uD7FF");
    CHECK(utf16_from_utf8("\xEE\x80\x80") == u"\uE000");
    CHECK(utf16_from_utf8("\xEF\xBF\xBD") == u"�");
    CHECK(utf16_from_utf8("\xF4\x8F\xBF\xBF") == u"\U0010FFFF");
    CHECK(utf16_from_utf8("\xF4\x90\x80\x80") == u"����");
    
    //3  Malformed sequences
    
    //3.1  Unexpected continuation bytes
    CHECK(utf16_from_utf8("\x80") == u"�");
    CHECK(utf16_from_utf8("\xBF") == u"�");
    CHECK(utf16_from_utf8("\x80\xBF") == u"��");
    CHECK(utf16_from_utf8("\x80\xBF\x80") == u"���");
    CHECK(utf16_from_utf8("\x80\xBF\x80\xBF") == u"����");
    CHECK(utf16_from_utf8("\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F"
                          "\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
                          "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF"
                          "\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF") 
                          ==
                          u"����������������"
                          u"����������������"
                          u"����������������"
                          u"����������������");
    
    //3.2  Lonely start characters
    CHECK(utf16_from_utf8("\xC0 \xC1 \xC2 \xC3 \xC4 \xC5 \xC6 \xC7 \xC8 \xC9 \xCA \xCB \xCC \xCD \xCE \xCF "
                          "\xD0 \xD1 \xD2 \xD3 \xD4 \xD5 \xD6 \xD7 \xD8 \xD9 \xDA \xDB \xDC \xDD \xDE \xDF ") 
                          ==
                          u"� � � � � � � � � � � � � � � � "
                          u"� � � � � � � � � � � � � � � � ");
    CHECK(utf16_from_utf8("\xE0 \xE1 \xE2 \xE3 \xE4 \xE5 \xE6 \xE7 \xE8 \xE9 \xEA \xEB \xEC \xED \xEE \xEF ") 
                          ==
                          u"� � � � � � � � � � � � � � � � ");
    CHECK(utf16_from_utf8("\xF0 \xF1 \xF2 \xF3 \xF4 \xF5 \xF6 \xF7 ") == u"� � � � � � � � ");
    CHECK(utf16_from_utf8("\xF8 \xF9 \xFA \xFB ") == u"� � � � ");
    CHECK(utf16_from_utf8("\xFC \xFD ") == u"� � ");
    
    //3.3  Sequences with last continuation byte missing
    CHECK(utf16_from_utf8("\xC0") == u"�");
    CHECK(utf16_from_utf8("\xE0\x80") == u"��");
    CHECK(utf16_from_utf8("\xF0\x80\x80") == u"���");
    CHECK(utf16_from_utf8("\xDF") == u"�");
    CHECK(utf16_from_utf8("\xEF\xBF") == u"�"); 
    CHECK(utf16_from_utf8("\xF7\xBF\xBF") == u"���");
    
    //3.4  Concatenation of incomplete sequences
    CHECK(utf16_from_utf8("\xC0\xE0\x80\xF0\x80\x80\xDF\xEF\xBF\xF7\xBF\xBF") == u"�����������");
    
    //3.5  Impossible bytes
    CHECK(utf16_from_utf8("\xFE") == u"�");
    CHECK(utf16_from_utf8("\xFF") == u"�");
    CHECK(utf16_from_utf8("\xFE\xFE\xFF\xFF") == u"����");
    
    //4  Overlong sequences
    
    //4.1  Examples of an overlong ASCII character
    CHECK(utf16_from_utf8("\xC0\xAF") == u"��");
    CHECK(utf16_from_utf8("\xE0\x80\xAF") == u"���");
    CHECK(utf16_from_utf8("\xF0\x80\x80\xAF") == u"����");
    
    //4.2  Maximum overlong sequences
    CHECK(utf16_from_utf8("\xC1\xBF") == u"��");
    CHECK(utf16_from_utf8("\xE0\x9F\xBF") == u"���");
    CHECK(utf16_from_utf8("\xF0\x8F\xBF\xBF") == u"����");
    
    //4.3  Overlong representation of the NUL character
    CHECK(utf16_from_utf8("\xC0\x80") == u"��");
    CHECK(utf16_from_utf8("\xE0\x80\x80") == u"���");
    CHECK(utf16_from_utf8("\xF0\x80\x80\x80") == u"����");
    
    //5  Illegal code positions
    
    //5.1 Single UTF-16 surrogates
    CHECK(utf16_from_utf8("\xED\xA0\x80") == u"���");
    CHECK(utf16_from_utf8("\xED\xAD\xBF") == u"���");
    CHECK(utf16_from_utf8("\xED\xAE\x80") == u"���");
    CHECK(utf16_from_utf8("\xED\xAF\xBF") == u"���");
    CHECK(utf16_from_utf8("\xED\xB0\x80") == u"���");
    CHECK(utf16_from_utf8("\xED\xBE\x80") == u"���");
    CHECK(utf16_from_utf8("\xED\xBF\xBF") == u"���");
    
    //5.2 Paired UTF-16 surrogates
    CHECK(utf16_from_utf8("\xED\xA0\x80\xED\xB0\x80") == u"������");
    CHECK(utf16_from_utf8("\xED\xA0\x80\xED\xBF\xBF") == u"������");
    CHECK(utf16_from_utf8("\xED\xAD\xBF\xED\xB0\x80") == u"������");
    CHECK(utf16_from_utf8("\xED\xAD\xBF\xED\xBF\xBF") == u"������");
    CHECK(utf16_from_utf8("\xED\xAE\x80\xED\xB0\x80") == u"������");
    CHECK(utf16_from_utf8("\xED\xAE\x80\xED\xBF\xBF") == u"������");
    CHECK(utf16_from_utf8("\xED\xAF\xBF\xED\xB0\x80") == u"������");
    CHECK(utf16_from_utf8("\xED\xAF\xBF\xED\xBF\xBF") == u"������");
    
    //5.3 Other illegal code positions
    CHECK(utf16_from_utf8("\xEF\xBF\xBE") == u"\uFFFE");
    CHECK(utf16_from_utf8("\xEF\xBF\xBF") == u"\uFFFF");
    
    //From https://www.unicode.org/versions/Unicode10.0.0/ch03.pdf
    CHECK(utf16_from_utf8("\x61\xF1\x80\x80\xE1\x80\xC2\x62\x80\x63\x80\xBF\x64") == u"a���b�c��d");
}

TEST_SUITE_END;