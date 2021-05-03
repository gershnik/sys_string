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

#include <cstdarg>

namespace sysstr::util
{
    template<class StringOrChar32>
    class string_or_char32_char_access;

    template<class StringOrChar32>
    string_or_char32_char_access(const StringOrChar32 &) -> string_or_char32_char_access<StringOrChar32>;

    template<>
    class string_or_char32_char_access<sys_string>
    {
    public:
        string_or_char32_char_access(const sys_string & str) noexcept:
            m_access(str)
        {}
        auto begin() const noexcept
            { return m_access.begin(); }
        auto end() const noexcept
            { return m_access.end(); }
        auto size() const noexcept
            { return m_access.size(); }
        
        constexpr auto is_valid() const noexcept -> bool
            { return true; }
    private:
        sys_string::char_access m_access;
    };

    template<>
    class string_or_char32_char_access<char32_t>
    {
    public:
        string_or_char32_char_access(char32_t c) noexcept:
            m_valid(m_encoded.put(c))
        {}
        
        auto begin() const noexcept
            { return m_encoded.begin(); }
        auto end() const noexcept
            { return m_encoded.end(); }
        auto size() const noexcept
            { return sys_string::size_type(m_encoded.end() - m_encoded.begin()); }
        
        auto is_valid() const noexcept -> bool
            { return m_valid; }
    private:
        utf_codepoint_encoder<utf_encoding_of<sys_string::storage_type>, true> m_encoded;
        bool m_valid;
    };

    template<class Cont>
    inline auto has_prefix(const sys_string::char_access & access, const Cont & test) noexcept -> bool
    {
        if (!test.is_valid())
            return false;
        
        const auto test_size = test.size();
        if (test_size > access.size())
            return false;
        
        return std::equal(test.begin(), test.end(), access.begin());
    }
}

namespace sysstr
{

    inline
    auto operator<<(std::ostream & str, const sys_string & val) -> std::ostream &
    {
        for(char c: sys_string::utf8_view(val))
            str << c;
        return str;
    }

#if defined(_WIN32) 
        
    inline
    auto operator<<(std::wostream & str, const sys_string & val) -> std::wostream &
    {
        for(char16_t c: sys_string::utf16_view(val))
            str << wchar_t(c);
        return str;
    }

#elif defined(__STDC_ISO_10646__)

    inline
    auto operator<<(std::wostream & str, const sys_string & val) -> std::wostream &
    {
        for(char32_t c: sys_string::utf32_view(val))
            str << wchar_t(c);
        return str;
    }

#endif

}

inline
auto sysstr::sys_string::format(const char * format, ...) -> sys_string
{
    va_list vl;
    va_start(vl, format);
    return formatv(format, vl);
}


inline
auto sysstr::sys_string::to_lower() const -> sys_string
{
    sys_string::utf32_view view(*this);
    sys_string_builder builder;
    tolower<utf_encoding_of<sys_string_builder::storage_type>>()(view.cursor_begin<true>(), std::back_inserter(builder.chars()));
    return builder.build();
}

inline
auto sysstr::sys_string::to_upper() const -> sys_string
{
    sys_string::utf32_view view(*this);
    sys_string_builder builder;
    toupper<utf_encoding_of<sys_string_builder::storage_type>>()(view.cursor_begin<true>(), std::back_inserter(builder.chars()));
    return builder.build();
}


template<class Pred>
auto sysstr::sys_string::trim(Pred pred) const -> sys_string
{
    sys_string::utf32_view view(*this);
    auto trimmed_start = view.cursor_begin<true>();
    for( ; ; ++trimmed_start)
    {
        if (!trimmed_start)
            return sys_string();

        char32_t c = *trimmed_start;
        if (!pred(c))
            break;
    }

    auto trimmed_end = view.cursor_begin<false>();
    for( ; ; ++trimmed_end)
    {
        char32_t c = *trimmed_end;
        if (!pred(c))
            break;
    }
    
    return sys_string(trimmed_start.storage_cursor(), trimmed_end.storage_pos() - trimmed_start.storage_pos());
}

template<class Pred>
auto sysstr::sys_string::ltrim(Pred pred) const -> sys_string
{
    sys_string::utf32_view view(*this);
    auto trimmed_start = view.cursor_begin<true>();
    for( ; ; ++trimmed_start)
    {
        if (!trimmed_start)
            return sys_string();

        char32_t c = *trimmed_start;
        if (!pred(c))
            break;
    }

    return sys_string(trimmed_start.storage_cursor(), this->storage_size() - trimmed_start.storage_pos());
}

template<class Pred>
inline
auto sysstr::sys_string::rtrim(Pred pred) const -> sys_string
{
    sys_string::utf32_view view(*this);
    auto trimmed_end = view.cursor_begin<false>();
    for( ; ; ++trimmed_end)
    {
        if (!trimmed_end)
            return sys_string();
        
        char32_t c = *trimmed_end;
        if (!pred(c))
            break;
    }
    
    return sys_string(trimmed_end.storage_cursor(), trimmed_end.storage_pos());
}

template<class Search, class OutIt>
auto sysstr::sys_string::split(OutIt dest, Search search) const ->
    std::enable_if_t<std::is_invocable_v<Search, sys_string::utf32_view::iterator, sys_string::utf32_view::iterator>, OutIt>
{
    sys_string::utf32_view view(*this);
    return util::split<sys_string>(view.begin(), view.end(), dest, search);
}

template<class StringOrChar, class OutIt>
auto sysstr::sys_string::split(OutIt dest, const StringOrChar & sep, size_t max_split) const ->
    std::enable_if_t<util::is_string_or_char<StringOrChar>, OutIt>
{
    util::string_or_char32_char_access sep_access(sep);
    if (!sep_access.is_valid() || sep_access.size() == 0)
    {
        *dest++ = *this;
        return dest;
    }
    
    sys_string::char_access access(*this);
    
    auto sep_begin = sep_access.begin();
    auto sep_end = sep_access.end();
    auto sep_size = sep_access.size();
    
    return util::split<sys_string>(access.begin(), access.end(), dest,
                       [sep_begin, sep_end, sep_size, max_split] (auto str_start, auto str_end) mutable noexcept {
        
        if (max_split == 0)
            return std::pair(str_end, str_end);
        --max_split;
        
        auto found_it = std::search(str_start, str_end, sep_begin, sep_end);
        if (found_it == str_end)
            return std::pair(str_end, str_end);
        return std::pair(found_it, found_it + sep_size);
    });
}

template<class FwdIt>
auto sysstr::sys_string::join(FwdIt first, FwdIt last) const -> sys_string
{
    sys_string_builder builder;
    bool has_one = false;
    for( ; first != last; ++first)
    {
        if (has_one)
            builder.append(*this);
        else
            has_one = true;
        builder.append(*first);
    }
    return builder.build();
}

namespace sysstr::util
{
    template<class Cont>
    inline auto has_suffix(const sys_string::char_access & access, const Cont & test) noexcept -> bool
    {
        if (!test.is_valid())
            return false;
        
        const auto test_size = test.size();
        if (test_size > access.size())
            return false;
        
        return std::equal(test.begin(), test.end(), access.end() - test_size);
    }

    template<class Cont>
    inline auto has_infix(const sys_string::char_access & access, const Cont & test) noexcept -> bool
    {
        if (!test.is_valid())
            return false;
        
        const auto test_size = test.size();
        const auto access_size = access.size();
        if (test_size > access.size())
            return false;
        
        if (access_size == 0)
            return true;
        
        auto access_end = access.end();
        return std::search(access.begin(), access_end, test.begin(), test.end()) != access_end;
    }
}

template<class StringOrChar>
auto sysstr::sys_string::starts_with(const StringOrChar & prefix) const -> std::enable_if_t<util::is_string_or_char<StringOrChar>, bool>
{
    return util::has_prefix(sys_string::char_access(*this), util::string_or_char32_char_access(prefix));
}


template<class InIt>
auto sysstr::sys_string::find_prefix(InIt first, InIt last) const -> InIt
{
    using item_type = typename std::iterator_traits<InIt>::value_type;
    
    sys_string::char_access str_access(*this);
    return std::find_if(first, last, [&str_access] (auto & item) {
        return util::has_prefix(str_access, util::string_or_char32_char_access<item_type>(item));
    });
}

template<class StringOrChar>
auto sysstr::sys_string::remove_prefix(const StringOrChar & prefix) const -> std::enable_if_t<util::is_string_or_char<StringOrChar>, sys_string>
{
    sys_string::char_access access(*this);
    util::string_or_char32_char_access prefix_access(prefix);
    if (!has_prefix(access, prefix_access))
        return *this;
    return sys_string(access.begin() + prefix_access.size(), access.end());
}


template<class StringOrChar>
auto sysstr::sys_string::ends_with(const StringOrChar & suffix) const -> std::enable_if_t<util::is_string_or_char<StringOrChar>, bool>
{
    return util::has_suffix(sys_string::char_access(*this), util::string_or_char32_char_access(suffix));
}

template<class StringOrChar>
auto sysstr::sys_string::remove_suffix(const StringOrChar & suffix) const -> std::enable_if_t<util::is_string_or_char<StringOrChar>, sys_string>
{
    sys_string::char_access access(*this);
    util::string_or_char32_char_access suffix_access(suffix);
    if (!has_suffix(access, suffix_access))
        return *this;
    return sys_string(access.begin(), access.end() - suffix_access.size());
}

template<class InIt>
auto sysstr::sys_string::find_suffix(InIt first, InIt last) const -> InIt
{
    using item_type = typename std::iterator_traits<InIt>::value_type;
    
    sys_string::char_access str_access(*this);
    return std::find_if(first, last, [&str_access] (auto & item) {
        return util::has_suffix(str_access, util::string_or_char32_char_access<item_type>(item));
    });
}

template<class StringOrChar>
auto sysstr::sys_string::contains(const StringOrChar & inner) const -> std::enable_if_t<util::is_string_or_char<StringOrChar>, bool>
{
    return util::has_infix(sys_string::char_access(*this), util::string_or_char32_char_access(inner));
}

template<class InIt>
auto sysstr::sys_string::find_contained(InIt first, InIt last) const -> InIt
{
    using item_type = typename std::iterator_traits<InIt>::value_type;
    
    sys_string::char_access str_access(*this);
    return std::find_if(first, last, [&str_access] (auto & item) {
        return util::has_infix(str_access, util::string_or_char32_char_access<item_type>(item));
    });
}

template<class StringOrChar1, class StringOrChar2>
auto sysstr::sys_string::replace(const StringOrChar1 & old, const StringOrChar2 & new_, size_t max_count) const ->
    std::enable_if_t<util::is_string_or_char<StringOrChar1> && util::is_string_or_char<StringOrChar2>, sys_string>
{
    if (max_count == 0)
        return *this;
    
    util::string_or_char32_char_access old_access(old);
    if (!old_access.is_valid() || old_access.size() == 0)
        return *this;
    
    sys_string::char_access str_access(*this);
    auto first = str_access.begin();
    const auto last = str_access.end();
    
    sys_string_builder builder;
    
    for( ; ; )
    {
        auto found = std::search(first, last, old_access.begin(), old_access.end());
        std::copy(first, found, std::back_inserter(builder.chars()));
        if (found == last)
            break;
        builder.append(new_);
        first = found + old_access.size();
        if (--max_count == 0)
        {
            std::copy(first, last, std::back_inserter(builder.chars()));
            break;
        }
    }
    
    return builder.build();
}
