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

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <winstring.h>

namespace sysstr
{

    struct sys_string_traits
    {
        using storage_type = char16_t;
        using size_type = UINT32;
        using hash_type = size_t;
        using native_handle_type = HSTRING;

        static constexpr size_type max_size = std::numeric_limits<UINT32>::max() / sizeof(char16_t);
    };
}

namespace sysstr::util
{
    //Taken from: https://github.com/microsoft/cppwinrt/blob/master/strings/base_string.h
    struct hstring_header
    {
        uint32_t flags;
        uint32_t length;
        uint32_t padding1;
        uint32_t padding2;
        const char16_t * ptr;
    };

    /*struct dynamic_hstring_header : hstring_header
    {
        std::atomic<int32_t> count;
        char16_t buffer[0];
    };*/

    //HSTRING needs to be 4 byte aligned. We can use the bottom bit to indicate static allocation by us.
    static constexpr uintptr_t hstring_static_allocation_bit = 0b1;

    class builder_storage
    {
    public:
        using value_type = sys_string_traits::storage_type;
        using size_type = sys_string_traits::size_type;

        constexpr builder_storage() noexcept = default;

        builder_storage(const builder_storage & src) = delete;
        builder_storage & operator=(const builder_storage & src) = delete;

        constexpr builder_storage(builder_storage && src) noexcept:
            m_handle(src.m_handle),
            m_buffer(src.m_buffer),
            m_capacity(src.m_capacity)
        {
            src.m_handle = nullptr;
            src.m_buffer = nullptr;
            src.m_capacity = 0;
        }
        constexpr builder_storage & operator=(builder_storage && src) noexcept
        {
            if (this != &src)
            {
                if (m_handle) 
                    WindowsDeleteStringBuffer(m_handle);
                m_handle = src.m_handle;
                m_buffer = src.m_buffer;
                m_capacity = src.m_capacity;
                
                src.m_handle = nullptr;
                src.m_buffer = nullptr;
                src.m_capacity = 0;
            }
            return *this;

        }

        ~builder_storage() noexcept
        {
            if (m_handle) 
                WindowsDeleteStringBuffer(m_handle); 
        }

        constexpr size_type capacity() const noexcept
            { return m_capacity; }
        constexpr value_type * buffer() const noexcept
            { return m_buffer; }
        static constexpr size_type max_size() noexcept
            { return sys_string_traits::max_size; }

        void reallocate(size_type size, size_type used_size)
        {
            value_type * new_buffer;
            HSTRING_BUFFER new_handle;
            auto res = WindowsPreallocateStringBuffer(size, (WCHAR**)&new_buffer, &new_handle);
            if (FAILED(res))
            {
                if (res == MEM_E_INVALID_SIZE || res == E_OUTOFMEMORY)
                    throw std::bad_alloc();
                std::terminate();
            }
            if (m_buffer)
                memcpy(new_buffer, m_buffer, used_size * sizeof(char16_t));
            if (m_handle)
                WindowsDeleteStringBuffer(m_handle); 
            m_handle = new_handle;
            m_buffer = new_buffer;
            m_capacity = size;
        }

        HSTRING_BUFFER release() noexcept
        {
            HSTRING_BUFFER ret = m_handle;
            m_handle = nullptr;
            m_buffer = nullptr;
            m_capacity = 0;
            return ret;
        }
    private:
        HSTRING_BUFFER m_handle = nullptr;
        value_type * m_buffer = nullptr;
        size_type m_capacity = 0;
    };

    using builder_impl = char_buffer<builder_storage>;

    inline HSTRING convert_to_string(builder_impl & builder) noexcept
    {
        [[maybe_unused]] auto capacity = builder.capacity();
        auto size = builder.size();
        HSTRING_BUFFER handle = builder.release();
        HSTRING ret;
        auto res = WindowsPromoteStringBuffer(handle, &ret);
        if (FAILED(res))
            std::terminate();
        if (ret)
        { 
            auto header = (hstring_header *)ret;
            assert(header->length == capacity);
            header->length = size;
        }
        return ret;
    }

    sys_string build(builder_impl & builder) noexcept;

    class char_access
    {
    public:
        using value_type = sys_string_traits::storage_type;
        using size_type = sys_string_traits::size_type;
        using reference = const value_type &;
        using pointer = const value_type *;
        
        using iterator = const value_type *;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<const value_type *>;
        using const_reverse_iterator = reverse_iterator;

        using cursor = iter_cursor<const char_access, true>;
        using reverse_cursor = iter_cursor<const char_access, false>;
    public:
        char_access(const sys_string& src) noexcept;
        ~char_access() noexcept
            { if (m_c_str) delete [] m_c_str; }

        pointer data() const noexcept
            { return m_buffer; }
        size_type size() const noexcept
            { return m_size; }
        reference operator[](sys_string_traits::size_type idx) const noexcept
            { return m_buffer[idx]; }
        
        iterator begin() const noexcept
            { return iterator(this->m_buffer); }
        iterator end() const noexcept
            { return iterator(this->m_buffer + this->m_size); }
        const_iterator cbegin() const noexcept
            { return begin(); }
        const_iterator cend() const noexcept
            { return end(); }
        reverse_iterator rbegin() const noexcept
            { return reverse_iterator(this->m_buffer + this->m_size); }
        reverse_iterator rend() const noexcept
            { return reverse_iterator(this->m_buffer); }
        const_reverse_iterator crbegin() const noexcept
            { return rbegin(); }
        const_reverse_iterator crend() const noexcept
            { return rend(); }
        
        const wchar_t * c_wstr() const noexcept
            { return (const wchar_t *)data(); }
        
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
            { return lhs.m_buffer == rhs.m_buffer; }
        friend bool operator!=(const char_access & lhs, const char_access & rhs) noexcept
            { return !(lhs == rhs); }
    private:
        mutable const char * m_c_str = nullptr;
        pointer m_buffer;
        size_type m_size;
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
            m_str(create(src, first, last))
        {}

        storage(const storage & src, size_type first, size_type last) :
            storage(src.m_str, first, last)
        {}

        storage(const char * str, size_t len) :
            storage(buffer_from(str, len), handle_retain::no)
        {}

        storage(const char16_t * str, size_t len) :
            m_str(create(str, len))
        {}

        storage(const char32_t * str, size_t len) :
            storage(buffer_from(str, len), handle_retain::no)
        {}

        ~storage() noexcept
            { release(m_str); }

        storage(const storage & src) noexcept : m_str(retain(src.m_str))
        {}

        storage(storage && src) noexcept : m_str(src.m_str)
            { src.m_str = nullptr; }

        auto operator=(const storage & rhs) noexcept -> storage &
        {
            HSTRING temp = m_str;
            m_str = retain(rhs.m_str);
            release(temp);
            return *this;
        }

        auto operator=(storage && rhs) noexcept -> storage &
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
            { return real_handle(m_str); }

        auto data() const noexcept -> const storage_type *
        {
            size_type unused;
            return m_str ? (const storage_type*)WindowsGetStringRawBuffer(real_handle(m_str), &unused) : nullptr;
        }

        auto copy_data(size_type idx, storage_type* buf, size_type buf_size) const noexcept -> size_type
        {
            if (!m_str)
                return 0;
            size_type length;
            auto chars = (const storage_type*)WindowsGetStringRawBuffer(real_handle(m_str), &length);
            if (idx >= length)
                return 0;
            auto ret = std::min(buf_size, length - idx);
            memcpy(buf, chars + idx, ret * sizeof(char16_t));
            return ret;
        }

        auto size() const noexcept -> size_type
        {
            return m_str ? WindowsGetStringLen(real_handle(m_str)) : 0;
        }
    private:
        static bool is_static(HSTRING str) noexcept
            { return uintptr_t(str) & hstring_static_allocation_bit; }
        static HSTRING real_handle(HSTRING str) noexcept
            { return HSTRING(uintptr_t(str) & ~hstring_static_allocation_bit); }
        static HSTRING retain(HSTRING str) noexcept
        {
            if (!str || is_static(str))
                return str;
            HSTRING ret;
            auto res = WindowsDuplicateString(str, &ret);
            if (!SUCCEEDED(res))
                std::terminate();
            return ret;
        }
        static void release(HSTRING str) noexcept
        { 
            if (str && !is_static(str)) 
                WindowsDeleteString(str); 
        }

        static HSTRING create(const char16_t * str, size_t length)
        {
            if (length > sys_string_traits::max_size)
                throw std::bad_alloc();
            HSTRING ret;
            auto res = WindowsCreateString((PCNZWCH)str, size_type(length), &ret);
            if (SUCCEEDED(res))
                return ret;
            if (res == E_OUTOFMEMORY)
                throw std::bad_alloc();
            std::terminate();
        }
        static HSTRING create(HSTRING src, size_type first, size_type last)
        {
            HSTRING ret;
            auto res = WindowsSubstringWithSpecifiedLength(src, first, last - first, &ret);
            if (SUCCEEDED(res))
                return ret;
            if (res == E_OUTOFMEMORY)
                throw std::bad_alloc();
            std::terminate();
        }

        template<class Char>
        static HSTRING buffer_from(const Char * str, size_t len)
        {
            using converter = utf_converter<utf_encoding_of<Char>, utf16>;
            builder_impl buf;
         //   size_t utf16_count = converter::converted_length(str, str + len);
         //   if (utf16_count > sys_string_traits::max_size)
         //       throw std::bad_alloc();
         //   buf.resize(size_type(utf16_count));
         //   converter::convert(str, str + len, buf.begin());
            converter::convert(str, str + len, std::back_inserter(buf));
            return convert_to_string(buf);
        }
    private:
        HSTRING m_str = nullptr;
    };

}

