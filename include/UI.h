#ifndef UI_H
#define UI_H

#include <string>
#include "Date.h"
#include "Result.h"

// Nhập / xuất trên console: kiểm tra dữ liệu nhập, in bảng, màu
namespace UI {
    void clearScreen();
    void printTitle(const std::string& title);
    void printMessage(const std::string& msg, bool ok);
    void printResult(const Result& r);
    void pause();

    int  inputInt(const std::string& prompt, int minVal, int maxVal);
    std::string inputString(const std::string& prompt, int maxLen);       // bắt buộc nhập
    std::string inputOptional(const std::string& prompt, int maxLen);     // Enter = giữ nguyên
    bool inputDate(const std::string& prompt, Date& out);
    bool confirm(const std::string& prompt);

    // In bảng: n cột, widths[i] = độ rộng cột i (ký tự hiển thị)
    void printLine(const int widths[], int n);
    void printRow(const std::string cols[], const int widths[], int n);

    void setColor(bool enabled);
}

#endif
