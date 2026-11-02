#ifndef UI_H
#define UI_H

#include <string>
#include "Date.h"

// Nhập / xuất trên console: kiểm tra dữ liệu nhập, in bảng, màu
namespace UI {
    void clearScreen();
    void printTitle(const std::string& title);
    void printMessage(const std::string& msg, bool ok);
    void pause();

    int  inputInt(const std::string& prompt, int minVal, int maxVal);
    std::string inputString(const std::string& prompt, int maxLen);
    bool inputDate(const std::string& prompt, Date& out);

    void setColor(bool enabled);
}

#endif
