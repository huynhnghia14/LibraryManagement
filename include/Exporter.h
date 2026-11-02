#ifndef EXPORTER_H
#define EXPORTER_H

#include <string>
#include "Result.h"

class Library;

// Import / export danh mục sách và báo cáo ra file văn bản / CSV
class Exporter {
public:
    static Result importBooksTxt(Library& lib, const std::string& path);   // ISBN|Tên|Tác giả|Thể loại|Năm|SL
    static Result exportBooksTxt(Library& lib, const std::string& path);
    static Result exportBooksCsv(Library& lib, const std::string& path);
    static Result writeReport(const std::string& path, const std::string& content);
};

#endif
