#include "FileStorage.h"

#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif

namespace {
    // Liệt kê tên các mục trong thư mục (bỏ "." và ".."); dirsOnly: chỉ lấy thư mục con
    DynamicArray<std::string> listEntries(const std::string& path, bool dirsOnly) {
        DynamicArray<std::string> names;
#ifdef _WIN32
        _finddata_t fd;
        intptr_t h = _findfirst((path + "/*").c_str(), &fd);
        if (h == -1) return names;
        do {
            std::string n = fd.name;
            bool isDir = (fd.attrib & _A_SUBDIR) != 0;
            if (n != "." && n != ".." && (!dirsOnly || isDir)) names.pushBack(n);
        } while (_findnext(h, &fd) == 0);
        _findclose(h);
#else
        DIR* d = opendir(path.c_str());
        if (!d) return names;
        while (dirent* e = readdir(d)) {
            std::string n = e->d_name;
            if (n == "." || n == "..") continue;
            struct stat st;
            bool isDir = stat((path + "/" + n).c_str(), &st) == 0 && S_ISDIR(st.st_mode);
            if (!dirsOnly || isDir) names.pushBack(n);
        }
        closedir(d);
#endif
        return names;
    }
}

bool FileStorage::fileExists(const std::string& path) {
    FILE* f = fopen(path.c_str(), "rb");
    if (!f) return false;
    fclose(f);
    return true;
}

bool FileStorage::copyFile(const std::string& from, const std::string& to) {
    FILE* in = fopen(from.c_str(), "rb");
    if (!in) return false;
    FILE* out = fopen(to.c_str(), "wb");
    if (!out) {
        fclose(in);
        return false;
    }
    char buf[8192];
    size_t n;
    bool ok = true;
    while (ok && (n = fread(buf, 1, sizeof buf, in)) > 0) ok = fwrite(buf, 1, n, out) == n;
    fclose(in);
    return (fclose(out) == 0) && ok;
}

bool FileStorage::makeDir(const std::string& path) {
#ifdef _WIN32
    int rc = _mkdir(path.c_str());
#else
    int rc = mkdir(path.c_str(), 0755);
#endif
    struct stat st;
    return rc == 0 || (stat(path.c_str(), &st) == 0 && (st.st_mode & S_IFDIR));
}

bool FileStorage::removeDir(const std::string& path) {
    DynamicArray<std::string> files = listEntries(path, false);
    for (int i = 0; i < files.size(); ++i) std::remove((path + "/" + files[i]).c_str());
#ifdef _WIN32
    return _rmdir(path.c_str()) == 0;
#else
    return rmdir(path.c_str()) == 0;
#endif
}

DynamicArray<std::string> FileStorage::listSubdirs(const std::string& path) {
    DynamicArray<std::string> dirs = listEntries(path, true);
    dirs.sort([](const std::string& a, const std::string& b) { return a < b; });
    return dirs;
}
