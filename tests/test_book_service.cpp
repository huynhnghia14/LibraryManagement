#include "BookService.h"

#include <cassert>
#include <iostream>
#include <string>

Book sample(const std::string& isbn, const std::string& title, int qty = 3) {
    return BookService::makeBook(isbn, title, "Nguyen Van A", "CNTT", 2020, qty);
}

void testAddFindAndDuplicate() {
    Library lib;
    BookService svc(lib);
    assert(svc.addBook(sample("9786040000002", "Lập trình C++")).ok);
    assert(!svc.addBook(sample("9786040000002", "Trùng ISBN")).ok);
    assert(svc.findByIsbn("9786040000002") != nullptr);
    assert(svc.count() == 1 && lib.dirty);
}

void testValidation() {
    Library lib;
    BookService svc(lib);
    assert(!svc.addBook(sample("123", "Sai ISBN")).ok);
    assert(!svc.addBook(sample("9786040000003", "Sai so luong", 0)).ok);
    Book b = sample("9786040000004", "Nam sai");
    b.year = 1800;
    assert(!svc.addBook(b).ok);
}

void testListByIsbnAndTitle() {
    Library lib;
    BookService svc(lib);
    svc.addBook(sample("9786040000003", "Toán rời rạc"));
    svc.addBook(sample("9786040000001", "Giải thuật"));
    svc.addBook(sample("9786040000002", "Cấu trúc dữ liệu"));
    std::string isbns, titles;
    svc.listByIsbn([&](const Book& b) { isbns += b.isbn[12]; });
    svc.listByTitle([&](const Book& b) { titles += b.isbn[12]; });
    assert(isbns == "123");
    assert(titles == "213");                      // Cau truc < Giai thuat < Toan
}

void testEditRenamesInBstAndKeepsBorrowed() {
    Library lib;
    BookService svc(lib);
    svc.addBook(sample("9786040000001", "Ten cu", 5));
    lib.books.find("9786040000001")->available = 3;      // giả lập 2 cuốn đang mượn
    Book upd = sample("9786040000001", "Ten moi", 1);
    assert(!svc.editBook("9786040000001", upd).ok);      // 1 < 2 cuốn đang mượn
    upd.quantity = 4;
    assert(svc.editBook("9786040000001", upd).ok);
    assert(lib.books.find("9786040000001")->available == 2);
    assert(lib.byTitle.find("ten moi") && !lib.byTitle.find("ten cu"));
}

void testRemove() {
    Library lib;
    BookService svc(lib);
    svc.addBook(sample("9786040000001", "A"));
    svc.addBook(sample("9786040000002", "B"));
    lib.books.find("9786040000002")->available = 2;
    assert(!svc.removeBook("9786040000002").ok);         // đang được mượn
    assert(svc.removeBook("9786040000001").ok);
    assert(!svc.removeBook("9786040000001").ok);
    assert(svc.count() == 1 && lib.byTitle.size() == 1);
}

int main() {
    testAddFindAndDuplicate();
    testValidation();
    testListByIsbnAndTitle();
    testEditRenamesInBstAndKeepsBorrowed();
    testRemove();
    std::cout << "test_book_service: passed" << std::endl;
    return 0;
}
