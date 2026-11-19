#include "ReaderService.h"
#include "Utils.h"

#include <cstring>

const char* ReaderService::cardTypeName(int type) {
    switch (type) {
        case CARD_STUDENT:  return "Sinh vien";
        case CARD_LECTURER: return "Giang vien";
        case CARD_NORMAL:   return "Thuong";
        default:            return "?";
    }
}

Reader ReaderService::makeReader(const std::string& cmnd, const std::string& name, const std::string& email,
                                 const std::string& phone, int cardType, const Date& regDate) const {
    Reader r;
    memset(&r, 0, sizeof r);
    Utils::copyStr(r.cmnd, Utils::trim(cmnd), CMND_LEN);
    Utils::copyStr(r.name, Utils::trim(name), NAME_LEN);
    Utils::copyStr(r.email, Utils::trim(email), EMAIL_LEN);
    Utils::copyStr(r.phone, Utils::trim(phone), PHONE_LEN);
    r.cardType = cardType;
    r.regDate = regDate;
    r.expDate = regDate.addMonths(lib.config.cardValidMonths);
    r.status = READER_ACTIVE;
    return r;
}

Result ReaderService::validate(const Reader& r) {
    if (!Utils::isValidCmnd(r.cmnd))         return Result::fail("CMND/CCCD phai gom 9 hoac 12 chu so");
    if (Utils::normalize(r.name).empty())    return Result::fail("Ho ten khong hop le");
    if (!Utils::isValidEmail(r.email))       return Result::fail("Email khong hop le");
    if (!Utils::isValidPhone(r.phone))       return Result::fail("So dien thoai phai gom 10 chu so, bat dau bang 0");
    if (r.cardType < CARD_STUDENT || r.cardType > CARD_NORMAL) return Result::fail("Loai the khong hop le");
    if (!r.regDate.isValid() || !r.expDate.isValid())          return Result::fail("Ngay dang ky / het han khong hop le");
    return Result::success();
}

Result ReaderService::addReader(const Reader& r) {
    Result v = validate(r);
    if (!v.ok) return v;
    if (!lib.readers.insert(r.cmnd, r)) return Result::fail(std::string("Da ton tai doc gia co CMND ") + r.cmnd);
    lib.markDirty();
    return Result::success(std::string("Da them doc gia ") + r.cmnd);
}

Result ReaderService::editReader(const std::string& cmnd, const Reader& updated) {
    Reader* cur = lib.readers.find(cmnd);
    if (!cur) return Result::fail("Khong tim thay doc gia co CMND " + cmnd);
    Reader r = *cur;
    Utils::copyStr(r.name, updated.name, NAME_LEN);
    Utils::copyStr(r.email, updated.email, EMAIL_LEN);
    Utils::copyStr(r.phone, updated.phone, PHONE_LEN);
    r.cardType = updated.cardType;
    Result v = validate(r);
    if (!v.ok) return v;
    *cur = r;
    lib.markDirty();
    return Result::success("Da cap nhat doc gia " + cmnd);
}

Result ReaderService::renewCard(const std::string& cmnd, const Date& today) {
    Reader* r = lib.readers.find(cmnd);
    if (!r) return Result::fail("Khong tim thay doc gia co CMND " + cmnd);
    Date from = r->expDate < today ? today : r->expDate;    // thẻ đã hết hạn thì tính từ hôm nay
    r->expDate = from.addMonths(lib.config.cardValidMonths);
    lib.markDirty();
    return Result::success("Gia han the den " + r->expDate.toString());
}

Result ReaderService::lockReader(const std::string& cmnd, bool locked) {
    Reader* r = lib.readers.find(cmnd);
    if (!r) return Result::fail("Khong tim thay doc gia co CMND " + cmnd);
    r->status = locked ? READER_LOCKED : READER_ACTIVE;
    lib.markDirty();
    return Result::success(locked ? "Da khoa the " + cmnd : "Da mo khoa the " + cmnd);
}

Result ReaderService::removeReader(const std::string& cmnd) {
    if (!lib.readers.find(cmnd)) return Result::fail("Khong tim thay doc gia co CMND " + cmnd);
    lib.readers.remove(cmnd);
    lib.markDirty();
    return Result::success("Da xoa doc gia " + cmnd);
}

Reader* ReaderService::findByCmnd(const std::string& cmnd) {
    return lib.readers.find(Utils::trim(cmnd));
}

void ReaderService::statsByCardType(int counts[3]) {
    counts[0] = counts[1] = counts[2] = 0;
    listAll([&](const Reader& r) {
        if (r.cardType >= 0 && r.cardType < 3) ++counts[r.cardType];
    });
}
