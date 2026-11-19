#ifndef MENU_H
#define MENU_H

#include <string>
#include "Library.h"
#include "BookService.h"
#include "ReaderService.h"

// Menu chính và các menu con; mỗi mục: nhập -> gọi Service -> hiển thị Result
class Menu {
    Library&      lib;
    BookService   bookSvc;
    ReaderService readerSvc;

    // Quản lý sách (MenuBook.cpp)
    void bookMenu();
    bool inputBook(Book& b, const Book* old);
    void addBookUI();
    void editBookUI();
    void removeBookUI();
    void findBookUI();
    void listBooksUI(bool byTitle);
    void importBooksUI();
    void exportBooksUI();
    void printBookHeader();
    void printBook(const Book& b);

    // Quản lý độc giả (MenuReader.cpp)
    void readerMenu();
    bool inputReaderInfo(Reader& r, const Reader* old);
    void addReaderUI();
    void editReaderUI();
    void removeReaderUI();
    void findReaderUI();
    void listReadersUI();
    void readerStatsUI();
    void printReaderHeader();
    void printReader(const Reader& r);

    void borrowMenu();
    void statsMenu();
    void recommendMenu();
    // Hệ thống (MenuSystem.cpp)
    void systemMenu();
    void backupUI();
    void restoreUI();
    void viewLogUI();

    void commit(const Result& r);   // in kết quả, lưu file ngay nếu dữ liệu thay đổi
    void notImplemented(int week);

public:
    explicit Menu(Library& l);
    void run();
};

// In menu và trả về lựa chọn (0..n); giá trị < 0 nếu hết input
int showMenu(const std::string& title, const char* const items[], int n);

#endif
