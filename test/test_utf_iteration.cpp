//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
// _🪦🐮🐌_

#include <sys_string/utf_view.h>


#include <doctest/doctest.h>

#include <vector>
#include <string>
#include <string_view>
#include <ostream>

using namespace sysstr;
using namespace std::literals;

#ifdef __clang__
//Workaround for clang bug on Linux
//See https://github.com/llvm/llvm-project/issues/55560
[[maybe_unused]] static std::u16string clang_string_workaround(const char16_t* a, const char16_t* b) { return {a, b}; }
[[maybe_unused]] static std::u32string clang_string_workaround(const char32_t* a, const char32_t* b) { return {a, b}; }
#endif

static_assert(std::ranges::forward_range<utf_view<utf8, std::vector<char>>>);
static_assert(!std::ranges::bidirectional_range<utf_view<utf8, std::vector<char>>>);
static_assert(!std::ranges::common_range<utf_view<utf8, std::vector<char>>>);
static_assert(std::ranges::view<utf_view<utf8, std::vector<char>>>);
static_assert(std::ranges::borrowed_range<utf_view<utf8, std::vector<char>>>);
static_assert(std::ranges::viewable_range<utf_view<utf8, std::vector<char>>>);
static_assert(ranges::custom_reverse_traversable_range<utf_view<utf8, std::vector<char>>>);

static_assert(std::ranges::forward_range<utf_view<utf16, std::vector<char>>>);
static_assert(!std::ranges::bidirectional_range<utf_view<utf16, std::vector<char>>>);
static_assert(!std::ranges::common_range<utf_view<utf16, std::vector<char>>>);
static_assert(std::ranges::view<utf_view<utf16, std::vector<char>>>);
static_assert(std::ranges::borrowed_range<utf_view<utf16, std::vector<char>>>);
static_assert(std::ranges::viewable_range<utf_view<utf16, std::vector<char>>>);
static_assert(ranges::custom_reverse_traversable_range<utf_view<utf16, std::vector<char>>>);

static_assert(std::ranges::forward_range<utf_view<utf32, std::vector<char>>>);
static_assert(!std::ranges::bidirectional_range<utf_view<utf32, std::vector<char>>>);
static_assert(!std::ranges::common_range<utf_view<utf32, std::vector<char>>>);
static_assert(std::ranges::view<utf_view<utf32, std::vector<char>>>);
static_assert(std::ranges::borrowed_range<utf_view<utf32, std::vector<char>>>);
static_assert(std::ranges::viewable_range<utf_view<utf32, std::vector<char>>>);
static_assert(ranges::custom_reverse_traversable_range<utf_view<utf32, std::vector<char>>>);

namespace
{
    const std::string_view & printable(const std::string_view & str)
    {
        return str;
    }


    std::string printable(const std::u8string_view & str)
    {
        return std::string((const char *)str.data(), str.size());
    }


    std::string printable(const std::u16string_view & str)
    {
        std::string ret;
        utf_converter<utf16, utf8>::convert(str, std::back_inserter(ret));
        return ret;
    }

    std::string printable(const std::u32string_view & str)
    {
        std::string ret;
        utf_converter<utf32, utf8>::convert(str, std::back_inserter(ret));
        return ret;
    }
}

template<class Cont>
std::basic_string<std::ranges::range_value_t<Cont>> reverse(const Cont & str)
{
    return std::basic_string<std::ranges::range_value_t<Cont>>(std::crbegin(str), std::crend(str));
}

template<class View, class It, class EndIt>
void check_reversibility(const View & view, It first, EndIt last)
{
    auto rlast = view.reverse(first);
    for(auto it = first; it != last; ++it) {
        std::basic_string<decltype(*it)> forward_reversed;
        std::copy(first, it, std::back_inserter(forward_reversed));
        forward_reversed = reverse(forward_reversed);
        std::basic_string<decltype(*it)> reversed;
        std::copy(view.reverse(it), rlast, std::back_inserter(reversed));
        CHECK(forward_reversed == reversed);
    }
}


template<utf_encoding SrcEnc, utf_encoding DstEnc,
         std::ranges::input_range SrcCont,
         std::ranges::forward_range ExpectedCont,
         std::ranges::forward_range ExpectedBackwardCont>
requires(utf_encoding_of<std::ranges::range_value_t<SrcCont>> == SrcEnc &&
         utf_encoding_of<std::ranges::range_value_t<ExpectedCont>> == DstEnc &&
         std::is_same_v<std::ranges::range_value_t<ExpectedCont>, std::ranges::range_value_t<ExpectedBackwardCont>>)
void do_check_iteration(const SrcCont & src,
                        const ExpectedCont & expected,
                        const ExpectedBackwardCont & expected_backward,
                        bool check_reverse)
{
    INFO( "From: utf" << SrcEnc << " to: utf" << DstEnc << " src: `" << printable(src) << '`');

    using view_type = utf_view<DstEnc, const SrcCont>;
    
    view_type view(src);
    
    std::basic_string<typename view_type::value_type> expected_str(std::begin(expected), std::end(expected));
    std::basic_string<typename view_type::value_type> expected_backward_str(std::begin(expected_backward), std::end(expected_backward));
    std::basic_string<typename view_type::value_type> dest;
    std::ranges::copy(view.begin(), view.end(), std::back_inserter(dest));
    CHECK(dest == expected_str);
    dest.clear();
    std::ranges::copy(view.cbegin(), view.cend(), std::back_inserter(dest));
    CHECK(dest == expected_str);
    dest.clear();
    std::ranges::copy(view.rbegin(), view.rend(), std::back_inserter(dest));
    CHECK(dest == expected_backward_str);
    dest.clear();
    std::ranges::copy(view.crbegin(), view.crend(), std::back_inserter(dest));
    CHECK(dest == expected_backward_str);

    auto last = view.begin();
    while(last != view.end())
        ++last;

    auto clast = view.cbegin();
    while(clast != view.cend())
        ++clast;
    
    auto rlast = view.rbegin();
    while(rlast != view.rend())
        ++rlast;

    auto crlast = view.crbegin();
    while(crlast != view.crend())
        ++crlast;
      
    CHECK(view.reverse(view.begin()) == rlast);
    CHECK(view.reverse(view.rbegin()) == last);
    CHECK(view.reverse(last) == view.rbegin());
    CHECK(view.reverse(rlast) == view.begin());

    CHECK(view.reverse(view.cbegin()) == crlast);
    CHECK(view.reverse(view.crbegin()) == clast);
    CHECK(view.reverse(clast) == view.crbegin());
    CHECK(view.reverse(crlast) == view.cbegin());
    
    if (check_reverse)
    {
        check_reversibility(view, view.begin(), view.end());
        check_reversibility(view, view.rbegin(), view.rend());
    }
}

template<utf_encoding SrcEnc, utf_encoding DstEnc,
         std::ranges::input_range SrcCont,
         std::ranges::bidirectional_range ExpectedCont>
requires(utf_encoding_of<std::ranges::range_value_t<SrcCont>> == SrcEnc &&
         utf_encoding_of<std::ranges::range_value_t<ExpectedCont>> == DstEnc)
void check_iteration(const SrcCont & src,
                     const ExpectedCont & expected)
{
    do_check_iteration<SrcEnc, DstEnc>(src, expected, reverse(expected), true);
}

template<utf_encoding SrcEnc, utf_encoding DstEnc,
         std::ranges::input_range SrcCont,
         std::ranges::forward_range ExpectedCont,
         std::ranges::bidirectional_range ExpectedBackwardCont>
requires(utf_encoding_of<std::ranges::range_value_t<SrcCont>> == SrcEnc &&
         utf_encoding_of<std::ranges::range_value_t<ExpectedCont>> == DstEnc &&
         std::is_same_v<std::ranges::range_value_t<ExpectedCont>, std::ranges::range_value_t<ExpectedBackwardCont>>)
void check_iteration(const SrcCont & src,
                     const ExpectedCont & expected,
                     const ExpectedBackwardCont & expected_backward)
{
    do_check_iteration<SrcEnc, DstEnc>(src, expected, reverse(expected_backward), false);
}

TEST_SUITE("utf_iteration") {

TEST_CASE( "UTF8 Iteration on UTF8 sequence" ) {

    constexpr auto from = utf8;
    constexpr auto to = utf8;
    
    check_iteration<from, to>("", "");
    check_iteration<from, to>("a", "a");
    check_iteration<from, to>(u8"¢", "¢"); //2-byte
    check_iteration<from, to>("ह"s, "ह"sv); //3-byte
    check_iteration<from, to>("𐍈"sv, "𐍈"s); //4-byte
    check_iteration<from, to>("🤢"sv, "🤢"s); //out of bmp in surrogate range
    
    //1  Some correct UTF-8 text
    check_iteration<from, to>("a水𐀀𝄞bcå®®", "a水𐀀𝄞bcå®®");
    
    //2  Boundary condition test cases
    
    //2.1  First possible sequence of a certain length
    check_iteration<from, to>("\xC2\x80", "\u0080");
    check_iteration<from, to>("\xE0\xA0\x80", "ࠀ");
    check_iteration<from, to>("\xF0\x90\x80\x80", "𐀀");
    check_iteration<from, to>("\xF8\x88\x80\x80\x80", "�����");
    check_iteration<from, to>("\xFC\x84\x80\x80\x80\x80", "������");
    
    //2.2  Last possible sequence of a certain length
    check_iteration<from, to>("\x7F", "\u007F");
    check_iteration<from, to>("\xDF\xBF", "\u07FF");
    check_iteration<from, to>("\xEF\xBF\xBF", "\uFFFF");
    check_iteration<from, to>("\xF7\xBF\xBF\xBF", "����");
    check_iteration<from, to>("\xFB\xBF\xBF\xBF\xBF", "�����");
    check_iteration<from, to>("\xFD\xBF\xBF\xBF\xBF\xBF", "������");
    
    //2.3  Other boundary conditions
    check_iteration<from, to>("\xED\x9F\xBF", "\uD7FF");
    check_iteration<from, to>("\xEE\x80\x80", "\uE000");
    check_iteration<from, to>("\xEF\xBF\xBD", "�");
    check_iteration<from, to>("\xF4\x8F\xBF\xBF", "\U0010FFFF");
    check_iteration<from, to>("\xF4\x90\x80\x80", "����");
    
    //3  Malformed sequences
    
    //3.1  Unexpected continuation bytes
    check_iteration<from, to>("\x80", "�");
    check_iteration<from, to>("\xBF", "�");
    check_iteration<from, to>("\x80\xBF", "��", "�");
    check_iteration<from, to>("\x80\xBF\x80", "���", "�");
    check_iteration<from, to>("\x80\xBF\x80\xBF", "����", "��");
    check_iteration<from, to>("\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F"
                              "\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
                              "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF"
                              "\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF",
                              
                              "����������������"
                              "����������������"
                              "����������������"
                              "����������������",
                              
                              "����������������"
                              "����������������"
                              "����������������"
                              "��������������");
    
    //3.2  Lonely start characters
    check_iteration<from, to>("\xC0 \xC1 \xC2 \xC3 \xC4 \xC5 \xC6 \xC7 \xC8 \xC9 \xCA \xCB \xCC \xCD \xCE \xCF "
                              "\xD0 \xD1 \xD2 \xD3 \xD4 \xD5 \xD6 \xD7 \xD8 \xD9 \xDA \xDB \xDC \xDD \xDE \xDF ",
                              
                              "� � � � � � � � � � � � � � � � "
                              "� � � � � � � � � � � � � � � � ");
    check_iteration<from, to>("\xE0 \xE1 \xE2 \xE3 \xE4 \xE5 \xE6 \xE7 \xE8 \xE9 \xEA \xEB \xEC \xED \xEE \xEF ",
                              
                              "� � � � � � � � � � � � � � � � ");
    check_iteration<from, to>("\xF0 \xF1 \xF2 \xF3 \xF4 \xF5 \xF6 \xF7 ", "� � � � � � � � ");
    check_iteration<from, to>("\xF8 \xF9 \xFA \xFB ", "� � � � ");
    check_iteration<from, to>("\xFC \xFD ", "� � ");
    
    //3.3  Sequences with last continuation byte missing
    check_iteration<from, to>("\xC0", "�");
    check_iteration<from, to>("\xE0\x80", "��");
    check_iteration<from, to>("\xF0\x80\x80", "���");
    check_iteration<from, to>("\xDF", "�");
    check_iteration<from, to>("\xEF\xBF", "�", "��");
    check_iteration<from, to>("\xF7\xBF\xBF", "���");
    
    //3.4  Concatenation of incomplete sequences
    check_iteration<from, to>("\xC0\xE0\x80\xF0\x80\x80\xDF\xEF\xBF\xF7\xBF\xBF", "�����������", "������������");
    
    //3.5  Impossible bytes
    check_iteration<from, to>("\xFE", "�");
    check_iteration<from, to>("\xFF", "�");
    check_iteration<from, to>("\xFE\xFE\xFF\xFF", "����");
    
    //4  Overlong sequences
    
    //4.1  Examples of an overlong ASCII character
    check_iteration<from, to>("\xC0\xAF", "��");
    check_iteration<from, to>("\xE0\x80\xAF", "���");
    check_iteration<from, to>("\xF0\x80\x80\xAF", "����");
    
    //4.2  Maximum overlong sequences
    check_iteration<from, to>("\xC1\xBF", "��");
    check_iteration<from, to>("\xE0\x9F\xBF", "���");
    check_iteration<from, to>("\xF0\x8F\xBF\xBF", "����");
    
    //4.3  Overlong representation of the NUL character
    check_iteration<from, to>("\xC0\x80", "��");
    check_iteration<from, to>("\xE0\x80\x80", "���");
    check_iteration<from, to>("\xF0\x80\x80\x80", "����");
    
    //5  Illegal code positions
    
    //5.1 Single UTF-16 surrogates
    check_iteration<from, to>("\xED\xA0\x80", "���");
    check_iteration<from, to>("\xED\xAD\xBF", "���");
    check_iteration<from, to>("\xED\xAE\x80", "���");
    check_iteration<from, to>("\xED\xAF\xBF", "���");
    check_iteration<from, to>("\xED\xB0\x80", "���");
    check_iteration<from, to>("\xED\xBE\x80", "���");
    check_iteration<from, to>("\xED\xBF\xBF", "���");
    
    //5.2 Paired UTF-16 surrogates
    check_iteration<from, to>("\xED\xA0\x80\xED\xB0\x80", "������");
    check_iteration<from, to>("\xED\xA0\x80\xED\xBF\xBF", "������");
    check_iteration<from, to>("\xED\xAD\xBF\xED\xB0\x80", "������");
    check_iteration<from, to>("\xED\xAD\xBF\xED\xBF\xBF", "������");
    check_iteration<from, to>("\xED\xAE\x80\xED\xB0\x80", "������");
    check_iteration<from, to>("\xED\xAE\x80\xED\xBF\xBF", "������");
    check_iteration<from, to>("\xED\xAF\xBF\xED\xB0\x80", "������");
    check_iteration<from, to>("\xED\xAF\xBF\xED\xBF\xBF", "������");
    
    //5.3 Other illegal code positions
    check_iteration<from, to>("\xEF\xBF\xBE", "\uFFFE");
    check_iteration<from, to>("\xEF\xBF\xBF", "\uFFFF");
}

TEST_CASE( "UTF16 Iteration on UTF8 sequence" ) {

    constexpr auto from = utf8;
    constexpr auto to = utf16;
    
    check_iteration<from, to>("", u"");
    check_iteration<from, to>("a", u"a");
    check_iteration<from, to>(u8"¢", u"¢"); //2-byte
    check_iteration<from, to>("ह"s, u"ह"sv); //3-byte
    check_iteration<from, to>("𐍈"sv, u"𐍈"s); //4-byte
    check_iteration<from, to>("🤢"sv, u"🤢"s); //out of bmp in surrogate range
    
    //1  Some correct UTF-8 text
    check_iteration<from, to>("a水𐀀𝄞bcå®®", u"a水𐀀𝄞bcå®®");
    
    //2  Boundary condition test cases
    
    //2.1  First possible sequence of a certain length
    check_iteration<from, to>("\xC2\x80", u"\u0080");
    check_iteration<from, to>("\xE0\xA0\x80", u"ࠀ");
    check_iteration<from, to>("\xF0\x90\x80\x80", u"𐀀");
    check_iteration<from, to>("\xF8\x88\x80\x80\x80", u"�����");
    check_iteration<from, to>("\xFC\x84\x80\x80\x80\x80", u"������");
    
    //2.2  Last possible sequence of a certain length
    check_iteration<from, to>("\x7F", u"\u007F");
    check_iteration<from, to>("\xDF\xBF", u"\u07FF");
    check_iteration<from, to>("\xEF\xBF\xBF", u"\uFFFF");
    check_iteration<from, to>("\xF7\xBF\xBF\xBF", u"����");
    check_iteration<from, to>("\xFB\xBF\xBF\xBF\xBF", u"�����");
    check_iteration<from, to>("\xFD\xBF\xBF\xBF\xBF\xBF", u"������");
    
    //2.3  Other boundary conditions
    check_iteration<from, to>("\xED\x9F\xBF", u"\uD7FF");
    check_iteration<from, to>("\xEE\x80\x80", u"\uE000");
    check_iteration<from, to>("\xEF\xBF\xBD", u"�");
    check_iteration<from, to>("\xF4\x8F\xBF\xBF", u"\U0010FFFF");
    check_iteration<from, to>("\xF4\x90\x80\x80", u"����");
    
    //3  Malformed sequences
    
    //3.1  Unexpected continuation bytes
    check_iteration<from, to>("\x80", u"�");
    check_iteration<from, to>("\xBF", u"�");
    check_iteration<from, to>("\x80\xBF", u"��", u"�");
    check_iteration<from, to>("\x80\xBF\x80", u"���", u"�");
    check_iteration<from, to>("\x80\xBF\x80\xBF", u"����", u"��");
    check_iteration<from, to>("\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F"
                              "\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
                              "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF"
                              "\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF",
                              
                              u"����������������"
                              u"����������������"
                              u"����������������"
                              u"����������������",
                              
                              u"����������������"
                              u"����������������"
                              u"����������������"
                              u"��������������");
    
    //3.2  Lonely start characters
    check_iteration<from, to>("\xC0 \xC1 \xC2 \xC3 \xC4 \xC5 \xC6 \xC7 \xC8 \xC9 \xCA \xCB \xCC \xCD \xCE \xCF "
                              "\xD0 \xD1 \xD2 \xD3 \xD4 \xD5 \xD6 \xD7 \xD8 \xD9 \xDA \xDB \xDC \xDD \xDE \xDF ",
                              
                              u"� � � � � � � � � � � � � � � � "
                              u"� � � � � � � � � � � � � � � � ");
    check_iteration<from, to>("\xE0 \xE1 \xE2 \xE3 \xE4 \xE5 \xE6 \xE7 \xE8 \xE9 \xEA \xEB \xEC \xED \xEE \xEF ",
                              
                              u"� � � � � � � � � � � � � � � � ");
    check_iteration<from, to>("\xF0 \xF1 \xF2 \xF3 \xF4 \xF5 \xF6 \xF7 ", u"� � � � � � � � ");
    check_iteration<from, to>("\xF8 \xF9 \xFA \xFB ", u"� � � � ");
    check_iteration<from, to>("\xFC \xFD ", u"� � ");
    
    //3.3  Sequences with last continuation byte missing
    check_iteration<from, to>("\xC0", u"�");
    check_iteration<from, to>("\xE0\x80", u"��");
    check_iteration<from, to>("\xF0\x80\x80", u"���");
    check_iteration<from, to>("\xDF", u"�");
    check_iteration<from, to>("\xEF\xBF", u"�", u"��");
    check_iteration<from, to>("\xF7\xBF\xBF", u"���");
    
    //3.4  Concatenation of incomplete sequences
    check_iteration<from, to>("\xC0\xE0\x80\xF0\x80\x80\xDF\xEF\xBF\xF7\xBF\xBF", u"�����������", u"������������");
    
    //3.5  Impossible bytes
    check_iteration<from, to>("\xFE", u"�");
    check_iteration<from, to>("\xFF", u"�");
    check_iteration<from, to>("\xFE\xFE\xFF\xFF", u"����");
    
    //4  Overlong sequences
    
    //4.1  Examples of an overlong ASCII character
    check_iteration<from, to>("\xC0\xAF", u"��");
    check_iteration<from, to>("\xE0\x80\xAF", u"���");
    check_iteration<from, to>("\xF0\x80\x80\xAF", u"����");
    
    //4.2  Maximum overlong sequences
    check_iteration<from, to>("\xC1\xBF", u"��");
    check_iteration<from, to>("\xE0\x9F\xBF", u"���");
    check_iteration<from, to>("\xF0\x8F\xBF\xBF", u"����");
    
    //4.3  Overlong representation of the NUL character
    check_iteration<from, to>("\xC0\x80", u"��");
    check_iteration<from, to>("\xE0\x80\x80", u"���");
    check_iteration<from, to>("\xF0\x80\x80\x80", u"����");
    
    //5  Illegal code positions
    
    //5.1 Single UTF-16 surrogates
    check_iteration<from, to>("\xED\xA0\x80", u"���");
    check_iteration<from, to>("\xED\xAD\xBF", u"���");
    check_iteration<from, to>("\xED\xAE\x80", u"���");
    check_iteration<from, to>("\xED\xAF\xBF", u"���");
    check_iteration<from, to>("\xED\xB0\x80", u"���");
    check_iteration<from, to>("\xED\xBE\x80", u"���");
    check_iteration<from, to>("\xED\xBF\xBF", u"���");
    
    //5.2 Paired UTF-16 surrogates
    check_iteration<from, to>("\xED\xA0\x80\xED\xB0\x80", u"������");
    check_iteration<from, to>("\xED\xA0\x80\xED\xBF\xBF", u"������");
    check_iteration<from, to>("\xED\xAD\xBF\xED\xB0\x80", u"������");
    check_iteration<from, to>("\xED\xAD\xBF\xED\xBF\xBF", u"������");
    check_iteration<from, to>("\xED\xAE\x80\xED\xB0\x80", u"������");
    check_iteration<from, to>("\xED\xAE\x80\xED\xBF\xBF", u"������");
    check_iteration<from, to>("\xED\xAF\xBF\xED\xB0\x80", u"������");
    check_iteration<from, to>("\xED\xAF\xBF\xED\xBF\xBF", u"������");
    
    //5.3 Other illegal code positions
    check_iteration<from, to>("\xEF\xBF\xBE", u"\uFFFE");
    check_iteration<from, to>("\xEF\xBF\xBF", u"\uFFFF");
}

TEST_CASE( "UTF32 Iteration on UTF8 sequence" ) {

    constexpr auto from = utf8;
    constexpr auto to = utf32;
    
    check_iteration<from, to>("", U"");
    check_iteration<from, to>("a", U"a");
    check_iteration<from, to>(u8"¢", U"¢"); //2-byte
    check_iteration<from, to>("ह"s, U"ह"sv); //3-byte
    check_iteration<from, to>("𐍈"sv, U"𐍈"s); //4-byte
    check_iteration<from, to>("🤢"sv, U"🤢"s); //out of bmp in surrogate range
    
    //1  Some correct UTF-8 text
    check_iteration<from, to>("a水𐀀𝄞bcå®®", U"a水𐀀𝄞bcå®®");
    
    //2  Boundary condition test cases
    
    //2.1  First possible sequence of a certain length
    check_iteration<from, to>("\xC2\x80", U"\u0080");
    check_iteration<from, to>("\xE0\xA0\x80", U"ࠀ");
    check_iteration<from, to>("\xF0\x90\x80\x80", U"𐀀");
    check_iteration<from, to>("\xF8\x88\x80\x80\x80", U"�����");
    check_iteration<from, to>("\xFC\x84\x80\x80\x80\x80", U"������");
    
    //2.2  Last possible sequence of a certain length
    check_iteration<from, to>("\x7F", U"\u007F");
    check_iteration<from, to>("\xDF\xBF", U"\u07FF");
    check_iteration<from, to>("\xEF\xBF\xBF", U"\uFFFF");
    check_iteration<from, to>("\xF7\xBF\xBF\xBF", U"����");
    check_iteration<from, to>("\xFB\xBF\xBF\xBF\xBF", U"�����");
    check_iteration<from, to>("\xFD\xBF\xBF\xBF\xBF\xBF", U"������");
    
    //2.3  Other boundary conditions
    check_iteration<from, to>("\xED\x9F\xBF", U"\uD7FF");
    check_iteration<from, to>("\xEE\x80\x80", U"\uE000");
    check_iteration<from, to>("\xEF\xBF\xBD", U"�");
    check_iteration<from, to>("\xF4\x8F\xBF\xBF", U"\U0010FFFF");
    check_iteration<from, to>("\xF4\x90\x80\x80", U"����");
    
    //3  Malformed sequences
    
    //3.1  Unexpected continuation bytes
    check_iteration<from, to>("\x80", U"�");
    check_iteration<from, to>("\xBF", U"�");
    check_iteration<from, to>("\x80\xBF", U"��", U"�");
    check_iteration<from, to>("\x80\xBF\x80", U"���", U"�");
    check_iteration<from, to>("\x80\xBF\x80\xBF", U"����", U"��");
    check_iteration<from, to>("\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F"
                              "\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
                              "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF"
                              "\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF",
                              
                              U"����������������"
                              U"����������������"
                              U"����������������"
                              U"����������������",
                              
                              U"����������������"
                              U"����������������"
                              U"����������������"
                              U"��������������");
    
    //3.2  Lonely start characters
    check_iteration<from, to>("\xC0 \xC1 \xC2 \xC3 \xC4 \xC5 \xC6 \xC7 \xC8 \xC9 \xCA \xCB \xCC \xCD \xCE \xCF "
                              "\xD0 \xD1 \xD2 \xD3 \xD4 \xD5 \xD6 \xD7 \xD8 \xD9 \xDA \xDB \xDC \xDD \xDE \xDF ",
                              
                              U"� � � � � � � � � � � � � � � � "
                              U"� � � � � � � � � � � � � � � � ");
    check_iteration<from, to>("\xE0 \xE1 \xE2 \xE3 \xE4 \xE5 \xE6 \xE7 \xE8 \xE9 \xEA \xEB \xEC \xED \xEE \xEF ",
                              
                              U"� � � � � � � � � � � � � � � � ");
    check_iteration<from, to>("\xF0 \xF1 \xF2 \xF3 \xF4 \xF5 \xF6 \xF7 ", U"� � � � � � � � ");
    check_iteration<from, to>("\xF8 \xF9 \xFA \xFB ", U"� � � � ");
    check_iteration<from, to>("\xFC \xFD ", U"� � ");
    
    //3.3  Sequences with last continuation byte missing
    check_iteration<from, to>("\xC0", U"�");
    check_iteration<from, to>("\xE0\x80", U"��");
    check_iteration<from, to>("\xF0\x80\x80", U"���");
    check_iteration<from, to>("\xDF", U"�");
    check_iteration<from, to>("\xEF\xBF", U"�", U"��");
    check_iteration<from, to>("\xF7\xBF\xBF", U"���");
    
    //3.4  Concatenation of incomplete sequences
    check_iteration<from, to>("\xC0\xE0\x80\xF0\x80\x80\xDF\xEF\xBF\xF7\xBF\xBF", U"�����������", U"������������");
    
    //3.5  Impossible bytes
    check_iteration<from, to>("\xFE", U"�");
    check_iteration<from, to>("\xFF", U"�");
    check_iteration<from, to>("\xFE\xFE\xFF\xFF", U"����");
    
    //4  Overlong sequences
    
    //4.1  Examples of an overlong ASCII character
    check_iteration<from, to>("\xC0\xAF", U"��");
    check_iteration<from, to>("\xE0\x80\xAF", U"���");
    check_iteration<from, to>("\xF0\x80\x80\xAF", U"����");
    
    //4.2  Maximum overlong sequences
    check_iteration<from, to>("\xC1\xBF", U"��");
    check_iteration<from, to>("\xE0\x9F\xBF", U"���");
    check_iteration<from, to>("\xF0\x8F\xBF\xBF", U"����");
    
    //4.3  Overlong representation of the NUL character
    check_iteration<from, to>("\xC0\x80", U"��");
    check_iteration<from, to>("\xE0\x80\x80", U"���");
    check_iteration<from, to>("\xF0\x80\x80\x80", U"����");
    
    //5  Illegal code positions
    
    //5.1 Single UTF-16 surrogates
    check_iteration<from, to>("\xED\xA0\x80", U"���");
    check_iteration<from, to>("\xED\xAD\xBF", U"���");
    check_iteration<from, to>("\xED\xAE\x80", U"���");
    check_iteration<from, to>("\xED\xAF\xBF", U"���");
    check_iteration<from, to>("\xED\xB0\x80", U"���");
    check_iteration<from, to>("\xED\xBE\x80", U"���");
    check_iteration<from, to>("\xED\xBF\xBF", U"���");
    
    //5.2 Paired UTF-16 surrogates
    check_iteration<from, to>("\xED\xA0\x80\xED\xB0\x80", U"������");
    check_iteration<from, to>("\xED\xA0\x80\xED\xBF\xBF", U"������");
    check_iteration<from, to>("\xED\xAD\xBF\xED\xB0\x80", U"������");
    check_iteration<from, to>("\xED\xAD\xBF\xED\xBF\xBF", U"������");
    check_iteration<from, to>("\xED\xAE\x80\xED\xB0\x80", U"������");
    check_iteration<from, to>("\xED\xAE\x80\xED\xBF\xBF", U"������");
    check_iteration<from, to>("\xED\xAF\xBF\xED\xB0\x80", U"������");
    check_iteration<from, to>("\xED\xAF\xBF\xED\xBF\xBF", U"������");
    
    //5.3 Other illegal code positions
    check_iteration<from, to>("\xEF\xBF\xBE", U"\uFFFE");
    check_iteration<from, to>("\xEF\xBF\xBF", U"\uFFFF");
}

TEST_CASE( "UTF8 Iteration on UTF16 sequence" ) {

    check_iteration<utf16, utf8>(u"", "");
    check_iteration<utf16, utf8>(u"a", "a");
    check_iteration<utf16, utf8>(u"ab", "ab");
    
    check_iteration<utf16, utf8>(u"¢", "¢"); //2-byte
    check_iteration<utf16, utf8>(u"ह", "ह"); //3-byte
    check_iteration<utf16, utf8>(u"𐍈", "𐍈"); //4-byte
    check_iteration<utf16, utf8>(u"🤢", "🤢"); //out of bmp in surrogate range
    
    check_iteration<utf16, utf8>(u"\xD801", "\uFFFD"); //hanging lead surrogate
    check_iteration<utf16, utf8>(u"\xDC37", "\uFFFD"); //hanging trail surrogate
    check_iteration<utf16, utf8>(u"\xD801""a", "\uFFFDa"); //hanging lead surrogate + good
    check_iteration<utf16, utf8>(u"a\xDC37", "a\uFFFD"); //good + hanging trail byte
}

TEST_CASE( "UTF16 Iteration on UTF16 sequence" ) {

    check_iteration<utf16, utf16>(u"", u"");
    check_iteration<utf16, utf16>(u"a", u"a");
    check_iteration<utf16, utf16>(u"ab", u"ab");
    
    check_iteration<utf16, utf16>(u"¢", u"¢"); //2-byte
    check_iteration<utf16, utf16>(u"ह", u"ह"); //3-byte
    check_iteration<utf16, utf16>(u"𐍈", u"𐍈"); //4-byte
    check_iteration<utf16, utf16>(u"🤢", u"🤢"); //out of bmp in surrogate range
    
    check_iteration<utf16, utf16>(u"\xD801", u"\uFFFD"); //hanging lead surrogate
    check_iteration<utf16, utf16>(u"\xDC37", u"\uFFFD"); //hanging trail surrogate
    check_iteration<utf16, utf16>(u"\xD801""a", u"\uFFFDa"); //hanging lead surrogate + good
    check_iteration<utf16, utf16>(u"a\xDC37", u"a\uFFFD"); //good + hanging trail byte
}

TEST_CASE( "UTF32 Iteration on UTF16 sequence" ) {

    check_iteration<utf16, utf32>(u"", U"");
    check_iteration<utf16, utf32>(u"a", U"a");
    check_iteration<utf16, utf32>(u"ab", U"ab");
    
    check_iteration<utf16, utf32>(u"¢", U"¢"); //2-byte
    check_iteration<utf16, utf32>(u"ह", U"ह"); //3-byte
    check_iteration<utf16, utf32>(u"𐍈", U"𐍈"); //4-byte
    check_iteration<utf16, utf32>(u"🤢", U"🤢"); //out of bmp in surrogate range
    
    check_iteration<utf16, utf32>(u"\xD801", U"\uFFFD"); //hanging lead surrogate
    check_iteration<utf16, utf32>(u"\xDC37", U"\uFFFD"); //hanging trail surrogate
    check_iteration<utf16, utf32>(u"\xD801""a", U"\uFFFDa"); //hanging lead surrogate + good
    check_iteration<utf16, utf32>(u"a\xDC37", U"a\uFFFD"); //good + hanging trail byte
}


TEST_CASE( "UTF8 Iteration on UTF32 sequence" ) {

    check_iteration<utf32, utf8>(U""s, ""sv);
    check_iteration<utf32, utf8>(U"a"s, "a"sv);
    check_iteration<utf32, utf8>(U"ab"s, "ab"sv);
    
    check_iteration<utf32, utf8>(U"¢", "¢"); //2-byte
    check_iteration<utf32, utf8>(U"ह", "ह"); //3-byte
    check_iteration<utf32, utf8>(U"𐍈", "𐍈"); //4-byte
    check_iteration<utf32, utf8>(U"🤢", "🤢"); //out of bmp in surrogate range
    
    check_iteration<utf32, utf8>(U"\xD801", "\uFFFD"); //hanging lead surrogate
    check_iteration<utf32, utf8>(U"\xDC37", "\uFFFD"); //hanging trail surrogate
    check_iteration<utf32, utf8>(U"\xD801""a", "\uFFFDa"); //hanging lead surrogate + good
    check_iteration<utf32, utf8>(U"a\xDC37", "a\uFFFD"); //good + hanging trail byte
    
    check_iteration<utf32, utf8>(U"\x110000", "\uFFFD"); //too large
    check_iteration<utf32, utf8>(U"a\x110000", "a\uFFFD"); //good + too large
    check_iteration<utf32, utf8>(U"\x110000""a", "\uFFFDa"); //too large + good
}

TEST_CASE( "UTF16 Iteration on UTF32 sequence" ) {

    check_iteration<utf32, utf16>(U""s, u""sv);
    check_iteration<utf32, utf16>(U"a"s, u"a"sv);
    check_iteration<utf32, utf16>(U"ab"s, u"ab"sv);
    
    check_iteration<utf32, utf16>(U"¢", u"¢"); //2-byte
    check_iteration<utf32, utf16>(U"ह", u"ह"); //3-byte
    check_iteration<utf32, utf16>(U"𐍈", u"𐍈"); //4-byte
    check_iteration<utf32, utf16>(U"🤢", u"🤢"); //out of bmp in surrogate range
    
    check_iteration<utf32, utf16>(U"\xD801", u"\uFFFD"); //hanging lead surrogate
    check_iteration<utf32, utf16>(U"\xDC37", u"\uFFFD"); //hanging trail surrogate
    check_iteration<utf32, utf16>(U"\xD801""a", u"\uFFFDa"); //hanging lead surrogate + good
    check_iteration<utf32, utf16>(U"a\xDC37", u"a\uFFFD"); //good + hanging trail byte
    
    check_iteration<utf32, utf16>(U"\x110000", u"\uFFFD"); //too large
    check_iteration<utf32, utf16>(U"a\x110000", u"a\uFFFD"); //good + too large
    check_iteration<utf32, utf16>(U"\x110000""a", u"\uFFFDa"); //too large + good
}

TEST_CASE( "UTF32 Iteration on UTF32 sequence" ) {

    check_iteration<utf32, utf32>(U"", U"");
    check_iteration<utf32, utf32>(U"a", U"a");
    check_iteration<utf32, utf32>(U"ab", U"ab");
    
    check_iteration<utf32, utf32>(U"¢", U"¢"); //2-byte
    check_iteration<utf32, utf32>(U"ह", U"ह"); //3-byte
    check_iteration<utf32, utf32>(U"𐍈", U"𐍈"); //4-byte
    check_iteration<utf32, utf32>(U"🤢", U"🤢"); //out of bmp in surrogate range
    
    check_iteration<utf32, utf32>(U"\xD801", U"\uFFFD"); //hanging lead surrogate
    check_iteration<utf32, utf32>(U"\xDC37", U"\uFFFD"); //hanging trail surrogate
    check_iteration<utf32, utf32>(U"\xD801""a", U"\uFFFDa"); //hanging lead surrogate + good
    check_iteration<utf32, utf32>(U"a\xDC37", U"a\uFFFD"); //good + hanging trail byte
    
    check_iteration<utf32, utf32>(U"\x110000", U"\uFFFD"); //too large
    check_iteration<utf32, utf32>(U"a\x110000", U"a\uFFFD"); //good + too large
    check_iteration<utf32, utf32>(U"\x110000""a", U"\uFFFDa"); //too large + good
}

#if __cpp_lib_ranges >= 202202L

TEST_CASE( "Ranges" ) {
    CHECK(std::ranges::equal(as_utf32(std::vector({'a', 'b', 'c'})), std::array{U'a', U'b', U'c'}));
    CHECK(std::ranges::equal(std::vector({'a', 'b', 'c'}) | as_utf32, std::array{U'a', U'b', U'c'}));

    CHECK(std::ranges::equal(as_utf16(std::vector({'a', 'b', 'c'})), std::array{u'a', u'b', u'c'}));
    CHECK(std::ranges::equal(std::vector({'a', 'b', 'c'}) | as_utf16, std::array{u'a', u'b', u'c'}));

    CHECK(std::ranges::equal(as_utf8(std::vector({u'a', u'b', u'c'})), std::array{'a', 'b', 'c'}));
    CHECK(std::ranges::equal(std::vector({u'a', u'b', u'c'}) | as_utf8, std::array{'a', 'b', 'c'}));
}

#endif

}
