#include "Library.h"
#include "Menu.h"
#include "UI.h"

int main() {
    UI::printTitle("HE THONG QUAN LY THU VIEN");
    Library lib;
    Menu menu(lib);
    menu.run();
    return 0;
}
