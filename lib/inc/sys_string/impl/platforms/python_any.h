//
// Copyright 2023 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_H_INSIDE
#error This header must not be included directly. Please include sys_string.h
#endif

#include <sys_string/impl/util/char_vector.h>

#include <variant>

namespace sysstr
{
    class py_storage;
}

namespace sysstr::util
{
    struct py_traits
    {
        using storage_type = char32_t; //we pretend that Python strings store char32_t and convert transparently
        static_assert(sizeof(storage_type) == sizeof(Py_UCS4));
        using size_type = Py_ssize_t;
        using hash_type = size_t;
        using native_handle_type = PyObject *;
        
        static constexpr size_type max_size = std::numeric_limits<size_type>::max() / sizeof(char32_t);
    };

    inline PyObject * check_create(PyObject * src)
    {
        if (!src) {
            PyErr_Clear();
            throw std::runtime_error("Python string creation failed");
        }
        return src;
    }
    
    template<size_t N>
    constexpr auto find_max_codepoint(const char32_t (&ar)[N]) -> char32_t
    {
        char32_t max = 0;
        for(char32_t c: ar) {
            if (c > max)
                max = c;
        }
        return max;
    }
    
    class py_builder_storage
    {
    public:
        using value_type = py_traits::storage_type;
        using size_type = py_traits::size_type;
        
        class dynamic_t
        {
        public:
            dynamic_t() noexcept = default;
            dynamic_t(size_t size):
                m_ptr((value_type *)malloc(size * sizeof(value_type)))
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
                    free(m_ptr);
            }
            dynamic_t & operator=(const dynamic_t &) = delete;
            dynamic_t & operator=(dynamic_t && rhs) noexcept
            {
                if (this != &rhs)
                {
                    if (m_ptr)
                        free(m_ptr);
                    m_ptr = rhs.m_ptr;
                    rhs.m_ptr = nullptr;
                }
                return *this;
            }
            
            constexpr value_type * data() const noexcept
                { return m_ptr; }
            
            void reallocate(size_type size)
            {
                auto result = (value_type *)realloc(m_ptr, size * sizeof(value_type));
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
        
        static constexpr size_type minimum_capacity = 32;
        
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
            { return py_traits::max_size; }
        
        void reallocate(size_type size, size_type used_size)
        {
            struct reallocator
            {
                py_builder_storage * me;
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
            return std::move(m_buffer);
        }
    private:
        buffer_t m_buffer;
        size_type m_capacity = minimum_capacity;
    };

    using py_builder_impl = char_vector<py_builder_storage>;

    inline PyObject * convert_to_string(py_builder_impl & builder) noexcept
    {
        auto size = builder.size();
        return std::visit([&](auto && buf) {
            int dumbPyPy = (std::endian::native != std::endian::little) - (std::endian::native == std::endian::little);
            return size ? check_create(PyUnicode_DecodeUTF32((const char *)buf.data(), size * sizeof(char32_t), "replace", &dumbPyPy)) :
                                        nullptr;
        }, builder.release());
    }

    class py_char_access
    {
    public:
        using value_type = py_traits::storage_type;
        using size_type = py_traits::size_type;
        using reference = value_type;
        using pointer = void;
        
        using iterator = index_iterator<const py_char_access, iter_direction::forward>;
        using const_iterator = iterator;
        using reverse_iterator = index_iterator<const py_char_access, iter_direction::backward>;
        using const_reverse_iterator = reverse_iterator;

    public:
        py_char_access(const sys_string_t<py_storage> & src) noexcept;
        
        
        py_char_access(const py_char_access & src) noexcept = delete;
        py_char_access(py_char_access && src) noexcept = delete;
        py_char_access & operator=(const py_char_access & src) = delete;
        py_char_access & operator=(py_char_access && src) = delete;

        PyObject * get_string() const noexcept
            { return m_str; }

        value_type operator[](size_type idx) const noexcept
        {
            assert (idx >= 0 && idx < m_size);
            return char32_t(PyUnicode_READ(m_kind, m_data, idx));
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
            { return PyUnicode_AsUTF8(m_str); }

        friend bool operator==(const py_char_access & lhs, const py_char_access & rhs) noexcept
            { return lhs.m_str == rhs.m_str; }
    private:
        PyObject * m_str = nullptr;
        PyUnicode_Kind m_kind = PyUnicode_4BYTE_KIND;
        void * m_data = nullptr;
        size_type m_size = 0;
    };

}


namespace sysstr
{
  
    class py_storage
    {
    public:
        using size_type = util::py_traits::size_type;
        using storage_type = util::py_traits::storage_type;
        using native_handle_type = util::py_traits::native_handle_type;
        using hash_type = util::py_traits::hash_type;
        using char_access = util::py_char_access;
        
        using builder_impl = util::py_builder_impl;
        
        static constexpr size_type max_size = util::py_traits::max_size;

    public:
        py_storage() noexcept:
            m_str(null_string())
        {}
        
        py_storage(native_handle_type str, handle_retain retain_handle = handle_retain::yes) noexcept :
            m_str(canonicalize(str, retain_handle))
        {}

    protected:
        
        py_storage(native_handle_type src, size_type first, size_type last) :
            m_str(src ? util::check_create(PyUnicode_Substring(src, first, last)) : null_string())
        {}

        py_storage(const py_storage & src, size_type first, size_type last) :
            py_storage(src.m_str, first, last)
        {}
        
        template<class Char>
        py_storage(const Char * str, size_t len):
            py_storage(create_from(str, len), handle_retain::no)
        {}

        py_storage(const char_access::iterator & first, size_type length):
            py_storage(first.container() ? first.container()->get_string() : nullptr, first.index(), first.index() + length)
        {}
        
        
        ~py_storage() noexcept
            { release(m_str); }

        py_storage(const py_storage & src) noexcept : m_str(retain(src.m_str))
        {}

        py_storage(py_storage && src) noexcept : m_str(src.m_str)
        {
            src.m_str = null_string();
        }

        auto operator=(const py_storage & rhs) noexcept -> py_storage &
        {
            PyObject * temp = m_str;
            m_str = rhs.m_str;
            retain(m_str);
            release(temp);
            return *this;
        }

        inline auto operator=(py_storage && rhs) noexcept -> py_storage &
        {
            if (this != &rhs)
            {
                release(m_str);
                m_str = rhs.m_str;
                rhs.m_str = null_string();
            }
            return *this;
        }
        
        auto swap(py_storage & other) noexcept -> void
        {
            using std::swap;
            swap(m_str, other.m_str);
        }

    public:
        
        auto py_str() const noexcept -> native_handle_type
            { return m_str; }

        auto data() const noexcept -> const storage_type * { 
            auto kind = PyUnicode_KIND(m_str);
            if (kind == PyUnicode_4BYTE_KIND)
                return (const storage_type *)PyUnicode_DATA(m_str); 

            return nullptr;
        }

        auto copy_data(size_type idx, storage_type * buf, size_type buf_size) const noexcept -> size_type
        {
            auto kind = PyUnicode_KIND(m_str);
            auto size = PyUnicode_GET_LENGTH(m_str);
            auto data = PyUnicode_DATA(m_str);

            if (idx >= size)
                return 0;

            size_type ret;
            for(ret = 0; ret < buf_size && idx + ret < size; ++ret)
                buf[ret] = PyUnicode_READ(kind, data, idx + ret);
            return ret;
        }

    protected:

        auto size() const noexcept -> size_type
            { return PyUnicode_GET_LENGTH(m_str); }

    private:
        static PyObject * retain(PyObject * src) noexcept
        {
            Py_INCREF(src);
            return src;
        }
        static void release(PyObject * src) noexcept
        {
            Py_DECREF(src);
        }
        
        static PyObject * null_string() noexcept
        {
            static py_storage null(util::check_create(PyUnicode_FromString("")), ::sysstr::handle_retain::no);
            return retain(null.m_str);
        }

        template<class Char>
        static PyObject * create_from(const Char * str, size_t len) 
        {
            if constexpr (utf_encoding_of<Char> == utf_encoding::utf8) {
                return util::check_create(PyUnicode_DecodeUTF8((const char *)str, len, "replace"));
            } else if constexpr (utf_encoding_of<Char> == utf_encoding::utf16) {
                int dumbPyPy = (std::endian::native != std::endian::little) - (std::endian::native == std::endian::little);
                return util::check_create(PyUnicode_DecodeUTF16((const char *)str, len * sizeof(char16_t), "replace", &dumbPyPy));
            } else if constexpr (utf_encoding_of<Char> == utf_encoding::utf32) {
                int dumbPyPy = (std::endian::native != std::endian::little) - (std::endian::native == std::endian::little);
                return util::check_create(PyUnicode_DecodeUTF32((const char *)str, len * sizeof(char32_t), "replace", &dumbPyPy));
            }
        }

        static inline PyObject * canonicalize(PyObject * str, handle_retain retain_handle)
        {
            if (!str)
                return null_string();
        #if (PY_MAJOR_VERSION == 3 && PY_MINOR_VERSION < 12) 
            if (PyUnicode_READY(str) != 0)  
                throw std::bad_alloc();
        #endif
            return (retain_handle == handle_retain::yes ? retain(str) : str);
        }

    private:
        PyObject * m_str = nullptr;
    };
}

namespace sysstr::util
{
    inline py_char_access::py_char_access(const sys_string_t<py_storage> & src) noexcept:
        m_str(src.py_str()),
        m_kind(PyUnicode_Kind(PyUnicode_KIND(m_str))),
        m_data(PyUnicode_DATA(m_str)),
        m_size(PyUnicode_GET_LENGTH(m_str))
    {}
    
    template<>
    inline sys_string_t<py_storage> build(py_builder_impl & builder) noexcept
    {
        auto str = convert_to_string(builder);
        return sys_string_t<py_storage>(str, handle_retain::no);
    }

    template <typename T, typename = int>
    struct HasHashMember : std::false_type { };

    template <typename T>
    struct HasHashMember <T, decltype((void)T::hash, 0)> : std::true_type { };

    template<class T>
    constexpr void gccIsAPieceOfShit_assignHash(T & obj) {
        if constexpr(HasHashMember<T>::value)
            obj.hash = -1;
    }

    template <typename T, typename = int>
    struct HasReadyMember : std::false_type { };

    template <typename T>
    struct HasReadyMember <T, decltype((void)T::ready, 0)> : std::true_type { };

    template<class T>
    constexpr void gccIsAPieceOfShit_assignReady(T & obj, int value) {
        if constexpr(HasReadyMember<T>::value)
            obj.ready = value;
    }

    template<class T>
    constexpr void gccIsAPieceOfShit_assignData(T & obj, void * data) {
        obj.any = data;
    }

    constexpr void gccIsAPieceOfShit_assignData(void * & obj, void * data) {
        obj = data;
    }
    
    template<PyUnicode_Kind Kind>
    struct PyUnicodeObject_wrapper : PyUnicodeObject
    {
        #ifdef __GNUC__
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
        #endif
        constexpr PyUnicodeObject_wrapper(size_t size, const void * chars):
            PyUnicodeObject{{{PyObject_HEAD_INIT(&PyUnicode_Type)}}}
        {
            this->_base._base.length = size;
            gccIsAPieceOfShit_assignHash(this->_base._base); //pypy lacks hash in PyASCIIObject
            this->_base._base.state.kind = Kind;
            gccIsAPieceOfShit_assignReady(this->_base._base.state, 1);
            gccIsAPieceOfShit_assignData(this->data, const_cast<void *>(chars));
            if constexpr (Kind == PyUnicode_1BYTE_KIND)
            {
                this->_base._base.state.ascii = 1;
                this->_base.utf8 = static_cast<char *>(const_cast<void *>(chars));
                this->_base.utf8_length = size;
            }
        }
        #ifdef __GNUC__
        #pragma GCC diagnostic pop
        #endif
        
        sys_string_t<py_storage> as_string() noexcept
        {
            auto ptr = reinterpret_cast<PyObject *>(this);
            return sys_string_t<py_storage>(ptr);
        }
    };
}

namespace sysstr
{
    using sys_string_pystr = sys_string_t<py_storage>;
    using sys_string_pystr_builder = sys_string_builder_t<py_storage>;
}

namespace sysstr::util 
{
    template<util::ct_string U32Str, util::ct_string U16Str, util::ct_string U8Str>
    inline auto make_static_sys_string_pystr() noexcept -> sys_string_pystr
    {
        constexpr ::size_t size = U32Str.size();
        constexpr auto maxChar = find_max_codepoint(U32Str.chars);
        if constexpr (maxChar <= 0x7fu) { 
            static PyUnicodeObject_wrapper<PyUnicode_1BYTE_KIND> str(size - 1, U8Str.chars);
            return str.as_string();
        } else if constexpr (maxChar <= 0xffffu) {
            static PyUnicodeObject_wrapper<PyUnicode_2BYTE_KIND> str(size - 1, U16Str.chars);
            return str.as_string();
        } else {
            static PyUnicodeObject_wrapper<PyUnicode_4BYTE_KIND> str(size - 1, U32Str.chars);
            return str.as_string();
        }
    }
}


#define SYS_STRING_STATIC_PYSTR(x) ::sysstr::util::make_static_sys_string_pystr<U##x, u##x, u8##x>()

