#include "Library.h"
#include "Logger.h"
#include "Utils.h"

#include <cstdio>
#include <cstdlib>

namespace {
    const char* const DAT_FILES[] = {"books.dat", "readers.dat", "tickets.dat", "history.dat"};
    const int DAT_COUNT = 4;

    std::string isoDate(const Date& d) {
        char buf[16];
        snprintf(buf, sizeof buf, "%04d-%02d-%02d", d.year, d.month, d.day);
        return buf;
    }
}

Library::Library(const std::string& dir)
    : dataDir(dir), nextTicketNo(1), nextTransId(1), dirty(false) {}

void Library::clearAll() {
    books.clear();
    byTitle.clear();
    readers.clear();
    tickets.clear();
    history.clear();
}

LoadStatus Library::load(std::string& error) {
    config.load(path("config.txt"));

    DynamicArray<Book> bs;
    DynamicArray<Reader> rs;
    DynamicArray<BorrowTicket> ts;
    DynamicArray<Transaction> hs;
    LoadStatus st[DAT_COUNT] = {
        FileStorage::readRecords(path("books.dat"), "LIBB", bs),
        FileStorage::readRecords(path("readers.dat"), "LIBR", rs),
        FileStorage::readRecords(path("tickets.dat"), "LIBT", ts),
        FileStorage::readRecords(path("history.dat"), "LIBH", hs),
    };
    for (int i = 0; i < DAT_COUNT; ++i) {
        if (st[i] == LOAD_CORRUPT) {
            error = std::string("File ") + DAT_FILES[i] + " bi hong hoac sai dinh dang";
            return LOAD_CORRUPT;
        }
    }
    if (st[0] == LOAD_MISSING && st[1] == LOAD_MISSING) return LOAD_MISSING;   // lần chạy đầu

    clearAll();
    for (int i = 0; i < bs.size(); ++i) books.insert(bs[i].isbn, bs[i]);
    for (int i = 0; i < rs.size(); ++i) readers.insert(rs[i].cmnd, rs[i]);
    for (int i = 0; i < ts.size(); ++i) tickets.pushBack(ts[i]);
    history.swap(hs);
    rebuildIndexes();
    dirty = false;
    return LOAD_OK;
}

void Library::rebuildIndexes() {
    byTitle.clear();
    books.inorder([&](const std::string& isbn, Book& b) { byTitle.insert(Utils::normalize(b.title), isbn); });

    nextTicketNo = 1;
    tickets.forEach([&](const BorrowTicket& t) {
        long no = atol(t.id + 2);                    // "PM00031" -> 31
        if (no >= nextTicketNo) nextTicketNo = no + 1;
    });
    nextTransId = 1;
    for (int i = 0; i < history.size(); ++i) {
        if (history[i].id >= nextTransId) nextTransId = history[i].id + 1;
    }
}

bool Library::save() {
    FileStorage::makeDir(dataDir);
    DynamicArray<Book> bs;
    DynamicArray<Reader> rs;
    DynamicArray<BorrowTicket> ts;
    bs.reserve(books.size());
    books.inorder([&](const std::string&, Book& b) { bs.pushBack(b); });     // ghi theo thứ tự ISBN
    rs.reserve(readers.size());
    readers.forEach([&](const std::string&, Reader& r) { rs.pushBack(r); });
    ts.reserve(tickets.size());
    tickets.forEach([&](const BorrowTicket& t) { ts.pushBack(t); });

    bool ok = FileStorage::writeRecords(path("books.dat"), "LIBB", bs.raw(), bs.size()) &&
              FileStorage::writeRecords(path("readers.dat"), "LIBR", rs.raw(), rs.size()) &&
              FileStorage::writeRecords(path("tickets.dat"), "LIBT", ts.raw(), ts.size()) &&
              FileStorage::writeRecords(path("history.dat"), "LIBH", history.raw(), history.size());
    if (ok) dirty = false;
    else Logger::log("SAVE", "ghi file that bai", false);
    return ok;
}

bool Library::backup(const Date& today) {
    std::string root = path("backup");
    std::string target = root + "/" + isoDate(today);
    if (!FileStorage::makeDir(root) || !FileStorage::makeDir(target)) return false;
    bool ok = true;
    for (int i = 0; i < DAT_COUNT; ++i) {
        std::string src = path(DAT_FILES[i]);
        if (FileStorage::fileExists(src)) ok = FileStorage::copyFile(src, target + "/" + DAT_FILES[i]) && ok;
    }
    // chỉ giữ config.backupKeep bản mới nhất (tên thư mục YYYY-MM-DD sắp tăng dần theo thời gian)
    DynamicArray<std::string> all = FileStorage::listSubdirs(root);
    for (int i = 0; i + config.backupKeep < all.size(); ++i) FileStorage::removeDir(root + "/" + all[i]);
    Logger::log("BACKUP", target, ok);
    return ok;
}

bool Library::restore(const std::string& backupName, std::string& error) {
    std::string src = path("backup") + "/" + backupName;
    if (!FileStorage::fileExists(src + "/books.dat")) {
        error = "Khong tim thay ban sao luu " + backupName;
        return false;
    }
    for (int i = 0; i < DAT_COUNT; ++i) {
        std::string from = src + "/" + DAT_FILES[i];
        if (FileStorage::fileExists(from) && !FileStorage::copyFile(from, path(DAT_FILES[i]))) {
            error = std::string("Khong chep duoc ") + DAT_FILES[i];
            return false;
        }
    }
    bool ok = load(error) == LOAD_OK;
    Logger::log("RESTORE", backupName, ok);
    return ok;
}

DynamicArray<std::string> Library::listBackups() const {
    return FileStorage::listSubdirs(path("backup"));
}
