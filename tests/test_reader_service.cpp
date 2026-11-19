#include "ReaderService.h"

#include <cassert>
#include <iostream>
#include <string>

const Date REG = {1, 9, 2026};

void testAddFindDuplicateAndExpiry() {
    Library lib;
    ReaderService svc(lib);
    Reader r = svc.makeReader("079012345678", "Nguyễn Văn An", "an@gmail.com", "0901234567", CARD_STUDENT, REG);
    assert(svc.addReader(r).ok);
    assert(!svc.addReader(r).ok);
    Reader* f = svc.findByCmnd("079012345678");
    assert(f && f->expDate.year == 2027 && f->expDate.month == 9);
}

void testValidation() {
    Library lib;
    ReaderService svc(lib);
    assert(!svc.addReader(svc.makeReader("123", "A", "a@b.com", "0901234567", 0, REG)).ok);
    assert(!svc.addReader(svc.makeReader("123456789", "A", "sai-email", "0901234567", 0, REG)).ok);
    assert(!svc.addReader(svc.makeReader("123456789", "A", "a@b.com", "123", 0, REG)).ok);
    assert(!svc.addReader(svc.makeReader("123456789", "A", "a@b.com", "0901234567", 5, REG)).ok);
}

void testEditRenewLockRemove() {
    Library lib;
    ReaderService svc(lib);
    svc.addReader(svc.makeReader("123456789", "Ten cu", "a@b.com", "0901234567", 0, REG));
    Reader upd = svc.makeReader("123456789", "Ten moi", "moi@b.com", "0907654321", CARD_LECTURER, REG);
    assert(svc.editReader("123456789", upd).ok);
    assert(std::string(svc.findByCmnd("123456789")->name) == "Ten moi");

    Date today = {1, 1, 2028};                    // thẻ đã hết hạn 01/09/2027 -> gia hạn từ hôm nay
    assert(svc.renewCard("123456789", today).ok);
    assert(svc.findByCmnd("123456789")->expDate.year == 2029);

    assert(svc.lockReader("123456789", true).ok);
    assert(svc.findByCmnd("123456789")->status == READER_LOCKED);
    assert(svc.removeReader("123456789").ok && svc.count() == 0);
}

void testStatsByCardType() {
    Library lib;
    ReaderService svc(lib);
    for (int i = 0; i < 6; ++i) {
        std::string cmnd = "10000000" + std::to_string(i);
        svc.addReader(svc.makeReader(cmnd, "Doc gia", "x@y.com", "0900000000", i % 3, REG));
    }
    int counts[3];
    svc.statsByCardType(counts);
    assert(counts[0] == 2 && counts[1] == 2 && counts[2] == 2);
}

int main() {
    testAddFindDuplicateAndExpiry();
    testValidation();
    testEditRenewLockRemove();
    testStatsByCardType();
    std::cout << "test_reader_service: passed" << std::endl;
    return 0;
}
