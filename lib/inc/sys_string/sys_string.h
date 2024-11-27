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

#if __has_include(<format>)
    #include <format>
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

    template<class T, class Storage>
    concept sys_string_or_char = std::is_same_v<std::remove_cvref_t<T>, sys_string_t<Storage>> ||
                                 std::is_same_v<std::remove_cvref_t<T>, char32_t>;

    template<class T, class Storage>
    concept builder_appendable = requires(sys_string_builder_t<Storage> builder, T t) {
        builder.append(t);
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
    template<class Storage>
    class sys_string_t : public Storage
    {
        friend std::hash<sys_string_t>;
        friend typename Storage::char_access;
    #if SYS_STRING_SUPPORTS_STD_FORMAT
        friend std::formatter<sysstr::sys_string_t<Storage>>;
    #if SYS_STRING_WCHAR_T_IS_UTF16 || SYS_STRING_WCHAR_T_IS_UTF32
        friend std::formatter<sysstr::sys_string_t<Storage>, wchar_t>;
    #endif
    #endif
    private:
        using storage = Storage;
    public:
        using size_type = typename storage::size_type;
        using storage_type = typename storage::storage_type;
        using hash_type = typename storage::hash_type;
        
        static constexpr size_type max_size = storage::max_size;
        
        using char_access = typename storage::char_access;
        static_assert(std::ranges::random_access_range<char_access>);

        template<utf_encoding Enc>
        class utf_access : private util::utf_view<Enc, char_access, util::byval>
        {
        private:
            using super = util::utf_view<Enc, char_access, util::byval>;
        public:
            using typename super::iterator;
            using typename super::const_iterator;
            using typename super::reverse_iterator;
            using typename super::const_reverse_iterator;
            
            using typename super::value_type;
            using typename super::reference;
            using typename super::const_reference;
            using typename super::pointer;
            using typename super::const_pointer;
            
        public:
            utf_access(const sys_string_t & src) noexcept(noexcept(super(src))) :
                super(src)
            {}

            using super::begin;
            using super::end;
            using super::cbegin;
            using super::cend;
            using super::rbegin;
            using super::rend;
            using super::crbegin;
            using super::crend;
            
            using super::reverse;
            using super::each;
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

        //Inherit any storage specific constructors
        
        using storage::storage;
     
        //Constructors from raw strings and character ranges

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
        sys_string_t(const Range & val) :
            sys_string_t(std::ranges::data(val), std::ranges::size(val))
        {}

        //Slice constructors from utf32_access::iterators

        sys_string_t(const typename utf32_access::iterator & first, const typename utf32_access::iterator & last):
            sys_string_t(first.storage_current(), last.storage_current())
        {}

        sys_string_t(const typename utf32_access::reverse_iterator & first, const typename utf32_access::reverse_iterator & last):
            sys_string_t(first.storage_current(), last.storage_current())
        {}

        sys_string_t(const typename utf32_access::iterator & first, std::default_sentinel_t):
            sys_string_t(first.storage_current(), first.storage_last())
        {}

        sys_string_t(const typename utf32_access::reverse_iterator & first, std::default_sentinel_t):
            sys_string_t(first.storage_current(), first.storage_last())
        {}

        template<std::ranges::range Range>
        requires(std::is_same_v<std::ranges::iterator_t<Range>, typename utf32_access::iterator> &&
                 (std::is_same_v<std::ranges::sentinel_t<Range>, typename utf32_access::iterator> ||
                  std::is_same_v<std::ranges::sentinel_t<Range>, std::default_sentinel_t>))
        sys_string_t(const Range & range):
            sys_string_t(std::ranges::begin(range), std::ranges::end(range))
        {}

        template<std::ranges::range Range>
        requires(std::is_same_v<std::ranges::iterator_t<Range>, typename utf32_access::reverse_iterator> &&
                 (std::is_same_v<std::ranges::sentinel_t<Range>, typename utf32_access::reverse_iterator> ||
                  std::is_same_v<std::ranges::sentinel_t<Range>, std::default_sentinel_t>))
        sys_string_t(const Range & range):
            sys_string_t(std::ranges::begin(range), std::ranges::end(range))
        {}

        //Slice constructors from char_access::iterators
        
        template<std::sized_sentinel_for<typename char_access::iterator> EndIt>
        sys_string_t(const typename char_access::iterator & first, const EndIt & last):
            sys_string_t(first, last - first)
        {}

        template<std::sized_sentinel_for<typename char_access::reverse_iterator> EndIt>
        sys_string_t(const typename char_access::reverse_iterator & first, const EndIt & last):
            sys_string_t(first.base() - (last - first), first.base())
        {}

        template<std::ranges::range Range>
        requires(std::is_same_v<std::ranges::iterator_t<Range>, typename char_access::iterator> &&
                 std::sized_sentinel_for<std::ranges::sentinel_t<Range>, typename char_access::iterator> &&
                 (!std::ranges::contiguous_range<Range> || !has_utf_encoding<std::ranges::range_value_t<Range>>))
        sys_string_t(const Range & range):
            sys_string_t(std::ranges::begin(range), std::ranges::end(range))
        {}

        template<std::ranges::range Range>
        requires(std::is_same_v<std::ranges::iterator_t<Range>, typename char_access::reverse_iterator> &&
                 std::sized_sentinel_for<std::ranges::sentinel_t<Range>, typename char_access::reverse_iterator>)
        sys_string_t(const Range & range):
            sys_string_t(std::ranges::begin(range), std::ranges::end(range))
        {}

        // Construct moving from underlying storage

        explicit sys_string_t(storage && st) noexcept : storage(std::move(st))
        {}
        
        auto storage_size() const noexcept -> size_type
            { return storage::size(); }
        
        using storage::data;
        using storage::copy_data;
        
        auto empty() const noexcept -> bool
            { return storage::size() == 0; }
        
        static 
    #ifdef __GNUC__
        __attribute__((format(printf, 1, 2)))
    #endif
        auto format(
    #ifdef _MSC_VER
            _Printf_format_string_
    #endif
            const char * spec, ...) -> sys_string_t;

        static 
    #ifdef __GNUC__
        __attribute__((format(printf, 1, 0)))
    #endif
        auto formatv(
    #ifdef _MSC_VER
            _Printf_format_string_
    #endif
            const char * spec, va_list vl) -> sys_string_t;

    #if SYS_STRING_SUPPORTS_STD_FORMAT
        template<class... Args>
        static auto std_format(std::format_string<Args...> fmt, Args &&... args) -> sys_string_t;
        static auto std_vformat(std::string_view fmt, std::format_args args) -> sys_string_t;
    #endif

        friend auto operator<<(std::ostream & str, const sys_string_t & val) -> std::ostream &
        {
            val.print_with([&](const auto & view) {
                std::ranges::copy(view, std::ostreambuf_iterator<char>(str));
            });
            return str;
        }
    #if SYS_STRING_WCHAR_T_IS_UTF16 || SYS_STRING_WCHAR_T_IS_UTF32
        friend auto operator<<(std::wostream & str, const sys_string_t & val) -> std::wostream &
        {
            val.wprint_with([&](const auto & view) {
                std::ranges::copy(view, std::ostreambuf_iterator<wchar_t>(str));
            });
            return str;
        }
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

        template<std::input_iterator It, std::sentinel_for<It> EndIt>
        requires(builder_appendable<std::iter_value_t<It>, Storage>)
        auto join(It first, EndIt last) const -> sys_string_t;

        template<std::ranges::input_range Range>
        requires(builder_appendable<std::ranges::range_value_t<Range>, Storage>)
        auto join(const Range & range) const -> sys_string_t
            { return this->join(std::ranges::begin(range), std::ranges::end(range)); }

        template<sys_string_or_char<Storage> StringOrChar>
        auto starts_with(const StringOrChar & prefix) const -> bool;

        template<std::input_iterator It, std::sentinel_for<It> EndIt>
        requires(sys_string_or_char<std::iter_value_t<It>, Storage>)
        auto find_prefix(It first, EndIt last) const -> It;

        template<std::ranges::input_range Range>
        requires(sys_string_or_char<std::ranges::range_value_t<Range>, Storage>)
        auto find_prefix(Range && range) const -> std::ranges::borrowed_iterator_t<Range>
            { return this->find_prefix(std::ranges::begin(range), std::ranges::end(range)); }

        template<sys_string_or_char<Storage> StringOrChar>
        auto remove_prefix(const StringOrChar & prefix) const -> sys_string_t;
        
        template<sys_string_or_char<Storage> StringOrChar>
        auto ends_with(const StringOrChar & suffix) const -> bool;
        
        template<std::input_iterator It, std::sentinel_for<It> EndIt>
        requires(sys_string_or_char<std::iter_value_t<It>, Storage>)
        auto find_suffix(It first, EndIt last) const -> It;

        template<std::ranges::input_range Range>
        requires(sys_string_or_char<std::ranges::range_value_t<Range>, Storage>)
        auto find_suffix(Range && range) const -> std::ranges::borrowed_iterator_t<Range>
            { return this->find_suffix(std::ranges::begin(range), std::ranges::end(range)); }

        template<sys_string_or_char<Storage> StringOrChar>
        auto remove_suffix(const StringOrChar & prefix) const -> sys_string_t;
        
        template<sys_string_or_char<Storage> StringOrChar>
        auto contains(const StringOrChar & inner) const -> bool;

        template<std::input_iterator It, std::sentinel_for<It> EndIt>
        requires(sys_string_or_char<std::iter_value_t<It>, Storage>)
        auto find_contained(It first, EndIt last) const -> It;

        template<std::ranges::input_range Range>
        requires(sys_string_or_char<std::ranges::range_value_t<Range>, Storage>)
        auto find_contained(Range && range) const -> std::ranges::borrowed_iterator_t<Range>
            { return this->find_contained(std::ranges::begin(range), std::ranges::end(range)); }

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

        template <std::invocable<std::string_view> Func>
        auto print_with(Func func) const -> decltype(func(std::string_view{}));

    #if SYS_STRING_WCHAR_T_IS_UTF16 || SYS_STRING_WCHAR_T_IS_UTF32
        template <std::invocable<std::wstring_view> Func>
        auto wprint_with(Func func) const -> decltype(func(std::wstring_view{}));
    #endif
    };

    namespace util 
    {

        template<utf_encoding Enc, class Storage>
        class builder_utf_view : private util::utf_view<Enc, typename Storage::builder_impl, util::byref>
        {
        private:
            using super = util::utf_view<Enc, typename Storage::builder_impl, util::byref>;
        public:
            using typename super::iterator;
            using typename super::const_iterator;
            using typename super::reverse_iterator;
            using typename super::const_reverse_iterator;
            
            using typename super::value_type;
            using typename super::reference;
            using typename super::const_reference;
            using typename super::pointer;
            using typename super::const_pointer;
            
        public:
            builder_utf_view(const sys_string_builder_t<Storage> & src) noexcept(noexcept(super(src.chars()))) :
                super(src.chars())
            {}
            builder_utf_view(sys_string_builder_t<Storage> && src) = delete;

            using super::begin;
            using super::end;
            using super::cbegin;
            using super::cend;
            using super::rbegin;
            using super::rend;
            using super::crbegin;
            using super::crend;
            
            using super::reverse;
            using super::each;
        };
    }
}

namespace std::ranges {
    template<sysstr::utf_encoding Enc, class Storage>
    constexpr bool enable_view<sysstr::util::builder_utf_view<Enc, Storage>> = true;

    template<sysstr::utf_encoding Enc, class Storage>
    constexpr bool enable_borrowed_range<sysstr::util::builder_utf_view<Enc, Storage>> = true;
}


namespace sysstr 
{

    template<class Storage>
    class sys_string_builder_t
    {
    private:
        using impl_type = typename Storage::builder_impl;
        static_assert(std::ranges::random_access_range<impl_type>);
        
    public:
        using size_type = typename impl_type::size_type;
        using value_type = char32_t;
        using storage_type = typename impl_type::value_type;

        template<utf_encoding Enc>
        using utf_view = util::builder_utf_view<Enc, Storage>;

        using utf8_view  = utf_view<utf8>;
        using utf16_view = utf_view<utf16>;
        using utf32_view = utf_view<utf32>;
        
        using iterator = typename utf32_view::iterator;
        using const_iterator = typename utf32_view::const_iterator;
        using reverse_iterator = typename utf32_view::reverse_iterator;
        using const_reverse_iterator = typename utf32_view::const_reverse_iterator;
        
    private:
        using const_storage_iterator = decltype(std::ranges::begin(std::declval<const impl_type &>()));
        using const_storage_sentinel = decltype(std::ranges::end(std::declval<const impl_type &>()));
        using const_storage_reverse_iterator = decltype(std::ranges::rbegin(std::declval<const impl_type &>()));
        using const_storage_reverse_sentinel = decltype(std::ranges::rend(std::declval<const impl_type &>()));

    public:

        void clear() noexcept
            { m_impl.clear(); }
        
        bool empty() const noexcept
            { return m_impl.empty(); }

        size_type storage_size() const noexcept
            { return m_impl.size(); }
        
        void reserve_storage(size_type size)
            { m_impl.reserve(size); }
        
        void resize_storage(size_type new_size)
            { m_impl.resize(new_size); }
        
        iterator begin() const noexcept
            { return iterator(std::ranges::begin(m_impl), std::ranges::end(m_impl)); }
        std::default_sentinel_t end() const noexcept
            { return std::default_sentinel; }
        const_iterator cbegin() const noexcept
            { return begin(); }
        std::default_sentinel_t cend() const noexcept
            { return end(); }
        reverse_iterator rbegin() const noexcept
            { return reverse_iterator(std::ranges::rbegin(m_impl), std::ranges::rend(m_impl)); }
        std::default_sentinel_t rend() const noexcept
            { return std::default_sentinel; }
        const_reverse_iterator crbegin() const noexcept
            { return rbegin(); }
        std::default_sentinel_t crend() const noexcept
            { return rend(); }

        reverse_iterator reverse(iterator it) const
            { return reverse_iterator(it, std::ranges::rend(this->m_impl)); }

        iterator reverse(reverse_iterator it) const
            { return iterator(it, std::ranges::end(this->m_impl)); }
        

        void push_back(char32_t c)
            { append_one(m_impl, c); }
        void pop_back() noexcept
        {
            auto it = rbegin();
            m_impl.erase(it.storage_next().base(), std::ranges::end(m_impl));
        }
        
        iterator insert(iterator where, char32_t c)
        {
            auto res = insert_one(m_impl, where.storage_current(), c);
            return iterator(res, std::ranges::end(m_impl));
        }

        iterator insert(std::default_sentinel_t, char32_t c)
        {
            auto res = insert_one(m_impl, std::ranges::end(m_impl), c);
            return iterator(res, std::ranges::end(m_impl));
        }

        template<has_utf_encoding Char>
        iterator insert(iterator where, const Char * str)
        {
            auto res = insert_many(m_impl, where.storage_current(), str, std::char_traits<Char>::length(str));
            return iterator(res, std::ranges::end(m_impl));
        }

        template<has_utf_encoding Char>
        iterator insert(std::default_sentinel_t, const Char * str)
        {
            auto res = insert_many(m_impl, std::ranges::end(m_impl), str, std::char_traits<Char>::length(str));
            return iterator(res, std::ranges::end(m_impl));
        }
        
        template<has_utf_encoding Char>
        iterator insert(iterator where, const Char * str, size_t len)
        {
            auto res = insert_many(m_impl, where.storage_current(), str, len);
            return iterator(res, std::ranges::end(m_impl));
        }

        template<has_utf_encoding Char>
        iterator insert(std::default_sentinel_t, const Char * str, size_t len)
        {
            auto res = insert_many(m_impl, std::ranges::end(m_impl), str, len);
            return iterator(res, std::ranges::end(m_impl));
        }

        template<std::ranges::contiguous_range Range>
        requires(has_utf_encoding<std::ranges::range_value_t<Range>>)
        iterator insert(iterator where, const Range & range)
        {
            auto res = insert_many(m_impl, where.storage_current(), std::ranges::data(range), std::ranges::size(range));
            return iterator(res, std::ranges::end(m_impl));
        }

        template<std::ranges::contiguous_range Range>
        requires(has_utf_encoding<std::ranges::range_value_t<Range>>)
        iterator insert(std::default_sentinel_t, const Range & range)
        {
            auto res = insert_many(m_impl, std::ranges::end(m_impl), std::ranges::data(range), std::ranges::size(range));
            return iterator(res, std::ranges::end(m_impl));
        }
        
        iterator erase(iterator where) noexcept
        {
            auto res = m_impl.erase(where.storage_current(), where.storage_next());
            return iterator(res, std::ranges::end(m_impl));
        }

        iterator erase(iterator first, iterator last) noexcept
        {
            auto res = m_impl.erase(first.storage_current(), last.storage_current());
            return iterator(res, std::ranges::end(m_impl));
        }

        iterator erase(iterator first, std::default_sentinel_t) noexcept
        {
            auto res = m_impl.erase(first.storage_current(), std::ranges::end(m_impl));
            return iterator(res, std::ranges::end(m_impl));
        }
        
        sys_string_builder_t & append(char32_t c)
            { append_one(m_impl, c); return *this; }

        template<has_utf_encoding Char>
        sys_string_builder_t & append(const Char * str, size_t len)
            { append_many(m_impl, str, len); return *this; }
        
        template<has_utf_encoding Char>
        sys_string_builder_t & append(const Char * str)
            { append_many(m_impl, str, std::char_traits<Char>::length(str)); return *this; }

        template<std::ranges::contiguous_range Range>
        requires(has_utf_encoding<std::ranges::range_value_t<Range>>)
        sys_string_builder_t & append(const Range & range)
            { append_many(m_impl, std::ranges::data(range), std::ranges::size(range)); return *this; }

        sys_string_builder_t & append(const sys_string_t<Storage> & str)
            { append_range(typename sys_string_t<Storage>::char_access(str)); return *this; }

        sys_string_t<Storage> build() noexcept
            { return util::build<Storage>(m_impl); }
        
        const impl_type & chars() const
            { return m_impl; }
        impl_type & chars()
            { return m_impl; }

    private:
        static size_type limit_size(size_t len)
        {
            if (len > Storage::max_size)
                throw std::bad_alloc();
            return size_type(len);
        }
        static void append_one(impl_type & impl, char32_t c);
        
        static typename impl_type::iterator insert_one(impl_type & impl, typename impl_type::iterator where, char32_t c);
        
        template<has_utf_encoding Char>
        static void append_many(impl_type & impl, const Char * str, size_t len);
        
        template<has_utf_encoding Char>
        static typename impl_type::iterator insert_many(impl_type & impl, typename impl_type::iterator pos, const Char * str, size_t len);
        
        template<std::ranges::input_range Range>
        void append_range(const Range & range);
        
    private:
        impl_type m_impl;
    };

    template<utf_encoding Enc, class Storage>
    struct utf_view_traits<Enc, sys_string_builder_t<Storage>>
    {
        static auto as_utf(const sys_string_builder_t<Storage> & builder) -> typename sys_string_builder_t<Storage>::template utf_view<Enc>
            { return typename sys_string_builder_t<Storage>::template utf_view<Enc>(builder); }

        static auto as_utf(sys_string_builder_t<Storage> && builder) -> typename sys_string_builder_t<Storage>::template utf_view<Enc> = delete;
    };
}


#include <sys_string/impl/platform.h>


#include <sys_string/impl/compare.h>
#include <sys_string/impl/hash.h>
#include <sys_string/impl/builder.h>
#include <sys_string/impl/addition.h>
#include <sys_string/impl/misc.h>
#include <sys_string/impl/format.h>
#include <sys_string/impl/print.h>


#undef HEADER_SYS_STRING_H_INSIDE

#if ! SYS_STRING_NO_S_MACRO

    #define S(x) SYS_STRING_STATIC(x)

#endif

#endif 
