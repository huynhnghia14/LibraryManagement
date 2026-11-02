#include "UI.h"

#include <iostream>
#include <limits>

namespace {
    bool useColor = true;

    const char* GREEN = "\033[32m";
    const char* RED   = "\033[31m";
    const char* CYAN  = "\033[36m";
    const char* RESET = "\033[0m";

    const int MAX_TRIES = 3;
}

namespace UI {

void setColor(bool enabled) {
    useColor = enabled;
}

void clearScreen() {
    std::cout << std::string(2, '\n');
}

void printTitle(const std::string& title) {
    std::string line(50, '=');
    if (useColor) std::cout << CYAN;
    std::cout << line << '\n' << "  " << title << '\n' << line << '\n';
    if (useColor) std::cout << RESET;
}

void printMessage(const std::string& msg, bool ok) {
    if (useColor) std::cout << (ok ? GREEN : RED);
    std::cout << (ok ? "[OK] " : "[LOI] ") << msg << '\n';
    if (useColor) std::cout << RESET;
}

void pause() {
    std::cout << "Nhan Enter de tiep tuc...";
    std::string dummy;
    std::getline(std::cin, dummy);
}

// Nhập số nguyên trong [minVal, maxVal]; trả về minVal - 1 nếu nhập sai quá 3 lần hoặc hết input
int inputInt(const std::string& prompt, int minVal, int maxVal) {
    for (int i = 0; i < MAX_TRIES; ++i) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) return minVal - 1;
        try {
            size_t pos = 0;
            int v = std::stoi(line, &pos);
            if (pos == line.size() && v >= minVal && v <= maxVal) return v;
        } catch (...) {
            // nhập không phải số -> hỏi lại
        }
        printMessage("Vui long nhap so tu " + std::to_string(minVal) + " den " + std::to_string(maxVal), false);
    }
    return minVal - 1;
}

std::string inputString(const std::string& prompt, int maxLen) {
    for (int i = 0; i < MAX_TRIES; ++i) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) return "";
        if (!line.empty() && (int)line.size() <= maxLen) return line;
        printMessage("Khong duoc de trong va toi da " + std::to_string(maxLen) + " ky tu", false);
    }
    return "";
}

bool inputDate(const std::string& prompt, Date& out) {
    for (int i = 0; i < MAX_TRIES; ++i) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) return false;
        if (Date::parse(line, out)) return true;
        printMessage("Ngay khong hop le (dd/mm/yyyy)", false);
    }
    return false;
}

}
