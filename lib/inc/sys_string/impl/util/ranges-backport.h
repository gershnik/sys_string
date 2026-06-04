//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_RANGES_BACKPORT_H_INCLUDED
#define HEADER_SYS_STRING_RANGES_BACKPORT_H_INCLUDED

#include <ranges>
#include <iterator>

#if defined(_LIBCPP_VERSION) && !SYS_STRING_LIBCPP_AT_LEAST(15000)

#define SYSSTR_AUTO_CAST(expr) static_cast<::std::decay_t<decltype((expr))> >(expr)


namespace std::ranges {

#if !SYS_STRING_LIBCPP_AT_LEAST(14000)
    struct distance_fn
    {
        template<class I, std::sentinel_for<I> S>
            requires (!std::sized_sentinel_for<S, I>)
        constexpr std::iter_difference_t<I> operator()(I first, S last) const
        {
            std::iter_difference_t<I> result = 0;
            while (first != last)
            {
                ++first;
                ++result;
            }
            return result;
        }
        
        template<class I, std::sized_sentinel_for<std::decay_t<I>> S>
        constexpr std::iter_difference_t<I> operator()(const I& first, S last) const
        {
            return last - first;
        }
        
        template<range R>
        constexpr range_difference_t<R> operator()(R&& r) const
        {
            if constexpr (sized_range<std::remove_cvref_t<R>>)
                return static_cast<range_difference_t<R>>(std::ranges::size(r));
            else
                return (*this)(std::ranges::begin(r), std::ranges::end(r));
        }
    };

    inline constexpr auto distance = distance_fn{};

#endif

    template <class T>
    concept can_borrow = std::is_lvalue_reference_v<T> || enable_borrowed_range<std::remove_cvref_t<T>>;

    template <class T>
    concept class_or_enum = std::is_class_v<T> || std::is_union_v<T> || std::is_enum_v<T>;

    namespace rbegin_impl {
        template <class T>
        concept member_rbegin = can_borrow<T> && requires(T&& t) {
            { SYSSTR_AUTO_CAST(t.rbegin()) } -> std::input_or_output_iterator;
        };

        void rbegin() = delete;

        template <class T>
        concept unqualified_rbegin =
            !member_rbegin<T> && can_borrow<T> && class_or_enum<std::remove_cvref_t<T>> && requires(T&& t) {
            { SYSSTR_AUTO_CAST(rbegin(t)) } -> std::input_or_output_iterator;
        };

        template <class T>
        concept can_reverse =
            can_borrow<T> && !member_rbegin<T> && !unqualified_rbegin<T> && requires(T&& t) {
            { std::ranges::begin(t) } -> std::same_as<decltype(std::ranges::end(t))>;
            { std::ranges::begin(t) } -> std::bidirectional_iterator;
        };

        struct fn {
            template <class T>
                requires member_rbegin<T>
            [[nodiscard]] constexpr auto operator()(T&& t) const
                noexcept(noexcept(SYSSTR_AUTO_CAST(t.rbegin()))) {
                return SYSSTR_AUTO_CAST(t.rbegin());
            }

            template <class T>
                requires unqualified_rbegin<T>
            [[nodiscard]] constexpr auto operator()(T&& t) const
                noexcept(noexcept(SYSSTR_AUTO_CAST(rbegin(t)))) {
                return SYSSTR_AUTO_CAST(rbegin(t));
            }

            template <class T>
                requires can_reverse<T>
            [[nodiscard]] constexpr auto operator()(T&& t) const noexcept(noexcept(std::ranges::end(t))) {
                return std::make_reverse_iterator(std::ranges::end(t));
            }

            void operator()(auto&&) const = delete;
        };
    } 

    inline constexpr auto rbegin = rbegin_impl::fn{};

    namespace crbegin_impl {
        struct fn {
            template <class T>
                requires std::is_lvalue_reference_v<T&&>
            [[nodiscard]] constexpr auto operator()(T&& t) const
                noexcept(noexcept(std::ranges::rbegin(static_cast<const std::remove_reference_t<T>&>(t))))
                    -> decltype(std::ranges::rbegin(static_cast<const std::remove_reference_t<T>&>(t))) {
                return std::ranges::rbegin(static_cast<const std::remove_reference_t<T>&>(t));
            }

            template <class T>
                requires std::is_rvalue_reference_v<T&&>
            [[nodiscard]] constexpr auto operator()(T&& t) const
                noexcept(noexcept(std::ranges::rbegin(static_cast<const T&&>(t))))
                    -> decltype(std::ranges::rbegin(static_cast<const T&&>(t))) {
                return std::ranges::rbegin(static_cast<const T&&>(t));
            }
        };
    } 

    inline constexpr auto crbegin = crbegin_impl::fn{};


    namespace rend_impl {
        template <class T>
        concept member_rend = can_borrow<T> && requires(T&& t) {
            std::ranges::rbegin(t);
            { SYSSTR_AUTO_CAST(t.rend()) } -> std::sentinel_for<decltype(std::ranges::rbegin(t))>;
        };

        void rend() = delete;

        template <class T>
        concept unqualified_rend =
            !member_rend<T> && can_borrow<T> && class_or_enum<std::remove_cvref_t<T>> && requires(T&& t) {
            std::ranges::rbegin(t);
            { SYSSTR_AUTO_CAST(rend(t)) } -> std::sentinel_for<decltype(std::ranges::rbegin(t))>;
        };

        template <class T>
        concept can_reverse = can_borrow<T> && !member_rend<T> && !unqualified_rend<T> && requires(T&& t) {
            { std::ranges::begin(t) } -> std::same_as<decltype(std::ranges::end(t))>;
            { std::ranges::begin(t) } -> std::bidirectional_iterator;
        };

        struct fn {
            template <class T>
                requires member_rend<T>
            [[nodiscard]] constexpr auto operator()(T&& t) const
                noexcept(noexcept(SYSSTR_AUTO_CAST(t.rend()))) {
                return SYSSTR_AUTO_CAST(t.rend());
            }

            template <class T>
                requires unqualified_rend<T>
            [[nodiscard]] constexpr auto operator()(T&& t) const
                noexcept(noexcept(SYSSTR_AUTO_CAST(rend(t)))) {
                return SYSSTR_AUTO_CAST(rend(t));
            }

            template <class T>
                requires can_reverse<T>
            [[nodiscard]] constexpr auto operator()(T&& t) const
                noexcept(noexcept(std::ranges::begin(t))) {
                return std::make_reverse_iterator(std::ranges::begin(t));
            }

            void operator()(auto&&) const = delete;
        };
    } 

    inline constexpr auto rend = rend_impl::fn{};

    namespace crend_impl {
        struct fn {
            template <class T>
                requires std::is_lvalue_reference_v<T&&>
            [[nodiscard]] constexpr auto operator()(T&& t) const
                noexcept(noexcept(std::ranges::rend(static_cast<const std::remove_reference_t<T>&>(t))))
                    -> decltype(std::ranges::rend(static_cast<const std::remove_reference_t<T>&>(t))) {
                return std::ranges::rend(static_cast<const std::remove_reference_t<T>&>(t));
            }

            template <class T>
                requires std::is_rvalue_reference_v<T&&>
            [[nodiscard]] constexpr auto operator()(T&& t) const noexcept(
                noexcept(std::ranges::rend(static_cast<const T&&>(t)))) -> decltype(std::ranges::rend(static_cast<const T&&>(t))) {
                return std::ranges::rend(static_cast<const T&&>(t));
            }
        };
    } 

    inline constexpr auto crend = crend_impl::fn{};

    template< class In, class Out >
    concept indirectly_copyable =
        std::indirectly_readable<In> &&
        std::indirectly_writable<Out, std::iter_reference_t<In>>;

#if !SYS_STRING_LIBCPP_AT_LEAST(14000)

    template<class I, class O>
    struct in_out_result
    {
        [[no_unique_address]] I in;
        [[no_unique_address]] O out;

        template<class I2, class O2>
        requires std::convertible_to<const I&, I2> && std::convertible_to<const O&, O2>
        constexpr operator in_out_result<I2, O2>() const &
        {
            return {in, out};
        }

        template<class I2, class O2>
        requires std::convertible_to<I, I2> && std::convertible_to<O, O2>
        constexpr operator in_out_result<I2, O2>() &&
        {
            return {std::move(in), std::move(out)};
        }
    };

#endif


    template< class I, class O >
    using copy_result = in_out_result<I, O>;

    struct copy_fn
    {
        template<std::input_iterator I, std::sentinel_for<I> S, std::weakly_incrementable O>
        requires indirectly_copyable<I, O>
        constexpr copy_result<I, O> operator()(I first, S last, O result) const
        {
            for (; first != last; ++first, (void)++result)
                *result = *first;
            return {std::move(first), std::move(result)};
        }

        template<input_range R, std::weakly_incrementable O>
        requires indirectly_copyable<iterator_t<R>, O>
        constexpr copy_result<borrowed_iterator_t<R>, O>
            operator()(R&& r, O result) const
        {
            return (*this)(std::ranges::begin(r), std::ranges::end(r), std::move(result));
        }
    };

    inline constexpr copy_fn copy;

    template <class I1, class I2, class R, class P1 = std::identity, class P2 = std::identity>
    concept indirectly_comparable = std::indirect_binary_predicate<R, std::projected<I1, P1>, std::projected<I2, P2>>;


    struct equal_fn
    {
        template<std::input_iterator I1, std::sentinel_for<I1> S1,
                std::input_iterator I2, std::sentinel_for<I2> S2,
                class Pred = equal_to,
                class Proj1 = std::identity, class Proj2 = std::identity>
        requires indirectly_comparable<I1, I2, Pred, Proj1, Proj2>
        constexpr bool
            operator()(I1 first1, S1 last1, I2 first2, S2 last2,
                        Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
        {
            if constexpr (std::sized_sentinel_for<S1, I1> and std::sized_sentinel_for<S2, I2>)
                if (std::ranges::distance(first1, last1) != std::ranges::distance(first2, last2))
                    return false;

            for (; first1 != last1; ++first1, (void)++first2)
                if (!std::invoke(pred, std::invoke(proj1, *first1), std::invoke(proj2, *first2)))
                    return false;
            return true;
        }

        template<input_range R1, input_range R2,
                class Pred = equal_to,
                class Proj1 = std::identity, class Proj2 = std::identity>
        requires indirectly_comparable<iterator_t<R1>, iterator_t<R2>,
                                            Pred, Proj1, Proj2>
        constexpr bool
            operator()(R1&& r1, R2&& r2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
        {
            return (*this)(std::ranges::begin(r1), std::ranges::end(r1),
                            std::ranges::begin(r2), std::ranges::end(r2),
                            std::ref(pred), std::ref(proj1), std::ref(proj2));
        }
    };

    inline constexpr equal_fn equal;


    struct search_impl {
        
        template <class Iter1, class Sent1, class Iter2, class Sent2, class Pred, class Proj1, class Proj2>
        static constexpr std::pair<Iter1, Iter1> forward_impl(Iter1 first1, Sent1 last1, Iter2 first2, Sent2 last2, Pred& pred, Proj1& proj1, Proj2& proj2) {
            if (first2 == last2)
                return std::make_pair(first1, first1); // Everything matches an empty sequence
            while (true) {
                // Find first element in sequence 1 that matchs *first2, with a mininum of loop checks
                while (true) {
                    if (first1 == last1) { // return last1 if no element matches *first2
                        std::ranges::advance(first1, last1);
                        return std::make_pair(first1, first1);
                    }
                    if (std::invoke(pred, std::invoke(proj1, *first1), std::invoke(proj2, *first2)))
                        break;
                    ++first1;
                }
                // *first1 matches *first2, now match elements after here
                Iter1 m1 = first1;
                Iter2 m2 = first2;
                while (true) {
                    if (++m2 == last2) // If pattern exhausted, first1 is the answer (works for 1 element pattern)
                        return std::make_pair(first1, ++m1);
                    if (++m1 == last1) { // Otherwise if source exhaused, pattern not found
                        return std::make_pair(m1, m1);
                    }

                    // if there is a mismatch, restart with a new first1
                    if (!std::invoke(pred, std::invoke(proj1, *m1), std::invoke(proj2, *m2))) {
                        ++first1;
                        break;
                    } // else there is a match, check next elements
                }
            }
        }

        template <class Iter1, class Sent1, class Iter2, class Sent2, class Pred, class Proj1, class Proj2,
                  class DiffT1, class DiffT2>
        static constexpr std::pair<Iter1, Iter1> random_impl(Iter1 first1, Sent1 last1, Iter2 first2, Sent2 last2, Pred& pred, Proj1& proj1, Proj2& proj2,
                                                      DiffT1 size1, DiffT2 size2) {
            const Iter1 s = first1 + size1 - DiffT1(size2 - 1); // Start of pattern match can't go beyond here

            while (true) {
                while (true) {
                    if (first1 == s) {
                        std::ranges::advance(first1, last1);
                        return std::make_pair(first1, first1);
                    }
                    if (std::invoke(pred, std::invoke(proj1, *first1), std::invoke(proj2, *first2)))
                        break;
                    ++first1;
                }

                Iter1 m1 = first1;
                Iter2 m2 = first2;
                while (true) {
                    if (++m2 == last2)
                        return std::make_pair(first1, first1 + DiffT1(size2));
                    ++m1; // no need to check range on m1 because s guarantees we have enough source
                    if (!std::invoke(pred, std::invoke(proj1, *m1), std::invoke(proj2, *m2))) {
                        ++first1;
                        break;
                    }
                }
            }
        }


        template <class Iter1, class Sent1, class Iter2, class Sent2, class Pred, class Proj1, class Proj2>
        static constexpr subrange<Iter1> do_search(Iter1 first1,
                                                    Sent1 last1,
                                                    Iter2 first2,
                                                    Sent2 last2,
                                                    Pred& pred,
                                                    Proj1& proj1,
                                                    Proj2& proj2) {
            if constexpr (std::sized_sentinel_for<Sent2, Iter2>) {
                auto size2 = std::ranges::distance(first2, last2);
                if (size2 == 0)
                    return {first1, first1};

                if constexpr (std::sized_sentinel_for<Sent1, Iter1>) {
                    auto size1 = std::ranges::distance(first1, last1);
                    if (size1 < size2) {
                        std::ranges::advance(first1, last1);
                        return {first1, first1};
                    }

                    if constexpr (std::random_access_iterator<Iter1> && std::random_access_iterator<Iter2>) {
                        auto ret = random_impl(first1, last1, first2, last2, pred, proj1, proj2, size1, size2);
                        return {ret.first, ret.second};
                    }
                }
            }

            auto ret = forward_impl(first1, last1, first2, last2, pred, proj1, proj2);
            return {ret.first, ret.second};
        }

        template <std::forward_iterator Iter1,
                  std::sentinel_for<Iter1> Sent1,
                  std::forward_iterator Iter2,
                  std::sentinel_for<Iter2> Sent2,
                  class Pred  = std::ranges::equal_to,
                  class Proj1 = std::identity,
                  class Proj2 = std::identity>
            requires indirectly_comparable<Iter1, Iter2, Pred, Proj1, Proj2>
        [[nodiscard]] constexpr subrange<Iter1> operator()(Iter1 first1,
                                                           Sent1 last1,
                                                           Iter2 first2,
                                                           Sent2 last2,
                                                           Pred pred   = {},
                                                           Proj1 proj1 = {},
                                                           Proj2 proj2 = {}) const {
            return do_search(first1, last1, first2, last2, pred, proj1, proj2);
        }

        template <forward_range Range1,
                  forward_range Range2,
                  class Pred  = std::ranges::equal_to,
                  class Proj1 = std::identity,
                  class Proj2 = std::identity>
            requires indirectly_comparable<iterator_t<Range1>, iterator_t<Range2>, Pred, Proj1, Proj2>
        [[nodiscard]] constexpr borrowed_subrange_t<Range1> operator()(Range1&& range1,
                                                                       Range2&& range2,
                                                                       Pred pred = {}, 
                                                                       Proj1 proj1 = {},
                                                                       Proj2 proj2 = {}) const {
            auto first1 = std::ranges::begin(range1);
            if constexpr (sized_range<Range2>) {
                auto size2 = std::ranges::size(range2);
                if (size2 == 0)
                    return {first1, first1};
                if constexpr (sized_range<Range1>) {
                    auto size1 = std::ranges::size(range1);
                    if (size1 < size2) {
                        std::ranges::advance(first1, std::ranges::end(range1));
                        return {first1, first1};
                    }
                }
            }

            return do_search(std::ranges::begin(range1),
                             std::ranges::end(range1),
                             std::ranges::begin(range2),
                             std::ranges::end(range2),
                             pred,
                             proj1,
                             proj2);
        }
    };


    inline constexpr auto search = search_impl{};


    
    class find_if_impl {

        template <class I, class S, class Pred, class Proj>
        static constexpr I do_find(I first, S last, Pred& pred, Proj& proj) {
            for (; first != last; ++first) {
                if (std::invoke(pred, std::invoke(proj, *first)))
                break;
            }
            return first;
        }

    public:
        template <input_iterator I, sentinel_for<I> S, class Proj = identity,
                  indirect_unary_predicate<projected<I, Proj>> Pred>
        [[nodiscard]] constexpr I operator()(I first, S last, Pred pred, Proj proj = {}) const {
            return do_find(std::move(first), std::move(last), pred, proj);
        }

        template <input_range R, class Proj = identity, indirect_unary_predicate<projected<iterator_t<R>, Proj>> Pred>
        [[nodiscard]] constexpr borrowed_iterator_t<R>
        operator()(R&& r, Pred pred, Proj proj = {}) const {
            return do_find(ranges::begin(r), ranges::end(r), pred, proj);
        }
    };

    inline constexpr auto find_if = find_if_impl{};

    struct find_first_of_impl {
        template <class Iter1, class Sent1, class Iter2, class Sent2, class Pred, class Proj1, class Proj2>
        constexpr static Iter1 impl(Iter1 first1, Sent1 last1, Iter2 first2, Sent2 last2, Pred& pred, Proj1& proj1, Proj2& proj2) {
            for (; first1 != last1; ++first1) {
                for (auto j = first2; j != last2; ++j) {
                    if (std::invoke(pred, std::invoke(proj1, *first1), std::invoke(proj2, *j)))
                        return first1;
                }
            }
            return first1;
        }

        template <input_iterator Iter1,
                  sentinel_for<Iter1> Sent1,
                  forward_iterator Iter2,
                  sentinel_for<Iter2> Sent2,
                  class Pred  = ranges::equal_to,
                  class Proj1 = identity,
                  class Proj2 = identity>
        requires indirectly_comparable<Iter1, Iter2, Pred, Proj1, Proj2>
        [[nodiscard]] constexpr Iter1 operator()(Iter1 first1, Sent1 last1, Iter2 first2, Sent2 last2, 
                                                 Pred pred   = {},
                                                 Proj1 proj1 = {},
                                                 Proj2 proj2 = {}) const {
            return impl(std::move(first1), std::move(last1), std::move(first2), std::move(last2), pred, proj1, proj2);
        }

        template <input_range Range1,
                  forward_range Range2,
                  class Pred  = ranges::equal_to,
                  class Proj1 = identity,
                  class Proj2 = identity>
        requires indirectly_comparable<iterator_t<Range1>, iterator_t<Range2>, Pred, Proj1, Proj2>
        [[nodiscard]] constexpr borrowed_iterator_t<Range1> operator()(Range1&& range1, 
                                                                       Range2&& range2, 
                                                                       Pred pred = {},
                                                                       Proj1 proj1 = {}, 
                                                                       Proj2 proj2 = {}) const {
            return impl(ranges::begin(range1), ranges::end(range1),
                        ranges::begin(range2), ranges::end(range2),
                        pred, proj1, proj2);
        }
    };

    inline constexpr auto find_first_of = find_first_of_impl{};
}


#endif


#endif
