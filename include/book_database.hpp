#pragma once

#include <list>
#include <print>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases
    using value_type = typename BookContainer::value_type;
    using reference = typename BookContainer::reference;
    using const_reference = typename BookContainer::const_reference;
    using iterator = typename BookContainer::iterator;
    using const_iterator = typename BookContainer::const_iterator;
    using reverse_iterator = typename BookContainer::reverse_iterator;
    using const_reverse_iterator = typename BookContainer::const_reverse_iterator;
    using difference_type = typename BookContainer::difference_type;
    using size_type = typename BookContainer::size_type;

    using AuthorContainer = std::unordered_map<std::string, std::list<std::string>,
                                               TransparentStringHash, TransparentStringEqual>;

    BookDatabase() = default;

    BookDatabase(std::initializer_list<Book> books) : books_{books} {}

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    // Standard container interface methods
    bool empty() const noexcept { return books_.empty(); }

    size_type size() const noexcept { return books_.size(); }

    iterator begin() { return books_.begin(); }

    const_iterator cbegin() const { return books_.cbegin(); }

    iterator end() { return books_.end(); }

    const_iterator cend() const { return books_.cend(); }

    reverse_iterator rbegin() { return std::make_reverse_iterator(end()); }

    const_reverse_iterator crbegin() const { return std::make_reverse_iterator(cend()); }

    reverse_iterator rend() { return std::make_reverse_iterator(begin()); }

    const_reverse_iterator crend() const { return std::make_reverse_iterator(cbegin()); }

    reference operator[](const size_t idx) noexcept { return books_[idx]; }

    const_reference operator[](const size_t idx) const noexcept { return books_[idx]; }

    reference at(const size_t idx) { return books_.at(idx); }

    const_reference at(const size_t idx) const { return books_.at(idx); }

    reference front() noexcept { return books_[0]; }

    const_reference front() const noexcept { return books_[0]; }

    reference back() noexcept { return books_[size() - 1]; }

    const_reference back() const noexcept { return books_[size() - 1]; }

    // Ваш код здесь
    void PushBack(Book &&book) {
        books_.push_back(std::move(book));
        authors_emplace();
    }

    template <class... Args>
    void EmplaceBack(Args &&...args) {
        books_.emplace_back(std::forward<Args>(args)...);
        authors_emplace();
    }

    [[nodiscard]] const BookContainer &GetBooks() const noexcept { return books_; }

    [[nodiscard]] const AuthorContainer &GetAuthors() const noexcept { return authors_; }

private:
    void authors_emplace() {
        auto it = std::prev(books_.end());
        auto [author_it, inserted] = authors_.try_emplace(std::string(it->author));
        if (inserted) {
            author_it->second = std::list<std::string>{it->title};
        } else {
            author_it->second.emplace_back(it->title);
        }
        it->author = author_it->first;
    }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto& book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto& [author, books] : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
