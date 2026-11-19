#include "Menu.h"
#include "UI.h"
#include "Logger.h"

#include <cstdio>
#include <iostream>

namespace {
    const int LOG_LINES_SHOWN = 20;
}

void Menu::systemMenu() {
    static const char* const ITEMS[] = {
        "1. Sao luu du lieu", "2. Phuc hoi tu sao luu", "3. Quan ly tai khoan (Admin)",
        "4. Doi mat khau", "5. Ngon ngu Viet / Anh", "6. Xem nhat ky (log)"
    };
    int c;
    while ((c = showMenu("HE THONG", ITEMS, 6)) > 0) {
        switch (c) {
            case 1: backupUI(); break;
            case 2: restoreUI(); break;
            case 6: viewLogUI(); break;
            default: notImplemented(8); break;
        }
    }
}

void Menu::backupUI() {
    UI::printTitle("SAO LUU DU LIEU");
    if (lib.dirty && !lib.save()) {
        UI::printMessage("Khong ghi duoc du lieu hien tai, huy sao luu", false);
        return;
    }
    bool ok = lib.backup(Date::today());
    UI::printMessage(ok ? "Da sao luu vao " + lib.dir() + "/backup/" : "Sao luu that bai", ok);
}

void Menu::restoreUI() {
    UI::printTitle("PHUC HOI TU SAO LUU");
    DynamicArray<std::string> list = lib.listBackups();
    if (list.empty()) {
        UI::printMessage("Chua co ban sao luu nao", false);
        return;
    }
    for (int i = 0; i < list.size(); ++i) std::cout << "  " << i + 1 << ". " << list[i] << '\n';
    int c = UI::inputInt("Chon ban sao luu (0 = huy): ", 0, list.size());
    if (c <= 0) return;
    if (!UI::confirm("Du lieu hien tai se bi ghi de. Tiep tuc?")) return;
    std::string err;
    if (lib.restore(list[c - 1], err)) UI::printMessage("Da phuc hoi du lieu ngay " + list[c - 1], true);
    else UI::printMessage(err, false);
}

// In LOG_LINES_SHOWN dòng cuối của file log (giữ vòng tròn các dòng gần nhất)
void Menu::viewLogUI() {
    UI::printTitle("NHAT KY THAO TAC (" + std::to_string(LOG_LINES_SHOWN) + " dong cuoi)");
    FILE* f = fopen(Logger::filePath().c_str(), "r");
    if (!f) {
        UI::printMessage("Chua co nhat ky", false);
        return;
    }
    std::string ring[LOG_LINES_SHOWN];
    int total = 0;
    char buf[512];
    while (fgets(buf, sizeof buf, f)) ring[total++ % LOG_LINES_SHOWN] = buf;
    fclose(f);
    int start = total > LOG_LINES_SHOWN ? total - LOG_LINES_SHOWN : 0;
    for (int i = start; i < total; ++i) std::cout << ring[i % LOG_LINES_SHOWN];
    std::cout << "(Tong " << total << " dong trong " << Logger::filePath() << ")\n";
}
