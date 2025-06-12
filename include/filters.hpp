#pragma once

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

auto YearBetween(int start, int end) {
    return [start, end](const Book &book) { return book.year >= start && book.year <= end; };
}

auto RatingAbove(double min_rating) {
    return [min_rating](const Book &book) { return book.rating >= min_rating; };
}

auto GenreIs(Genre genre) {
    return [genre](const Book &book) { return book.genre == genre; };
}

template <typename... Predicates>
auto all_of(Predicates &&...preds) {
    return [preds...](const Book &book) { return (preds(book) && ...); };
}

template <typename... Predicates>
auto any_of(Predicates &&...preds) {
    return [preds...](const Book &book) { return (preds(book) || ...); };
}

template <BookIterator It>
[[nodiscard]] std::vector<std::reference_wrapper<const Book>> filterBooks(const It begin,
                                                            const It end,
                                                            std::predicate<const Book &> auto predicate) {

    std::vector<std::reference_wrapper<const Book>> result;
    result.reserve(std::distance(begin, end));

    std::copy_if(begin, end, std::back_inserter(result), [&predicate](const Book &book) { return predicate(book); });
    return result;
}

template <typename T>
[[nodiscard]] std::vector<std::reference_wrapper<const Book>> filterBooks(std::span<T> span, const auto& predicate) {
    std::vector<std::reference_wrapper<const Book>> result;
    for (const auto& book : span) {
        if (predicate(book)) {
            result.push_back(std::cref(book));
        }
    }
    return result;
}

}  // namespace bookdb
