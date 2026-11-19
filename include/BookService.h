#ifndef BOOK_SERVICE_H
#define BOOK_SERVICE_H

#include <string>
#include "Library.h"
#include "Result.h"

// Quản lý sách (FR-B): thêm / sửa / xóa / tìm / in danh mục
class BookService {
    Library& lib;

public:
    explicit BookService(Library& l) : lib(l) {}

    static Book   makeBook(const std::string& isbn, const std::string& title, const std::string& author,
                           const std::string& category, int year, int quantity);
    static Result validate(const Book& b);

    Result addBook(const Book& b);
    Result editBook(const std::string& isbn, const Book& updated);   // không đổi ISBN
    Result removeBook(const std::string& isbn);
    Book*  findByIsbn(const std::string& isbn);
    int    count() const { return lib.books.size(); }

    // visit(const Book&) theo ISBN tăng dần – duyệt in-order cây AVL
    template <typename F>
    void listByIsbn(F visit) {
        lib.books.inorder([&](const std::string&, Book& b) { visit(b); });
    }

    // visit(const Book&) theo tên A-Z – duyệt in-order cây BST
    template <typename F>
    void listByTitle(F visit) {
        lib.byTitle.inorder([&](const std::string&, std::string& isbn) {
            Book* b = lib.books.find(isbn);
            if (b) visit(*b);
        });
    }
};

#endif
