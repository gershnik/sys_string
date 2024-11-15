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
#include <cstdarg>


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


    template<class Storage>
    class sys_string_t : public Storage
    {
    friend std::hash<sys_string_t>;
    friend typename Storage::char_access;
    private:
        using storage = Storage;

        static_assert(std::ranges::random_access_range<typename storage::char_access>);

    public:
        using size_type = typename storage::size_type;
        using storage_type = typename storage::storage_type;
        using hash_type = typename storage::hash_type;
        
        static constexpr size_type max_size = storage::max_size;
        
        using char_access = typename storage::char_access;

        template<utf_encoding Enc>
        class utf_access
        {
        private:
            static constexpr auto source_encoding = utf_encoding_of<std::ranges::range_value_t<decltype(std::declval<char_access>())>>;
            
            using char_iterator = decltype(std::begin(std::declval<char_access>()));
            using char_sentinel = decltype(std::end(std::declval<char_access>()));
            using char_reverse_iterator = decltype(std::rbegin(std::declval<char_access>()));
            using char_reverse_sentinel = decltype(std::rend(std::declval<char_access>()));
        public:
            using iterator = util::utf_iterator<Enc, char_iterator, char_sentinel, util::iter_direction::forward>;
            using const_iterator = iterator;
            using reverse_iterator = util::utf_iterator<Enc, char_reverse_iterator, char_reverse_sentinel, util::iter_direction::backward>;
            using const_reverse_iterator = reverse_iterator;
            
            using value_type = typename iterator::value_type;
            using reference = typename iterator::reference;
            using const_reference = reference;
            using pointer = typename iterator::pointer;
            using const_pointer = pointer;
            
        public:
            utf_access(const sys_string_t & src) noexcept(noexcept(char_access(src))) :
                m_access(src)
            {}
            SYS_STRING_FORCE_INLINE iterator begin() const
                { return iterator(std::begin(m_access), std::end(m_access)); }
            SYS_STRING_FORCE_INLINE std::default_sentinel_t end() const
                { return std::default_sentinel; }
            SYS_STRING_FORCE_INLINE const_iterator cbegin() const
                { return begin(); }
            SYS_STRING_FORCE_INLINE std::default_sentinel_t cend() const
                { return end(); }
            SYS_STRING_FORCE_INLINE reverse_iterator rbegin() const 
                { return reverse_iterator(std::rbegin(m_access), std::rend(m_access)); }
            SYS_STRING_FORCE_INLINE std::default_sentinel_t rend() const 
                { return std::default_sentinel; }
            SYS_STRING_FORCE_INLINE const_reverse_iterator crbegin() const 
                { return rbegin(); }
            SYS_STRING_FORCE_INLINE std::default_sentinel_t crend() const 
                { return rend(); }

            reverse_iterator reverse(iterator it) const 
                { return reverse_iterator(it, std::rend(m_access)); }

            iterator reverse(reverse_iterator it) const 
                { return iterator(it, std::end(m_access)); }
            
            template<class Func>
            decltype(auto) each(Func func) const
                { return utf_converter<source_encoding, Enc>::for_each_converted(m_access, func); }
            
        private:
            char_access m_access;
        };
        using utf8_access  = utf_access<utf8>;
        using utf16_access = utf_access<utf16>;
        using utf32_access = utf_access<utf32>;
        
        template<utf_encoding Enc>
        using utf_view [[deprecated("use utf_access")]] = utf_access<Enc>;
        
        using utf8_view [[deprecated("use utf8_access")]] = utf8_access;
        using utf16_view [[deprecated("use utf16_access")]] = utf16_access;
        using utf32_view [[deprecated("use utf32_access")]] = utf32_access;

        using compare_result [[deprecated("use std::strong_ordering")]] = std::strong_ordering;
        static constexpr std::strong_ordering ordering_less [[deprecated("use std::strong_ordering::less")]] = std::strong_ordering::less;
        static constexpr std::strong_ordering ordering_equal [[deprecated("use std::strong_ordering::less")]] = std::strong_ordering::equal;
        static constexpr std::strong_ordering ordering_greater [[deprecated("use std::strong_ordering::less")]] = std::strong_ordering::greater;
        
        
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

        sys_string_t(const typename utf32_access::iterator & first, const typename utf32_access::iterator & last):
            sys_string_t(first.storage_current(), last.storage_current())
        {}

        sys_string_t(const typename utf32_access::iterator & first, std::default_sentinel_t):
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
    #if SYS_STRING_WCHAR_T_IS_UTF16 || SYS_STRING_WCHAR_T_IS_UTF32
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
        
        
        friend auto compare(const sys_string_t & lhs, const sys_string_t & rhs) noexcept -> std::strong_ordering
            { return sys_string_t::compare(lhs, rhs); }
        friend auto compare_no_case(const sys_string_t lhs, const sys_string_t & rhs) noexcept -> std::strong_ordering
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
                 std::is_invocable_v<Search, typename sys_string_t::utf32_access::iterator, std::default_sentinel_t>);

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
        static auto compare(const sys_string_t & lhs, const sys_string_t & rhs) noexcept -> std::strong_ordering;
        static auto compare_no_case(const sys_string_t lhs, const sys_string_t & rhs) noexcept -> std::strong_ordering;
        
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
