#include "Config.h"
#include "Utils.h"

#include <cstdio>
#include <cstdlib>

// Đọc file dạng key=value, bỏ qua dòng trống và dòng bắt đầu bằng '#'; key lạ hoặc giá trị sai giữ mặc định
bool Config::load(const std::string& path) {
    FILE* f = fopen(path.c_str(), "r");
    if (!f) return false;
    char buf[256];
    while (fgets(buf, sizeof buf, f)) {
        std::string line = Utils::trim(buf);
        if (line.empty() || line[0] == '#') continue;
        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = Utils::trim(line.substr(0, eq)), val = Utils::trim(line.substr(eq + 1));
        long v = atol(val.c_str());
        if (key == "finePerDay" && v >= 0) finePerDay = v;
        else if (key == "maxBooksStudent" && v > 0) maxBooks[0] = (int)v;
        else if (key == "maxBooksLecturer" && v > 0) maxBooks[1] = (int)v;
        else if (key == "maxBooksNormal" && v > 0) maxBooks[2] = (int)v;
        else if (key == "loanDaysStudent" && v > 0) loanDays[0] = (int)v;
        else if (key == "loanDaysLecturer" && v > 0) loanDays[1] = (int)v;
        else if (key == "loanDaysNormal" && v > 0) loanDays[2] = (int)v;
        else if (key == "cardValidMonths" && v > 0) cardValidMonths = (int)v;
        else if (key == "backupKeep" && v > 0) backupKeep = (int)v;
        else if (key == "useColor") useColor = (val == "1" || val == "true");
        else if (key == "lang" && (val == "vi" || val == "en")) lang = val;
    }
    fclose(f);
    return true;
}
