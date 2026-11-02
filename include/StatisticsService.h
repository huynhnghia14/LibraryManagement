#ifndef STATISTICS_SERVICE_H
#define STATISTICS_SERVICE_H

#include <string>
#include "Library.h"
#include "Result.h"

struct CountItem {
    std::string key;
    int count;
};

// Thống kê – báo cáo (FR-S-01..07)
class StatisticsService {
    Library& lib;

public:
    explicit StatisticsService(Library& l) : lib(l) {}

    void byMonth(int year, int counts[12]);
    DynamicArray<CountItem> byCategory();
    DynamicArray<CountItem> byReader();
    DynamicArray<CountItem> top10Books();
    DynamicArray<CountItem> top10Readers();
    void   fineByMonth(int year, long totals[12]);
    Result exportReport(const std::string& path);
};

#endif
