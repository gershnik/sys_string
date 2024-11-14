//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_CHAR_BUFFER_H_INCLUDED
#define HEADER_SYS_STRING_CHAR_BUFFER_H_INCLUDED


#include <algorithm>
#include <cstring>
#include <cassert>

namespace sysstr::util
{ 

    template<class Storage>
    class char_buffer 
    {
    private:
        static_assert(std::is_trivially_copyable_v<typename Storage::value_type>, "Storage::value_type must be trivially copyable");
    public:
        using value_type = typename Storage::value_type;
        using size_type = typename Storage::size_type;
        using difference_type = std::make_signed_t<size_type>;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;
    
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        static constexpr size_type growth_mul_factor = 3;
        static constexpr size_type growth_div_factor = 2;
        static constexpr size_type growth_increment = size_type(std::max(size_t(2), std::max(size_t(16), sizeof(value_type)) / sizeof(value_type)));
    
    public:
        constexpr char_buffer() noexcept = default;
        ~char_buffer() noexcept = default;
        char_buffer(const char_buffer & src) = delete;
        char_buffer & operator=(const char_buffer & src) = delete;
        constexpr char_buffer(char_buffer && src) noexcept:
            m_storage(std::move(src.m_storage)),
            m_size(src.m_size)
        {
            src.m_size = 0;
        }
        constexpr char_buffer & operator=(char_buffer && src) noexcept
        {
            if (this != &src)
            {
                this->m_storage = std::move(src.m_storage);
                this->m_size = src.m_size;
                src.m_size = 0;
            }
            return *this;
        }
    
        constexpr void clear() noexcept
            { this->m_size = 0; }
    
        constexpr size_type size() const noexcept
            { return this->m_size; }
        constexpr size_type capacity() const noexcept
            { return this->m_storage.capacity(); }
        static constexpr size_type max_size() noexcept
            { return Storage::max_size(); }
    
        constexpr bool empty() const noexcept
            { return this->m_size == 0; }
    
        void reserve(size_type new_capacity)
        {
            assert(new_capacity >= 0);
        
            if (new_capacity > this->max_size())
                throw std::bad_alloc();
        
            if (new_capacity <= this->capacity())
                return;
        
            this->m_storage.reallocate(new_capacity, this->m_size);
        }
    
        void resize(size_type new_size)
        {
            assert(new_size >= 0);
        
            if (new_size > this->max_size())
                throw std::bad_alloc();
        
            if (new_size > this->capacity())
                this->m_storage.reallocate(new_size, this->m_size);
        
            if (new_size > this->m_size)
                memset(this->begin() + this->m_size, 0, (new_size - this->m_size) * sizeof(value_type));
        
            this->m_size = new_size;
        }
    
        void push_back(value_type val)
        {
            this->grow();
            this->begin()[this->m_size++] = val;
        }

        void append(const value_type * str, size_type size)
        {
            assert(size >= 0);
            this->grow_by(size);
            memcpy(&this->begin()[this->m_size], str, size * sizeof(value_type));
            this->m_size += size;
        }
    
        constexpr value_type & operator[](size_type idx) noexcept
            { return this->begin()[idx]; }
        constexpr const value_type & operator[](size_type idx) const noexcept
            { return this->begin()[idx]; }
    
        iterator begin() const noexcept
            { return this->m_storage.data(); }
        iterator end() const noexcept
            { return this->m_storage.data() + this->m_size; }
        const_iterator cbegin() const noexcept
            { return this->m_storage.data(); }
        const_iterator cend() const noexcept
            { return this->m_storage.data() + this->m_size; }
        reverse_iterator rbegin() const noexcept
            { return reverse_iterator(end()); }
        reverse_iterator rend() const noexcept
            { return reverse_iterator(begin()); }
        const_reverse_iterator crbegin() const noexcept
            { return const_reverse_iterator(cend()); }
        const_reverse_iterator crend() const noexcept
            { return const_reverse_iterator(cbegin()); }
    
        iterator erase(const_iterator first, const_iterator last) noexcept
        {
            assert(std::less_equal()(this->begin(), first) && std::less_equal()(first, last) && std::less_equal()(last, this->end()));
            iterator mfirst = const_cast<iterator>(first);
            memcpy(mfirst, last, (this->m_size - (last - this->begin())) * sizeof(value_type));
            this->m_size -= size_type(last - first);
            return mfirst;
        }
    
        iterator insert(const_iterator where, value_type val) noexcept
        {
            assert(std::less_equal()(this->begin(), where) && std::less_equal()(where, this->end()));
            auto pos = size_type(where - this->begin());
            this->grow();
            iterator mwhere = this->begin() + pos;
            if (pos != this->m_size)
                memmove(mwhere + 1, mwhere, (this->m_size - pos) * sizeof(value_type));
            *mwhere = val;
            ++this->m_size;
            return mwhere;
        }
    
        iterator insert(const_iterator where, const value_type * str, size_type size) noexcept
        {
            assert(std::less_equal()(this->begin(), where) && std::less_equal()(where, this->end()));
            assert(size >= 0);
            auto pos = size_type(where - this->begin());
            this->grow_by(size);
            iterator mwhere = this->begin() + pos;
            if (pos != this->m_size)
                memmove(mwhere + size, mwhere, (this->m_size - pos) * sizeof(value_type));
            memcpy(mwhere, str, size * sizeof(value_type));
            this->m_size += size;
            return mwhere;
        }
                
        constexpr auto release() noexcept
        {
            this->m_size = 0;
            return this->m_storage.release();
        }
    
    private:
    
        void grow()
        {
            if (this->m_size < this->capacity())
                return;
            
            size_type desired;
        
            if (this->capacity() == 0)
                desired = growth_increment;
            else if (this->capacity() / growth_div_factor < this->max_size() / growth_mul_factor)
                desired = (this->capacity() * growth_mul_factor) / growth_div_factor;
            else if (this->capacity() < this->max_size() - growth_increment)
                desired = this->capacity() + growth_increment;
            else if (this->capacity() == this->max_size())
                throw std::bad_alloc();
            else
                desired = this->max_size();
        
            this->m_storage.reallocate(desired, this->m_size);
        }
    
        void grow_by(size_type size)
        {
            if (this->capacity() - this->m_size >= size)
                return;
        
            if (size > this->max_size() - this->m_size)
                throw std::bad_alloc();

            size_type desired_size = this->m_size + size;
            size_type min_increment = std::max(desired_size - this->capacity(), growth_increment);
        
            size_type desired_capacity;
        
            if (this->capacity() == 0)
                desired_capacity = min_increment;
            else if (this->capacity() / growth_div_factor < this->max_size() / growth_mul_factor)
                desired_capacity = std::max(desired_size, (this->capacity() * growth_mul_factor) / growth_div_factor);
            else if (this->capacity() < this->max_size() - min_increment)
                desired_capacity = this->capacity() + min_increment;
            else
                desired_capacity = desired_size;
        
            this->m_storage.reallocate(desired_capacity, this->m_size);
        
        }
    private:
        Storage m_storage;
        size_type m_size = 0;
    };
}


#endif
