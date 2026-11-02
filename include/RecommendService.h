#ifndef RECOMMEND_SERVICE_H
#define RECOMMEND_SERVICE_H

#include <string>
#include "Library.h"

// Gợi ý – tìm kiếm nâng cao (FR-G-01..05)
class RecommendService {
    Library& lib;

public:
    explicit RecommendService(Library& l) : lib(l) {}

    DynamicArray<std::string> sameCategory(const std::string& isbn, int limit = 10);     // BFS cạnh loại 0
    DynamicArray<std::string> byHistoryOfBook(const std::string& isbn, int limit = 10);  // cạnh loại 1
    DynamicArray<std::string> byHistoryOfReader(const std::string& cmnd, int limit = 10);
    void rebuildHistoryEdges();
    int  dfsComponents();
    void demoTrieBenchmark(int queries);
};

#endif
