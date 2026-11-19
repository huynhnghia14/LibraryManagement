#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
    std::string normalize(const std::string& s);    // bỏ dấu tiếng Việt, chữ thường, chỉ giữ a-z 0-9 và 1 khoảng trắng
    std::string trim(const std::string& s);
    bool isDigits(const std::string& s, int minLen, int maxLen);
    bool isValidIsbn(const std::string& s);         // 13 chữ số
    bool isValidCmnd(const std::string& s);         // 9 hoặc 12 chữ số
    bool isValidEmail(const std::string& s);
    bool isValidPhone(const std::string& s);        // 10 chữ số, bắt đầu bằng 0
    void copyStr(char* dst, const std::string& src, int maxLen);   // chép tối đa maxLen ký tự + '\0'
    int  displayWidth(const std::string& s);        // số ký tự hiển thị của chuỗi UTF-8
    int  currentYear();
    int  split(const std::string& line, char sep, std::string parts[], int maxParts);   // trả về số phần
}

#endif
