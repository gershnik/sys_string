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
#include <ostream>

#if SYS_STRING_USE_SPACESHIP_OPERATOR
    #include <compare>
#endif
#if SYS_STRING_USE_SPAN
    #include <span>
#endif

#define HEADER_SYS_STRING_H_INSIDE

namespace sysstr
{
    class sys_string;

    enum class handle_retain
    {
        no,
        yes
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

    template<class StringOrChar>
    constexpr bool is_string_or_char = std::is_same_v<std::remove_cv_t<std::remove_reference_t<StringOrChar>>, sys_string> ||
                                       std::is_same_v<std::remove_cv_t<std::remove_reference_t<StringOrChar>>, char32_t>;

    template<class First, class Second>
    class addition;
}


#if (defined(__APPLE__) && defined(__MACH__))
    #include <sys_string/impl/platforms/apple_def.h>
#elif defined(__ANDROID__)
    #include <sys_string/impl/platforms/android_def.h>
#elif defined(_WIN32)
    #include <sys_string/impl/platforms/windows_def.h>
#elif defined(__linux__)
    #include <sys_string/impl/platforms/linux_def.h>
#else
    #error Unsupported platform
#endif

namespace sysstr
{
    class sys_string
    {
    friend std::hash<sys_string>;
    friend util::char_access;
    private:
        using storage = util::storage;
        
        struct view_traits
        {
            using char_access = util::char_access;
            
            static constexpr const sys_string & adapt(const sys_string & str) noexcept
                { return str; }
        };
    public:
        using size_type = storage::size_type;
        using storage_type = storage::storage_type;
        
        static constexpr size_type max_size = sys_string_traits::max_size;
        
        using char_access = util::char_access;
        
        using utf8_view  = utf_view<utf8, sys_string, view_traits>;
        using utf16_view = utf_view<utf16, sys_string, view_traits>;
        using utf32_view = utf_view<utf32, sys_string, view_traits>;
        
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
        sys_string() noexcept = default;
        sys_string(const sys_string &) noexcept = default;
        sys_string(sys_string &&) noexcept = default;
        sys_string & operator=(const sys_string &) noexcept = default;
        sys_string & operator=(sys_string &&) noexcept = default;
        ~sys_string() noexcept = default;
        
    #if (defined(__APPLE__) && defined(__MACH__))
        sys_string(CFStringRef str, handle_retain retain_handle = handle_retain::yes) noexcept :
            m_storage(str, retain_handle)
        {}
        
        #ifdef __OBJC__
        sys_string(NSString * str) noexcept :
            m_storage((__bridge CFStringRef)str, handle_retain::yes)
        {}
        #endif

        auto cf_str() const noexcept -> CFStringRef
            { return m_storage.native_handle(); }
        
        #ifdef __OBJC__
        auto ns_str() const noexcept -> NSString *
            { return (__bridge NSString *)m_storage.native_handle(); }
        #endif
        
    #elif defined(__ANDROID__)
        sys_string(JNIEnv * env, jstring str) : m_storage(env, str)
        {}
        
        sys_string(util::buffer && buffer) noexcept:  m_storage(std::move(buffer))
        {}
        
        auto make_jstring(JNIEnv * env) const -> jstring
            { return m_storage.make_jstring(env); }

    #elif defined(_WIN32)
        sys_string(HSTRING str, handle_retain retain_handle = handle_retain::yes) noexcept :
            m_storage(str, retain_handle)
        {}

        auto h_str() const noexcept -> HSTRING
            { return m_storage.native_handle(); }

        auto w_str() const noexcept -> const wchar_t *
            { return WindowsGetStringRawBuffer(m_storage.native_handle(), nullptr); }
        
    #elif defined(__linux__)
        
        sys_string(util::buffer && buffer) noexcept:  m_storage(std::move(buffer))
        {}

        auto c_str() const noexcept -> const char *
        { 
            auto ret = m_storage.data(); 
            return ret ? ret : "";
        }
        
    #endif

        template<class Char>
        sys_string(const Char * str, size_t len, std::enable_if_t<has_utf_encoding<Char>> * = nullptr) :
            m_storage(str, len)
        {}
        
        template<class Char>
        sys_string(const Char * str, std::enable_if_t<has_utf_encoding<Char>> * = nullptr) :
            sys_string(str, str ? std::char_traits<Char>::length(str) : 0)
        {}
        
        template<class Char>
        sys_string(char val, std::enable_if_t<has_utf_encoding<Char>> * = nullptr) :
            sys_string(&val, 1)
        {}
    
        template<class Char>
        sys_string(const std::basic_string<Char> & val, std::enable_if_t<has_utf_encoding<Char>> * = nullptr) :
            sys_string(val.data(), val.size())
        {}

        template<class Char>
        sys_string(const std::basic_string_view<Char> & val, std::enable_if_t<has_utf_encoding<Char>> * = nullptr) :
            sys_string(val.data(), val.size())
        {}
        
        template<class Char>
        sys_string(const std::vector<Char> & val, std::enable_if_t<has_utf_encoding<Char>> * = nullptr) :
            sys_string(val.data(), val.size())
        {}

    #if SYS_STRING_USE_SPAN
        template<class Char, std::size_t Extent>
        sys_string(const std::span<Char, Extent> & val, std::enable_if_t<has_utf_encoding<Char>> * = nullptr) :
            sys_string(val.data(), val.size())
        {}
    #endif
        
        
        sys_string(const utf32_view::iterator & first, const utf32_view::iterator & last):
            sys_string(first.storage_cursor(), last.storage_pos() - first.storage_pos())
        {}
        
        sys_string(const char_access::iterator & first, const char_access::iterator & last);

        sys_string(const char_access::cursor & src, size_type length);
        
        sys_string(const char_access::reverse_cursor & src, size_type length);
        
        auto storage_size() const noexcept -> size_type
            { return m_storage.size(); }
        
        auto data() const noexcept -> const storage_type *
            { return m_storage.data(); }

        auto copy_data(size_type idx, storage_type * buf, size_type buf_size) const noexcept -> size_type
            { return m_storage.copy_data(idx, buf, buf_size); }
        
        auto empty() const noexcept -> bool
            { return m_storage.size() == 0; }
        
        static auto format(const char * spec, ...) -> sys_string;
        static auto formatv(const char * spec, va_list vl) -> sys_string;

        friend auto operator<<(std::ostream & str, const sys_string & val) -> std::ostream &;
    #if defined(_WIN32) || defined(__STDC_ISO_10646__)
        friend auto operator<<(std::wostream & str, const sys_string & val) -> std::wostream &;
    #endif

        auto swap(sys_string & other) noexcept -> void
            { m_storage.swap(other.m_storage); }
        
        friend auto swap(sys_string & lhs, sys_string & rhs) noexcept -> void
            { lhs.swap(rhs); }
        
        friend auto operator==(const sys_string & lhs, const sys_string & rhs) noexcept -> bool
            { return compare(lhs, rhs) == 0; }
        friend auto operator!=(const sys_string & lhs, const sys_string & rhs) noexcept -> bool
            { return compare(lhs, rhs) != 0; }
        
    #if SYS_STRING_USE_SPACESHIP_OPERATOR

        friend auto operator<=>(const sys_string & lhs, const sys_string & rhs) noexcept -> std::strong_ordering
            { return compare(lhs, rhs); }
        
    #else

        friend auto operator<(const sys_string & lhs, const sys_string & rhs) noexcept -> bool
            { return compare(lhs, rhs) < 0; }
        friend auto operator<=(const sys_string & lhs, const sys_string & rhs) noexcept -> bool
            { return compare(lhs, rhs) <= 0; }
        friend auto operator>(const sys_string & lhs, const sys_string & rhs) noexcept -> bool
            { return compare(lhs, rhs) > 0; }
        friend auto operator>=(const sys_string & lhs, const sys_string & rhs) noexcept -> bool
            { return compare(lhs, rhs) >= 0; }
        
    #endif
        
        friend auto compare(const sys_string & lhs, const sys_string & rhs) noexcept -> compare_result
            { return sys_string::compare(lhs, rhs); }
        friend auto compare_no_case(const sys_string lhs, const sys_string & rhs) noexcept -> compare_result
            { return sys_string::compare_no_case(lhs, rhs); }
        
        template<class StringOrChar1, class StringOrChar2>
        friend auto operator+(const StringOrChar1 lhs, const StringOrChar2 rhs) ->
                std::enable_if_t<util::is_string_or_char<StringOrChar1> && util::is_string_or_char<StringOrChar2>,
                util::addition<StringOrChar1, StringOrChar2>>
            { return util::addition<StringOrChar1, StringOrChar2>(std::move(lhs), std::move(rhs)); }
        
        auto to_lower() const -> sys_string;
        auto to_upper() const -> sys_string;

        template<class Pred = isspace>
        auto trim(Pred pred = Pred()) const -> sys_string;
        template<class Pred = isspace>
        auto ltrim(Pred pred = Pred()) const -> sys_string;
        template<class Pred = isspace>
        auto rtrim(Pred pred = Pred()) const -> sys_string;

        template<class Search, class OutIt>
        auto split(OutIt dest, Search pred) const ->
            std::enable_if_t<std::is_invocable_v<Search, sys_string::utf32_view::iterator, sys_string::utf32_view::iterator>, OutIt>;
        template<class StringOrChar, class OutIt>
        auto split(OutIt dest, const StringOrChar & sep, size_t max_split = std::numeric_limits<size_t>::max()) const ->
            std::enable_if_t<util::is_string_or_char<StringOrChar>, OutIt>;

        template<class FwdIt>
        auto join(FwdIt first, FwdIt last) const -> sys_string;

        template<class StringOrChar>
        auto starts_with(const StringOrChar & prefix) const -> std::enable_if_t<util::is_string_or_char<StringOrChar>, bool>;
        template<class InIt>
        auto find_prefix(InIt first, InIt last) const -> InIt;

        template<class StringOrChar>
        auto remove_prefix(const StringOrChar & prefix) const -> std::enable_if_t<util::is_string_or_char<StringOrChar>, sys_string>;
        
        template<class StringOrChar>
        auto ends_with(const StringOrChar & suffix) const -> std::enable_if_t<util::is_string_or_char<StringOrChar>, bool>;
        template<class InIt>
        auto find_suffix(InIt first, InIt last) const -> InIt;

        template<class StringOrChar>
        auto remove_suffix(const StringOrChar & prefix) const -> std::enable_if_t<util::is_string_or_char<StringOrChar>, sys_string>;
        
        template<class StringOrChar>
        auto contains(const StringOrChar & inner) const -> std::enable_if_t<util::is_string_or_char<StringOrChar>, bool>;
        template<class InIt>
        auto find_contained(InIt first, InIt last) const -> InIt;

        template<class StringOrChar1, class StringOrChar2>
        auto replace(const StringOrChar1 & old, const StringOrChar2 & new_,
                     size_t max_count = std::numeric_limits<size_t>::max()) const ->
            std::enable_if_t<util::is_string_or_char<StringOrChar1> && util::is_string_or_char<StringOrChar2>, sys_string>;

    private:
        
    #if (defined(__APPLE__) && defined(__MACH__))
        sys_string(CFStringRef src, size_type first, size_type last): m_storage(src, first, last)
        {}
    #endif
        
        static auto compare(const sys_string & lhs, const sys_string & rhs) noexcept -> compare_result;
        static auto compare_no_case(const sys_string lhs, const sys_string & rhs) noexcept -> compare_result;
        
    private:
        util::storage m_storage;
    };


    auto operator<<(std::ostream & str, const sys_string & val) -> std::ostream &;

}

namespace std
{
    template<>
    struct hash<::sysstr::sys_string>
    {
        auto operator()(const ::sysstr::sys_string & val) const noexcept -> ::sysstr::sys_string_traits::hash_type;
    };
}



#if (defined(__APPLE__) && defined(__MACH__))
    #include <sys_string/impl/platforms/apple_impl.h>
#elif defined(__ANDROID__)
    #include <sys_string/impl/platforms/android_impl.h>
#elif defined(_WIN32)
    #include <sys_string/impl/platforms/windows_impl.h>
#elif defined(__linux__)
    #include <sys_string/impl/platforms/linux_impl.h>
#else
    #error Unsupported platform
#endif

#if ! SYS_STRING_NO_S_MACRO

    #define S(x) SYS_STRING_STATIC(x)

#endif


#include <sys_string/impl/compare.h>
#include <sys_string/impl/hash.h>
#include <sys_string/impl/builder.h>
#include <sys_string/impl/addition.h>
#include <sys_string/impl/misc.h>


#undef HEADER_SYS_STRING_H_INSIDE

#endif 
