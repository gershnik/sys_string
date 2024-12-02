
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

namespace sysstr::util
{
    template<class Storage, class T>
    class addend;

    template<class Storage>
    class addend<Storage, sys_string_t<Storage>>
    {
    public:
        addend(const sys_string_t<Storage> & str) noexcept:
            m_val(str)
        {}
        
        auto storage_size() const noexcept -> typename sys_string_t<Storage>::size_type
            { return this->m_val.storage_size(); }
        
        auto append_to(sys_string_builder_t<Storage> & builder) const -> void
            { builder.append(this->m_val); }
    private:
        const sys_string_t<Storage> & m_val;
    };

    template<class Storage>
    class addend<Storage, char32_t>
    {
    public:
        addend(char32_t c) noexcept
        {
            if (!m_encoded.put(c))
                m_encoded.put(U'\uFFFD');
        }
        
        auto storage_size() const noexcept -> typename sys_string_t<Storage>::size_type
            { return typename sys_string_t<Storage>::size_type(this->m_encoded.end() - this->m_encoded.begin()); }
        
        auto append_to(sys_string_builder_t<Storage> & builder) const -> void
            { builder.chars().append(this->m_encoded.begin(), storage_size()); }
    private:
        utf_codepoint_encoder<utf_encoding_of<typename sys_string_t<Storage>::storage_type>, true> m_encoded;
    };

    template<class Storage, class T>
    requires(
        std::is_pointer_v<std::decay_t<T>> &&
        has_utf_encoding<std::remove_const_t<std::remove_pointer_t<std::decay_t<T>>>>
    )
    class addend<Storage, T>
    {
    private: 
        using Char = std::remove_const_t<std::remove_pointer_t<std::decay_t<T>>>;
    public:
        addend(const Char * ptr) noexcept:
            m_view(ptr)
        {}
        
        auto storage_size() const -> typename sys_string_t<Storage>::size_type
        { 
            size_t count = m_view.size();
            if (count > size_t(Storage::max_size))
                throw std::bad_alloc();
            return static_cast<typename sys_string_t<Storage>::size_type>(count);
        }
        
        auto append_to(sys_string_builder_t<Storage> & builder) const -> void
            { builder.append(m_view); }
    private:
        std::basic_string_view<Char> m_view;
    };

    template<class Storage, std::ranges::forward_range Range>
    requires(
        !std::is_pointer_v<std::decay_t<Range>> &&
        has_utf_encoding<std::ranges::range_value_t<Range>>)
    class addend<Storage, Range>
    {
    public:
        addend(const Range & range) noexcept:
            m_range(range)
        {}
        
        auto storage_size() const -> typename sys_string_t<Storage>::size_type
        { 
            size_t count;
            if constexpr (std::is_same_v<std::ranges::range_value_t<Range>, typename sys_string_t<Storage>::storage_type>)
            {
                if constexpr (std::ranges::sized_range<Range>)
                {
                    count = std::ranges::size(this->m_range);
                }
                else
                {
                    count = 0;
                    for(auto & x: this->m_range)
                        ++count;
                }
            }
            else
            {
                using converter = utf_converter<utf_encoding_of<std::ranges::range_value_t<Range>>, 
                                            utf_encoding_of<typename sys_string_t<Storage>::storage_type>>;
                count = converter::converted_length(this->m_range);
            }
            if (count > size_t(Storage::max_size))
                throw std::bad_alloc();
            return static_cast<typename sys_string_t<Storage>::size_type>(count);
        }
        
        auto append_to(sys_string_builder_t<Storage> & builder) const -> void
            { builder.append(this->m_range); }
    private:
        const Range & m_range;
    };

    template<class Storage, class First, class Second>
    class addend<Storage, addition<Storage, First, Second>>
    {
    public:
        addend(const addition<Storage, First, Second> & val) noexcept:
            m_val(val)
        {}
        
        auto storage_size() const -> typename sys_string_t<Storage>::size_type
            { return this->m_val.storage_size(); }
        
        auto append_to(sys_string_builder_t<Storage> & builder) const -> void
            { this->m_val.append_to(builder); }
    private:
        const addition<Storage, First, Second> & m_val;
    };

    template<class Storage, class First, class Second>
    class addition
    {
    public:
        addition(const First & left, const Second & right) noexcept:
            m_first{left},
            m_second{right}
        {}
        
        friend auto operator+(const addition & lhs, const sys_string_t<Storage> & rhs) noexcept
            { return addition<Storage, addition, sys_string_t<Storage>>(lhs, rhs); }
        friend auto operator+(const addition & lhs, char32_t rhs) noexcept
            { return addition<Storage, addition, char32_t>(lhs, rhs); }
        template <std::ranges::forward_range Range>
        friend auto operator+(const addition & lhs, const Range & rhs) noexcept
            { return addition<Storage, addition, Range>(lhs, rhs); }
        
        friend auto operator+(const sys_string_t<Storage> & lhs, const addition & rhs) noexcept
            { return addition<Storage, sys_string_t<Storage>, addition>(lhs, rhs); }
        friend auto operator+(char32_t lhs, const addition & rhs) noexcept
            { return addition<Storage, char32_t, addition>(lhs, rhs); }
        template <std::ranges::forward_range Range>
        friend auto operator+(const Range & lhs, const addition & rhs) noexcept
            { return addition<Storage, Range, addition>(lhs, rhs); }
        
        template<class RHSFirst, class RHSSecond>
        friend auto operator+(const addition & lhs, const addition<Storage, RHSFirst, RHSSecond> & rhs) noexcept
            { return addition<Storage, addition, addition<Storage, RHSFirst, RHSSecond>>(lhs, rhs); }
        
        operator sys_string_t<Storage>() const
        {
            sys_string_builder_t<Storage> builder;
            builder.reserve_storage(this->storage_size());
            this->append_to(builder);
            return builder.build();
        }
        
        auto storage_size() const -> typename sys_string_t<Storage>::size_type
        { 
            auto s1 = this->m_first.storage_size();
            auto s2 = this->m_second.storage_size();
            if (Storage::max_size - s1 < s2)
                throw std::bad_alloc();
            return s1 + s2; 
        }
        
        void append_to(sys_string_builder_t<Storage> & builder) const
        {
            this->m_first.append_to(builder);
            this->m_second.append_to(builder);
        }
    private:
        addend<Storage, First> m_first;
        addend<Storage, Second> m_second;
    };
}
