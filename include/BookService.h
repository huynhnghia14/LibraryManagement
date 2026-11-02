#ifndef BOOK_SERVICE_H
#define BOOK_SERVICE_H

#include <string>
#include "Library.h"
#include "Result.h"

// Quản lý sách (FR-B-01..09)
class BookService {
    Library& lib;

public:
    explicit BookService(Library& l) : lib(l) {}

    Result addBook(const Book& b);
    Result editBook(const std::string& isbn, const Book& updated);
    Result removeBook(const std::string& isbn);
    Book*  findByIsbn(const std::string& isbn);
    template <typename F> void listByIsbn(F visit);     // in-order AVL
    template <typename F> void listByTitle(F visit);    // in-order BST
    DynamicArray<std::string> suggestTitle(const std::string& prefix, int limit = 20);  // Trie
    Result importTxt(const std::string& path);
    Result exportTxt(const std::string& path);
};

#endif
