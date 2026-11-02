#ifndef CONFIG_H
#define CONFIG_H

#include <string>

// Tham số nghiệp vụ, đọc từ data/config.txt (có giá trị mặc định)
struct Config {
    long finePerDay = 2000;               // đồng / ngày / cuốn
    int  maxBooks[3] = {3, 5, 2};         // theo CardType: SV, GV, Thường
    int  loanDays[3] = {14, 30, 7};
    int  cardValidMonths = 12;
    int  backupKeep = 30;
    bool useColor = true;
    std::string lang = "vi";

    bool load(const std::string& path);
};

#endif
