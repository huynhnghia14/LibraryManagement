#ifndef EXPORTER_H
#define EXPORTER_H

#include <string>
#include "Library.h"
#include "Result.h"

// Import / export dữ liệu dạng văn bản
//   sách:    ISBN|Tên|Tác giả|Thể loại|Năm|Số lượng
//   độc giả: CMND|Họ tên|Email|SĐT|Loại thẻ (0/1/2)|Ngày đăng ký dd/mm/yyyy
// Dòng trống và dòng bắt đầu bằng '#' được bỏ qua; dòng lỗi được báo kèm số dòng.
class Exporter {
public:
    static Result importBooksTxt(Library& lib, const std::string& path);
    static Result importReadersTxt(Library& lib, const std::string& path);
    static Result exportBooksTxt(Library& lib, const std::string& path);
    static Result exportBooksCsv(Library& lib, const std::string& path);
    static Result writeText(const std::string& path, const std::string& content);
    static std::string csvField(const std::string& s);
};

#endif
