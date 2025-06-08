#pragma once

#include <algorithm>
#include <flat_map>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>

#include "book_database.hpp"
#include "comparators.hpp"

#include <print>

namespace bookdb {

template <BookContainerLike T>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont) {
    std::flat_map<std::string, size_t, LessByAuthor> histogram;

    for (const auto &[author, books] : cont.GetAuthors()) {
        histogram.emplace(author, books.size());
    }

    return histogram;
}

template <BookContainerLike T>
auto calculateGenreRatings(const BookDatabase<T> &cont) {
    std::flat_map<Genre, std::pair<double, size_t>, LessByGenre> genre_stats;

    for (const auto &book : cont.GetBooks()) {
        auto [it, inserted] = genre_stats.try_emplace(book.genre, std::make_pair(0.0, 0));
        auto &[sum, count] = it->second;
        sum += book.rating;
        count++;
    }

    std::flat_map<Genre, double, LessByGenre> ratings;
    for (const auto &[genre, stats] : genre_stats) {
        const auto &[sum, count] = stats;
        ratings.emplace(genre, sum / count);
    }

    return ratings;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &cont) {
    if (cont.empty()) {
        return 0.0;
    }

    const double sum = std::accumulate(cont.GetBooks().begin(), cont.GetBooks().end(), 0.0,
                                       [](double acc, const Book &book) { return acc + book.rating; });

    return sum / cont.size();
}

template <BookContainerLike T>
std::vector<std::reference_wrapper<const Book>> sampleRandomBooks(const BookDatabase<T> &cont, size_t count) {
    std::vector<std::reference_wrapper<const Book>> result;

    if (cont.empty() || count == 0) {
        return result;
    }
    count = std::min(count, cont.size());

    result.reserve(count);
    std::sample(cont.GetBooks().begin(), cont.GetBooks().end(), std::back_inserter(result), count,
                std::mt19937{std::random_device{}()});

    return result;
}

template <BookContainerLike T>
std::vector<std::reference_wrapper<const Book>> getTopNBy(BookDatabase<T> &cont, size_t count) {
    std::vector<std::reference_wrapper<const Book>> result;

    if (cont.empty() || count == 0) {
        return result;
    }

    count = std::min(count, cont.size());
    result.reserve(count);

    auto &books = const_cast<std::vector<Book> &>(cont.GetBooks());

    std::partial_sort(books.begin(), books.begin() + count, books.end(), GreaterByRating());

    for (size_t i = 0; i < count; ++i) {
        result.push_back(std::cref(books[i]));
    }

    return result;
}

}  // namespace bookdb
