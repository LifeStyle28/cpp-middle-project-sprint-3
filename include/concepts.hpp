#pragma once

#include <concepts>
#include <iterator>
#include <ranges>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T container) {
    requires std::ranges::range<T>;
    requires std::same_as<std::ranges::range_value_t<T>, Book>;
    requires std::random_access_iterator<typename T::iterator>;

    { container.begin() } -> std::same_as<typename T::iterator>;
    { container.end() } -> std::same_as<typename T::iterator>;
    { container.size() } -> std::convertible_to<std::size_t>;
    { container.empty() } -> std::convertible_to<bool>;
};

template <typename T>
concept BookIterator = requires(T iterator) {
    requires std::input_iterator<T>;
    requires std::same_as<std::iter_value_t<T>, Book>;
    { *iterator } -> std::same_as<Book &>;
    { ++iterator } -> std::same_as<T &>;
    { iterator++ } -> std::same_as<T>;
};

template <typename S, typename I>
concept BookSentinel = requires(S sentinel, I iterator) {
    requires std::sentinel_for<S, I>;
    requires BookIterator<I>;
};

template <typename P>
concept BookPredicate = requires(P predicate, const Book &book) {
    requires std::predicate<P, const Book &>;
    { predicate(book) } -> std::convertible_to<bool>;
};

template <typename C>
concept BookComparator = requires(C comparator, const Book &lhs, const Book &rhs) {
    requires std::strict_weak_order<C, const Book &, const Book &>;
    { comparator(lhs, rhs) } -> std::convertible_to<bool>;
};

}  // namespace bookdb
