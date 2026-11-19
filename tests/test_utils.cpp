#include "Utils.h"

#include <cassert>
#include <iostream>

void testNormalizeVietnamese() {
    assert(Utils::normalize("Lập Trình  C++ Căn Bản") == "lap trinh c can ban");
    assert(Utils::normalize("  Đại số tuyến tính ") == "dai so tuyen tinh");
    assert(Utils::normalize("GIẢI TÍCH 1") == "giai tich 1");
    assert(Utils::normalize("") == "");
}

void testValidators() {
    assert(Utils::isValidIsbn("9786041234567"));
    assert(!Utils::isValidIsbn("978604123456"));
    assert(!Utils::isValidIsbn("97860412345a7"));
    assert(Utils::isValidCmnd("079012345678") && Utils::isValidCmnd("123456789"));
    assert(!Utils::isValidCmnd("1234567890"));
    assert(Utils::isValidEmail("an.nguyen@gmail.com"));
    assert(!Utils::isValidEmail("an@gmail") && !Utils::isValidEmail("@gmail.com"));
    assert(Utils::isValidPhone("0901234567") && !Utils::isValidPhone("9012345678"));
}

void testCopyStrDoesNotSplitUtf8() {
    char buf[6];
    Utils::copyStr(buf, "Lập trình", 5);         // "Lậ" chiếm 4 byte, "p" -> 5 byte
    assert(std::string(buf) == "Lập");
    Utils::copyStr(buf, "ăăă", 5);               // mỗi 'ă' 2 byte -> chỉ chép được 2 ký tự
    assert(std::string(buf) == "ăă");
}

int main() {
    testNormalizeVietnamese();
    testValidators();
    testCopyStrDoesNotSplitUtf8();
    std::cout << "test_utils: passed" << std::endl;
    return 0;
}
