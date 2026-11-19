#include "Menu.h"
#include "UI.h"
#include "Utils.h"

#include <iostream>

namespace {
    const int READER_COLS = 7;
    const int READER_WIDTHS[READER_COLS] = {12, 24, 24, 10, 10, 10, 9};

    const char* statusName(int s) { return s == READER_LOCKED ? "Khoa" : "Hoat dong"; }
}

void Menu::readerMenu() {
    static const char* const ITEMS[] = {
        "1. Them doc gia", "2. Sua / gia han / khoa the", "3. Xoa doc gia",
        "4. Tim theo CMND (Hash)", "5. Danh sach doc gia", "6. Thong ke theo loai the"
    };
    int c;
    while ((c = showMenu("QUAN LY DOC GIA", ITEMS, 6)) > 0) {
        switch (c) {
            case 1: addReaderUI(); break;
            case 2: editReaderUI(); break;
            case 3: removeReaderUI(); break;
            case 4: findReaderUI(); break;
            case 5: listReadersUI(); break;
            case 6: readerStatsUI(); break;
        }
    }
}

void Menu::printReaderHeader() {
    static const std::string HEAD[READER_COLS] = {"CMND", "Ho ten", "Email", "SDT", "Loai the", "Het han", "TT"};
    UI::printLine(READER_WIDTHS, READER_COLS);
    UI::printRow(HEAD, READER_WIDTHS, READER_COLS);
    UI::printLine(READER_WIDTHS, READER_COLS);
}

void Menu::printReader(const Reader& r) {
    std::string cols[READER_COLS] = {r.cmnd, r.name, r.email, r.phone, ReaderService::cardTypeName(r.cardType),
                                     r.expDate.toString(), statusName(r.status)};
    UI::printRow(cols, READER_WIDTHS, READER_COLS);
}

// Nhập họ tên, email, SĐT, loại thẻ; old != nullptr: Enter để giữ giá trị cũ
bool Menu::inputReaderInfo(Reader& r, const Reader* old) {
    std::string name, email, phone;
    if (old) {
        std::cout << "(Nhan Enter de giu nguyen gia tri cu)\n";
        name  = UI::inputOptional("Ho ten [" + std::string(old->name) + "]: ", NAME_LEN);
        email = UI::inputOptional("Email [" + std::string(old->email) + "]: ", EMAIL_LEN);
        phone = UI::inputOptional("SDT [" + std::string(old->phone) + "]: ", PHONE_LEN);
        if (name.empty()) name = old->name;
        if (email.empty()) email = old->email;
        if (phone.empty()) phone = old->phone;
    } else {
        name  = UI::inputString("Ho ten: ", NAME_LEN);
        email = UI::inputString("Email: ", EMAIL_LEN);
        phone = UI::inputString("So dien thoai: ", PHONE_LEN);
    }
    if (name.empty() || email.empty() || phone.empty()) return false;
    int type = UI::inputInt("Loai the (0 = Sinh vien, 1 = Giang vien, 2 = Thuong): ", 0, 2);
    if (type < 0) return false;
    Date reg = old ? old->regDate : Date::today();
    r = readerSvc.makeReader(r.cmnd, name, email, phone, type, reg);
    return true;
}

void Menu::addReaderUI() {
    UI::printTitle("THEM DOC GIA");
    std::string cmnd = UI::inputString("CMND/CCCD (9 hoac 12 chu so): ", CMND_LEN);
    if (!Utils::isValidCmnd(cmnd)) {
        UI::printMessage("CMND/CCCD phai gom 9 hoac 12 chu so", false);
        return;
    }
    if (readerSvc.findByCmnd(cmnd)) {
        UI::printMessage("Da ton tai doc gia co CMND " + cmnd, false);
        return;
    }
    Reader r = readerSvc.makeReader(cmnd, "", "", "", CARD_NORMAL, Date::today());
    if (!inputReaderInfo(r, nullptr)) return;
    commit(readerSvc.addReader(r));
}

void Menu::editReaderUI() {
    UI::printTitle("SUA / GIA HAN / KHOA THE");
    std::string cmnd = UI::inputString("CMND: ", CMND_LEN);
    Reader* cur = readerSvc.findByCmnd(cmnd);
    if (!cur) {
        UI::printMessage("Khong tim thay doc gia co CMND " + cmnd, false);
        return;
    }
    printReaderHeader();
    printReader(*cur);
    UI::printLine(READER_WIDTHS, READER_COLS);

    static const char* const ITEMS[] = {"1. Sua thong tin", "2. Gia han the", "3. Khoa the", "4. Mo khoa the"};
    int c = showMenu("CHON THAO TAC", ITEMS, 4);
    if (c == 1) {
        Reader r = *cur;
        if (inputReaderInfo(r, cur)) commit(readerSvc.editReader(cmnd, r));
    } else if (c == 2) {
        commit(readerSvc.renewCard(cmnd, Date::today()));
    } else if (c == 3 || c == 4) {
        commit(readerSvc.lockReader(cmnd, c == 3));
    }
}

void Menu::removeReaderUI() {
    UI::printTitle("XOA DOC GIA");
    std::string cmnd = UI::inputString("CMND can xoa: ", CMND_LEN);
    Reader* cur = readerSvc.findByCmnd(cmnd);
    if (!cur) {
        UI::printMessage("Khong tim thay doc gia co CMND " + cmnd, false);
        return;
    }
    printReaderHeader();
    printReader(*cur);
    UI::printLine(READER_WIDTHS, READER_COLS);
    if (!UI::confirm("Xac nhan xoa doc gia nay?")) return;
    commit(readerSvc.removeReader(cmnd));
}

void Menu::findReaderUI() {
    UI::printTitle("TIM DOC GIA THEO CMND (Hash Table)");
    std::string cmnd = UI::inputString("CMND: ", CMND_LEN);
    Reader* r = readerSvc.findByCmnd(cmnd);
    if (!r) {
        UI::printMessage("Khong tim thay doc gia co CMND " + cmnd, false);
        return;
    }
    printReaderHeader();
    printReader(*r);
    UI::printLine(READER_WIDTHS, READER_COLS);
    std::cout << "Ngay dang ky: " << r->regDate.toString() << '\n';
}

void Menu::listReadersUI() {
    UI::printTitle("DANH SACH DOC GIA");
    if (readerSvc.count() == 0) {
        UI::printMessage("Chua co doc gia nao", false);
        return;
    }
    printReaderHeader();
    readerSvc.listAll([&](const Reader& r) { printReader(r); });
    UI::printLine(READER_WIDTHS, READER_COLS);
    std::cout << "Tong so: " << readerSvc.count() << " doc gia | Bang bam: " << lib.readers.bucketCount()
              << " bucket, he so tai " << lib.readers.loadFactor()
              << ", chuoi dai nhat " << lib.readers.longestChain() << '\n';
}

void Menu::readerStatsUI() {
    UI::printTitle("THONG KE DOC GIA THEO LOAI THE");
    int counts[3];
    readerSvc.statsByCardType(counts);
    static const int W[2] = {14, 10};
    static const std::string HEAD[2] = {"Loai the", "So luong"};
    UI::printLine(W, 2);
    UI::printRow(HEAD, W, 2);
    UI::printLine(W, 2);
    for (int t = 0; t < 3; ++t) {
        std::string row[2] = {ReaderService::cardTypeName(t), std::to_string(counts[t])};
        UI::printRow(row, W, 2);
    }
    UI::printLine(W, 2);
    std::cout << "Tong: " << readerSvc.count() << '\n';
}
