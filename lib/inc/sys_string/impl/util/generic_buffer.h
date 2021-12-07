
//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_GENERIC_BUFFER_H_INCLUDED
#define HEADER_SYS_STRING_GENERIC_BUFFER_H_INCLUDED

#include <sys_string/impl/util/util.h>
#include <sys_string/impl/util/char_buffer.h>

#include <atomic>
#include <new>
#include <algorithm>
#include <cstring>

namespace sysstr::util::generic
{
    template<class CharT, class SizeT>
    class dynamic_buffer
    {
    public:
        using size_type = SizeT;
        using value_type = CharT;
    public:
        static dynamic_buffer * create(SizeT len)
            { return new (len) dynamic_buffer(); }
        
        static dynamic_buffer * reallocate(dynamic_buffer * original, SizeT len)
        {
            dynamic_buffer * new_ptr = (dynamic_buffer *)realloc(original, sizeof(dynamic_buffer) + len * sizeof(CharT));
            if (!new_ptr)
                throw std::bad_alloc();
            if (!original)
                new (new_ptr) dynamic_buffer();
            return new_ptr;
        }

        void add_ref() const noexcept
        {
            if (!this->m_is_static)
                this->m_ref_count.fetch_add(1, std::memory_order_relaxed);
        }
    
        void sub_ref() const noexcept
        {
            if (!this->m_is_static)
            {
                if (this->m_ref_count.fetch_sub(1, std::memory_order_release) == 1)
                {
                    std::atomic_thread_fence(std::memory_order_acquire);
                    delete this;
                }
            }
        }

        constexpr const CharT * data() const noexcept
            { return this->m_chars; }
        constexpr CharT * data() noexcept
            { return this->m_chars; }
        
    private:
        dynamic_buffer() noexcept = default;
        dynamic_buffer(const dynamic_buffer &) = delete;
        dynamic_buffer & operator=(const dynamic_buffer &) = delete;
        ~dynamic_buffer() = default;
        
        static void * operator new(size_t length, SizeT str_length)
        { 
            void * ret = malloc(length + str_length * sizeof(CharT)); 
            if (!ret)
                throw std::bad_alloc();
            return ret;
        }
    
        static void * operator new(size_t /*length*/, void * ptr) noexcept
            { return ptr; }
    
        static void operator delete(void * ptr, SizeT /*str_length*/) noexcept
            { free(ptr); }
    
        static void operator delete(void * ptr) noexcept
            { free(ptr); }
    private:
        mutable std::atomic<int32_t> m_ref_count{1};
        bool m_is_static = false;
        CharT m_chars[1]; //this accounts for null terminator
    };

    //this needs to be layout compatible with dynamic_buffer
    template<class CharT, class SizeT, size_t N>
    struct static_buffer
    {
        int32_t not_a_ref_count;
        bool is_static;
        CharT chars[N + 1];
    };

    template<class CharT, class SizeT>
    class buffer
    {
    private:
        static constexpr uintptr_t dynamic_flag = 0b01;
        using dynamic_type = dynamic_buffer<CharT, SizeT>;
    public:
        static constexpr SizeT max_size = SizeT(std::numeric_limits<SizeT>::max() - sizeof(dynamic_type)) / sizeof(CharT);
    public:
        buffer() noexcept
        {
            this->set_empty();
        }
        buffer(nullptr_t) noexcept
        {
            this->set_dynamic(nullptr, 0);
        }
        buffer(dynamic_type * buf, SizeT size, int) noexcept
        {
            this->set_dynamic(buf, size);
        }
        buffer(SizeT length) 
        {
            if (length <= max_small_length)
            {
                this->set_static_size(length);
            }
            else
            {
                auto * buf = dynamic_type::create(length);
                this->set_dynamic(buf, length);
            }
        }

        template<class Char>
        buffer(const Char * str, size_t length)
        {
            if constexpr (utf_encoding_of<Char> == utf_encoding_of<CharT>)
            {
                SizeT our_size = buffer::ensure_valid_size(length);
                CharT * data;
                if (our_size <= max_small_length)
                {
                    this->set_static_size(our_size);
                    data = this->get_static_data();
                }
                else
                {
                    auto * buf = dynamic_type::create(our_size);
                    this->set_dynamic(buf, our_size);
                    data = buf->data();
                }
                memcpy(data, str, our_size * sizeof(CharT));
                data[our_size] = 0;
            }
            else
            {
                using converter = utf_converter<utf_encoding_of<Char>, utf_encoding_of<CharT>>;
                SizeT our_size = buffer::ensure_valid_size(converter::converted_length(str, str + length));
                CharT * data;
                if (our_size <= max_small_length)
                {
                    this->set_static_size(our_size);
                    data = this->get_static_data();
                }
                else
                {
                    auto * buf = dynamic_type::create(our_size);
                    this->set_dynamic(buf, our_size);
                    data = buf->data();
                }
                converter::convert(str, str + length, data);
                data[our_size] = 0;
            }
        }

        void reallocate(SizeT size, SizeT used_size)
        {   
            if (this->is_dynamic())
            {
                auto buf = this->get_dynamic();
                if (size <= max_small_length)
                {
                    this->set_static_size(size);
                    if (buf)
                    {
                        memcpy(this->get_static_data(), buf->data(), used_size * sizeof(CharT));
                        buf->sub_ref();
                    }
                }
                else
                {
                    buf = dynamic_type::reallocate(buf, size);
                    this->set_dynamic(buf, size);
                }
            }
            else
            {
                if (size > max_small_length)
                {
                    auto * buf = dynamic_type::create(size);
                    memcpy(buf->data(), this->get_static_data(), used_size * sizeof(CharT));
                    this->set_dynamic(buf, size);
                }
            }
        }


        ~buffer() noexcept
        { 
            if (auto buf = (this->is_dynamic() ? this->get_dynamic() : nullptr))
                buf->sub_ref();
        }
        buffer(const buffer & src) noexcept
        { 
            memcpy(&m_data, &src.m_data, sizeof(m_data));
            if (auto buf = (this->is_dynamic() ? this->get_dynamic() : nullptr))
                buf->add_ref();
        }
        buffer(buffer && src) noexcept
        { 
            memcpy(&m_data, &src.m_data, sizeof(m_data));
            src.set_empty();
        }
        buffer & operator=(const buffer & rhs) noexcept
        {
            buffer temp(rhs);
            return *this = std::move(temp);
        }
        buffer & operator=(buffer && rhs) noexcept
        {
            this->~buffer();
            new (this) buffer(std::move(rhs));
            return *this;
        }

        void swap(buffer & other) noexcept
        {
            decltype(m_data) temp;
            memcpy(&temp, &other.m_data, sizeof(m_data));
            memcpy(&other.m_data, &m_data, sizeof(m_data));
            memcpy(&m_data, &temp, sizeof(m_data));
        }

        const CharT * data() const noexcept
        { 
            if (this->is_dynamic())
            {
                if (auto ptr = this->get_dynamic())
                    return ptr->data();
                else 
                    return nullptr;
            }
            return this->get_static_data();
        }

        CharT * data() noexcept
            { return const_cast<CharT *>(const_cast<const buffer *>(this)->data()); }

        SizeT size() const noexcept
            {  return this->is_dynamic() ? this->get_dynamic_size() : this->get_static_size(); }

        void truncate_size(SizeT size) noexcept
        {
            if (this->is_dynamic())
            {
                this->set_dynamic_size(size);
                if (auto ptr = this->get_dynamic())
                    ptr->data()[size] = 0;
            }
            else
            {
                this->set_static_size(size);
                this->get_static_data()[size] = 0;
            }
        }

        static constexpr SizeT minimum_capacity() noexcept
            { return max_small_length; }
    private:
        void set_empty() noexcept
        {
            this->marker_byte() = std::byte(0);
        }
        void set_dynamic(dynamic_type * buffer, SizeT size) noexcept
        {
            this->dynamic_ptr_value() = (uintptr_t(buffer) | buffer::dynamic_flag);
            this->dynamic_size_value() = size;
        }
        bool is_dynamic() const noexcept
            { return this->dynamic_ptr_value() & buffer::dynamic_flag; }
        dynamic_type * get_dynamic() const noexcept
            { return reinterpret_cast<dynamic_type *>(this->dynamic_ptr_value() & ~buffer::dynamic_flag); }
        SizeT get_dynamic_size() const noexcept
            { return this->dynamic_size_value(); }
        void set_dynamic_size(SizeT size) noexcept
            { this->dynamic_size_value() = size; }


        SizeT get_static_size() const noexcept
            { return SizeT(this->marker_byte() >> 1); }
        void set_static_size(SizeT size) noexcept
            { this->marker_byte() = (std::byte(size) << 1); }
        
        static SizeT ensure_valid_size(size_t len)
        {
            if (len > buffer::max_size)
                throw std::bad_alloc();
            return SizeT(len);
        }
    private:
        static_assert(endian::native == endian::little || endian::native == endian::big, 
                      "Only little or big endian architectures are currently supported");
        
        static constexpr size_t data_size = 4 * sizeof(void *);
        //1 CharT stolen for size+dynamic_flag and 1 for null terminator
        static constexpr SizeT max_small_length = data_size / sizeof(CharT) - 2;

        alignas(void *) std::byte m_data[data_size];

        
        const uintptr_t & dynamic_ptr_value() const noexcept
        {
            if constexpr (endian::native == endian::little)
                return *reinterpret_cast<const uintptr_t *>(&this->m_data[0]);
            else 
                return *(reinterpret_cast<const uintptr_t *>(&this->m_data[data_size]) - 1);
        }
        uintptr_t & dynamic_ptr_value() noexcept
            { return const_cast<uintptr_t &>(const_cast<const buffer *>(this)->dynamic_ptr_value()); }

        const uintptr_t & dynamic_size_value() const noexcept
        {
            if constexpr (endian::native == endian::little)
                return *(reinterpret_cast<const uintptr_t *>(&this->m_data[0]) + 1);
            else 
                return *(reinterpret_cast<const uintptr_t *>(&this->m_data[data_size]) - 2);
        }
        uintptr_t & dynamic_size_value() noexcept
            { return const_cast<uintptr_t &>(const_cast<const buffer *>(this)->dynamic_size_value()); }

        const CharT * get_static_data() const noexcept
        {
            if constexpr (endian::native == endian::little)
                return reinterpret_cast<const CharT *>(&this->m_data[0]) + 1;
            else
                return reinterpret_cast<const CharT *>(&this->m_data[0]);
        }
        CharT * get_static_data() noexcept
            { return const_cast<CharT *>(const_cast<const buffer *>(this)->get_static_data()); }

        const std::byte & marker_byte() const noexcept
        {
            if constexpr (endian::native == endian::little)
                return this->m_data[0]; 
            else
                return this->m_data[data_size - 1]; 
        }
        std::byte & marker_byte() noexcept
            { return const_cast<std::byte &>(const_cast<const buffer *>(this)->marker_byte()); }
    };

    
    template<class CharT> struct c_str_holder
    {
        ~c_str_holder() noexcept
            { if (m_c_str) delete [] m_c_str; }
        mutable const char * m_c_str = nullptr;
    };
    template<> struct c_str_holder<char>
    {};

    template<class CharT, class SizeT>
    class char_access : private c_str_holder<CharT>
    {
    public:
        using value_type = CharT;
        using size_type = SizeT;
        using reference = const value_type &;
        using pointer = const value_type *;
        
        using iterator = const CharT *;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<const CharT *>;
        using const_reverse_iterator = reverse_iterator;
        
        using cursor = iter_cursor<char_access::const_iterator, char_access::const_iterator, cursor_direction::forward, size_type>;
        using reverse_cursor = iter_cursor<char_access::const_iterator, char_access::const_iterator, cursor_direction::backward, size_type>;
    public:
        char_access(const buffer<CharT, SizeT> & buffer) noexcept:
            m_chars(buffer.data()),
            m_size(buffer.size())
        {
        }
        template<class Storage>
        char_access(const sys_string_t<Storage> & str) noexcept;
        ~char_access() noexcept = default;

        char_access(const char_access & src) noexcept = delete;
        char_access(char_access && src) noexcept = delete;
        char_access & operator=(const char_access & src) = delete;
        char_access & operator=(char_access && src) = delete;

        const CharT * data() const noexcept
            { return this->m_chars; }
        SizeT size() const noexcept
            { return this->m_size; }
        const CharT & operator[](SizeT idx) const noexcept
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
            if constexpr (std::is_same_v<char, CharT>)
            {
                return m_chars;
            }
            else
            {
                if (this->m_c_str)
                    return this->m_c_str;
                auto count = utf_converter<utf_encoding_of<CharT>, utf8>::converted_length(this->begin(), this->end());
                auto ret = new char[count + 1];
                utf_converter<utf_encoding_of<CharT>, utf8>::convert(this->begin(), this->end(), ret);
                ret[count] = 0;
                this->m_c_str = ret;
                return ret;
            }
        }
    private:
        const CharT * m_chars = nullptr;
        SizeT m_size = 0;
    };
    

    template<class CharT, class SizeT>
    class builder_storage
    {
    private:
        using buffer_t = generic::buffer<CharT, SizeT>;
    public:
        using value_type = CharT;
        using size_type = SizeT;

    public:
        builder_storage() noexcept = default;
        ~builder_storage() noexcept = default;
        
        builder_storage(const builder_storage & src) = delete;
        builder_storage(builder_storage && src) noexcept :
            m_buf(std::move(src.m_buf)),
            m_capacity(src.m_capacity)
        { src.m_capacity = buffer_t::minimum_capacity(); }
        builder_storage & operator=(const builder_storage & src) = delete;
        builder_storage & operator=(builder_storage && src) noexcept
        {
            this->m_buf = std::move(src.m_buf);
            this->m_capacity = src.m_capacity;
            src.m_capacity = buffer_t::minimum_capacity();
            return *this;
        }

        constexpr size_type capacity() const noexcept
            { return this->m_capacity; }
        constexpr value_type * buffer() const noexcept
            { return this->m_buf.data(); }
        static constexpr size_type max_size() noexcept
            { return buffer_t::max_size; }
    
        void reallocate(size_type size, size_type used_size)
        { 
            this->m_buf.reallocate(size, used_size); 
            this->m_capacity = size;
        }

        buffer_t release() noexcept
        {
            this->m_capacity = buffer_t::minimum_capacity();
            return std::move(m_buf);
        }

    private:
        mutable buffer_t m_buf;
        size_t m_capacity{buffer_t::minimum_capacity()}; 
    };

    template<class CharT, class SizeT>
    using buffer_builder= char_buffer<builder_storage<CharT, SizeT>>;

    template<class CharT, class SizeT>
    inline buffer<CharT, SizeT> convert_to_string(buffer_builder<CharT, SizeT> & builder) noexcept
    {
        auto size = builder.size();
        auto buf = builder.release();
        buf.truncate_size(size);
        return buf;
    }

    template<class CharT, class SizeT>
    class storage 
    {
    friend generic::char_access<CharT, SizeT>;
    protected:
        using buffer = generic::buffer<CharT, SizeT>;
        using char_access = generic::char_access<CharT, SizeT>;
    public:
        using size_type = SizeT;
        using storage_type = CharT;

    public:
        storage() noexcept = default;
        
        storage(buffer && buffer) noexcept:  m_buffer(std::move(buffer))
        {}
        
    protected:
        ~storage() noexcept = default;
        storage(const storage & src) noexcept = default;
        storage(storage && src) noexcept = default;
        storage & operator=(const storage & rhs) noexcept = default;
        storage & operator=(storage && rhs) noexcept = default;

        template<class Char>
        storage(const Char * str, size_t len, std::enable_if_t<has_utf_encoding<Char>> * = nullptr) :
            m_buffer(str, len)
        {}
        
        storage(const storage & src, SizeT first, SizeT last) :
            m_buffer(char_access(src.m_buffer.get()).data() + first, last - first)
        {}


    public:
        auto data() const noexcept -> const CharT *
            { return m_buffer.data(); }

        auto copy_data(SizeT idx, CharT * buf, SizeT buf_size) const noexcept -> SizeT
        {
            SizeT ret = std::min(buf_size, this->size() - idx);
            memcpy(buf, m_buffer.data() + idx, ret * sizeof(CharT));
            return ret;
        }

    protected:
        
        auto size() const noexcept -> size_type
            { return m_buffer.size(); }

        auto operator[](SizeT idx) const noexcept -> CharT
            { return m_buffer.buffer()[idx]; }

        auto swap(storage & other) noexcept -> void
            { m_buffer.swap(other.m_buffer); }

        const buffer & get_buffer() const noexcept
            { return m_buffer; }
    private:
        buffer m_buffer{nullptr};
    };
    
}

#endif
