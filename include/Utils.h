#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
    std::string normalize(const std::string& s);    // bỏ dấu tiếng Việt, chữ thường, gộp khoảng trắng
    std::string trim(const std::string& s);
    bool isDigits(const std::string& s, int minLen, int maxLen);
    bool isValidIsbn(const std::string& s);         // 13 chữ số
    bool isValidCmnd(const std::string& s);         // 9 hoặc 12 chữ số
    bool isValidEmail(const std::string& s);
    bool isValidPhone(const std::string& s);        // 10 chữ số, bắt đầu bằng 0
    void copyStr(char* dst, const std::string& src, int maxLen);
}

#endif
