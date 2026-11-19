#ifndef MENU_H
#define MENU_H

#include <string>
#include "Library.h"
#include "BookService.h"

// Menu chính và các menu con; mỗi mục: nhập -> gọi Service -> hiển thị Result
class Menu {
    Library&    lib;
    BookService bookSvc;

    // Quản lý sách (MenuBook.cpp)
    void bookMenu();
    bool inputBook(Book& b, const Book* old);
    void addBookUI();
    void editBookUI();
    void removeBookUI();
    void findBookUI();
    void listBooksUI(bool byTitle);
    void printBookHeader();
    void printBook(const Book& b);

    void readerMenu();
    void borrowMenu();
    void statsMenu();
    void recommendMenu();
    void systemMenu();
    void notImplemented(int week);

public:
    explicit Menu(Library& l);
    void run();
};

// In menu và trả về lựa chọn (0..n); giá trị < 0 nếu hết input
int showMenu(const std::string& title, const char* const items[], int n);

#endif
