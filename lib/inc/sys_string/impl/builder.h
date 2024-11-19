
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

namespace sysstr
{
    template<class Storage> struct utf_view_adapter_for<sys_string_builder_t<Storage>>
        { using type = typename sys_string_builder_t<Storage>::utf_view_adapter; };

    template<class Storage>
    class sys_string_builder_t
    {
    private:
        using impl_type = typename Storage::builder_impl;
        
    public:
        using size_type = typename impl_type::size_type;
        using storage_type = typename impl_type::value_type;
        

        struct utf_view_adapter
        {
            static constexpr const impl_type * adapt(const sys_string_builder_t & builder) noexcept
                { return &builder.m_impl; }
        };

        template<utf_encoding Enc>
        using utf_view = utf_view<Enc, sys_string_builder_t>;

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
        
        auto storage_begin() const 
            { return std::ranges::begin(m_impl); }
        auto storage_end() const 
            { return std::ranges::end(m_impl); }

        auto storage_rbegin() const 
            { return std::ranges::rbegin(m_impl); }
        auto storage_rend() const 
            { return std::ranges::rend(m_impl); }
        
    private:
        impl_type m_impl;
    };

    template<class Storage>
    void sys_string_builder_t<Storage>::append_one(impl_type & impl, char32_t c)
    {
        if constexpr (std::is_same_v<storage_type, char32_t>)
        {
            impl.push_back(c);
        }
        else
        {
            using converter = utf_converter<utf32, utf_encoding_of<storage_type>>;
            converter::convert(&c, &c + 1, std::back_inserter(impl));
        }
    }

    template<class Storage>
    auto sys_string_builder_t<Storage>::insert_one(impl_type & impl, typename impl_type::iterator where, char32_t c) ->
        typename impl_type::iterator
    {
        if constexpr (std::is_same_v<storage_type, char32_t>)
        {
            return impl.insert(where, c);
        }
        else
        {
            using converter = utf_converter<utf32, utf_encoding_of<storage_type>>;
            auto pos = where - std::ranges::begin(impl);
            converter::convert(&c, &c + 1, std::inserter(impl, where));
            return std::ranges::begin(impl) + pos;
        }
    }

    template<class Storage>
    template<has_utf_encoding Char>
    void sys_string_builder_t<Storage>::append_many(impl_type & impl, const Char * str, size_t len)
    {
        if constexpr (std::is_same_v<storage_type, Char>)
        {
            impl.append(str, limit_size(len));
        }
        else
        {
            using converter = utf_converter<utf_encoding_of<Char>, utf_encoding_of<storage_type>>;
            converter::convert(str, str + len, std::back_inserter(impl));
        }
    }


    template<class Storage>
    template<has_utf_encoding Char>
    auto sys_string_builder_t<Storage>::insert_many(impl_type & impl, typename impl_type::iterator where, const Char * str, size_t len) ->
    typename impl_type::iterator
    {
        if constexpr (std::is_same_v<storage_type, Char>)
        {
            return impl.insert(where, str, limit_size(len));
        }
        else
        {
            using converter = utf_converter<utf_encoding_of<Char>, utf_encoding_of<storage_type>>;
            auto pos = where - std::ranges::begin(impl);
            converter::convert(str, str + len, std::inserter(impl, where));
            return std::ranges::begin(impl) + pos;
        }
    }

    template<class Storage>
    template<std::ranges::input_range Range>
    inline void sys_string_builder_t<Storage>::append_range(const Range & range)
    {
        static_assert(std::is_same_v<std::ranges::range_value_t<Range>, storage_type>);

        if constexpr (std::ranges::contiguous_range<Range>)
        {
            m_impl.append(std::data(range), std::size(range));
        }
        else
        {
            for(storage_type c: range)
                m_impl.push_back(c);
        }
    }

}
