//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_H_INCLUDED
#define HEADER_SYS_STRING_H_INCLUDED

#include <sys_string/config.h>
#include <sys_string/utf_view.h>
#include <sys_string/impl/unicode/algorithms.h>

#include <functional>
#include <vector>
#include <string>
#include <string_view>
#include <optional>
#include <ostream>
#include <limits>

#if SYS_STRING_USE_SPACESHIP_OPERATOR
    #include <compare>
#endif
#if SYS_STRING_USE_SPAN
    #include <span>
#endif

#define HEADER_SYS_STRING_H_INSIDE

namespace sysstr
{
    template<class Storage> class sys_string_t;
    template<class Storage> class sys_string_builder_t;

    enum class handle_retain
    {
        no,
        yes
    };

    enum handle_transfer
    {
        copy_content,
        attach_pointer
    };
}

namespace sysstr::util
{
    struct char_access_data_detector
    {
        template<class CharAccess>
        auto operator()(const CharAccess * access) noexcept(noexcept(access->data())) -> decltype(access->data());
    };

    template<class CharAccess>
    constexpr bool has_contiguous_data =
            std::is_nothrow_invocable_r_v<const typename CharAccess::value_type *,char_access_data_detector, const CharAccess *>;

    template<class Storage, class First, class Second>
    class addition;
    
    template<class Storage>
    sys_string_t<Storage> build(typename Storage::builder_impl & builder) noexcept;
}

namespace sysstr
{
    template<class Storage>
    class sys_string_t : public Storage
    {
    friend std::hash<sys_string_t>;
    friend typename Storage::char_access;
    private:
        using storage = Storage;
        
        struct view_traits
        {
            using char_access = typename Storage::char_access;
            
            static constexpr const sys_string_t & adapt(const sys_string_t & str) noexcept
                { return str; }
        };

        template<class StringOrChar>
        static constexpr bool is_string_or_char = std::is_same_v<std::remove_cv_t<std::remove_reference_t<StringOrChar>>, sys_string_t> ||
                                                  std::is_same_v<std::remove_cv_t<std::remove_reference_t<StringOrChar>>, char32_t>;
    public:
        using size_type = typename storage::size_type;
        using storage_type = typename storage::storage_type;
        using hash_type = typename storage::hash_type;
        
        static constexpr size_type max_size = storage::max_size;
        
        using char_access = typename storage::char_access;
        
        template<utf_encoding Enc>
        using utf_view = utf_view<Enc, sys_string_t, view_traits>;
        
        using utf8_view  = utf_view<utf8>;
        using utf16_view = utf_view<utf16>;
        using utf32_view = utf_view<utf32>;
        
    #if SYS_STRING_USE_SPACESHIP_OPERATOR
        using compare_result = std::strong_ordering;
        static constexpr compare_result ordering_less = std::strong_ordering::less;
        static constexpr compare_result ordering_equal = std::strong_ordering::equal;
        static constexpr compare_result ordering_greater = std::strong_ordering::greater;
    #else
        using compare_result = int;
        static constexpr compare_result ordering_less = -1;
        static constexpr compare_result ordering_equal = 0;
        static constexpr compare_result ordering_greater = 1;
    #endif
        
    public:
        sys_string_t() noexcept = default;
        sys_string_t(const sys_string_t &) noexcept = default;
        sys_string_t(sys_string_t &&) noexcept = default;
        sys_string_t & operator=(const sys_string_t &) noexcept = default;
        sys_string_t & operator=(sys_string_t &&) noexcept = default;
        ~sys_string_t() noexcept = default;
        
        using storage::storage;
     

        template<class Char>
        sys_string_t(const Char * str, size_t len, std::enable_if_t<has_utf_encoding<Char>> * = nullptr) :
            storage(str, len)
        {}
        
        template<class Char>
        sys_string_t(const Char * str, std::enable_if_t<has_utf_encoding<Char>> * = nullptr) :
            sys_string_t(str, str ? std::char_traits<Char>::length(str) : 0)
        {}
        
        template<class Char>
        sys_string_t(char val, std::enable_if_t<has_utf_encoding<Char>> * = nullptr) :
            sys_string_t(&val, 1)
        {}
    
        template<class Char>
        sys_string_t(const std::basic_string<Char> & val, std::enable_if_t<has_utf_encoding<Char>> * = nullptr) :
            sys_string_t(val.data(), val.size())
        {}

        template<class Char>
        sys_string_t(const std::basic_string_view<Char> & val, std::enable_if_t<has_utf_encoding<Char>> * = nullptr) :
            sys_string_t(val.data(), val.size())
        {}
        
        template<class Char>
        sys_string_t(const std::vector<Char> & val, std::enable_if_t<has_utf_encoding<Char>> * = nullptr) :
            sys_string_t(val.data(), val.size())
        {}

    #if SYS_STRING_USE_SPAN
        template<class Char, std::size_t Extent>
        sys_string_t(const std::span<Char, Extent> & val, std::enable_if_t<has_utf_encoding<Char>> * = nullptr) :
            sys_string_t(val.data(), val.size())
        {}
    #endif
        
        
        sys_string_t(const typename utf32_view::iterator & first, const typename utf32_view::iterator & last):
            sys_string_t(first.storage_cursor(), last.storage_pos() - first.storage_pos())
        {}
        
        sys_string_t(const typename char_access::iterator & first, const typename char_access::iterator & last);

        sys_string_t(const typename char_access::cursor & src, size_type length);
        
        sys_string_t(const typename char_access::reverse_cursor & src, size_type length);

        explicit sys_string_t(storage && st) noexcept : storage(std::move(st))
        {}
        
        auto storage_size() const noexcept -> size_type
            { return storage::size(); }
        
        using storage::data;
        using storage::copy_data;
        
        auto empty() const noexcept -> bool
            { return storage::size() == 0; }
        
        static auto format(const char * spec, ...) -> sys_string_t;
        static auto formatv(const char * spec, va_list vl) -> sys_string_t;

        template<class S>
        friend auto operator<<(std::ostream & str, const sys_string_t<S> & val) -> std::ostream &;
    #if defined(_WIN32) || defined(__STDC_ISO_10646__)
        template<class S>
        friend auto operator<<(std::wostream & str, const sys_string_t<S> & val) -> std::wostream &;
    #endif

        auto swap(sys_string_t & other) noexcept -> void
            { storage::swap(static_cast<storage &>(other)); }

        friend auto swap(sys_string_t & lhs, sys_string_t & rhs) noexcept -> void
            { lhs.swap(rhs); }
        
        friend auto operator==(const sys_string_t & lhs, const sys_string_t & rhs) noexcept -> bool
            { return compare(lhs, rhs) == 0; }
        friend auto operator!=(const sys_string_t & lhs, const sys_string_t & rhs) noexcept -> bool
            { return compare(lhs, rhs) != 0; }
        
    #if SYS_STRING_USE_SPACESHIP_OPERATOR

        friend auto operator<=>(const sys_string_t & lhs, const sys_string_t & rhs) noexcept -> std::strong_ordering
            { return compare(lhs, rhs); }
        
    #else

        friend auto operator<(const sys_string_t & lhs, const sys_string_t & rhs) noexcept -> bool
            { return compare(lhs, rhs) < 0; }
        friend auto operator<=(const sys_string_t & lhs, const sys_string_t & rhs) noexcept -> bool
            { return compare(lhs, rhs) <= 0; }
        friend auto operator>(const sys_string_t & lhs, const sys_string_t & rhs) noexcept -> bool
            { return compare(lhs, rhs) > 0; }
        friend auto operator>=(const sys_string_t & lhs, const sys_string_t & rhs) noexcept -> bool
            { return compare(lhs, rhs) >= 0; }
        
    #endif
        
        friend auto compare(const sys_string_t & lhs, const sys_string_t & rhs) noexcept -> compare_result
            { return sys_string_t::compare(lhs, rhs); }
        friend auto compare_no_case(const sys_string_t lhs, const sys_string_t & rhs) noexcept -> compare_result
            { return sys_string_t::compare_no_case(lhs, rhs); }
        
        template<class StringOrChar1, class StringOrChar2>
        friend auto operator+(const StringOrChar1 lhs, const StringOrChar2 rhs) ->
                std::enable_if_t<sys_string_t<Storage>::is_string_or_char<StringOrChar1> && sys_string_t<Storage>::is_string_or_char<StringOrChar2>,
                util::addition<Storage, StringOrChar1, StringOrChar2>>
            { return util::addition<Storage, StringOrChar1, StringOrChar2>(std::move(lhs), std::move(rhs)); }
        
        auto to_lower() const -> sys_string_t;
        auto to_upper() const -> sys_string_t;

        template<class Pred = isspace>
        auto trim(Pred pred = Pred()) const -> sys_string_t;
        template<class Pred = isspace>
        auto ltrim(Pred pred = Pred()) const -> sys_string_t;
        template<class Pred = isspace>
        auto rtrim(Pred pred = Pred()) const -> sys_string_t;

        template<class Search, class OutIt>
        auto split(OutIt dest, Search pred) const ->
            std::enable_if_t<std::is_invocable_v<Search, typename utf32_view::iterator, typename utf32_view::iterator>, OutIt>;
        template<class StringOrChar, class OutIt>
        auto split(OutIt dest, const StringOrChar & sep, size_t max_split = std::numeric_limits<size_t>::max()) const ->
            std::enable_if_t<is_string_or_char<StringOrChar>, OutIt>;

        template<class FwdIt>
        auto join(FwdIt first, FwdIt last) const -> sys_string_t;

        template<class StringOrChar>
        auto starts_with(const StringOrChar & prefix) const -> std::enable_if_t<is_string_or_char<StringOrChar>, bool>;
        template<class InIt>
        auto find_prefix(InIt first, InIt last) const -> InIt;

        template<class StringOrChar>
        auto remove_prefix(const StringOrChar & prefix) const -> std::enable_if_t<is_string_or_char<StringOrChar>, sys_string_t>;
        
        template<class StringOrChar>
        auto ends_with(const StringOrChar & suffix) const -> std::enable_if_t<is_string_or_char<StringOrChar>, bool>;
        template<class InIt>
        auto find_suffix(InIt first, InIt last) const -> InIt;

        template<class StringOrChar>
        auto remove_suffix(const StringOrChar & prefix) const -> std::enable_if_t<is_string_or_char<StringOrChar>, sys_string_t>;
        
        template<class StringOrChar>
        auto contains(const StringOrChar & inner) const -> std::enable_if_t<is_string_or_char<StringOrChar>, bool>;
        template<class InIt>
        auto find_contained(InIt first, InIt last) const -> InIt;

        template<class StringOrChar1, class StringOrChar2>
        auto replace(const StringOrChar1 & old, const StringOrChar2 & new_,
                     size_t max_count = std::numeric_limits<size_t>::max()) const ->
            std::enable_if_t<is_string_or_char<StringOrChar1> && is_string_or_char<StringOrChar2>, sys_string_t>;

        template<class StringOrChar>
        auto suffix_after_first(const StringOrChar & divider) const -> 
            std::enable_if_t<is_string_or_char<StringOrChar>, std::optional<sys_string_t>>;
        template<class StringOrChar>
        auto prefix_before_first(const StringOrChar & divider) const -> 
            std::enable_if_t<is_string_or_char<StringOrChar>, std::optional<sys_string_t>>;
        template<class StringOrChar>
        auto suffix_after_last(const StringOrChar & divider) const -> 
            std::enable_if_t<is_string_or_char<StringOrChar>, std::optional<sys_string_t>>;
        template<class StringOrChar>
        auto prefix_before_last(const StringOrChar & divider) const -> 
            std::enable_if_t<is_string_or_char<StringOrChar>, std::optional<sys_string_t>>;

        template<class StringOrChar>
        auto partition_at_first(const StringOrChar & divider) const -> 
            std::enable_if_t<is_string_or_char<StringOrChar>, std::optional<std::pair<sys_string_t, sys_string_t>>>;
        template<class StringOrChar>
        auto partition_at_last(const StringOrChar & divider) const -> 
            std::enable_if_t<is_string_or_char<StringOrChar>, std::optional<std::pair<sys_string_t, sys_string_t>>>;

    private:
        static auto compare(const sys_string_t & lhs, const sys_string_t & rhs) noexcept -> compare_result;
        static auto compare_no_case(const sys_string_t lhs, const sys_string_t & rhs) noexcept -> compare_result;
        
    };

}


#include <sys_string/impl/platform.h>


#include <sys_string/impl/compare.h>
#include <sys_string/impl/hash.h>
#include <sys_string/impl/builder.h>
#include <sys_string/impl/addition.h>
#include <sys_string/impl/misc.h>
#include <sys_string/impl/format.h>


#undef HEADER_SYS_STRING_H_INSIDE

#if ! SYS_STRING_NO_S_MACRO

    #define S(x) SYS_STRING_STATIC(x)

#endif

#endif 
