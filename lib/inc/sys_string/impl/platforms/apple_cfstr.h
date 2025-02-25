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

#include <sys_string/impl/util/char_vector.h>
#include <sys_string/impl/util/generic_impl.h>

#include <variant>

namespace sysstr
{
    class cf_storage;
}

namespace sysstr::util
{
    struct cf_traits
    {
        using storage_type = char16_t;
        using size_type = CFIndex;
        using hash_type = CFHashCode;
        using native_handle_type = CFStringRef;
        
        static constexpr size_type max_size = std::numeric_limits<CFIndex>::max() / sizeof(UniChar);
    };
    
    inline CFStringRef check_create(CFStringRef src)
    {
        if (!src)
            throw std::runtime_error("CFString creation failed");
        return src;
    }
    
    class cf_builder_storage
    {
    public:
        using value_type = cf_traits::storage_type;
        using size_type = cf_traits::size_type;
        
        class dynamic_t
        {
        public:
            dynamic_t() noexcept = default;
            dynamic_t(size_t size):
                m_ptr((value_type *)CFAllocatorAllocate(nullptr, size * sizeof(value_type), 0))
            {
                if (!m_ptr)
                    throw std::bad_alloc();
            }
            dynamic_t(const dynamic_t &) = delete;
            dynamic_t(dynamic_t && src) noexcept :
                m_ptr(src.m_ptr)
            { src.m_ptr = nullptr; }
            ~dynamic_t() noexcept
            {
                if (m_ptr)
                    CFAllocatorDeallocate(nullptr, m_ptr);
            }
            dynamic_t & operator=(const dynamic_t &) = delete;
            dynamic_t & operator=(dynamic_t && rhs) noexcept
            {
                if (this != &rhs)
                {
                    if (m_ptr)
                        CFAllocatorDeallocate(nullptr, m_ptr);
                    m_ptr = rhs.m_ptr;
                    rhs.m_ptr = nullptr;
                }
                return *this;
            }
            
            constexpr value_type * data() const noexcept
                { return m_ptr; }
            
            void reallocate(size_type size)
            {
                auto result = (value_type *)CFAllocatorReallocate(nullptr, m_ptr, size * sizeof(value_type), 0);
                if (!result)
                    throw std::bad_alloc();
                m_ptr = result;
            }
            
            value_type * release() noexcept
            {
                auto ret = m_ptr;
                m_ptr = nullptr;
                return ret;
            }
        private:
            value_type * m_ptr = nullptr;
        };
        
        static constexpr size_type minimum_capacity = 64;
        
        using static_t = std::array<value_type, minimum_capacity>;
        
        using buffer_t = std::variant<static_t, dynamic_t>;
        
    public:
        constexpr size_type capacity() const noexcept
            { return m_capacity; }
        constexpr value_type * data() const noexcept
        {
            return std::visit([](const auto & val) {
                return const_cast<value_type *>(val.data());
            }, m_buffer);
        }
        static constexpr size_type max_size() noexcept
            { return cf_traits::max_size; }
        
        void reallocate(size_type size, size_type used_size)
        {
            struct reallocator
            {
                cf_builder_storage * me;
                size_type size;
                size_type used_size;
                
                void operator()(dynamic_t & buf) const
                {
                    buf.reallocate(size);
                    me->m_capacity = size;
                }
                
                void operator()(static_t & buf) const
                {
                    if (size > minimum_capacity)
                    {
                        dynamic_t new_buf(size);
                        memcpy(new_buf.data(), buf.data(), used_size * sizeof(value_type));
                        me->m_buffer = std::move(new_buf);
                        me->m_capacity = size;
                    } else {
                        me->m_capacity = minimum_capacity;
                    }
                }
            };
            
            std::visit(reallocator{this, size, used_size}, m_buffer);
        }
        
        buffer_t release() noexcept
        {
            this->m_capacity = minimum_capacity;
            buffer_t ret = std::move(m_buffer);
            m_buffer.emplace<static_t>();
            return ret;
        }
    private:
        buffer_t m_buffer;
        size_type m_capacity = minimum_capacity;
    };

    using cf_builder_impl = char_vector<cf_builder_storage>;

    inline CFStringRef convert_to_string(cf_builder_impl & builder) noexcept
    {
        struct converter
        {
            cf_traits::size_type size;
            
            CFStringRef operator()(cf_builder_storage::dynamic_t && buf) const
            {
                CFStringRef str = size ? check_create(CFStringCreateWithCharactersNoCopy(nullptr, (const UniChar *)buf.data(), size, nullptr)) :
                                        nullptr;
                buf.release();
                return str;
            }
            
            CFStringRef operator()(cf_builder_storage::static_t && buf) const
            {
                return size ? check_create(CFStringCreateWithCharacters(nullptr, (const UniChar *)buf.data(), size)) :
                                        nullptr;
            }
        };
        return std::visit(converter{builder.size()}, builder.release());
    }

    class cf_char_access
    {
    private:
        enum buffer_type
        {
            direct_unichar,
            direct_cstring,
            indirect
        };
    public:
        using value_type = cf_traits::storage_type;
        using size_type = cf_traits::size_type;
        using reference = value_type;
        using pointer = void;
        
        using iterator = index_iterator<const cf_char_access, iter_direction::forward>;
        using const_iterator = iterator;
        using reverse_iterator = index_iterator<const cf_char_access, iter_direction::backward>;
        using const_reverse_iterator = reverse_iterator;
    public:
        cf_char_access(const sys_string_t<cf_storage> & src) noexcept;
        ~cf_char_access() noexcept
            { if (m_c_str) delete [] m_c_str; }
        
        cf_char_access(const cf_char_access & src) noexcept = delete;
        cf_char_access(cf_char_access && src) noexcept = delete;
        cf_char_access & operator=(const cf_char_access & src) = delete;
        cf_char_access & operator=(cf_char_access && src) = delete;

        CFStringRef get_string() const noexcept
            { return m_string ? m_string : CFSTR(""); }


        value_type operator[](CFIndex idx) const noexcept
        {
            assert (idx >= 0 && idx < m_size);
            
            if (m_direct_unichar_buffer)
                return m_direct_unichar_buffer[idx];
            if (m_direct_cstring_buffer)
                return value_type(m_direct_cstring_buffer[idx]);
                
            if (idx >= m_end || idx < m_start)
                fill(idx);
            return m_buffer[idx - m_start];
        }
        
        size_type size() const noexcept
            { return m_size; }
        
        iterator begin() const noexcept
            { return iterator(*this, 0); }
        std::default_sentinel_t end() const noexcept
            { return std::default_sentinel; }
        const_iterator cbegin() const noexcept
            { return begin(); }
        std::default_sentinel_t cend() const noexcept
            { return end(); }
        reverse_iterator rbegin() const noexcept
            { return reverse_iterator(*this, m_size); }
        std::default_sentinel_t rend() const noexcept
            { return std::default_sentinel; }
        const_reverse_iterator crbegin() const noexcept
            { return rbegin(); }
        std::default_sentinel_t crend() const noexcept
            { return rend(); }
        
        const char * c_str() const noexcept
        {
            if (m_c_str)
                return m_c_str;
            auto count = utf_converter<utf16, utf8>::converted_length(begin(), end());
            auto ret = new char[count + 1];
            utf_converter<utf16, utf8>::convert(begin(), end(), ret);
            ret[count] = 0;
            m_c_str = ret;
            return ret;
        }
        
        friend bool operator==(const cf_char_access & lhs, const cf_char_access & rhs) noexcept
            { return lhs.m_string == rhs.m_string; }
    private:
        void fill(CFIndex idx) const
        {
            if ((m_start = idx - 4) < 0)
                m_start = 0;
            m_end = m_start + std::size(m_buffer);
            if (m_end > m_size)
                m_end = m_size;
            CFStringGetCharacters(m_string, CFRangeMake(m_start, m_end - m_start), m_buffer);
        }
    private:
        CFStringRef m_string = nullptr;
        mutable UniChar m_buffer[64];
        const UniChar * m_direct_unichar_buffer = nullptr;
        const char * m_direct_cstring_buffer = nullptr;
        mutable const char * m_c_str = nullptr;
        mutable CFIndex m_start = 0;
        mutable CFIndex m_end = 0;
        CFIndex m_size = 0;
    };
}

namespace sysstr
{
    class cf_storage
    {
    public:
        using size_type = util::cf_traits::size_type;
        using storage_type = util::cf_traits::storage_type;
        using native_handle_type = util::cf_traits::native_handle_type;
        using hash_type = util::cf_traits::hash_type;
        using char_access = util::cf_char_access;
        
        using builder_impl = util::cf_builder_impl;
        
        static constexpr size_type max_size = util::cf_traits::max_size;
    public:
        cf_storage() noexcept = default;
        
        cf_storage(native_handle_type str, handle_retain retain_handle = handle_retain::yes) noexcept :
            m_str(retain_handle == handle_retain::yes ? retain(str) : str)
        {}
        
#ifdef __OBJC__
        cf_storage(NSString * str) noexcept :
            cf_storage((__bridge CFStringRef)str, handle_retain::yes)
        {}
#endif
        
    protected:
        
        cf_storage(native_handle_type src, size_type first, size_type last) :
            m_str(src ? util::check_create(CFStringCreateWithSubstring(nullptr, src, {first, last - first})) : nullptr)
        {}
        
        cf_storage(const cf_storage & src, size_type first, size_type last) :
            cf_storage(src.m_str, first, last)
        {}
        
        template<class Char>
        cf_storage(const Char * str, size_t len);
        
        template<>
        cf_storage(const char * str, size_t len):
            cf_storage(buffer_from(str, len), handle_retain::no)
        {}
        
        template<>
        cf_storage(const char8_t * str, size_t len):
            cf_storage(buffer_from((const char *)str, len), handle_retain::no)
        {}
        
        template<>
        cf_storage(const char16_t * str, size_t len) :
            m_str(util::check_create(
                CFStringCreateWithCharacters(nullptr, (const UniChar *)str, len)))
        {}

        template<>
        cf_storage(const char32_t * str, size_t len):
            cf_storage(buffer_from(str, len), handle_retain::no)
        {}

        cf_storage(const char_access::iterator & first, size_type length):
            cf_storage(first.container() ? first.container()->get_string() : nullptr, first.index(), first.index() + length)
        {}
        
        ~cf_storage() noexcept
            { release(m_str); }

        cf_storage(const cf_storage & src) noexcept : m_str(retain(src.m_str))
        {}

        cf_storage(cf_storage && src) noexcept : m_str(src.m_str)
        {
            src.m_str = nullptr;
        }

        auto operator=(const cf_storage & rhs) noexcept -> cf_storage &
        {
            CFStringRef temp = m_str;
            m_str = rhs.m_str;
            retain(m_str);
            release(temp);
            return *this;
        }

        inline auto operator=(cf_storage && rhs) noexcept -> cf_storage &
        {
            if (this != &rhs)
            {
                release(m_str);
                m_str = rhs.m_str;
                rhs.m_str = nullptr;
            }
            return *this;
        }
        
        auto swap(cf_storage & other) noexcept -> void
        {
            using std::swap;
            swap(m_str, other.m_str);
        }
        
    public:
        
        auto cf_str() const noexcept -> native_handle_type
            { return m_str; }
        
        #ifdef __OBJC__
            auto ns_str() const noexcept -> NSString *
            { return (__bridge NSString *)m_str; }
        #endif
        
        auto data() const noexcept -> const storage_type *
            { return m_str ? (const storage_type *)CFStringGetCharactersPtr(m_str): nullptr; }

        auto copy_data(size_type idx, storage_type * buf, size_type buf_size) const noexcept -> size_type
        {
            if (m_str)
            {
                size_type ret = std::min(buf_size, CFStringGetLength(m_str) - idx);
                CFStringGetCharacters(m_str, {idx, ret}, (UniChar *)buf);
                return ret;
            }
            return 0;
        }
        
    protected:

        auto size() const noexcept -> size_type
            { return m_str ? CFStringGetLength(m_str):  0; }

        auto operator[](size_type idx) const noexcept -> storage_type
            { return CFStringGetCharacterAtIndex(m_str, idx); }

    private:
        static CFStringRef retain(CFStringRef src) noexcept
        {
            if (src)
                CFRetain(src);
            return src;
        }
        static void release(CFStringRef src) noexcept
        {
            if (src)
                CFRelease(src);
        }
        
        template<class Char>
        static CFStringRef buffer_from(const Char * str, size_t len)
        {
            using converter = utf_converter<utf_encoding_of<Char>, utf16>;
            builder_impl buf;
            size_t utf16_count = converter::converted_length(str, str + len);
            if (utf16_count > size_t(max_size))
                throw std::bad_alloc();
            buf.resize(utf16_count);
            converter::convert(str, str + len, buf.begin());
//            converter::convert(str, str + len, std::back_inserter(buf));
            return convert_to_string(buf);
        }
    private:
        CFStringRef m_str = nullptr;
    };

}

namespace sysstr::util
{
    inline cf_char_access::cf_char_access(const sys_string_t<cf_storage> & src) noexcept:
        m_string(src.cf_str())
    {
        if (m_string)
        {
            m_size = CFStringGetLength(m_string);
            if ( (m_direct_unichar_buffer = CFStringGetCharactersPtr(m_string)) )
                return;
            m_direct_cstring_buffer = CFStringGetCStringPtr(m_string, kCFStringEncodingASCII);
        }
    }
    
    template<>
    inline sys_string_t<cf_storage> build(cf_builder_impl & builder) noexcept
    {
        auto str = convert_to_string(builder);
        return sys_string_t<cf_storage>(str, handle_retain::no);
    }
}

namespace sysstr
{
    
    using sys_string_cfstr = sys_string_t<cf_storage>;
    using sys_string_cfstr_builder = sys_string_builder_t<cf_storage>;
}

namespace sysstr::util 
{
    inline auto make_static_sys_string_cfstr(CFStringRef str) noexcept -> sys_string_cfstr
    {
        return sys_string_cfstr(str, handle_retain::no);
    }
}

#define SYS_STRING_STATIC_CFSTR(x) ::sysstr::util::make_static_sys_string_cfstr(CFSTR(x))







