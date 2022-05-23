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
    template<class Storage, class StringOrChar32>
    class string_or_char32_char_access;

    template<class Storage>
    class string_or_char32_char_access<Storage, sys_string_t<Storage>>
    {
    public:
        string_or_char32_char_access(const sys_string_t<Storage> & str) noexcept:
            m_access(str)
        {}
        auto begin() const noexcept
            { return m_access.begin(); }
        auto end() const noexcept
            { return m_access.end(); }
        auto rbegin() const noexcept
            { return m_access.rbegin(); }
        auto rend() const noexcept
            { return m_access.rend(); }
        auto size() const noexcept
            { return m_access.size(); }
        
        constexpr auto is_valid() const noexcept -> bool
            { return true; }
    private:
        typename sys_string_t<Storage>::char_access m_access;
    };

    template<class Storage>
    class string_or_char32_char_access<Storage, char32_t>
    {
    public:
        string_or_char32_char_access(char32_t c) noexcept:
            m_valid(m_encoded.put(c))
        {}
        
        auto begin() const noexcept
            { return m_encoded.begin(); }
        auto end() const noexcept
            { return m_encoded.end(); }
        auto rbegin() const noexcept
            { return std::reverse_iterator(m_encoded.end()); }
        auto rend() const noexcept
            { return std::reverse_iterator(m_encoded.begin()); }
        auto size() const noexcept
            { return typename sys_string_t<Storage>::size_type(m_encoded.end() - m_encoded.begin()); }
        
        auto is_valid() const noexcept -> bool
            { return m_valid; }
    private:
        utf_codepoint_encoder<utf_encoding_of<typename sys_string_t<Storage>::storage_type>, true> m_encoded;
        bool m_valid;
    };

    template<class CharAccess, class Cont>
    inline auto has_prefix(const CharAccess & access, const Cont & test) noexcept -> bool
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

    template<class Storage>
    inline
    auto operator<<(std::ostream & str, const sys_string_t<Storage> & val) -> std::ostream &
    {
        for(char c: typename sys_string_t<Storage>::utf8_view(val))
            str << c;
        return str;
    }

#if defined(_WIN32) 
        
    template<class Storage>
    inline
    auto operator<<(std::wostream & str, const sys_string_t<Storage> & val) -> std::wostream &
    {
        for(char16_t c: typename sys_string_t<Storage>::utf16_view(val))
            str << wchar_t(c);
        return str;
    }

#elif defined(__STDC_ISO_10646__)

    template<class Storage>
    inline
    auto operator<<(std::wostream & str, const sys_string_t<Storage> & val) -> std::wostream &
    {
        for(char32_t c: typename sys_string_t<Storage>::utf32_view(val))
            str << wchar_t(c);
        return str;
    }

#endif

}



template<class Storage>
inline
auto sysstr::sys_string_t<Storage>::to_lower() const -> sys_string_t<Storage>
{
    sys_string_t<Storage>::utf32_view view(*this);
    sys_string_builder_t<Storage> builder;
    tolower<utf_encoding_of<storage_type>>()(view.cursor_begin(), std::back_inserter(builder.chars()));
    return builder.build();
}

template<class Storage>
inline
auto sysstr::sys_string_t<Storage>::to_upper() const -> sys_string_t<Storage>
{
    sys_string_t<Storage>::utf32_view view(*this);
    sys_string_builder_t<Storage> builder;
    toupper<utf_encoding_of<storage_type>>()(view.cursor_begin(), std::back_inserter(builder.chars()));
    return builder.build();
}


template<class Storage>
template<class Pred>
auto sysstr::sys_string_t<Storage>::trim(Pred pred) const -> sys_string_t<Storage>
{
    sys_string_t<Storage>::utf32_view view(*this);
    auto trimmed_start = view.cursor_begin();
    for( ; ; ++trimmed_start)
    {
        if (!trimmed_start)
            return sys_string_t<Storage>();

        char32_t c = *trimmed_start;
        if (!pred(c))
            break;
    }

    auto trimmed_end = view.template cursor_begin<view.backward>();
    for( ; ; ++trimmed_end)
    {
        char32_t c = *trimmed_end;
        if (!pred(c))
            break;
    }
    
    return sys_string_t<Storage>(trimmed_start.storage_cursor(), trimmed_end.storage_pos() - trimmed_start.storage_pos());
}

template<class Storage>
template<class Pred>
auto sysstr::sys_string_t<Storage>::ltrim(Pred pred) const -> sys_string_t<Storage>
{
    sys_string_t<Storage>::utf32_view view(*this);
    auto trimmed_start = view.cursor_begin();
    for( ; ; ++trimmed_start)
    {
        if (!trimmed_start)
            return sys_string_t<Storage>();

        char32_t c = *trimmed_start;
        if (!pred(c))
            break;
    }

    return sys_string_t<Storage>(trimmed_start.storage_cursor(), this->storage_size() - trimmed_start.storage_pos());
}

template<class Storage>
template<class Pred>
inline
auto sysstr::sys_string_t<Storage>::rtrim(Pred pred) const -> sys_string_t<Storage>
{
    sys_string_t<Storage>::utf32_view view(*this);
    auto trimmed_end = view.template cursor_begin<view.backward>();
    for( ; ; ++trimmed_end)
    {
        if (!trimmed_end)
            return sys_string_t<Storage>();
        
        char32_t c = *trimmed_end;
        if (!pred(c))
            break;
    }
    
    return sys_string_t<Storage>(trimmed_end.storage_cursor(), trimmed_end.storage_pos());
}

template<class Storage>
template<class Search, class OutIt>
auto sysstr::sys_string_t<Storage>::split(OutIt dest, Search search) const ->
    std::enable_if_t<std::is_invocable_v<Search, 
                                         typename utf32_view::iterator, 
                                         typename utf32_view::iterator>, OutIt>
{
    utf32_view view(*this);
    return util::split<sys_string_t<Storage>>(view.begin(), view.end(), dest, search);
}

template<class Storage>
template<class StringOrChar, class OutIt>
auto sysstr::sys_string_t<Storage>::split(OutIt dest, const StringOrChar & sep, size_t max_split) const ->
    std::enable_if_t<is_string_or_char<StringOrChar>, OutIt>
{
    util::string_or_char32_char_access<Storage, StringOrChar> sep_access(sep);
    if (!sep_access.is_valid() || sep_access.size() == 0)
    {
        *dest++ = *this;
        return dest;
    }
    
    sys_string_t<Storage>::char_access access(*this);
    
    auto sep_begin = sep_access.begin();
    auto sep_end = sep_access.end();
    auto sep_size = sep_access.size();
    
    return util::split<sys_string_t<Storage>>(access.begin(), access.end(), dest,
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

template<class Storage>
template<class FwdIt>
auto sysstr::sys_string_t<Storage>::join(FwdIt first, FwdIt last) const -> sys_string_t<Storage>
{
    sys_string_builder_t<Storage> builder;
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
    template<class CharAccess, class Cont>
    inline auto has_suffix(const CharAccess & access, const Cont & test) noexcept -> bool
    {
        if (!test.is_valid())
            return false;
        
        const auto test_size = test.size();
        if (test_size > access.size())
            return false;
        
        return std::equal(test.begin(), test.end(), access.end() - test_size);
    }

    template<class CharAccess, class Cont>
    inline auto has_infix(const CharAccess & access, const Cont & test) noexcept -> bool
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

template<class Storage>
template<class StringOrChar>
auto sysstr::sys_string_t<Storage>::starts_with(const StringOrChar & prefix) const -> std::enable_if_t<is_string_or_char<StringOrChar>, bool>
{
    return util::has_prefix(sys_string_t<Storage>::char_access(*this), util::string_or_char32_char_access<Storage, StringOrChar>(prefix));
}


template<class Storage>
template<class InIt>
auto sysstr::sys_string_t<Storage>::find_prefix(InIt first, InIt last) const -> InIt
{
    using item_type = typename std::iterator_traits<InIt>::value_type;
    
    sys_string_t<Storage>::char_access str_access(*this);
    return std::find_if(first, last, [&str_access] (auto & item) {
        return util::has_prefix(str_access, util::string_or_char32_char_access<Storage, item_type>(item));
    });
}

template<class Storage>
template<class StringOrChar>
auto sysstr::sys_string_t<Storage>::remove_prefix(const StringOrChar & prefix) const 
    -> std::enable_if_t<is_string_or_char<StringOrChar>, sys_string_t<Storage>>
{
    sys_string_t<Storage>::char_access access(*this);
    util::string_or_char32_char_access<Storage, StringOrChar> prefix_access(prefix);
    if (!has_prefix(access, prefix_access))
        return *this;
    return sys_string_t<Storage>(access.begin() + prefix_access.size(), access.end());
}


template<class Storage>
template<class StringOrChar>
auto sysstr::sys_string_t<Storage>::ends_with(const StringOrChar & suffix) const 
    -> std::enable_if_t<is_string_or_char<StringOrChar>, bool>
{
    return util::has_suffix(sys_string_t<Storage>::char_access(*this), util::string_or_char32_char_access<Storage, StringOrChar>(suffix));
}

template<class Storage>
template<class StringOrChar>
auto sysstr::sys_string_t<Storage>::remove_suffix(const StringOrChar & suffix) const 
    -> std::enable_if_t<is_string_or_char<StringOrChar>, sys_string_t<Storage>>
{
    sys_string_t<Storage>::char_access access(*this);
    util::string_or_char32_char_access<Storage, StringOrChar> suffix_access(suffix);
    if (!has_suffix(access, suffix_access))
        return *this;
    return sys_string_t<Storage>(access.begin(), access.end() - suffix_access.size());
}

template<class Storage>
template<class InIt>
auto sysstr::sys_string_t<Storage>::find_suffix(InIt first, InIt last) const -> InIt
{
    using item_type = typename std::iterator_traits<InIt>::value_type;
    
    sys_string_t<Storage>::char_access str_access(*this);
    return std::find_if(first, last, [&str_access] (auto & item) {
        return util::has_suffix(str_access, util::string_or_char32_char_access<Storage, item_type>(item));
    });
}

template<class Storage>
template<class StringOrChar>
auto sysstr::sys_string_t<Storage>::contains(const StringOrChar & inner) const -> std::enable_if_t<is_string_or_char<StringOrChar>, bool>
{
    return util::has_infix(sys_string_t<Storage>::char_access(*this), util::string_or_char32_char_access<Storage, StringOrChar>(inner));
}

template<class Storage>
template<class InIt>
auto sysstr::sys_string_t<Storage>::find_contained(InIt first, InIt last) const -> InIt
{
    using item_type = typename std::iterator_traits<InIt>::value_type;
    
    sys_string_t<Storage>::char_access str_access(*this);
    return std::find_if(first, last, [&str_access] (auto & item) {
        return util::has_infix(str_access, util::string_or_char32_char_access<Storage, item_type>(item));
    });
}

template<class Storage>
template<class StringOrChar1, class StringOrChar2>
auto sysstr::sys_string_t<Storage>::replace(const StringOrChar1 & old, const StringOrChar2 & new_, size_t max_count) const ->
    std::enable_if_t<is_string_or_char<StringOrChar1> && is_string_or_char<StringOrChar2>, sys_string_t<Storage>>
{
    if (max_count == 0)
        return *this;
    
    util::string_or_char32_char_access<Storage, StringOrChar1> old_access(old);
    if (!old_access.is_valid() || old_access.size() == 0)
        return *this;
    
    sys_string_t<Storage>::char_access str_access(*this);
    auto first = str_access.begin();
    const auto last = str_access.end();
    
    sys_string_builder_t<Storage> builder;
    
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

template<class Storage>
template<class StringOrChar>
auto sysstr::sys_string_t<Storage>::tailAfterFirst(const StringOrChar & divider) const -> 
    std::enable_if_t<is_string_or_char<StringOrChar>, sys_string_t>
{
    sys_string_t<Storage>::char_access my_access(*this);
    util::string_or_char32_char_access<Storage, StringOrChar> divider_access(divider);
    auto it = std::search(my_access.begin(), my_access.end(), divider_access.begin(), divider_access.end());
    if (it != my_access.end())
        it += divider_access.size();
    return sys_string_t(it, my_access.end());
}

template<class Storage>
template<class StringOrChar>
auto sysstr::sys_string_t<Storage>::headBeforeFirst(const StringOrChar & divider) const -> 
    std::enable_if_t<is_string_or_char<StringOrChar>, sys_string_t>
{
    sys_string_t<Storage>::char_access my_access(*this);
    util::string_or_char32_char_access<Storage, StringOrChar> divider_access(divider);
    auto it = std::search(my_access.begin(), my_access.end(), divider_access.begin(), divider_access.end());
    return sys_string_t(my_access.begin(), it);
}

template<class Storage>
template<class StringOrChar>
auto sysstr::sys_string_t<Storage>::tailAfterLast(const StringOrChar & divider) const -> 
    std::enable_if_t<is_string_or_char<StringOrChar>, sys_string_t>
{
    sys_string_t<Storage>::char_access my_access(*this);
    util::string_or_char32_char_access<Storage, StringOrChar> divider_access(divider);
    auto it = std::search(my_access.rbegin(), my_access.rend(), divider_access.rbegin(), divider_access.rend());
    return sys_string_t(my_access.begin() + (my_access.rend() - it), my_access.end());
}

template<class Storage>
template<class StringOrChar>
auto sysstr::sys_string_t<Storage>::headBeforeLast(const StringOrChar & divider) const -> 
    std::enable_if_t<is_string_or_char<StringOrChar>, sys_string_t>
{
    sys_string_t<Storage>::char_access my_access(*this);
    util::string_or_char32_char_access<Storage, StringOrChar> divider_access(divider);
    auto it = std::search(my_access.rbegin(), my_access.rend(), divider_access.rbegin(), divider_access.rend());
    if (it != my_access.rend())
        it += divider_access.size();
    return sys_string_t(my_access.begin(), my_access.begin() + (my_access.rend() - it));
}
