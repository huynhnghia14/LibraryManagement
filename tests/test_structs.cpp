// Kiểm tra các header biên dịch được và struct lưu file là POD (ghi bằng fwrite)
#include "Library.h"
#include "BookService.h"
#include "ReaderService.h"
#include "Menu.h"
#include "Utils.h"
#include "Result.h"

#include <cassert>
#include <iostream>
#include <type_traits>

static_assert(std::is_trivially_copyable<Date>::value, "Date phai la POD");
static_assert(std::is_trivially_copyable<Book>::value, "Book phai la POD");
static_assert(std::is_trivially_copyable<Reader>::value, "Reader phai la POD");

int main() {
    Result ok = Result::success("x");
    Result err = Result::fail("y");
    assert(ok.ok && !err.ok);

    Config cfg;
    assert(cfg.finePerDay == 2000 && cfg.maxBooks[CARD_STUDENT] == 3 && cfg.loanDays[CARD_LECTURER] == 30);

    std::cout << "test_structs: passed" << std::endl;
    return 0;
}
