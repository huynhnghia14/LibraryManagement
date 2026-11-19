#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H

#include <cstdio>
#include <cstring>
#include <string>
#include "DynamicArray.h"

const int FORMAT_VERSION = 1;

// Header 16 byte ở đầu mỗi file .dat
struct FileHeader {
    char magic[4];                  // "LIBB", "LIBR", "LIBT", "LIBH", "LIBA"
    int  version;
    int  count;
    int  checksum;                  // tổng byte mod 2^31
};

enum LoadStatus { LOAD_OK = 0, LOAD_MISSING = 1, LOAD_CORRUPT = 2 };

// Đọc / ghi file nhị phân, thao tác thư mục cho sao lưu – phục hồi
class FileStorage {
    static int checksumOf(const void* data, size_t bytes) {
        const unsigned char* p = static_cast<const unsigned char*>(data);
        unsigned long sum = 0;
        for (size_t i = 0; i < bytes; ++i) sum = (sum + p[i]) & 0x7FFFFFFF;
        return (int)sum;
    }

public:
    // Ghi ra file .tmp rồi đổi tên: nếu lỗi giữa chừng thì file cũ vẫn còn nguyên
    template <typename T>
    static bool writeRecords(const std::string& path, const char* magic, const T* data, int count) {
        std::string tmp = path + ".tmp";
        FILE* f = fopen(tmp.c_str(), "wb");
        if (!f) return false;
        FileHeader h;
        memcpy(h.magic, magic, 4);
        h.version = FORMAT_VERSION;
        h.count = count;
        h.checksum = checksumOf(data, sizeof(T) * (size_t)count);
        bool ok = fwrite(&h, sizeof h, 1, f) == 1 &&
                  (count == 0 || fwrite(data, sizeof(T), (size_t)count, f) == (size_t)count);
        ok = (fclose(f) == 0) && ok;
        if (!ok) {
            std::remove(tmp.c_str());
            return false;
        }
        std::remove(path.c_str());
        return std::rename(tmp.c_str(), path.c_str()) == 0;
    }

    // Kiểm tra magic, version, kích thước file và checksum trước khi nhận dữ liệu
    template <typename T>
    static LoadStatus readRecords(const std::string& path, const char* magic, DynamicArray<T>& out) {
        out.clear();
        FILE* f = fopen(path.c_str(), "rb");
        if (!f) return LOAD_MISSING;
        FileHeader h;
        bool ok = fread(&h, sizeof h, 1, f) == 1 && memcmp(h.magic, magic, 4) == 0 &&
                  h.version == FORMAT_VERSION && h.count >= 0;
        if (ok) {
            fseek(f, 0, SEEK_END);
            long expected = (long)sizeof h + (long)sizeof(T) * h.count;
            ok = ftell(f) == expected;
            fseek(f, (long)sizeof h, SEEK_SET);
        }
        if (ok && h.count > 0) {
            out.reserve(h.count);
            T rec;
            for (int i = 0; ok && i < h.count; ++i) {
                ok = fread(&rec, sizeof(T), 1, f) == 1;
                if (ok) out.pushBack(rec);
            }
            ok = ok && checksumOf(out.raw(), sizeof(T) * (size_t)out.size()) == h.checksum;
        }
        fclose(f);
        if (!ok) out.clear();
        return ok ? LOAD_OK : LOAD_CORRUPT;
    }

    static bool fileExists(const std::string& path);
    static bool copyFile(const std::string& from, const std::string& to);
    static bool makeDir(const std::string& path);
    static bool removeDir(const std::string& path);               // xóa thư mục và các file bên trong
    static DynamicArray<std::string> listSubdirs(const std::string& path);   // sắp tăng dần theo tên
};

#endif
