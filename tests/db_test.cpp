#include <gtest/gtest.h>

#include "book_database.hpp"
#include "filters.hpp"
#include "statsistics.hpp"

using namespace bookdb;

TEST(BookDataBaseTests, InsertAndGetElementsTest) {
    BookDatabase db;
    EXPECT_TRUE(db.empty());
    EXPECT_EQ(db.size(), 0);

    db.PushBack(Book{"Title1", "Author1", 2025, "SciFi", 1.8, 110});
    EXPECT_FALSE(db.empty());
    EXPECT_EQ(db.size(), 1);

    db.EmplaceBack("Title2", "Author2", 2022, "Fiction", 1.5, 150);
    EXPECT_EQ(db.size(), 2);

    EXPECT_EQ(db[0].author, "Author1");
    EXPECT_EQ(db[0].title, "Title1");
    EXPECT_EQ(db[0].year, 2025);
    EXPECT_EQ(db[0].genre, Genre::SciFi);
    EXPECT_EQ(db[0].rating, 1.8);
    EXPECT_EQ(db[0].read_count, 110);

    EXPECT_EQ(db[1].author, "Author2");
    EXPECT_EQ(db[1].title, "Title2");
    EXPECT_EQ(db[1].year, 2022);
    EXPECT_EQ(db[1].genre, Genre::Fiction);
    EXPECT_EQ(db[1].rating, 1.5);
    EXPECT_EQ(db[1].read_count, 150);

    EXPECT_EQ(db.at(0).author, "Author1");
    EXPECT_EQ(db.at(0).title, "Title1");
    EXPECT_EQ(db.at(0).year, 2025);
    EXPECT_EQ(db.at(0).genre, Genre::SciFi);
    EXPECT_EQ(db.at(0).rating, 1.8);
    EXPECT_EQ(db.at(0).read_count, 110);

    EXPECT_EQ(db.at(1).author, "Author2");
    EXPECT_EQ(db.at(1).title, "Title2");
    EXPECT_EQ(db.at(1).year, 2022);
    EXPECT_EQ(db.at(1).genre, Genre::Fiction);
    EXPECT_EQ(db.at(1).rating, 1.5);
    EXPECT_EQ(db.at(1).read_count, 150);

    EXPECT_EQ(db.front().author, "Author1");
    EXPECT_EQ(db.front().title, "Title1");
    EXPECT_EQ(db.front().year, 2025);
    EXPECT_EQ(db.front().genre, Genre::SciFi);
    EXPECT_EQ(db.front().rating, 1.8);
    EXPECT_EQ(db.front().read_count, 110);

    EXPECT_EQ(db.back().author, "Author2");
    EXPECT_EQ(db.back().title, "Title2");
    EXPECT_EQ(db.back().year, 2022);
    EXPECT_EQ(db.back().genre, Genre::Fiction);
    EXPECT_EQ(db.back().rating, 1.5);
    EXPECT_EQ(db.back().read_count, 150);

    EXPECT_THROW(db.at(2), std::out_of_range);

    db.Clear();
    EXPECT_TRUE(db.empty());
    EXPECT_EQ(db.size(), 0);
    EXPECT_THROW(db.at(0), std::out_of_range);

    db.EmplaceBack("Title3", "Author3", 2023, "Mystery", 4.5, 200);
    EXPECT_FALSE(db.empty());
    EXPECT_EQ(db.size(), 1);
    EXPECT_EQ(db[0].title, "Title3");
    EXPECT_EQ(db[0].author, "Author3");
    EXPECT_EQ(db[0].year, 2023);
    EXPECT_EQ(db[0].genre, Genre::Mystery);
    EXPECT_EQ(db[0].rating, 4.5);
    EXPECT_EQ(db[0].read_count, 200);
}

TEST(BookDataBaseTests, RangeForTest) {
    BookDatabase db{{"Title1", "Author1", 2025, "SciFi", 1.8, 110}, {"Title2", "Author2", 2022, "Fiction", 1.5, 150}};

    size_t idx = 0;
    for (const auto &book : db) {
        if (idx++ == 0) {
            EXPECT_EQ(book.author, "Author1");
            EXPECT_EQ(book.title, "Title1");
            EXPECT_EQ(book.year, 2025);
            EXPECT_EQ(book.genre, Genre::SciFi);
            EXPECT_EQ(book.rating, 1.8);
            EXPECT_EQ(book.read_count, 110);
        } else {
            EXPECT_EQ(book.author, "Author2");
            EXPECT_EQ(book.title, "Title2");
            EXPECT_EQ(book.year, 2022);
            EXPECT_EQ(book.genre, Genre::Fiction);
            EXPECT_EQ(book.rating, 1.5);
            EXPECT_EQ(book.read_count, 150);
        }
    }
}

TEST(BookDataBaseTests, GetAuthorsTest) {
    bookdb::BookDatabase db{{"Title1", "Author1", 2025, "SciFi", 1.8, 110},
                            {"Title2", "Author2", 2022, "Fiction", 1.5, 150}};

    for (const auto &[author, books] : db.GetAuthors()) {
        auto it = std::find_if(
            db.begin(), db.end(),
            [&author, &books](const bookdb::BookDatabase<std::vector<bookdb::Book>>::reference book) {
                return std::find_if(books.begin(), books.end(),
                                    [&book](const auto &book_it) { return &(*book_it) == &book; }) != books.end() &&
                       book.author == author;
            });
        EXPECT_NE(it, db.end());
    }
}

class StatisticsTest : public ::testing::Test {
protected:
    void SetUp() override {
        db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4.5, 190);
        db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.2, 143);
        db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.7, 120);
        db.EmplaceBack("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
        db.EmplaceBack("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.6, 178);
    }

    BookDatabase<std::vector<Book>> db;
};

TEST_F(StatisticsTest, CalculateGenreRatings) {
    auto ratings = calculateGenreRatings(db);
    EXPECT_EQ(ratings.size(), 2);
    EXPECT_NEAR(ratings[Genre::Fiction], 4.575, 0.001);
    EXPECT_NEAR(ratings[Genre::SciFi], 4.5, 0.001);
}

TEST_F(StatisticsTest, CalculateGenreRatingsEmptyDatabase) {
    BookDatabase empty_db;
    auto ratings = calculateGenreRatings(empty_db);
    EXPECT_TRUE(ratings.empty());
}

TEST_F(StatisticsTest, CalculateAverageRating) {
    double avg = calculateAverageRating(db);
    EXPECT_NEAR(avg, 4.56, 0.001);
}

TEST_F(StatisticsTest, CalculateAverageRatingEmptyDatabase) {
    BookDatabase empty_db;
    double avg = calculateAverageRating(empty_db);
    EXPECT_DOUBLE_EQ(avg, 0.0);
}

TEST_F(StatisticsTest, SampleRandomBooks) {
    auto samples = sampleRandomBooks(db, 3);
    EXPECT_EQ(samples.size(), 3);

    for (const auto &book_ref : samples) {
        auto it =
            std::find_if(db.begin(), db.end(), [&book_ref](const Book &book) { return &book == &book_ref.get(); });
        EXPECT_NE(it, db.end());
    }
}

TEST_F(StatisticsTest, SampleRandomBooksEdgeCases) {
    auto samples = sampleRandomBooks(db, 0);
    EXPECT_TRUE(samples.empty());

    samples = sampleRandomBooks(db, 10);
    EXPECT_EQ(samples.size(), db.size());

    BookDatabase empty_db;
    samples = sampleRandomBooks(empty_db, 5);
    EXPECT_TRUE(samples.empty());
}

TEST_F(StatisticsTest, GetTopNBy) {
    auto top_books = getTopNBy(db, 3);
    EXPECT_EQ(top_books.size(), 3);

    for (size_t i = 1; i < top_books.size(); ++i) {
        EXPECT_GE(top_books[i - 1].get().rating, top_books[i].get().rating);
    }
}

TEST_F(StatisticsTest, GetTopNByEdgeCases) {
    auto top_books = getTopNBy(db, 0);
    EXPECT_TRUE(top_books.empty());

    top_books = getTopNBy(db, 10);
    EXPECT_EQ(top_books.size(), db.size());

    BookDatabase empty_db;
    top_books = getTopNBy(empty_db, 5);
    EXPECT_TRUE(top_books.empty());
}

TEST_F(StatisticsTest, FilterBooks) {
    // почему-то так не работает, подскажите пж как исправить
    // auto filtered = filterBooks(db.begin(), db.end(), YearBetween(1900, 1950));
    auto filtered = filterBooks<std::vector<Book>>(db.begin(), db.end(), YearBetween(1900, 1950));
    EXPECT_EQ(filtered.size(), 3);

    filtered = filterBooks<std::vector<Book>>(db.begin(), db.end(), RatingAbove(4.6));
    EXPECT_EQ(filtered.size(), 3);

    filtered = filterBooks<std::vector<Book>>(db.begin(), db.end(), GenreIs(Genre::SciFi));
    EXPECT_EQ(filtered.size(), 1);

    filtered = filterBooks<std::vector<Book>>(db.begin(), db.end(),
                                              all_of(YearBetween(1900, 1950), RatingAbove(4.4), GenreIs(Genre::SciFi)));
    EXPECT_EQ(filtered.size(), 1);

    filtered = filterBooks<std::vector<Book>>(db.begin(), db.end(), any_of(YearBetween(1900, 1950), RatingAbove(4.7)));
    EXPECT_EQ(filtered.size(), 4);
}

TEST_F(StatisticsTest, FilterBooksEdgeCases) {
    auto filtered = filterBooks<std::vector<Book>>(db.begin(), db.begin(), YearBetween(1900, 1950));
    EXPECT_TRUE(filtered.empty());

    BookDatabase<std::vector<Book>> empty_db;
    filtered = filterBooks<std::vector<Book>>(empty_db.begin(), empty_db.end(), YearBetween(1900, 1950));
    EXPECT_TRUE(filtered.empty());

    filtered = filterBooks<std::vector<Book>>(db.begin(), db.end(), YearBetween(2000, 1900));
    EXPECT_TRUE(filtered.empty());
}

TEST_F(StatisticsTest, MemoryManagement) {
    auto top_books = getTopNBy(db, 3);
    auto first_book = top_books[0];

    db.EmplaceBack("New Book", "New Author", 2023, Genre::Fiction, 5.0, 100);

    EXPECT_EQ(first_book.get().title, "To Kill a Mockingbird");
    EXPECT_EQ(first_book.get().rating, 4.8);
}

TEST_F(StatisticsTest, ModernCppFeatures) {
    std::span<const Book> book_span{db.GetBooks()};
    auto filtered = filterBooks<std::span<const Book>>(book_span.begin(), book_span.end(), RatingAbove(4.5));
    EXPECT_EQ(filtered.size(), 4);

    auto ratings = calculateGenreRatings(db);
    std::string genre_str = std::format("Fiction: {:.2f}", ratings[Genre::Fiction]);
    EXPECT_TRUE(genre_str.find("4.57") != std::string::npos);
}
