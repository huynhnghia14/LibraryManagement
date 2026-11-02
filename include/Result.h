#ifndef RESULT_H
#define RESULT_H

#include <string>

// Kết quả trả về của các Service: thành công / thất bại kèm thông báo cho Menu hiển thị
struct Result {
    bool ok;
    std::string message;

    static Result success(const std::string& m = "") { Result r; r.ok = true;  r.message = m; return r; }
    static Result fail(const std::string& m)         { Result r; r.ok = false; r.message = m; return r; }
};

#endif
