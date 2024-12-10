//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_GRAPHEME_VIEW_H_INCLUDED
#define HEADER_SYS_STRING_GRAPHEME_VIEW_H_INCLUDED

#include <sys_string/utf_view.h>
#include <sys_string/impl/unicode/algorithms.h>

#include <iterator>
#include <ranges>

namespace sysstr
{
    namespace util
    {
        template<std::forward_iterator It, std::sentinel_for<It> EndIt>
        class grapheme_iterator
        {
        private:
            using iter_type = utf_iterator<utf32, It, EndIt, iter_direction::forward>;
        public:
            using difference_type = std::iter_difference_t<It>;
            using size_type = std::make_unsigned_t<difference_type>;
            using value_type = std::ranges::subrange<It, It>;
            using reference = const value_type;
            using pointer = void;

            using iterator_category = std::forward_iterator_tag;
        public:
            grapheme_iterator() = default;

            grapheme_iterator(It first, EndIt last):
                m_current(first, last),
                m_next(first, last)
            {
                advance_to_grapheme_cluster_break(m_next, std::default_sentinel);
            }

            value_type operator*() const noexcept
                { return value_type{this->m_current.storage_current(), this->m_next.storage_current()}; }

            grapheme_iterator & operator++()
            {
                m_current = m_next;
                advance_to_grapheme_cluster_break(m_next, std::default_sentinel);
                return *this;
            }

            grapheme_iterator operator++(int)
            {
                grapheme_iterator ret = *this;
                ++(*this);
                return ret;
            }

            friend constexpr bool operator==(const grapheme_iterator & lhs, const grapheme_iterator & rhs)
                { return lhs.m_current == rhs.m_current; }
            friend constexpr bool operator!=(const grapheme_iterator & lhs, const grapheme_iterator & rhs)
                { return !(lhs == rhs); }
            
            friend constexpr bool operator==(const grapheme_iterator & lhs, std::default_sentinel_t)
                { return lhs.m_current == std::default_sentinel; }
            friend constexpr bool operator==(std::default_sentinel_t, const grapheme_iterator & rhs)
                { return rhs.m_current == std::default_sentinel; }
            friend constexpr bool operator!=(const grapheme_iterator & lhs, std::default_sentinel_t rhs)
                { return !(lhs == rhs); }
            friend constexpr bool operator!=(std::default_sentinel_t lhs, const grapheme_iterator & rhs)
                { return !(lhs == rhs); }

            
        private:
            iter_type m_current;
            iter_type m_next;
        };

        template<std::input_iterator RawIt, std::sentinel_for<RawIt> RawEndIt>
        class grapheme_iterator<utf_iterator<utf32, RawIt, RawEndIt, iter_direction::forward>, std::default_sentinel_t>
        {
        private:
            using iter_type = utf_iterator<utf32, RawIt, RawEndIt, iter_direction::forward>;
        public:
            using difference_type = std::iter_difference_t<iter_type>;
            using size_type = std::make_unsigned_t<difference_type>;
            using value_type = std::ranges::subrange<iter_type, iter_type>;
            using reference = const value_type;
            using pointer = void;

            using iterator_category = std::forward_iterator_tag;
        public:
            grapheme_iterator() = default;

            grapheme_iterator(iter_type first, iter_type last):
                m_current(first, last),
                m_next(first, last)
            {
                advance_to_grapheme_cluster_break(m_next, std::default_sentinel);
            }

            value_type operator*() const noexcept
                { return value_type{this->m_current, this->m_next}; }

            grapheme_iterator & operator++()
            {
                m_current = m_next;
                advance_to_grapheme_cluster_break(m_next, std::default_sentinel);
                return *this;
            }

            grapheme_iterator operator++(int)
            {
                grapheme_iterator ret = *this;
                ++(*this);
                return ret;
            }

            friend constexpr bool operator==(const grapheme_iterator & lhs, const grapheme_iterator & rhs)
                { return lhs.m_current == rhs.m_current; }
            friend constexpr bool operator!=(const grapheme_iterator & lhs, const grapheme_iterator & rhs)
                { return !(lhs == rhs); }
            
            friend constexpr bool operator==(const grapheme_iterator & lhs, std::default_sentinel_t)
                { return lhs.m_current == std::default_sentinel; }
            friend constexpr bool operator==(std::default_sentinel_t, const grapheme_iterator & rhs)
                { return rhs.m_current == std::default_sentinel; }
            friend constexpr bool operator!=(const grapheme_iterator & lhs, std::default_sentinel_t rhs)
                { return !(lhs == rhs); }
            friend constexpr bool operator!=(std::default_sentinel_t lhs, const grapheme_iterator & rhs)
                { return !(lhs == rhs); }

            
        private:
            iter_type m_current;
            iter_type m_next;
        };
    }
}

#endif
