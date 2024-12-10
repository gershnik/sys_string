//
// Copyright 2024 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_BIT_ARRAY_H_INCLUDED
#define HEADER_SYS_STRING_BIT_ARRAY_H_INCLUDED

#include <sys_string/config.h>

#include <type_traits>
#include <limits>
#include <climits>
#include <initializer_list>


namespace sysstr
{
    template<size_t BitsPerUnit, size_t Count>
    class bit_array
    {
        static_assert(BitsPerUnit <= CHAR_BIT * sizeof(unsigned long long), "BitsPerUnit must be less than number of bits in unsigned long long");
        static_assert(std::numeric_limits<size_t>::max() / Count > BitsPerUnit, "Count * BitsPerUnit doesn't fit in size_t");
    public:
        using value_type = std::conditional_t<BitsPerUnit <= CHAR_BIT,                          unsigned char,
                           std::conditional_t<BitsPerUnit <= CHAR_BIT * sizeof(unsigned short), unsigned short,
                           std::conditional_t<BitsPerUnit <= CHAR_BIT * sizeof(unsigned int),   unsigned int,
                           std::conditional_t<BitsPerUnit <= CHAR_BIT * sizeof(unsigned long),  unsigned long,
                                                                                                unsigned long long>>>>;
        using reference              = value_type;
        using const_reference        = const value_type;
        using size_type              = size_t;
        using difference_type        = ptrdiff_t;

        
    private:
        static constexpr size_t s_total_bits = Count * BitsPerUnit;
        static constexpr size_t s_bits_per_value = sizeof(value_type) * CHAR_BIT;
        static constexpr size_t s_unit_offset_in_value = s_bits_per_value - BitsPerUnit;
        static constexpr value_type s_max_value = (std::numeric_limits<value_type>::max() >> s_unit_offset_in_value);

        static constexpr size_t value_count() noexcept
            { return s_total_bits / s_bits_per_value + bool(s_total_bits % s_bits_per_value); }

        constexpr void write(size_t idx, value_type val) noexcept
        {
            val &= s_max_value;

            size_t bit_idx = idx * BitsPerUnit;
            size_t start_idx = bit_idx / s_bits_per_value;
            size_t start_offset = bit_idx % s_bits_per_value;

            value_type & current0 = m_elems[start_idx];
            if (s_unit_offset_in_value >= start_offset)
            {
                auto shift = s_unit_offset_in_value - start_offset;
                current0 &= ~(s_max_value << shift);
                current0 |= (val << shift);
            }
            else
            {
                auto shift0 = start_offset - s_unit_offset_in_value;
                current0 &= ~(s_max_value >> shift0);
                current0 |= (val >> shift0);
                value_type & current1 = m_elems[start_idx + 1];
                auto shift1 = s_bits_per_value - shift0;
                current1 &= ~(s_max_value << shift1);
                current1 |= (val << shift1);
            }
        }

        constexpr value_type read(size_t idx) const noexcept
        {
            size_t bit_idx = idx * BitsPerUnit;
            size_t start_idx = bit_idx / s_bits_per_value;
            size_t start_offset = bit_idx % s_bits_per_value;

            value_type ret = 0;

            const value_type & current0 = m_elems[start_idx];
            if (s_unit_offset_in_value >= start_offset)
            {
                auto shift = s_unit_offset_in_value - start_offset;
                ret = (current0 >> shift) & s_max_value;
            }
            else
            {
                auto shift0 = start_offset - s_unit_offset_in_value;
                ret = (current0 & (s_max_value >> shift0)) << shift0;
                const value_type & current1 = m_elems[start_idx + 1];
                auto shift1 = s_bits_per_value - shift0;
                ret |= (current1 & (s_max_value << shift1)) >> shift1;
            }
            return ret;
        }

        class proxy
        {
        public:
            proxy(bit_array & owner, size_t idx) noexcept:
                m_owner(owner), m_idx(idx)
            {}
            operator value_type() const noexcept
                { return m_owner.read(m_idx); }
            proxy & operator=(value_type val) noexcept
            { 
                m_owner.write(m_idx, val);
                return *this;
            }
        private:
            bit_array & m_owner;
            size_t m_idx;
        };

    public:
        constexpr bit_array(const value_type (& init)[Count]) noexcept
        {
            size_t i = 0;
            for(auto val: init)
                write(i++, val);
        }

        constexpr value_type operator[](size_t idx) const noexcept
            { return read(idx); }
        constexpr proxy operator[](size_t idx) noexcept
            { return proxy(*this, idx); }

    private:                                                                                            
        value_type m_elems[value_count()] = {};
    };
}


#endif
