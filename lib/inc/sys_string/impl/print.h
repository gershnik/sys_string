//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_H_INSIDE
#error This header must not be included directly. Please include sys_string.h
#endif

namespace sysstr
{
    template<class Storage>
    template<std::invocable<std::string_view> Func>
    inline
    auto sys_string_t<Storage>::print_with(Func func) const -> decltype(func(std::string_view{}))
    {
        if constexpr (std::ranges::contiguous_range<typename sys_string_t<Storage>::char_access> &&
                        std::is_same_v<typename sys_string_t<Storage>::storage_type, char>)
        {
            typename sysstr::sys_string_t<Storage>::char_access access(*this);
            return func(std::string_view(access.data(), access.size()));
        } 
        else 
        {
            return func(sysstr::sys_string_t<Storage>::utf8_access(*this));
        }
    }

    #if SYS_STRING_WCHAR_T_IS_UTF16
        template<class Storage>
        template<std::invocable<std::wstring_view> Func>
        inline
        auto sys_string_t<Storage>::wprint_with(Func func) const -> decltype(func(std::wstring_view{}))
        {
            if constexpr (std::ranges::contiguous_range<typename sys_string_t<Storage>::char_access> &&
                            std::is_same_v<typename sys_string_t<Storage>::storage_type, char16_t>)
            {
                typename sysstr::sys_string_t<Storage>::char_access access(*this);
                return func(std::wstring_view((const wchar_t *)access.data(), access.size()));
            } 
            else 
            {
                return func(sysstr::sys_string_t<Storage>::utf16_access(*this));
            }
        }
    #elif SYS_STRING_WCHAR_T_IS_UTF32
        template<class Storage>
        template<std::invocable<std::wstring_view> Func>
        inline
        auto sys_string_t<Storage>::wprint_with(Func func) const -> decltype(func(std::wstring_view{}))
        {
            if constexpr (std::ranges::contiguous_range<typename sys_string_t<Storage>::char_access> &&
                            std::is_same_v<typename sys_string_t<Storage>::storage_type, char32_t>)
            {
                typename sysstr::sys_string_t<Storage>::char_access access(*this);
                return func(std::wstring_view((const wchar_t *)access.data(), access.size()));
            } 
            else 
            {
                return func(sysstr::sys_string_t<Storage>::utf32_access(*this));
            }
        }
    #endif
}

#if SYS_STRING_SUPPORTS_STD_FORMAT

template<class Storage> struct std::formatter<sysstr::sys_string_t<Storage>> 
{
    template<class ParseContext>
    constexpr auto parse(ParseContext & ctx) -> ParseContext::iterator
    {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}')
            throw std::format_error("Invalid format args");
 
        return it;
    }

    template <typename FormatContext>
    auto format(const sysstr::sys_string_t<Storage> & str, FormatContext & ctx) const -> decltype(ctx.out()) 
    {
        return str.print_with([&](const auto & view) -> decltype(ctx.out()) {
            return std::ranges::copy(view, ctx.out()).out;
        });
    }
};

#if SYS_STRING_WCHAR_T_IS_UTF16 || SYS_STRING_WCHAR_T_IS_UTF32

template<class Storage> struct std::formatter<sysstr::sys_string_t<Storage>, wchar_t> 
{
    template<class ParseContext>
    constexpr auto parse(ParseContext & ctx) -> ParseContext::iterator
    {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != L'}')
            throw std::format_error("Invalid format args");
 
        return it;
    }

    template <typename FormatContext>
    auto format(const sysstr::sys_string_t<Storage> & str, FormatContext & ctx) const -> decltype(ctx.out()) 
    {
        return str.wprint_with([&](const auto & view) -> decltype(ctx.out()) {
            return std::ranges::copy(view, ctx.out()).out;
        });
    }
};

#endif

#endif
