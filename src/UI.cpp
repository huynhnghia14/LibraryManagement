#include "UI.h"
#include "Utils.h"

#include <iostream>

namespace {
    bool useColor = true;

    const char* GREEN = "\033[32m";
    const char* RED   = "\033[31m";
    const char* CYAN  = "\033[36m";
    const char* RESET = "\033[0m";

    const int MAX_TRIES = 3;

    bool readLine(std::string& line) {
        if (!std::getline(std::cin, line)) return false;
        if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
        return true;
    }

    // Cắt / đệm chuỗi UTF-8 cho vừa đúng width ký tự hiển thị
    std::string fit(const std::string& s, int width) {
        std::string out;
        int w = 0;
        for (size_t i = 0; i < s.size(); ) {
            size_t len = 1;
            unsigned char c = s[i];
            if (c >= 0xF0) len = 4; else if (c >= 0xE0) len = 3; else if (c >= 0xC0) len = 2;
            if (w == width) break;
            out += s.substr(i, len);
            i += len;
            ++w;
        }
        if (w == width && Utils::displayWidth(s) > width && width > 1) {
            // đánh dấu bị cắt bằng '~' ở ký tự cuối
            size_t cut = out.size();
            do { --cut; } while (cut > 0 && ((unsigned char)out[cut] & 0xC0) == 0x80);
            out = out.substr(0, cut) + "~";
        }
        return out + std::string(width - Utils::displayWidth(out), ' ');
    }
}

namespace UI {

void setColor(bool enabled) {
    useColor = enabled;
}

void clearScreen() {
    std::cout << "\n\n";
}

void printTitle(const std::string& title) {
    std::string line(60, '=');
    if (useColor) std::cout << CYAN;
    std::cout << line << '\n' << "  " << title << '\n' << line << '\n';
    if (useColor) std::cout << RESET;
}

void printMessage(const std::string& msg, bool ok) {
    if (useColor) std::cout << (ok ? GREEN : RED);
    std::cout << (ok ? "[OK] " : "[LOI] ") << msg << '\n';
    if (useColor) std::cout << RESET;
}

void printResult(const Result& r) {
    printMessage(r.message, r.ok);
}

void pause() {
    std::cout << "Nhan Enter de tiep tuc...";
    std::string dummy;
    readLine(dummy);
}

// Nhập số nguyên trong [minVal, maxVal]; trả về minVal - 1 nếu nhập sai quá 3 lần hoặc hết input
int inputInt(const std::string& prompt, int minVal, int maxVal) {
    for (int i = 0; i < MAX_TRIES; ++i) {
        std::cout << prompt;
        std::string line;
        if (!readLine(line)) return minVal - 1;
        line = Utils::trim(line);
        try {
            size_t pos = 0;
            long v = std::stol(line, &pos);
            if (pos == line.size() && v >= minVal && v <= maxVal) return (int)v;
        } catch (...) {
            // không phải số -> hỏi lại
        }
        printMessage("Vui long nhap so tu " + std::to_string(minVal) + " den " + std::to_string(maxVal), false);
    }
    return minVal - 1;
}

std::string inputString(const std::string& prompt, int maxLen) {
    for (int i = 0; i < MAX_TRIES; ++i) {
        std::cout << prompt;
        std::string line;
        if (!readLine(line)) return "";
        line = Utils::trim(line);
        if (!line.empty() && Utils::displayWidth(line) <= maxLen) return line;
        printMessage("Khong duoc de trong va toi da " + std::to_string(maxLen) + " ky tu", false);
    }
    return "";
}

std::string inputOptional(const std::string& prompt, int maxLen) {
    for (int i = 0; i < MAX_TRIES; ++i) {
        std::cout << prompt;
        std::string line;
        if (!readLine(line)) return "";
        line = Utils::trim(line);
        if (Utils::displayWidth(line) <= maxLen) return line;
        printMessage("Toi da " + std::to_string(maxLen) + " ky tu", false);
    }
    return "";
}

bool inputDate(const std::string& prompt, Date& out) {
    for (int i = 0; i < MAX_TRIES; ++i) {
        std::cout << prompt;
        std::string line;
        if (!readLine(line)) return false;
        if (Date::parse(Utils::trim(line), out)) return true;
        printMessage("Ngay khong hop le (dd/mm/yyyy)", false);
    }
    return false;
}

bool confirm(const std::string& prompt) {
    std::cout << prompt << " (y/n): ";
    std::string line;
    if (!readLine(line)) return false;
    line = Utils::trim(line);
    return line == "y" || line == "Y";
}

void printLine(const int widths[], int n) {
    std::string s = "+";
    for (int i = 0; i < n; ++i) s += std::string(widths[i] + 2, '-') + "+";
    std::cout << s << '\n';
}

void printRow(const std::string cols[], const int widths[], int n) {
    std::cout << "|";
    for (int i = 0; i < n; ++i) std::cout << ' ' << fit(cols[i], widths[i]) << " |";
    std::cout << '\n';
}

}
