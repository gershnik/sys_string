//
// Copyright 2021 Eugene Gershnik
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

namespace sysstr
{
    class bstr_storage;
}

namespace sysstr::util
{
    struct bstr_traits
    {
        using storage_type = char16_t;
        using size_type = UINT;
        using hash_type = size_t;
        using native_handle_type = BSTR;

        static constexpr size_type max_size = std::numeric_limits<UINT>::max() / sizeof(char16_t) - 1;
    };

    class dynamic_bstr
    {
    public:
        dynamic_bstr() noexcept = delete;
        dynamic_bstr(const dynamic_bstr &) = delete;
        dynamic_bstr & operator=(const dynamic_bstr &) = delete;

        void * operator new  (size_t count, std::align_val_t al) = delete;
        void * operator new[](size_t count, std::align_val_t al) = delete;
        void operator delete[](void * ptr) = delete;

        static dynamic_bstr* from_bstr(BSTR str) noexcept
            { return (dynamic_bstr*)((UINT*)str - 1); }

        static dynamic_bstr * allocate(UINT length)
        {
            BSTR res = SysAllocStringLen(nullptr, length);
            if (!res)
                throw std::bad_alloc();
            dynamic_bstr * ret = (dynamic_bstr *)((UINT*)res - 1);
            ret->m_chars[length] = 0;
            return ret;
        }

        static dynamic_bstr* reallocate(dynamic_bstr* original, UINT length)
        {
            BSTR val;
            if (original)
            {
                val = BSTR(original->m_chars);
                auto res = SysReAllocStringLen(&val, nullptr, length);
                if (!res)
                    throw std::bad_alloc();
            }
            else
            {
                val = SysAllocStringLen(nullptr, length);
                if (!val)
                    throw std::bad_alloc();
            }
            dynamic_bstr* ret = from_bstr(val);
            ret->m_chars[length] = 0;
            return ret;
        }

        void operator delete(void * ptr) noexcept
        { 
            if (ptr) 
                SysFreeString(BSTR(((dynamic_bstr *)ptr)->m_chars)); 
        }

        char16_t * chars() noexcept
            { return m_chars; }

        UINT size() const noexcept
            { return m_size / sizeof(char16_t); }

        void truncate_size(UINT size) noexcept
        {
            m_size = size * sizeof(char16_t);
            m_chars[size] = 0;
        }

    private:
        UINT m_size;
        char16_t m_chars[1];
    };

    //layout must match dynamic_bstr
    template<UINT N>
    struct static_bstr
    {
        UINT m_size;
        char16_t m_buf[N + 1];
    };

    class bstr_buffer
    {
    private:
        static constexpr size_t size_in_pointers = 3;
        
        union data
        {
        private:
            static constexpr UINT dynamic_flag = 0;
            static constexpr UINT embedded_flag = 1;
            static constexpr UINT static_flag = 2;
            
            struct
            {
                UINT value;
                std::byte unused[size_in_pointers * sizeof(void *) - sizeof(UINT)];
            } flags;
            struct
            {
                void * unused[size_in_pointers - 1];
                dynamic_bstr * ptr;
            } dynamic_data;
            struct embedded_data_t
            {
                UINT unused;
                UINT size;
                char16_t chars[(size_in_pointers * sizeof(dynamic_bstr *) - 2 * sizeof(UINT)) / sizeof(char16_t)];
            } embedded_data;
        public:
            static constexpr UINT max_small_length = std::extent_v<decltype(embedded_data_t::chars)> - 1;
        public:
            data() noexcept //initializes to garbage, must call init or otherwise initialize
            {}
            ~data() noexcept
            {
                if (flags.value == dynamic_flag)
                    delete(dynamic_data.ptr);
            }
            data(const data & src) noexcept
            {
                memcpy(this, &src, sizeof(data));
                if (flags.value == dynamic_flag && dynamic_data.ptr)
                {
                    dynamic_data.ptr = dynamic_bstr::allocate(src.dynamic_data.ptr->size());
                    memcpy(dynamic_data.ptr->chars(), src.dynamic_data.ptr->chars(), src.dynamic_data.ptr->size() * sizeof(char16_t));
                }
            }
            data(data && src) noexcept
            {
                memcpy(this, &src, sizeof(data));
                src.init();
            }
            data & operator=(const data & rhs) noexcept
            {
                data temp(rhs);
                return *this = std::move(temp);
            }
            data & operator=(data && rhs) noexcept
            {
                this->~data();
                new (this) data(std::move(rhs));
                return *this;
            }

            void swap(data & other) noexcept
            {
                data temp;
                memcpy(&temp, &other, sizeof(data));
                memcpy(&other, this, sizeof(data));
                memcpy(this, &temp, sizeof(m_data));
            }

            void init() noexcept
            {
                flags.value = embedded_flag;
                embedded_data.size = 0;
            }

            void init(dynamic_bstr * bstr, int /*disambiguator*/)
            { 
                flags.value = static_flag;
                dynamic_data.ptr = bstr;
            }

            void init(std::nullptr_t)
            { 
                flags.value = dynamic_flag;
                dynamic_data.ptr = nullptr;
            }

            char16_t * init(UINT length)
            {
                if (length <= max_small_length)
                {
                    flags.value = embedded_flag;
                    embedded_data.size = length * sizeof(char16_t);
                    embedded_data.chars[length] = 0;
                    return embedded_data.chars;
                }
                else
                {
                    auto * bstr = dynamic_bstr::allocate(length);
                    flags.value = dynamic_flag;
                    dynamic_data.ptr = bstr;
                    return dynamic_data.ptr->chars();
                }
            }

            void init(BSTR str, handle_transfer transfer_type)
            {
                if (str)
                {
                    auto src = dynamic_bstr::from_bstr(str);
                    if (transfer_type == copy_content)
                    {
                       dynamic_data.ptr = dynamic_bstr::allocate(src->size());
                       memcpy(dynamic_data.ptr->chars(), src->chars(), src->size() * sizeof(char16_t));
                    }
                    else
                    {
                        dynamic_data.ptr = src;
                    }
                }
                else
                {
                    dynamic_data.ptr = nullptr;
                }
                flags.value = dynamic_flag;
            }

            void reallocate(UINT size, UINT used_size)
            {
                assert(flags.value != static_flag);

                if (flags.value == embedded_flag)
                {
                    if (size > max_small_length)
                    {
                        auto * buf = dynamic_bstr::allocate(size);
                        memcpy(buf->chars(), embedded_data.chars, used_size * sizeof(char16_t));
                        dynamic_data.ptr = buf;
                        flags.value = dynamic_flag;
                    }
                }
                else
                {
                    if (size <= max_small_length)
                    {
                        if (dynamic_data.ptr)
                        {
                            memcpy(embedded_data.chars, dynamic_data.ptr->chars(), used_size * sizeof(char16_t));
                            delete dynamic_data.ptr;
                        }
                        flags.value = embedded_flag;
                        embedded_data.size = size * sizeof(char16_t);
                    }
                    else
                    {
                        dynamic_data.ptr = dynamic_bstr::reallocate(dynamic_data.ptr, size);
                    }
                }
            }

            BSTR release() noexcept
            {
                BSTR ret;
                if (flags.value == embedded_flag)
                {
                    ret = BSTR(dynamic_bstr::allocate(embedded_data.size / sizeof(char16_t))->chars()); //crash if this allocation throws!
                    memcpy(ret, embedded_data.chars, embedded_data.size);
                }
                else if (flags.value == static_flag)
                {
                    ret = BSTR(dynamic_bstr::allocate(dynamic_data.ptr->size())->chars()); //crash if this allocation throws!
                    memcpy(ret, dynamic_data.ptr->chars(), dynamic_data.ptr->size() * sizeof(char16_t));
                }
                else
                {
                    ret = dynamic_data.ptr ? BSTR(dynamic_data.ptr->chars()) : nullptr;
                }
                init();
                return ret;
            }

            const char16_t * chars() const noexcept
            {
                if (flags.value == embedded_flag)
                    return embedded_data.chars;
                else
                    return dynamic_data.ptr ? dynamic_data.ptr->chars() : nullptr;
            }

            UINT size() const noexcept
            {
                if (flags.value == embedded_flag)
                    return embedded_data.size / sizeof(char16_t);
                else
                    return dynamic_data.ptr ? dynamic_data.ptr->size() : 0;
            }

            void truncate_size(UINT size) noexcept
            {
                assert(flags.value != static_flag);

                if (flags.value == embedded_flag)
                {
                    embedded_data.size = size * sizeof(char16_t);
                    embedded_data.chars[size] = 0;
                }
                else if (dynamic_data.ptr)
                {
                    dynamic_data.ptr->truncate_size(size);
                }
            }
        };
        static_assert(sizeof(data) == size_in_pointers * sizeof(void *));
        
    public:
        bstr_buffer() noexcept
            { m_data.init(); }
        bstr_buffer(std::nullptr_t) noexcept
            { m_data.init(nullptr); }
        bstr_buffer(dynamic_bstr * buf, int /*disambiguator*/) noexcept
            { m_data.init(buf, 0); }
        bstr_buffer(UINT length)
            { m_data.init(length); }
        bstr_buffer(BSTR str, handle_transfer transfer_type)
            { m_data.init(str, transfer_type); }

        template<class Char>
        bstr_buffer(const Char * str, size_t length)
        {
            if constexpr (utf_encoding_of<Char> == utf16)
            {
                UINT our_size = ensure_valid_size(length);
                char16_t * data = m_data.init(our_size);
                memcpy(data, str, our_size * sizeof(char16_t));
            }
            else
            {
                using converter = utf_converter<utf_encoding_of<Char>, utf16>;
                UINT our_size = ensure_valid_size(converter::converted_length(str, str + length));
                char16_t * data = m_data.init(our_size);
                converter::convert(str, str + our_size, data);
            }
        }

        void reallocate(UINT size, UINT used_size)
            { m_data.reallocate(size, used_size); }

        BSTR release() noexcept
            { return m_data.release(); }

        void swap(bstr_buffer & other) noexcept
            { m_data.swap(other.m_data); }

        const char16_t * chars() const noexcept
            { return m_data.chars(); }

        char16_t * chars() noexcept
            { return const_cast<char16_t *>(const_cast<const data *>(&m_data)->chars()); }

        UINT size() const noexcept
            { return m_data.size(); }

        void truncate_size(UINT size) noexcept
            { m_data.truncate_size(size); }

        static constexpr UINT minimum_capacity() noexcept
            { return data::max_small_length; }
    private:
        static UINT ensure_valid_size(size_t len)
        {
            if (len > bstr_traits::max_size)
                throw std::bad_alloc();
            return UINT(len);
        }
    private:
        data m_data;
    };

    
    class bstr_char_access : private generic::c_str_holder<char16_t>
    {
    public:
        using value_type = bstr_traits::storage_type;
        using size_type = bstr_traits::size_type;
        using reference = const value_type &;
        using pointer = const value_type *;

        using iterator = const value_type *;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<const value_type *>;
        using const_reverse_iterator = reverse_iterator;

    public:
        bstr_char_access(const bstr_buffer & buffer) noexcept :
            m_chars(buffer.chars()),
            m_size(buffer.size())
        {}
        bstr_char_access(const sys_string_t<bstr_storage> & str) noexcept;
        ~bstr_char_access() noexcept = default;

        bstr_char_access(const bstr_char_access & src) noexcept = delete;
        bstr_char_access(bstr_char_access && src) noexcept = delete;
        bstr_char_access & operator=(const bstr_char_access & src) = delete;
        bstr_char_access & operator=(bstr_char_access && src) = delete;

        const char16_t * data() const noexcept
            { return this->m_chars; }
        UINT size() const noexcept
            { return this->m_size; }
        const char16_t & operator[](UINT idx) const noexcept
            { return this->m_chars[idx]; }

        iterator begin() const noexcept
            { return iterator(this->m_chars); }
        iterator end() const noexcept
            { return iterator(this->m_chars + this->m_size); }
        const_iterator cbegin() const noexcept
            { return begin(); }
        const_iterator cend() const noexcept
            { return end(); }
        reverse_iterator rbegin() const noexcept
            { return reverse_iterator(this->m_chars + this->m_size); }
        reverse_iterator rend() const noexcept
            { return reverse_iterator(this->m_chars); }
        const_reverse_iterator crbegin() const noexcept
            { return rbegin(); }
        const_reverse_iterator crend() const noexcept
            { return rend(); }

        const char * c_str() const noexcept
        {
            if (m_c_str)
                return m_c_str;
            auto count = utf_converter<utf16, utf8>::converted_length(this->begin(), this->end());
            auto ret = new char[count + 1];
            utf_converter<utf16, utf8>::convert(this->begin(), this->end(), ret);
            ret[count] = 0;
            m_c_str = ret;
            return ret;
            
        }
    private:
        pointer m_chars = nullptr;
        size_type m_size = 0;
    };

    class bstr_builder_storage
    {
    public:
        using value_type = bstr_traits::storage_type;
        using size_type = bstr_traits::size_type;

    public:
        bstr_builder_storage() noexcept = default;
        ~bstr_builder_storage() noexcept = default;
        
        bstr_builder_storage(const bstr_builder_storage & src) = delete;
        bstr_builder_storage(bstr_builder_storage && src) noexcept :
            m_buf(std::move(src.m_buf)),
            m_capacity(src.m_capacity)
        { src.m_capacity = bstr_buffer::minimum_capacity(); }
        bstr_builder_storage & operator=(const bstr_builder_storage & src) = delete;
        bstr_builder_storage & operator=(bstr_builder_storage && src) noexcept
        {
            m_buf = std::move(src.m_buf);
            m_capacity = src.m_capacity;
            src.m_capacity = bstr_buffer::minimum_capacity();
            return *this;
        }

        constexpr size_type capacity() const noexcept
            { return m_capacity; }
        value_type * data() const noexcept
            { return m_buf.chars(); }
        static constexpr size_type max_size() noexcept
            { return bstr_traits::max_size; }
    
        void reallocate(size_type size, size_type used_size)
        { 
            m_buf.reallocate(size, used_size); 
            m_capacity = size;
        }

        bstr_buffer release() noexcept
        {
            m_capacity = bstr_buffer::minimum_capacity();
            return std::move(m_buf);
        }

    private:
        mutable bstr_buffer m_buf;
        UINT m_capacity{bstr_buffer::minimum_capacity()};
    };

    using bstr_builder_impl = char_vector<bstr_builder_storage>;

 }

 namespace sysstr
 {
    class bstr_storage 
    {
    friend util::bstr_char_access;
    public:
        using size_type = util::bstr_traits::size_type;
        using storage_type = util::bstr_traits::storage_type;
        using native_handle_type = util::bstr_traits::native_handle_type;
        using hash_type = util::bstr_traits::hash_type;
        using char_access = util::bstr_char_access;
        using builder_impl = util::bstr_builder_impl;

        static constexpr auto max_size = util::bstr_traits::max_size;

    public:
        bstr_storage() noexcept = default;

        bstr_storage(BSTR str, handle_transfer transfer_type) :
            m_buffer(str, transfer_type)
        {}

        bstr_storage(util::bstr_buffer && buffer) noexcept:  m_buffer(std::move(buffer))
        {}

    protected:

        template<has_utf_encoding Char>
        bstr_storage(const Char * str, size_t len):
            m_buffer(str, len)
        {}

        bstr_storage(const bstr_storage & src, UINT first, UINT last) :
            m_buffer(src.m_buffer.chars() + first, last - first)
        {}

        ~bstr_storage() noexcept = default;
        bstr_storage(const bstr_storage & src) noexcept = default;
        bstr_storage(bstr_storage && src) noexcept = default;
        bstr_storage & operator=(const bstr_storage & rhs) noexcept = default;
        bstr_storage & operator=(bstr_storage && rhs) noexcept = default;

    public:

        auto b_str() const noexcept -> native_handle_type
            { return BSTR(m_buffer.chars()); }

        auto release() noexcept -> native_handle_type
            { return m_buffer.release(); }

        auto data() const noexcept -> const char16_t *
            { return m_buffer.chars(); }

        auto copy_data(UINT idx, char16_t * buf, UINT buf_size) const noexcept -> UINT
        {
            UINT ret = std::min(buf_size, this->size() - idx);
            memcpy(buf, m_buffer.chars() + idx, ret * sizeof(char16_t));
            return ret;
        }

        auto w_str() const noexcept -> const wchar_t *
        { 
            auto ret = (const wchar_t *)data();
            return ret ? ret : L""; 
        }

    protected:
        auto size() const noexcept -> size_type
            { return m_buffer.size(); }

        auto operator[](UINT idx) const noexcept -> char16_t
            { return m_buffer.chars()[idx]; }

        auto swap(bstr_storage & other) noexcept -> void
            { m_buffer.swap(other.m_buffer); }

    private:
        const util::bstr_buffer & get_buffer() const noexcept
            { return m_buffer; }
    private:
        util::bstr_buffer m_buffer{nullptr};
    };

}

namespace sysstr::util
{
    inline bstr_char_access::bstr_char_access(const sys_string_t<bstr_storage> & src) noexcept:
        bstr_char_access(src.get_buffer())
    {}

    template<>
    inline sys_string_t<bstr_storage> build(bstr_builder_impl & builder) noexcept
    { 
        auto size = builder.size();
        auto buf = builder.release();
        buf.truncate_size(size);
        return sys_string_t<bstr_storage>(std::move(buf)); 
    }
}

namespace sysstr
{
    using sys_string_bstr = sys_string_t<bstr_storage>; 
    using sys_string_bstr_builder = sys_string_builder_t<bstr_storage>; 
}

#define SYS_STRING_STATIC_BSTR(x) ([] () noexcept -> ::sysstr::sys_string_bstr { \
        constexpr ::UINT size = sizeof(u##x); \
        static const ::sysstr::util::static_bstr<size> sbuf{size - sizeof(char16_t), u##x}; \
        ::sysstr::util::bstr_buffer buf((::sysstr::util::dynamic_bstr *)&sbuf, 0); \
        return *reinterpret_cast<::sysstr::sys_string_bstr *>(&buf); \
    }())

