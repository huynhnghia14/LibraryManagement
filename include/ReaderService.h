#ifndef READER_SERVICE_H
#define READER_SERVICE_H

#include <string>
#include "Library.h"
#include "Reader.h"
#include "Result.h"

// Quản lý độc giả (FR-R): thêm / sửa / gia hạn / khóa / xóa / tìm theo CMND / thống kê theo loại thẻ
class ReaderService {
    Library& lib;

public:
    explicit ReaderService(Library& l) : lib(l) {}

    static const char* cardTypeName(int type);
    Reader makeReader(const std::string& cmnd, const std::string& name, const std::string& email,
                      const std::string& phone, int cardType, const Date& regDate) const;
    static Result validate(const Reader& r);

    Result  addReader(const Reader& r);
    Result  editReader(const std::string& cmnd, const Reader& updated);   // không đổi CMND, ngày đăng ký
    Result  renewCard(const std::string& cmnd, const Date& today);
    Result  lockReader(const std::string& cmnd, bool locked);
    Result  removeReader(const std::string& cmnd);
    Reader* findByCmnd(const std::string& cmnd);
    int     count() const { return lib.readers.size(); }

    template <typename F>
    void listAll(F visit) {                          // visit(const Reader&)
        lib.readers.forEach([&](const std::string&, Reader& r) { visit(r); });
    }

    void statsByCardType(int counts[3]);
};

#endif
