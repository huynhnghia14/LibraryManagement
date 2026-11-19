#include "Logger.h"

#include <cstdio>
#include <ctime>

namespace {
    std::string logPath = "data/log.txt";
    std::string currentUser = "system";
}

void Logger::setFile(const std::string& path) {
    logPath = path;
}

void Logger::setUser(const std::string& username) {
    currentUser = username;
}

std::string Logger::filePath() {
    return logPath;
}

// Mỗi dòng: [2026-10-19 14:05:33] [thuthu01] BORROW PM00031 cmnd=... isbn=... OK
void Logger::log(const std::string& action, const std::string& detail, bool ok) {
    FILE* f = fopen(logPath.c_str(), "a");
    if (!f) return;                                  // không ghi được log thì bỏ qua, không làm dừng chương trình
    char stamp[20];
    time_t now = time(nullptr);
    strftime(stamp, sizeof stamp, "%Y-%m-%d %H:%M:%S", localtime(&now));
    fprintf(f, "[%s] [%s] %s %s %s\n", stamp, currentUser.c_str(), action.c_str(), detail.c_str(), ok ? "OK" : "FAIL");
    fclose(f);
}
