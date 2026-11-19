#include "Library.h"
#include "Menu.h"
#include "UI.h"
#include "Logger.h"
#include "Exporter.h"

#include <iostream>

namespace {
    const char* const DATA_DIR = "data";

    // Lần chạy đầu (chưa có file .dat): nạp dữ liệu mẫu từ data/sample/*.txt
    void seedSampleData(Library& lib) {
        std::string sample = lib.dir() + "/sample/";
        UI::printMessage("Chua co du lieu, nap du lieu mau tu " + sample, true);
        UI::printResult(Exporter::importBooksTxt(lib, sample + "books.txt"));
        UI::printResult(Exporter::importReadersTxt(lib, sample + "readers.txt"));
        if (!lib.save()) UI::printMessage("Khong ghi duoc du lieu ra thu muc " + lib.dir(), false);
    }

    // File hỏng: đề nghị phục hồi từ bản sao lưu mới nhất
    bool recover(Library& lib, const std::string& error) {
        UI::printMessage(error, false);
        DynamicArray<std::string> list = lib.listBackups();
        if (list.empty()) {
            UI::printMessage("Khong co ban sao luu de phuc hoi", false);
            return false;
        }
        std::string latest = list[list.size() - 1];
        if (!UI::confirm("Phuc hoi tu ban sao luu " + latest + "?")) return false;
        std::string err;
        bool ok = lib.restore(latest, err);
        UI::printMessage(ok ? "Da phuc hoi du lieu ngay " + latest : err, ok);
        return ok;
    }
}

int main() {
    UI::printTitle("HE THONG QUAN LY THU VIEN");
    Library lib(DATA_DIR);
    Logger::setFile(std::string(DATA_DIR) + "/log.txt");

    std::string error;
    LoadStatus st = lib.load(error);
    if (st == LOAD_MISSING) {
        seedSampleData(lib);
    } else if (st == LOAD_CORRUPT && !recover(lib, error)) {
        UI::printMessage("Thoat de tranh ghi de du lieu hong. Hay kiem tra thu muc data/", false);
        return 1;
    }
    UI::setColor(lib.config.useColor);
    std::cout << "Da nap " << lib.books.size() << " sach, " << lib.readers.size() << " doc gia, "
              << lib.tickets.size() << " phieu muon.\n";

    Menu menu(lib);
    menu.run();

    if (!lib.save()) UI::printMessage("Khong ghi duoc du lieu khi thoat!", false);
    lib.backup(Date::today());
    return 0;
}
