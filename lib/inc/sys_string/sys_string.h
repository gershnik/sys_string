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
#include <compare>
#include <stdarg.h>


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
    template<class T, class Storage>
    concept sys_string_or_char = std::is_same_v<std::remove_cvref_t<T>, sys_string_t<Storage>> ||
                                 std::is_same_v<std::remove_cvref_t<T>, char32_t>;


    template<class Storage> struct utf_access_traits_for<sys_string_t<Storage>>
        { using type = typename sys_string_t<Storage>::utf_view_traits; };

    template<class Storage>
    class sys_string_t : public Storage
    {
    friend std::hash<sys_string_t>;
    friend typename Storage::char_access;
    private:
        using storage = Storage;

    public:
        using size_type = typename storage::size_type;
        using storage_type = typename storage::storage_type;
        using hash_type = typename storage::hash_type;
        
        static constexpr size_type max_size = storage::max_size;
        
        using char_access = typename storage::char_access;

        struct utf_view_traits
        {
            using stored_reference = typename Storage::char_access;
            
            static constexpr bool enable_view = false;
            static constexpr bool enable_borrowed_range = false;
            
            static constexpr const sys_string_t & adapt(const sys_string_t & str) noexcept
                { return str; }
            static const stored_reference & access(const stored_reference & acc) noexcept
                { return acc; }
        };
        
        template<utf_encoding Enc>
        using utf_view = utf_view<Enc, sys_string_t>;
        
        using utf8_view  = utf_view<utf8>;
        using utf16_view = utf_view<utf16>;
        using utf32_view = utf_view<utf32>;
        
        using compare_result = std::strong_ordering;
        static constexpr compare_result ordering_less = std::strong_ordering::less;
        static constexpr compare_result ordering_equal = std::strong_ordering::equal;
        static constexpr compare_result ordering_greater = std::strong_ordering::greater;
        
    public:
        sys_string_t() noexcept = default;
        sys_string_t(const sys_string_t &) noexcept = default;
        sys_string_t(sys_string_t &&) noexcept = default;
        sys_string_t & operator=(const sys_string_t &) noexcept = default;
        sys_string_t & operator=(sys_string_t &&) noexcept = default;
        ~sys_string_t() noexcept = default;
        
        using storage::storage;
     

        template<has_utf_encoding Char>
        sys_string_t(const Char * str, size_t len) :
            storage(str, len)
        {}
        
        template<has_utf_encoding Char>
        sys_string_t(const Char * str) :
            sys_string_t(str, str ? std::char_traits<Char>::length(str) : 0)
        {}
        
        template<has_utf_encoding Char>
        sys_string_t(Char val) :
            sys_string_t(&val, 1)
        {}
    
        template<std::ranges::contiguous_range Range>
        requires(has_utf_encoding<std::ranges::range_value_t<Range>>)
        sys_string_t(Range && val) :
            sys_string_t(std::ranges::data(std::forward<Range>(val)), std::ranges::size(std::forward<Range>(val)))
        {}

        sys_string_t(const typename utf32_view::iterator & first, const typename utf32_view::iterator & last):
            sys_string_t(first.storage_current(), last.storage_current())
        {}

        sys_string_t(const typename utf32_view::iterator & first, std::default_sentinel_t):
            sys_string_t(first.storage_current(), first.storage_last())
        {}
        
        template<std::sized_sentinel_for<typename char_access::iterator> EndIt>
        sys_string_t(const typename char_access::iterator & first, const EndIt & last):
            sys_string_t(first, last - first)
        {}

        template<std::sized_sentinel_for<typename char_access::reverse_iterator> EndIt>
        sys_string_t(const typename char_access::reverse_iterator & first, const EndIt & last):
            sys_string_t(first.base() - (last - first), first.base())
        {}

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
        

        friend auto operator<=>(const sys_string_t & lhs, const sys_string_t & rhs) noexcept -> std::strong_ordering
            { return compare(lhs, rhs); }
        
        
        friend auto compare(const sys_string_t & lhs, const sys_string_t & rhs) noexcept -> compare_result
            { return sys_string_t::compare(lhs, rhs); }
        friend auto compare_no_case(const sys_string_t lhs, const sys_string_t & rhs) noexcept -> compare_result
            { return sys_string_t::compare_no_case(lhs, rhs); }
        
        template<sys_string_or_char<Storage> StringOrChar1, sys_string_or_char<Storage> StringOrChar2>
        friend auto operator+(const StringOrChar1 lhs, const StringOrChar2 rhs) -> util::addition<Storage, StringOrChar1, StringOrChar2>
            { return util::addition<Storage, StringOrChar1, StringOrChar2>(std::move(lhs), std::move(rhs)); }
        
        auto to_lower() const -> sys_string_t;
        auto to_upper() const -> sys_string_t;

        template<std::predicate<char32_t> Pred = isspace>
        auto trim(Pred pred = Pred()) const -> sys_string_t;
        template<std::predicate<char32_t> Pred = isspace>
        auto ltrim(Pred pred = Pred()) const -> sys_string_t;
        template<std::predicate<char32_t> Pred = isspace>
        auto rtrim(Pred pred = Pred()) const -> sys_string_t;

        template<class Search, class OutIt>
        auto split(OutIt dest, Search search) const -> OutIt
        requires(std::output_iterator<OutIt, sys_string_t> &&
                 std::is_invocable_v<Search, typename sys_string_t::utf32_view::iterator, std::default_sentinel_t>);

        template<sys_string_or_char<Storage> StringOrChar, class OutIt>
        auto split(OutIt dest, const StringOrChar & sep, size_t max_split = std::numeric_limits<size_t>::max()) const -> OutIt
        requires(std::output_iterator<OutIt, sys_string_t>);

        template<std::forward_iterator FwdIt>
        auto join(FwdIt first, FwdIt last) const -> sys_string_t;

        template<sys_string_or_char<Storage> StringOrChar>
        auto starts_with(const StringOrChar & prefix) const -> bool;

        template<std::input_iterator InIt>
        auto find_prefix(InIt first, InIt last) const -> InIt;

        template<sys_string_or_char<Storage> StringOrChar>
        auto remove_prefix(const StringOrChar & prefix) const -> sys_string_t;
        
        template<sys_string_or_char<Storage> StringOrChar>
        auto ends_with(const StringOrChar & suffix) const -> bool;
        
        template<std::input_iterator InIt>
        auto find_suffix(InIt first, InIt last) const -> InIt;

        template<sys_string_or_char<Storage> StringOrChar>
        auto remove_suffix(const StringOrChar & prefix) const -> sys_string_t;
        
        template<sys_string_or_char<Storage> StringOrChar>
        auto contains(const StringOrChar & inner) const -> bool;

        template<std::input_iterator InIt>
        auto find_contained(InIt first, InIt last) const -> InIt;

        template<sys_string_or_char<Storage> StringOrChar1, sys_string_or_char<Storage> StringOrChar2>
        auto replace(const StringOrChar1 & old, const StringOrChar2 & new_,
                     size_t max_count = std::numeric_limits<size_t>::max()) const -> sys_string_t;

        template<sys_string_or_char<Storage> StringOrChar>
        auto suffix_after_first(const StringOrChar & divider) const -> std::optional<sys_string_t>;
        template<sys_string_or_char<Storage> StringOrChar>
        auto prefix_before_first(const StringOrChar & divider) const -> std::optional<sys_string_t>;
        template<sys_string_or_char<Storage> StringOrChar>
        auto suffix_after_last(const StringOrChar & divider) const -> std::optional<sys_string_t>;
        template<sys_string_or_char<Storage> StringOrChar>
        auto prefix_before_last(const StringOrChar & divider) const -> std::optional<sys_string_t>;

        template<sys_string_or_char<Storage> StringOrChar>
        auto partition_at_first(const StringOrChar & divider) const -> std::optional<std::pair<sys_string_t, sys_string_t>>;
        template<sys_string_or_char<Storage> StringOrChar>
        auto partition_at_last(const StringOrChar & divider) const -> std::optional<std::pair<sys_string_t, sys_string_t>>;

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
