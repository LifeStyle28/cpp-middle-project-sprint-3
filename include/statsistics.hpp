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

namespace std {
template <typename K, typename V, typename C>
struct formatter<flat_map<K, V, C>> {
    template <typename FormatContext>
    auto format(const flat_map<K, V, C>& fmap, FormatContext& fc) const {
        for (const auto& [author, size] : fmap) {
            format_to(fc.out(), "{}: {}\n", author, size);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std

namespace bookdb {

template <BookContainerLike T>
[[nodiscard]] auto buildAuthorHistogramFlat(const BookDatabase<T>& cont) {
    std::flat_map<std::string, size_t, LessByAuthor> histogram;

    for (const auto& [author, books] : cont.GetAuthors()) {
        histogram.emplace(author, books.size());
    }

    return histogram;
}

class GenreStats {
public:
    void Add(double rating) {
        total_rating += rating;
        ++count;
    }
    double Average() const {
        return total_rating / count;
    }
    void Reset() {
        total_rating = 0.0;
        count = 0;
    }

private:
    double total_rating = 0.0;
    size_t count = 0;
};

template <BookIterator It>
[[nodiscard]] auto calculateGenreRatings(It begin, It end) {
    std::flat_map<Genre, GenreStats, LessByGenre> genre_stats;

    std::for_each(begin, end, [&genre_stats](const auto& book) {
        auto [it, inserted] = genre_stats.try_emplace(book.genre, GenreStats{});
        it->second.Add(book.rating);
    });

    std::flat_map<Genre, double, LessByGenre> ratings;
    for (const auto& [genre, stats] : genre_stats) {
        ratings.emplace(genre, stats.Average());
    }

    return ratings;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T>& cont) {
    if (cont.empty()) {
        return 0.0;
    }

    const double sum = std::accumulate(cont.GetBooks().begin(), cont.GetBooks().end(), 0.0,
                                       [](double acc, const Book& book) { return acc + book.rating; });

    return sum / cont.size();
}

template <BookContainerLike T>
[[nodiscard]] std::vector<std::reference_wrapper<const Book>> sampleRandomBooks(const BookDatabase<T>& cont, size_t count) {
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

template <BookContainerLike T, typename Comp>
[[nodiscard]] std::vector<std::reference_wrapper<const Book>> getTopNBy(BookDatabase<T>& cont, size_t count, Comp comp) {
    std::vector<std::reference_wrapper<const Book>> result;

    if (cont.empty() || count == 0) {
        return result;
    }

    count = std::min(count, cont.size());
    result.reserve(count);

    auto& books = const_cast<std::vector<Book>& >(cont.GetBooks());

    std::partial_sort(books.begin(), books.begin() + count, books.end(),
        [&comp](const Book& a, const Book& b) { return comp(a, b); });

    for (size_t i = 0; i < count; ++i) {
        result.push_back(std::cref(books[i]));
    }

    return result;
}

}  // namespace bookdb
