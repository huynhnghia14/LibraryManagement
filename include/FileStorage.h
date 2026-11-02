#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H

#include <string>

const int FORMAT_VERSION = 1;

// Header 16 byte ở đầu mỗi file .dat
struct FileHeader {
    char magic[4];                  // "LIBB", "LIBR", "LIBT", "LIBH", "LIBA"
    int  version;
    int  count;
    int  checksum;                  // tổng byte mod 2^31
};

// Đọc / ghi file nhị phân, sao lưu – phục hồi (cài đặt ở tuần 5)
class FileStorage {
public:
    template <typename T>
    static bool writeRecords(const char* path, const char* magic, const T* data, int count);
    template <typename T>
    static bool readRecords(const char* path, const char* magic, T*& data, int& count);

    static bool copyFile(const std::string& from, const std::string& to);
    static bool makeDir(const std::string& path);
    static std::string backupDir(const std::string& date);   // data/backup/YYYY-MM-DD/
};

#endif
