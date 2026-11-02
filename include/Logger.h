#ifndef LOGGER_H
#define LOGGER_H

#include <string>

// Ghi nhật ký thao tác vào data/log.txt (append)
class Logger {
public:
    static void setUser(const std::string& username);
    static void log(const std::string& action, const std::string& detail, bool ok);
};

#endif
