#include "Library.h"
#include "BookService.h"
#include "ReaderService.h"
#include "Exporter.h"
#include "Logger.h"

#include <cassert>
#include <cstdio>
#include <iostream>

const std::string DIR = "bin/test_data";

void cleanDir() {
    FileStorage::makeDir(DIR);
    DynamicArray<std::string> backups = FileStorage::listSubdirs(DIR + "/backup");
    for (int i = 0; i < backups.size(); ++i) FileStorage::removeDir(DIR + "/backup/" + backups[i]);
    const char* files[] = {"books.dat", "readers.dat", "tickets.dat", "history.dat", "log.txt", "export.txt"};
    for (const char* f : files) std::remove((DIR + "/" + f).c_str());
}

void fill(Library& lib) {
    BookService bs(lib);
    ReaderService rs(lib);
    Date reg = {1, 9, 2026};
    for (int i = 0; i < 20; ++i) {
        std::string isbn = "97860400000" + std::string(i < 10 ? "0" : "") + std::to_string(i);
        assert(bs.addBook(BookService::makeBook(isbn, "Sách số " + std::to_string(i), "Tác giả", "CNTT", 2020, 3)).ok);
    }
    assert(rs.addReader(rs.makeReader("079012345678", "Nguyễn Văn An", "an@gmail.com", "0901234567", 0, reg)).ok);
    BorrowTicket t = {};
    snprintf(t.id, sizeof t.id, "PM%05d", 7);
    lib.tickets.pushBack(t);
    Transaction tr = {};
    tr.id = 12;
    lib.history.pushBack(tr);
}

void testFirstRunIsMissing() {
    cleanDir();
    Library lib(DIR);
    std::string err;
    assert(lib.load(err) == LOAD_MISSING);
}

void testSaveLoadRoundTrip() {
    cleanDir();
    {
        Library lib(DIR);
        fill(lib);
        assert(lib.save() && !lib.dirty);
    }
    Library lib(DIR);
    std::string err;
    assert(lib.load(err) == LOAD_OK);
    assert(lib.books.size() == 20 && lib.readers.size() == 1 && lib.tickets.size() == 1 && lib.history.size() == 1);
    assert(lib.byTitle.size() == 20 && lib.books.isBalanced());
    assert(lib.nextTicketNo == 8 && lib.nextTransId == 13);
    assert(std::string(lib.readers.find("079012345678")->name) == "Nguyễn Văn An");
}

void testCorruptFileDetected() {
    cleanDir();
    {
        Library lib(DIR);
        fill(lib);
        lib.save();
    }
    FILE* f = fopen((DIR + "/books.dat").c_str(), "r+b");
    fseek(f, 40, SEEK_SET);
    fputc('X', f);                                        // sửa 1 byte -> sai checksum
    fclose(f);
    Library lib(DIR);
    std::string err;
    assert(lib.load(err) == LOAD_CORRUPT && !err.empty());
}

void testBackupAndRestore() {
    cleanDir();
    Library lib(DIR);
    fill(lib);
    lib.save();
    Date day = {2, 11, 2026};
    assert(lib.backup(day));
    BookService(lib).removeBook("9786040000001");
    lib.save();
    std::string err;
    assert(lib.restore("2026-11-02", err));
    assert(lib.books.find("9786040000001") != nullptr && lib.books.size() == 20);
    assert(!lib.restore("1999-01-01", err));
}

void testImportExport() {
    cleanDir();
    Library lib(DIR);
    fill(lib);
    assert(Exporter::exportBooksTxt(lib, DIR + "/export.txt").ok);
    Library other(DIR);
    Result r = Exporter::importBooksTxt(other, DIR + "/export.txt");
    assert(r.ok && other.books.size() == 20);
    Result again = Exporter::importBooksTxt(other, DIR + "/export.txt");   // trùng ISBN -> báo lỗi từng dòng
    assert(!again.ok && other.books.size() == 20);
    assert(!Exporter::importBooksTxt(other, DIR + "/khong_co.txt").ok);
}

int main() {
    Logger::setFile(DIR + "/log.txt");
    testFirstRunIsMissing();
    testSaveLoadRoundTrip();
    testCorruptFileDetected();
    testBackupAndRestore();
    testImportExport();
    std::cout << "test_storage: passed" << std::endl;
    return 0;
}
