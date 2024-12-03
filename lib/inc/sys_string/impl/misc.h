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
    namespace util 
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
        
            return std::ranges::equal(test.begin(), test.end(), access.begin(), access.begin() + test_size);
        }
    }

    template<class Storage>
    inline
    auto sys_string_t<Storage>::to_lower() const -> sys_string_t<Storage>
    {
        sys_string_t<Storage>::utf32_access access(*this);
        sys_string_builder_t<Storage> builder;
        tolower<utf_encoding_of<storage_type>>()(access, std::back_inserter(builder.chars()));
        return builder.build();
    }

    template<class Storage>
    inline
    auto sys_string_t<Storage>::to_upper() const -> sys_string_t<Storage>
    {
        sys_string_t<Storage>::utf32_access access(*this);
        sys_string_builder_t<Storage> builder;
        toupper<utf_encoding_of<storage_type>>()(access, std::back_inserter(builder.chars()));
        return builder.build();
    }


    template<class Storage>
    template<std::predicate<char32_t> Pred>
    auto sys_string_t<Storage>::trim(Pred pred) const -> sys_string_t<Storage>
    {
        sys_string_t<Storage>::utf32_access access(*this);
        auto trimmed_start = access.begin();
        for( ; ; ++trimmed_start)
        {
            if (trimmed_start == access.end())
                return sys_string_t<Storage>();

            char32_t c = *trimmed_start;
            if (!pred(c))
                break;
        }

        auto trimmed_end = access.rbegin();
        for( ; ; ++trimmed_end)
        {
            char32_t c = *trimmed_end;
            if (!pred(c))
                break;
        }
    
        return sys_string_t<Storage>(trimmed_start.storage_current(), trimmed_end.storage_current().base());
    }

    template<class Storage>
    template<std::predicate<char32_t> Pred>
    auto sys_string_t<Storage>::ltrim(Pred pred) const -> sys_string_t<Storage>
    {
        sys_string_t<Storage>::utf32_access access(*this);
        auto trimmed_start = access.begin();
        auto last = access.end();
        for( ; ; ++trimmed_start)
        {
            if (trimmed_start == last)
                return sys_string_t<Storage>();

            char32_t c = *trimmed_start;
            if (!pred(c))
                break;
        }

        return sys_string_t<Storage>(trimmed_start.storage_current(), trimmed_start.storage_last());
    }

    template<class Storage>
    template<std::predicate<char32_t> Pred>
    inline
    auto sys_string_t<Storage>::rtrim(Pred pred) const -> sys_string_t<Storage>
    {
        sys_string_t<Storage>::utf32_access access(*this);
        auto trimmed_end = access.rbegin();
        auto last = access.rend();
        for( ; ; ++trimmed_end)
        {
            if (trimmed_end == last)
                return sys_string_t<Storage>();
        
            char32_t c = *trimmed_end;
            if (!pred(c))
                break;
        }
    
        return sys_string_t<Storage>(trimmed_end.storage_current(), trimmed_end.storage_last());
    }

    template<class Storage>
    template<class Search, class OutIt>
    auto sys_string_t<Storage>::split(OutIt dest, Search search) const -> OutIt
    requires(std::output_iterator<OutIt, sys_string_t> &&
             std::is_invocable_v<Search, typename sys_string_t::utf32_access::iterator, std::default_sentinel_t>)
    {
        utf32_access access(*this);
        return util::split<sys_string_t<Storage>>(access.begin(), access.end(), dest, search);
    }

    template<class Storage>
    template<sys_string_or_char<Storage> StringOrChar, class OutIt>
    auto sys_string_t<Storage>::split(OutIt dest, const StringOrChar & sep, size_t max_split) const -> OutIt
    requires(std::output_iterator<OutIt, sys_string_t>)
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
    
        return util::split<sys_string_t<Storage>>(access.begin(), access.end(), dest,
                           [sep_begin, sep_end, max_split] (auto str_start, auto str_end) mutable noexcept {
        
            if (max_split == 0)
            {
                std::ranges::advance(str_start, str_end);
                return std::pair(str_start, str_start);
            }
        
            --max_split;
        
            auto result = std::ranges::search(str_start, str_end, sep_begin, sep_end);
            return std::pair(result.begin(), result.end());
        });
    }

    template<class Storage>
    template<std::input_iterator It, std::sentinel_for<It> EndIt>
    requires(builder_appendable<std::iter_value_t<It>, Storage>)
    auto sys_string_t<Storage>::join(It first, EndIt last) const -> sys_string_t<Storage>
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

    namespace util
    {
        template<class CharAccess, class Cont>
        inline auto has_suffix(const CharAccess & access, const Cont & test) noexcept -> bool
        {
            if (!test.is_valid())
                return false;
        
            const auto test_size = test.size();
            const auto access_size = access.size();
            if (test_size > access_size)
                return false;
        
            return std::ranges::equal(test.begin(), test.end(), access.begin() + access_size - test_size, access.end());
        }

        template<class CharAccess, class Cont>
        inline auto has_infix(const CharAccess & access, const Cont & test) noexcept -> bool
        {
            if (!test.is_valid())
                return false;
        
            const auto test_size = test.size();
            const auto access_size = access.size();
            if (test_size > access_size)
                return false;
            if (access_size == 0)
                return true;
        
            return std::ranges::search(access, test).begin() != access.end();
        }
    }

    template<class Storage>
    template<sys_string_or_char<Storage> StringOrChar>
    auto sys_string_t<Storage>::starts_with(const StringOrChar & prefix) const -> bool
    {
        return util::has_prefix(sys_string_t<Storage>::char_access(*this), util::string_or_char32_char_access<Storage, StringOrChar>(prefix));
    }


    template<class Storage>
    template<std::input_iterator It, std::sentinel_for<It> EndIt>
    requires(sys_string_or_char<std::iter_value_t<It>, Storage>)
    auto sys_string_t<Storage>::find_prefix(It first, EndIt last) const -> It
    {
        using item_type = std::iter_value_t<It>;
    
        sys_string_t<Storage>::char_access str_access(*this);
        return std::ranges::find_if(first, last, [&str_access] (auto & item) {
            return util::has_prefix(str_access, util::string_or_char32_char_access<Storage, item_type>(item));
        });
    }

    template<class Storage>
    template<sys_string_or_char<Storage> StringOrChar>
    auto sys_string_t<Storage>::remove_prefix(const StringOrChar & prefix) const -> sys_string_t<Storage>
    {
        sys_string_t<Storage>::char_access access(*this);
        util::string_or_char32_char_access<Storage, StringOrChar> prefix_access(prefix);
        if (!has_prefix(access, prefix_access))
            return *this;
        return sys_string_t<Storage>(access.begin() + prefix_access.size(), access.end());
    }


    template<class Storage>
    template<sys_string_or_char<Storage> StringOrChar>
    auto sys_string_t<Storage>::ends_with(const StringOrChar & suffix) const -> bool
    {
        return util::has_suffix(sys_string_t<Storage>::char_access(*this), util::string_or_char32_char_access<Storage, StringOrChar>(suffix));
    }

    template<class Storage>
    template<sys_string_or_char<Storage> StringOrChar>
    auto sys_string_t<Storage>::remove_suffix(const StringOrChar & suffix) const -> sys_string_t<Storage>
    {
        sys_string_t<Storage>::char_access access(*this);
        util::string_or_char32_char_access<Storage, StringOrChar> suffix_access(suffix);
        if (!has_suffix(access, suffix_access))
            return *this;
        return sys_string_t<Storage>(access.begin(), access.size() - suffix_access.size());
    }

    template<class Storage>
    template<std::input_iterator It, std::sentinel_for<It> EndIt>
    requires(sys_string_or_char<std::iter_value_t<It>, Storage>)
    auto sys_string_t<Storage>::find_suffix(It first, EndIt last) const -> It
    {
        using item_type = std::iter_value_t<It>;
    
        sys_string_t<Storage>::char_access str_access(*this);
        return std::find_if(first, last, [&str_access] (auto & item) {
            return util::has_suffix(str_access, util::string_or_char32_char_access<Storage, item_type>(item));
        });
    }

    template<class Storage>
    template<sys_string_or_char<Storage> StringOrChar>
    auto sys_string_t<Storage>::contains(const StringOrChar & inner) const -> bool
    {
        return util::has_infix(sys_string_t<Storage>::char_access(*this), util::string_or_char32_char_access<Storage, StringOrChar>(inner));
    }

    template<class Storage>
    template<std::input_iterator It, std::sentinel_for<It> EndIt>
    requires(sys_string_or_char<std::iter_value_t<It>, Storage>)
    auto sys_string_t<Storage>::find_contained(It first, EndIt last) const -> It
    {
        using item_type = std::iter_value_t<It>;
    
        sys_string_t<Storage>::char_access str_access(*this);
        return std::find_if(first, last, [&str_access] (auto & item) {
            return util::has_infix(str_access, util::string_or_char32_char_access<Storage, item_type>(item));
        });
    }

    template<class Storage>
    template<sys_string_or_char<Storage> StringOrChar1, sys_string_or_char<Storage> StringOrChar2>
    auto sys_string_t<Storage>::replace(const StringOrChar1 & old, const StringOrChar2 & new_, size_t max_count) const -> sys_string_t<Storage>
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
            auto found_range = std::ranges::search(first, last, old_access.begin(), old_access.end());
            std::ranges::copy(first, found_range.begin(), std::back_inserter(builder.chars()));
            if (found_range.begin() == last)
                break;
            builder.append(new_);
            first = found_range.end();
            if (--max_count == 0)
            {
                std::ranges::copy(first, last, std::back_inserter(builder.chars()));
                break;
            }
        }
    
        return builder.build();
    }

    template<class Storage>
    template<sys_string_or_char<Storage> StringOrChar>
    auto sys_string_t<Storage>::suffix_after_first(const StringOrChar & divider) const -> std::optional<sys_string_t>
    {
        sys_string_t<Storage>::char_access my_access(*this);
        util::string_or_char32_char_access<Storage, StringOrChar> divider_access(divider);
        auto found_range = std::ranges::search(my_access, divider_access);
        if (found_range.begin() == my_access.end() && divider_access.size() != 0)
            return std::nullopt;
        return sys_string_t(found_range.end(), my_access.end());
    }

    template<class Storage>
    template<sys_string_or_char<Storage> StringOrChar>
    auto sys_string_t<Storage>::prefix_before_first(const StringOrChar & divider) const -> std::optional<sys_string_t>
    {
        sys_string_t<Storage>::char_access my_access(*this);
        util::string_or_char32_char_access<Storage, StringOrChar> divider_access(divider);
        auto found_range = std::ranges::search(my_access, divider_access);
        if (found_range.begin() == my_access.end() && divider_access.size() != 0)
            return std::nullopt;
        return sys_string_t(my_access.begin(), found_range.begin());
    }

    template<class Storage>
    template<sys_string_or_char<Storage> StringOrChar>
    auto sys_string_t<Storage>::suffix_after_last(const StringOrChar & divider) const -> std::optional<sys_string_t>
    {
        sys_string_t<Storage>::char_access my_access(*this);
        util::string_or_char32_char_access<Storage, StringOrChar> divider_access(divider);
        auto found_range = std::ranges::search(my_access.rbegin(), my_access.rend(), divider_access.rbegin(), divider_access.rend());
        if (found_range.begin() == my_access.rend() && divider_access.size() != 0)
            return std::nullopt;
        return sys_string_t(found_range.begin().base(), my_access.end());
    }

    template<class Storage>
    template<sys_string_or_char<Storage> StringOrChar>
    auto sys_string_t<Storage>::prefix_before_last(const StringOrChar & divider) const -> std::optional<sys_string_t>
    {
        sys_string_t<Storage>::char_access my_access(*this);
        util::string_or_char32_char_access<Storage, StringOrChar> divider_access(divider);
        auto found_range = std::ranges::search(my_access.rbegin(), my_access.rend(), divider_access.rbegin(), divider_access.rend());
        if (found_range.begin() == my_access.rend() && divider_access.size() != 0)
            return std::nullopt;
        return sys_string_t(my_access.begin(), found_range.end().base());
    }

    template<class Storage>
    template<sys_string_or_char<Storage> StringOrChar>
    auto sys_string_t<Storage>::partition_at_first(const StringOrChar & divider) const ->
        std::optional<std::pair<sys_string_t, sys_string_t>>
    {
        sys_string_t<Storage>::char_access my_access(*this);
        util::string_or_char32_char_access<Storage, StringOrChar> divider_access(divider);
        auto found_range = std::ranges::search(my_access, divider_access);
        if (found_range.begin() == my_access.end() && divider_access.size() != 0)
            return std::nullopt;
        return std::pair(sys_string_t(my_access.begin(), found_range.begin()), sys_string_t(found_range.end(), my_access.end()));
    }

    template<class Storage>
    template<sys_string_or_char<Storage> StringOrChar>
    auto sys_string_t<Storage>::partition_at_last(const StringOrChar & divider) const ->
        std::optional<std::pair<sys_string_t, sys_string_t>>
    {
        sys_string_t<Storage>::char_access my_access(*this);
        util::string_or_char32_char_access<Storage, StringOrChar> divider_access(divider);
        auto found_range = std::ranges::search(my_access.rbegin(), my_access.rend(), divider_access.rbegin(), divider_access.rend());
        if (found_range.begin() == my_access.rend() && divider_access.size() != 0)
            return std::nullopt;
        return std::pair(sys_string_t(my_access.begin(), found_range.end().base()),
                         sys_string_t(found_range.begin().base(), my_access.end()));
    }
}
