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

#include <sys_string/impl/util/char_buffer.h>
#include <sys_string/impl/util/generic_buffer.h>

#include <variant>

#include <CoreFoundation/CoreFoundation.h>

namespace sysstr
{

    struct sys_string_traits
    {
        using storage_type = char16_t;
        using size_type = CFIndex;
        using hash_type = CFHashCode;
        using native_handle_type = CFStringRef;
        
        static constexpr size_type max_size = std::numeric_limits<CFIndex>::max() / sizeof(UniChar);
    };
}

namespace sysstr::util
{
    inline CFStringRef check_create(CFStringRef src)
    {
        if (!src)
            throw std::runtime_error("CFString creation failed");
        return src;
    }
    
    struct apple_reallocator
    {
        void * alloc(size_t size)
        {
            if (auto ret = ::CFAllocatorAllocate(nullptr, size, 0))
                return ret;
            throw std::bad_alloc();
        }
        
        void free(void * p) noexcept
            { ::CFAllocatorDeallocate(nullptr, p); }
        
        void * realloc(void * p, size_t size)
        {
            if (auto ret = ::CFAllocatorReallocate(nullptr, p, size, 0))
                return ret;
            throw std::bad_alloc();
        }
    };
    
    class builder_storage
    {
    public:
        using value_type = sys_string_traits::storage_type;
        using size_type = sys_string_traits::size_type;
        
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
        constexpr value_type * buffer() const noexcept
        {
            return std::visit([](const auto & val) {
                return const_cast<value_type *>(val.data());
            }, m_buffer);
        }
        static constexpr size_type max_size() noexcept
            { return sys_string_traits::max_size; }
        
        void reallocate(size_type size, size_type used_size)
        {
            struct reallocator
            {
                size_type size;
                size_type used_size;
                
                size_t operator()(dynamic_t & buf) const
                {
                    buf.reallocate(size);
                    return size;
                }
                
                size_t operator()(static_t & buf) const
                {
                    if (size > minimum_capacity)
                    {
                        dynamic_t new_buf(size);
                        memcpy(new_buf.data(), buf.data(), used_size * sizeof(value_type));
                        return size;
                    }
                    return minimum_capacity;
                }
            };
            
            m_capacity = std::visit(reallocator{size, used_size}, m_buffer);
        }
        
        buffer_t release() noexcept
        {
            this->m_capacity = minimum_capacity;
            return std::move(m_buffer);
        }
    private:
        buffer_t m_buffer;
        size_type m_capacity = minimum_capacity;
    };

    using builder_impl = char_buffer<builder_storage>;

    inline CFStringRef convert_to_string(builder_impl & builder) noexcept
    {
        struct converter
        {
            sys_string_traits::size_type size;
            
            CFStringRef operator()(builder_storage::dynamic_t && buf) const
            {
                CFStringRef str = size ? check_create(CFStringCreateWithCharactersNoCopy(nullptr, (const UniChar *)buf.data(), size, nullptr)) :
                                        nullptr;
                buf.release();
                return str;
            }
            
            CFStringRef operator()(builder_storage::static_t && buf) const
            {
                return size ? check_create(CFStringCreateWithCharacters(nullptr, (const UniChar *)buf.data(), size)) :
                                        nullptr;
            }
        };
        return std::visit(converter{builder.size()}, builder.release());
    }

    sys_string build(builder_impl & builder) noexcept;

    class char_access
    {
    private:
        enum buffer_type
        {
            direct_unichar,
            direct_cstring,
            indirect
        };
    public:
        using value_type = sys_string_traits::storage_type;
        using size_type = sys_string_traits::size_type;
        using reference = value_type;
        using pointer = void;
        
        using cursor = index_cursor<const char_access, cursor_direction::forward>;
        using reverse_cursor = index_cursor<const char_access, cursor_direction::backward>;
        
        using iterator = cursor;
        using const_iterator = iterator;
        using reverse_iterator = reverse_cursor;
        using const_reverse_iterator = reverse_iterator;
    public:
        char_access(const sys_string & src) noexcept;
        ~char_access() noexcept
            { if (m_c_str) delete [] m_c_str; }
        
        char_access(const char_access & src) noexcept = delete;
        char_access(char_access && src) noexcept = delete;
        char_access & operator=(const char_access & src) = delete;
        char_access & operator=(char_access && src) = delete;

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
        
        template<cursor_direction Direction>
        auto cursor_begin() const noexcept -> index_cursor<const char_access, Direction>
            { return index_cursor<const char_access, Direction>(*this, bool(Direction) ? 0 : m_size); }

        template<cursor_direction Direction>
        auto cursor_end() const noexcept -> index_cursor<const char_access, Direction>
            { return index_cursor<const char_access, Direction>(*this, bool(Direction) ? m_size : 0); }
        
        iterator begin() const noexcept
            { return cursor_begin<cursor_direction::forward>(); }
        iterator end() const noexcept
            { return cursor_end<cursor_direction::forward>(); }
        const_iterator cbegin() const noexcept
            { return begin(); }
        const_iterator cend() const noexcept
            { return end(); }
        reverse_iterator rbegin() const noexcept
            { return cursor_begin<cursor_direction::backward>(); }
        reverse_iterator rend() const noexcept
            { return cursor_end<cursor_direction::backward>(); }
        const_reverse_iterator crbegin() const noexcept
            { return rbegin(); }
        const_reverse_iterator crend() const noexcept
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
        
        friend bool operator==(const char_access & lhs, const char_access & rhs) noexcept
            { return lhs.m_string == rhs.m_string; }
        friend bool operator!=(const char_access & lhs, const char_access & rhs) noexcept
            { return !(lhs == rhs); }
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

    class storage
    {
    public:
        using size_type = sys_string_traits::size_type;
        using storage_type = sys_string_traits::storage_type;
        using native_handle_type = sys_string_traits::native_handle_type;
    public:
        storage() noexcept = default;
        
        storage(native_handle_type str, handle_retain retain_handle = handle_retain::yes) noexcept :
            m_str(retain_handle == handle_retain::yes ? retain(str) : str)
        {}
        
        storage(native_handle_type src, size_type first, size_type last) :
            m_str(src ? check_create(CFStringCreateWithSubstring(nullptr, src, {first, last - first})) : nullptr)
        {}
        
        storage(const storage & src, size_type first, size_type last) :
            storage(src.m_str, first, last)
        {}
        
        storage(const char * str, size_type len):
            storage(buffer_from(str, len), handle_retain::no)
        {}

        storage(const char16_t * str, size_type len) :
            m_str(check_create(
                CFStringCreateWithCharacters(nullptr, (const UniChar *)str, len)))
        {}

        storage(const char32_t * str, size_type len):
            storage(buffer_from(str, len), handle_retain::no)
        {}
        
        ~storage() noexcept
            { release(m_str); }

        storage(const storage & src) noexcept : m_str(retain(src.m_str))
        {}

        storage(storage && src) noexcept : m_str(src.m_str)
        {
            src.m_str = nullptr;
        }

        auto operator=(const storage & rhs) noexcept -> storage &
        {
            CFStringRef temp = m_str;
            m_str = rhs.m_str;
            retain(m_str);
            release(temp);
            return *this;
        }

        inline auto operator=(storage && rhs) noexcept -> storage &
        {
            if (this != &rhs)
            {
                release(m_str);
                m_str = rhs.m_str;
                rhs.m_str = nullptr;
            }
            return *this;
        }
        
        auto swap(storage & other) noexcept -> void
        {
            using std::swap;
            swap(m_str, other.m_str);
        }
        
        auto native_handle() const noexcept -> native_handle_type
            { return m_str; }
        
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
        static CFStringRef buffer_from(const Char * str, size_type len)
        {
            using converter = utf_converter<utf_encoding_of<Char>, utf16>;
            builder_impl buf;
            size_t utf16_count = converter::converted_length(str, str + len);
            buf.resize(utf16_count);
            converter::convert(str, str + len, buf.begin());
//            converter::convert(str, str + len, std::back_inserter(buf));
            return convert_to_string(buf);
        }
    private:
        CFStringRef m_str = nullptr;
    };

}






