
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
    template<class Storage>
    class sys_string_builder_t
    {
    private:
        using impl_type = typename Storage::builder_impl;
        
        struct view_traits
        {
            using char_access = const impl_type &;
            
            static constexpr const impl_type & adapt(const sys_string_builder_t & builder) noexcept
                { return builder.m_impl; }
        };
    public:
        using size_type = typename impl_type::size_type;
        using storage_type = typename  impl_type::value_type;
        
        template<utf_encoding Enc>
        using utf_view = utf_view<Enc, sys_string_builder_t, view_traits>;

        using utf8_view  = utf_view<utf8>;
        using utf16_view = utf_view<utf16>;
        using utf32_view = utf_view<utf32>;
        
        using iterator = typename utf32_view::iterator;
        using const_iterator = typename utf32_view::const_iterator;
        using reverse_iterator = typename utf32_view::reverse_iterator;
        using const_reverse_iterator = typename utf32_view::const_reverse_iterator;
        
    private:
        template<util::cursor_direction Direction>
        using const_storage_cursor = decltype(util::cursor_begin<Direction>(std::declval<const impl_type>()));
        
        template<util::cursor_direction Direction>
        using storage_cursor = decltype(util::cursor_begin<Direction>(std::declval<impl_type>()));

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
            { return storage_begin<util::cursor_direction::forward>(); }
        iterator end() const noexcept
            { return storage_end<util::cursor_direction::forward>(); }
        const_iterator cbegin() const noexcept
            { return begin(); }
        const_iterator cend() const noexcept
            { return end(); }
        reverse_iterator rbegin() const noexcept
            { return storage_begin<util::cursor_direction::backward>(); }
        reverse_iterator rend() const noexcept
            { return storage_end<util::cursor_direction::backward>(); }
        const_reverse_iterator crbegin() const noexcept
            { return rbegin(); }
        const_reverse_iterator crend() const noexcept
            { return rend(); }
        

        void push_back(char32_t c)
            { append_one(m_impl, c); }
        void pop_back() noexcept
        {
            auto it = ++rbegin();
            m_impl.erase(m_impl.begin() + it.storage_pos(), m_impl.end());
        }
        
        iterator insert(iterator where, char32_t c)
        {
            using util::cursor_at;

            auto pos = where.storage_pos();
            insert_one(m_impl, m_impl.begin() + pos, c);
            return storage_at<util::cursor_direction::forward>(pos);
        }
        
        template<class Char>
        iterator insert(iterator where, const Char * str, size_t len)
        {
            using util::cursor_at;

            auto pos = where.storage_pos();
            insert_many(m_impl, m_impl.begin() + pos, str, len);
            return storage_at<util::cursor_direction::forward>(pos);
        }
        
        iterator erase(iterator where) noexcept
        {
            using util::cursor_at;

            auto pos = where.storage_pos();
            auto where_it = m_impl.begin() + pos;
            m_impl.erase(where_it, where_it + where.storage_size());
            return storage_at<util::cursor_direction::forward>(pos);
        }
        iterator erase(iterator first, iterator last) noexcept
        {
            using util::cursor_at;
            
            auto first_pos = first.storage_pos();
            auto last_pos = last.storage_pos();
            m_impl.erase(m_impl.begin() + first_pos, m_impl.begin() + last_pos);
            return storage_at<util::cursor_direction::forward>(first_pos);
        }
        
        sys_string_builder_t & append(char32_t c)
            { append_one(m_impl, c); return *this; }

        template<class Char>
        sys_string_builder_t & append(const Char * str, size_t len)
            { append_many(m_impl, str, len); return *this; }
        template<class Char>
        sys_string_builder_t & append(const Char * str)
            { append_many(m_impl, str, std::char_traits<Char>::length(str)); return *this; }

        sys_string_builder_t & append(const sys_string_t<Storage> & str)
            { append_access(typename sys_string_t<Storage>::char_access(str)); return *this; }

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
        
        static void insert_one(impl_type & impl, typename impl_type::iterator where, char32_t c);
        
        template<class Char>
        static void append_many(impl_type & impl, const Char * str, size_t len);
        
        template<class Char>
        static void insert_many(impl_type & impl, typename impl_type::iterator pos, const Char * str, size_t len);
        
        template<class Access>
        void append_access(const Access & access);
        
        template<util::cursor_direction Direction>
        auto storage_begin() const -> const_storage_cursor<Direction>
            { return util::cursor_begin<Direction>(m_impl); }
        template<util::cursor_direction Direction>
        auto storage_end() const -> const_storage_cursor<Direction>
            { return util::cursor_end<Direction>(m_impl); }
        template<util::cursor_direction Direction>
        auto storage_at(size_type pos) const -> const_storage_cursor<Direction>
            { return util::cursor_at<Direction>(m_impl, pos); }
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
    void sys_string_builder_t<Storage>::insert_one(impl_type & impl, typename impl_type::iterator where, char32_t c)
    {
        if constexpr (std::is_same_v<storage_type, char32_t>)
        {
            impl.insert(where, c);
        }
        else
        {
            using converter = utf_converter<utf32, utf_encoding_of<storage_type>>;
            converter::convert(&c, &c + 1, std::inserter(impl, where));
        }
    }

    template<class Storage>
    template<class Char>
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
    template<class Char>
    void sys_string_builder_t<Storage>::insert_many(impl_type & impl, typename impl_type::iterator where, const Char * str, size_t len)
    {
        if constexpr (std::is_same_v<storage_type, Char>)
        {
            impl.insert(where, str, limit_size(len));
        }
        else
        {
            auto inserter = std::inserter(impl, where);
            using converter = utf_converter<utf_encoding_of<Char>, utf_encoding_of<storage_type>>;
            converter::convert(str, str + len, inserter);
        }
    }

    template<class Storage>
    template<class Access>
    inline void sys_string_builder_t<Storage>::append_access(const Access & access)
    {
        if constexpr (util::has_contiguous_data<Access>)
        {
            m_impl.append(access.data(), access.size());
        }
        else
        {
            for(typename Access::size_type i = 0, count = access.size(); i < count; ++i)
            {
                auto c = access[i];
                m_impl.push_back(c);
            }
        }
    }

}
