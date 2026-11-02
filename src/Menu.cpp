#include "Menu.h"
#include "UI.h"

#include <iostream>
#include <string>

namespace {
    // In menu và trả về lựa chọn (0..n); -1 nếu hết input
    int showMenu(const std::string& title, const char* const items[], int n) {
        UI::printTitle(title);
        for (int i = 0; i < n; ++i) std::cout << "  " << items[i] << '\n';
        std::cout << "  0. Quay lai / Thoat\n";
        return UI::inputInt("Chon [0-" + std::to_string(n) + "]: ", 0, n);
    }
}

void Menu::notImplemented(int week) {
    UI::printMessage("Chuc nang se duoc cai dat o tuan " + std::to_string(week), false);
}

void Menu::run() {
    static const char* const ITEMS[] = {
        "1. Quan ly sach", "2. Quan ly doc gia", "3. Muon - tra sach",
        "4. Thong ke - bao cao", "5. Goi y - tim kiem nang cao", "6. He thong"
    };
    while (true) {
        int c = showMenu("QUAN LY THU VIEN - MENU CHINH", ITEMS, 6);
        if (c <= 0) break;
        switch (c) {
            case 1: bookMenu(); break;
            case 2: readerMenu(); break;
            case 3: borrowMenu(); break;
            case 4: statsMenu(); break;
            case 5: recommendMenu(); break;
            case 6: systemMenu(); break;
        }
    }
    UI::printMessage("Da luu du lieu. Tam biet!", true);
}

void Menu::bookMenu() {
    static const char* const ITEMS[] = {
        "1. Them sach", "2. Sua sach", "3. Xoa sach", "4. Tim theo ISBN (AVL)",
        "5. In danh muc theo ISBN (AVL)", "6. In danh muc theo ten (BST)",
        "7. Tim theo tien to ten (Trie)", "8. Import tu file .txt", "9. Export ra file .txt"
    };
    int c;
    while ((c = showMenu("QUAN LY SACH", ITEMS, 9)) > 0) {
        notImplemented(c == 7 ? 8 : 3);
    }
}

void Menu::readerMenu() {
    static const char* const ITEMS[] = {
        "1. Them doc gia", "2. Sua / gia han / khoa the", "3. Xoa doc gia",
        "4. Tim theo CMND (Hash)", "5. Danh sach doc gia", "6. Thong ke theo loai the"
    };
    while (showMenu("QUAN LY DOC GIA", ITEMS, 6) > 0) {
        notImplemented(4);
    }
}

void Menu::borrowMenu() {
    static const char* const ITEMS[] = {
        "1. Lap phieu muon", "2. Tra sach (tinh phat)", "3. Phieu dang muon",
        "4. Phieu qua han", "5. Lich su giao dich"
    };
    while (showMenu("MUON - TRA SACH", ITEMS, 5) > 0) {
        notImplemented(6);
    }
}

void Menu::statsMenu() {
    static const char* const ITEMS[] = {
        "1. Luot muon theo thang", "2. Luot muon theo the loai", "3. Luot muon theo doc gia",
        "4. Top 10 sach", "5. Top 10 doc gia", "6. Xuat bao cao .txt / .csv"
    };
    while (showMenu("THONG KE - BAO CAO", ITEMS, 6) > 0) {
        notImplemented(7);
    }
}

void Menu::recommendMenu() {
    static const char* const ITEMS[] = {
        "1. Goi y cung the loai (BFS)", "2. Lien quan theo lich su muon",
        "3. Autocomplete ten sach (Trie)", "4. Demo & so sanh Trie"
    };
    int c;
    while ((c = showMenu("GOI Y - TIM KIEM NANG CAO", ITEMS, 4)) > 0) {
        notImplemented(c <= 2 ? 7 : 8);
    }
}

void Menu::systemMenu() {
    static const char* const ITEMS[] = {
        "1. Sao luu du lieu", "2. Phuc hoi tu sao luu", "3. Quan ly tai khoan (Admin)",
        "4. Doi mat khau", "5. Ngon ngu Viet / Anh", "6. Xem nhat ky (log)"
    };
    while (showMenu("HE THONG", ITEMS, 6) > 0) {
        notImplemented(5);
    }
}
