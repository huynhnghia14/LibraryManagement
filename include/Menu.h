#ifndef MENU_H
#define MENU_H

// Menu chính và các menu con; mỗi mục: nhập -> gọi Service -> hiển thị Result
class Menu {
    void bookMenu();
    void readerMenu();
    void borrowMenu();
    void statsMenu();
    void recommendMenu();
    void systemMenu();
    void notImplemented(int week);

public:
    void run();
};

#endif
