#pragma once

#include "book.hpp"
#include <functional>

namespace bookdb {

// Компаратор для сравнения книг по автору
struct LessByAuthor {
    constexpr bool operator()(const Book &lhs, const Book &rhs) const { return lhs.author < rhs.author; }
};

// Компаратор для сравнения книг по названию
struct LessByTitle {
    constexpr bool operator()(const Book &lhs, const Book &rhs) const { return lhs.title < rhs.title; }
};

// Компаратор для сравнения книг по году издания
struct LessByYear {
    constexpr bool operator()(const Book &lhs, const Book &rhs) const { return lhs.year < rhs.year; }
};

// Компаратор для сравнения книг по жанру
struct LessByGenre {
    constexpr bool operator()(const Genre &lhs, const Genre &rhs) const {
        return static_cast<int>(lhs) < static_cast<int>(rhs);
    }
};

// Компаратор для сравнения книг по рейтингу (по убыванию)
struct GreaterByRating {
    constexpr bool operator()(const Book &lhs, const Book &rhs) const {
        return lhs.rating > rhs.rating;  // Обратный порядок для сортировки по убыванию
    }
};

// Компаратор для сравнения книг по количеству прочтений (по убыванию)
struct GreaterByReadCount {
    constexpr bool operator()(const Book &lhs, const Book &rhs) const {
        return lhs.read_count > rhs.read_count;  // Обратный порядок для сортировки по убыванию
    }
};

}  // namespace bookdb
