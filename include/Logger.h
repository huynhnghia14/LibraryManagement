#ifndef LOGGER_H
#define LOGGER_H

#include <string>

// Ghi nhật ký thao tác vào file văn bản (mặc định data/log.txt), chế độ append
class Logger {
public:
    static void setFile(const std::string& path);
    static void setUser(const std::string& username);
    static void log(const std::string& action, const std::string& detail, bool ok = true);
    static std::string filePath();
};

#endif
