#include "BookService.h"
#include "Utils.h"

#include <cstring>

namespace {
    const int MAX_QUANTITY = 1000;
}

Book BookService::makeBook(const std::string& isbn, const std::string& title, const std::string& author,
                           const std::string& category, int year, int quantity) {
    Book b;
    memset(&b, 0, sizeof b);
    Utils::copyStr(b.isbn, Utils::trim(isbn), ISBN_LEN);
    Utils::copyStr(b.title, Utils::trim(title), TITLE_LEN);
    Utils::copyStr(b.author, Utils::trim(author), AUTHOR_LEN);
    Utils::copyStr(b.category, Utils::trim(category), CATEGORY_LEN);
    b.year = year;
    b.quantity = quantity;
    b.available = quantity;
    return b;
}

Result BookService::validate(const Book& b) {
    if (!Utils::isValidIsbn(b.isbn))          return Result::fail("ISBN phai gom dung 13 chu so");
    if (Utils::normalize(b.title).empty())    return Result::fail("Ten sach khong hop le");
    if (Utils::trim(b.author).empty())        return Result::fail("Tac gia khong duoc de trong");
    if (Utils::trim(b.category).empty())      return Result::fail("The loai khong duoc de trong");
    if (b.year < 1900 || b.year > Utils::currentYear())
        return Result::fail("Nam xuat ban phai tu 1900 den " + std::to_string(Utils::currentYear()));
    if (b.quantity < 1 || b.quantity > MAX_QUANTITY)
        return Result::fail("So luong phai tu 1 den " + std::to_string(MAX_QUANTITY));
    if (b.available < 0 || b.available > b.quantity) return Result::fail("So ban con lai khong hop le");
    return Result::success();
}

Result BookService::addBook(const Book& b) {
    Result v = validate(b);
    if (!v.ok) return v;
    if (!lib.books.insert(b.isbn, b)) return Result::fail(std::string("Da ton tai sach co ISBN ") + b.isbn);
    lib.byTitle.insert(Utils::normalize(b.title), b.isbn);
    lib.markDirty();
    return Result::success(std::string("Da them sach ") + b.isbn);
}

Result BookService::editBook(const std::string& isbn, const Book& updated) {
    Book* cur = lib.books.find(isbn);
    if (!cur) return Result::fail("Khong tim thay sach co ISBN " + isbn);

    int borrowed = cur->quantity - cur->available;
    if (updated.quantity < borrowed)
        return Result::fail("So luong moi nho hon so ban dang cho muon (" + std::to_string(borrowed) + ")");

    Book b = updated;
    Utils::copyStr(b.isbn, isbn, ISBN_LEN);           // không cho đổi khóa chính
    b.available = updated.quantity - borrowed;
    Result v = validate(b);
    if (!v.ok) return v;

    std::string oldKey = Utils::normalize(cur->title), newKey = Utils::normalize(b.title);
    if (oldKey != newKey) {                            // đổi tên -> cập nhật chỉ mục BST
        lib.byTitle.remove(oldKey, isbn);
        lib.byTitle.insert(newKey, isbn);
    }
    *cur = b;
    lib.markDirty();
    return Result::success("Da cap nhat sach " + isbn);
}

Result BookService::removeBook(const std::string& isbn) {
    Book* cur = lib.books.find(isbn);
    if (!cur) return Result::fail("Khong tim thay sach co ISBN " + isbn);
    if (cur->available < cur->quantity) return Result::fail("Sach dang duoc muon, khong the xoa");

    lib.byTitle.remove(Utils::normalize(cur->title), isbn);
    lib.books.remove(isbn);
    lib.markDirty();
    return Result::success("Da xoa sach " + isbn);
}

Book* BookService::findByIsbn(const std::string& isbn) {
    return lib.books.find(Utils::trim(isbn));
}
