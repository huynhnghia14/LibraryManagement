#include "Exporter.h"
#include "BookService.h"
#include "ReaderService.h"
#include "Logger.h"
#include "Utils.h"

#include <cstdio>
#include <cstdlib>

namespace {
    const int MAX_ERRORS_SHOWN = 5;
    const int LINE_MAX = 1024;

    // Đọc từng dòng, gọi handle(parts, n) -> Result; tổng hợp số dòng thành công / lỗi
    template <typename H>
    Result importLines(const std::string& path, int expectedParts, H handle) {
        FILE* f = fopen(path.c_str(), "r");
        if (!f) return Result::fail("Khong mo duoc file " + path);
        char buf[LINE_MAX];
        int lineNo = 0, ok = 0, bad = 0;
        std::string errors;
        while (fgets(buf, sizeof buf, f)) {
            ++lineNo;
            std::string line = Utils::trim(buf);
            if (lineNo == 1 && line.compare(0, 3, "\xEF\xBB\xBF") == 0) line = line.substr(3);   // bỏ BOM UTF-8
            if (line.empty() || line[0] == '#') continue;
            std::string parts[8];
            int n = Utils::split(line, '|', parts, 8);
            Result r = n == expectedParts ? handle(parts) : Result::fail("sai so truong (" + std::to_string(n) + ")");
            if (r.ok) {
                ++ok;
            } else {
                ++bad;
                if (bad <= MAX_ERRORS_SHOWN) errors += "\n   dong " + std::to_string(lineNo) + ": " + r.message;
            }
        }
        fclose(f);
        std::string msg = "Nhap thanh cong " + std::to_string(ok) + " dong, loi " + std::to_string(bad) + " dong" + errors;
        return ok > 0 || bad == 0 ? Result::success(msg) : Result::fail(msg);
    }
}

Result Exporter::importBooksTxt(Library& lib, const std::string& path) {
    BookService svc(lib);
    Result r = importLines(path, 6, [&](const std::string* p) -> Result {
        if (!Utils::isDigits(p[4], 4, 4) || !Utils::isDigits(p[5], 1, 4)) return Result::fail("nam / so luong khong phai so");
        return svc.addBook(BookService::makeBook(p[0], p[1], p[2], p[3], atoi(p[4].c_str()), atoi(p[5].c_str())));
    });
    Logger::log("IMPORT_BOOKS", path, r.ok);
    return r;
}

Result Exporter::importReadersTxt(Library& lib, const std::string& path) {
    ReaderService svc(lib);
    Result r = importLines(path, 6, [&](const std::string* p) -> Result {
        Date reg;
        if (!Utils::isDigits(p[4], 1, 1)) return Result::fail("loai the phai la 0, 1 hoac 2");
        if (!Date::parse(p[5], reg)) return Result::fail("ngay dang ky khong hop le");
        return svc.addReader(svc.makeReader(p[0], p[1], p[2], p[3], atoi(p[4].c_str()), reg));
    });
    Logger::log("IMPORT_READERS", path, r.ok);
    return r;
}

Result Exporter::exportBooksTxt(Library& lib, const std::string& path) {
    FILE* f = fopen(path.c_str(), "w");
    if (!f) return Result::fail("Khong tao duoc file " + path);
    fprintf(f, "# ISBN|Ten sach|Tac gia|The loai|Nam XB|So luong\n");
    lib.books.inorder([&](const std::string&, Book& b) {
        fprintf(f, "%s|%s|%s|%s|%d|%d\n", b.isbn, b.title, b.author, b.category, b.year, b.quantity);
    });
    fclose(f);
    Logger::log("EXPORT_BOOKS", path);
    return Result::success("Da xuat " + std::to_string(lib.books.size()) + " sach ra " + path);
}

std::string Exporter::csvField(const std::string& s) {
    if (s.find_first_of(",\"\n") == std::string::npos) return s;
    std::string out = "\"";
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '"') out += '"';
        out += s[i];
    }
    return out + "\"";
}

Result Exporter::exportBooksCsv(Library& lib, const std::string& path) {
    FILE* f = fopen(path.c_str(), "w");
    if (!f) return Result::fail("Khong tao duoc file " + path);
    fprintf(f, "\xEF\xBB\xBFISBN,Ten sach,Tac gia,The loai,Nam XB,So luong,Con lai\n");   // BOM để Excel đọc đúng UTF-8
    lib.books.inorder([&](const std::string&, Book& b) {
        fprintf(f, "%s,%s,%s,%s,%d,%d,%d\n", b.isbn, csvField(b.title).c_str(), csvField(b.author).c_str(),
                csvField(b.category).c_str(), b.year, b.quantity, b.available);
    });
    fclose(f);
    Logger::log("EXPORT_CSV", path);
    return Result::success("Da xuat " + std::to_string(lib.books.size()) + " sach ra " + path);
}

Result Exporter::writeText(const std::string& path, const std::string& content) {
    FILE* f = fopen(path.c_str(), "w");
    if (!f) return Result::fail("Khong tao duoc file " + path);
    bool ok = fputs(content.c_str(), f) >= 0;
    ok = (fclose(f) == 0) && ok;
    return ok ? Result::success("Da ghi file " + path) : Result::fail("Loi khi ghi file " + path);
}
