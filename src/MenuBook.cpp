#include "Menu.h"
#include "UI.h"
#include "Utils.h"
#include "Exporter.h"

#include <iostream>
#include <cstdlib>

namespace {
    const int BOOK_COLS = 7;
    const int BOOK_WIDTHS[BOOK_COLS] = {13, 32, 18, 14, 4, 4, 4};
}

void Menu::bookMenu() {
    static const char* const ITEMS[] = {
        "1. Them sach", "2. Sua sach", "3. Xoa sach", "4. Tim theo ISBN (AVL)",
        "5. In danh muc theo ISBN (AVL)", "6. In danh muc theo ten (BST)",
        "7. Tim theo tien to ten (Trie)", "8. Import tu file .txt", "9. Export ra file .txt"
    };
    int c;
    while ((c = showMenu("QUAN LY SACH", ITEMS, 9)) > 0) {
        switch (c) {
            case 1: addBookUI(); break;
            case 2: editBookUI(); break;
            case 3: removeBookUI(); break;
            case 4: findBookUI(); break;
            case 5: listBooksUI(false); break;
            case 6: listBooksUI(true); break;
            case 7: notImplemented(8); break;
            case 8: importBooksUI(); break;
            case 9: exportBooksUI(); break;
        }
    }
}

void Menu::printBookHeader() {
    static const std::string HEAD[BOOK_COLS] = {"ISBN", "Ten sach", "Tac gia", "The loai", "Nam", "SL", "Con"};
    UI::printLine(BOOK_WIDTHS, BOOK_COLS);
    UI::printRow(HEAD, BOOK_WIDTHS, BOOK_COLS);
    UI::printLine(BOOK_WIDTHS, BOOK_COLS);
}

void Menu::printBook(const Book& b) {
    std::string cols[BOOK_COLS] = {b.isbn, b.title, b.author, b.category,
                                   std::to_string(b.year), std::to_string(b.quantity), std::to_string(b.available)};
    UI::printRow(cols, BOOK_WIDTHS, BOOK_COLS);
}

// Nhập thông tin sách; old != nullptr: chế độ sửa, Enter để giữ giá trị cũ
bool Menu::inputBook(Book& b, const Book* old) {
    std::string title, author, category, year, qty;
    if (old) {
        std::cout << "(Nhan Enter de giu nguyen gia tri cu)\n";
        title    = UI::inputOptional("Ten sach [" + std::string(old->title) + "]: ", TITLE_LEN);
        author   = UI::inputOptional("Tac gia [" + std::string(old->author) + "]: ", AUTHOR_LEN);
        category = UI::inputOptional("The loai [" + std::string(old->category) + "]: ", CATEGORY_LEN);
        year     = UI::inputOptional("Nam XB [" + std::to_string(old->year) + "]: ", 4);
        qty      = UI::inputOptional("So luong [" + std::to_string(old->quantity) + "]: ", 4);
        if (title.empty()) title = old->title;
        if (author.empty()) author = old->author;
        if (category.empty()) category = old->category;
        if (year.empty()) year = std::to_string(old->year);
        if (qty.empty()) qty = std::to_string(old->quantity);
    } else {
        title    = UI::inputString("Ten sach: ", TITLE_LEN);
        author   = UI::inputString("Tac gia: ", AUTHOR_LEN);
        category = UI::inputString("The loai: ", CATEGORY_LEN);
        year     = UI::inputString("Nam XB: ", 4);
        qty      = UI::inputString("So luong: ", 4);
    }
    if (title.empty() || author.empty() || category.empty()) return false;
    if (!Utils::isDigits(year, 4, 4) || !Utils::isDigits(qty, 1, 4)) {
        UI::printMessage("Nam XB va so luong phai la so", false);
        return false;
    }
    b = BookService::makeBook(b.isbn, title, author, category, atoi(year.c_str()), atoi(qty.c_str()));
    return true;
}

void Menu::addBookUI() {
    UI::printTitle("THEM SACH");
    std::string isbn = UI::inputString("ISBN (13 chu so): ", ISBN_LEN);
    if (!Utils::isValidIsbn(isbn)) {
        UI::printMessage("ISBN phai gom dung 13 chu so", false);
        return;
    }
    if (bookSvc.findByIsbn(isbn)) {
        UI::printMessage("Da ton tai sach co ISBN " + isbn, false);
        return;
    }
    Book b = BookService::makeBook(isbn, "", "", "", 0, 0);
    if (!inputBook(b, nullptr)) return;
    commit(bookSvc.addBook(b));
}

void Menu::editBookUI() {
    UI::printTitle("SUA SACH");
    std::string isbn = UI::inputString("ISBN can sua: ", ISBN_LEN);
    Book* cur = bookSvc.findByIsbn(isbn);
    if (!cur) {
        UI::printMessage("Khong tim thay sach co ISBN " + isbn, false);
        return;
    }
    Book b = *cur;
    if (!inputBook(b, cur)) return;
    commit(bookSvc.editBook(isbn, b));
}

void Menu::removeBookUI() {
    UI::printTitle("XOA SACH");
    std::string isbn = UI::inputString("ISBN can xoa: ", ISBN_LEN);
    Book* cur = bookSvc.findByIsbn(isbn);
    if (!cur) {
        UI::printMessage("Khong tim thay sach co ISBN " + isbn, false);
        return;
    }
    printBookHeader();
    printBook(*cur);
    UI::printLine(BOOK_WIDTHS, BOOK_COLS);
    if (!UI::confirm("Xac nhan xoa sach nay?")) return;
    commit(bookSvc.removeBook(isbn));
}

void Menu::findBookUI() {
    UI::printTitle("TIM SACH THEO ISBN (AVL)");
    std::string isbn = UI::inputString("ISBN: ", ISBN_LEN);
    Book* b = bookSvc.findByIsbn(isbn);
    if (!b) {
        UI::printMessage("Khong tim thay sach co ISBN " + isbn, false);
        return;
    }
    printBookHeader();
    printBook(*b);
    UI::printLine(BOOK_WIDTHS, BOOK_COLS);
}

void Menu::listBooksUI(bool byTitle) {
    UI::printTitle(byTitle ? "DANH MUC SACH THEO TEN (BST in-order)" : "DANH MUC SACH THEO ISBN (AVL in-order)");
    if (bookSvc.count() == 0) {
        UI::printMessage("Chua co sach nao", false);
        return;
    }
    printBookHeader();
    if (byTitle) bookSvc.listByTitle([&](const Book& b) { printBook(b); });
    else         bookSvc.listByIsbn([&](const Book& b) { printBook(b); });
    UI::printLine(BOOK_WIDTHS, BOOK_COLS);
    std::cout << "Tong so: " << bookSvc.count() << " dau sach\n";
}

void Menu::importBooksUI() {
    UI::printTitle("IMPORT SACH TU FILE VAN BAN");
    std::cout << "Dinh dang moi dong: ISBN|Ten sach|Tac gia|The loai|Nam XB|So luong\n";
    std::string path = UI::inputString("Duong dan file (vd: data/sample/books.txt): ", 200);
    if (path.empty()) return;
    commit(Exporter::importBooksTxt(lib, path));
}

void Menu::exportBooksUI() {
    UI::printTitle("EXPORT DANH MUC SACH");
    int kind = UI::inputInt("Dinh dang (1 = .txt, 2 = .csv): ", 1, 2);
    if (kind < 1) return;
    std::string def = lib.dir() + (kind == 1 ? "/books_export.txt" : "/books_export.csv");
    std::string path = UI::inputOptional("Duong dan file [" + def + "]: ", 200);
    if (path.empty()) path = def;
    UI::printResult(kind == 1 ? Exporter::exportBooksTxt(lib, path) : Exporter::exportBooksCsv(lib, path));
}
