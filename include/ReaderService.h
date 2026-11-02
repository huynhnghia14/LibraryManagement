#ifndef READER_SERVICE_H
#define READER_SERVICE_H

#include <string>
#include "Library.h"
#include "Result.h"

// Quản lý độc giả (FR-R-01..06)
class ReaderService {
    Library& lib;

public:
    explicit ReaderService(Library& l) : lib(l) {}

    Result  addReader(const Reader& r);
    Result  editReader(const std::string& cmnd, const Reader& updated);
    Result  renewCard(const std::string& cmnd);
    Result  lockReader(const std::string& cmnd, bool locked);
    Result  removeReader(const std::string& cmnd);
    Reader* findByCmnd(const std::string& cmnd);
    template <typename F> void listAll(F visit);
    void    statsByCardType(int counts[3]);
};

#endif
