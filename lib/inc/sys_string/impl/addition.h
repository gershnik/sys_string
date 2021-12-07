
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
        addend(sys_string_t<Storage> && str) noexcept:
            m_val(std::move(str))
        {}
        
        auto storage_size() const noexcept -> typename sys_string_t<Storage>::size_type
            { return m_val.storage_size(); }
        
        auto append_to(sys_string_builder_t<Storage> & builder) const -> void
            { builder.append(m_val); }
    private:
        sys_string_t<Storage> m_val;
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
            { return typename sys_string_t<Storage>::size_type(m_encoded.end() - m_encoded.begin()); }
        
        auto append_to(sys_string_builder_t<Storage> & builder) const -> void
            { builder.chars().append(m_encoded.begin(), storage_size()); }
    private:
        utf_codepoint_encoder<utf_encoding_of<typename sys_string_t<Storage>::storage_type>, true> m_encoded;
    };

    template<class Storage, class First, class Second>
    class addend<Storage, addition<Storage, First, Second>>
    {
    public:
        addend(addition<Storage, First, Second> && val) noexcept:
            m_val(std::move(val))
        {}
        
        auto storage_size() const noexcept -> typename sys_string_t<Storage>::size_type
            { return m_val.storage_size(); }
        
        auto append_to(sys_string_builder_t<Storage> & builder) const -> void
            { m_val.append_to(builder); }
    private:
        addition<Storage, First, Second> m_val;
    };

    template<class Storage, class First, class Second>
    class addition
    {
    public:
        addition(First left, Second right) noexcept:
            m_first{std::move(left)},
            m_second{std::move(right)}
        {}
        
        friend auto operator+(addition lhs, sys_string_t<Storage> rhs) noexcept
            { return addition<Storage, addition, sys_string_t<Storage>>(std::move(lhs), std::move(rhs)); }
        friend auto operator+(addition lhs, char32_t rhs) noexcept
            { return addition<Storage, addition, char32_t>(std::move(lhs), rhs); }
        
        friend auto operator+(sys_string_t<Storage> lhs, addition rhs) noexcept
            { return addition<Storage, sys_string_t<Storage>, addition>(std::move(lhs), std::move(rhs)); }
        friend auto operator+(char32_t lhs, addition rhs) noexcept
            { return addition<Storage, char32_t, addition>(lhs, std::move(rhs)); }
        
        template<class RHSFirst, class RHSSecond>
        friend auto operator+(addition lhs, addition<Storage, RHSFirst, RHSSecond> rhs) noexcept
            { return addition<Storage, addition, addition<Storage, RHSFirst, RHSSecond>>(std::move(lhs), std::move(rhs)); }
        
        operator sys_string_t<Storage>() const
        {
            sys_string_builder_t<Storage> builder;
            builder.reserve_storage(this->storage_size());
            this->append_to(builder);
            return builder.build();
        }
        
        auto storage_size() const noexcept -> typename sys_string_t<Storage>::size_type
            { return this->m_first.storage_size() + this->m_second.storage_size(); }
        
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
