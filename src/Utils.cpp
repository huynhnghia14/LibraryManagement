#include "Utils.h"

#include <cstring>
#include <ctime>

namespace {
    // Mỗi dòng: chữ cái gốc + các biến thể có dấu (chữ thường và chữ hoa)
    const char* const VN_GROUPS[] = {
        "aàáạảãâầấậẩẫăằắặẳẵAÀÁẠẢÃÂẦẤẬẨẪĂẰẮẶẲẴ",
        "eèéẹẻẽêềếệểễEÈÉẸẺẼÊỀẾỆỂỄ",
        "iìíịỉĩIÌÍỊỈĨ",
        "oòóọỏõôồốộổỗơờớợởỡOÒÓỌỎÕÔỒỐỘỔỖƠỜỚỢỞỠ",
        "uùúụủũưừứựửữUÙÚỤỦŨƯỪỨỰỬỮ",
        "yỳýỵỷỹYỲÝỴỶỸ",
        "dđDĐ",
    };
    const int GROUP_COUNT = sizeof(VN_GROUPS) / sizeof(VN_GROUPS[0]);

    // Giải mã 1 ký tự UTF-8 tại vị trí i, trả về mã điểm và tăng i
    unsigned decodeUtf8(const std::string& s, size_t& i) {
        unsigned char c = s[i];
        int extra = 0;
        unsigned cp = c;
        if (c >= 0xF0)      { cp = c & 0x07; extra = 3; }
        else if (c >= 0xE0) { cp = c & 0x0F; extra = 2; }
        else if (c >= 0xC0) { cp = c & 0x1F; extra = 1; }
        ++i;
        for (int k = 0; k < extra && i < s.size(); ++k, ++i) cp = (cp << 6) | (s[i] & 0x3F);
        return cp;
    }

    // Tra mã điểm có dấu -> chữ cái thường không dấu; 0 nếu không thuộc bảng
    char baseLetter(unsigned cp) {
        for (int g = 0; g < GROUP_COUNT; ++g) {
            const std::string group = VN_GROUPS[g];
            size_t i = 0;
            while (i < group.size()) {
                if (decodeUtf8(group, i) == cp) return group[0];
            }
        }
        return 0;
    }
}

namespace Utils {

std::string normalize(const std::string& s) {
    std::string out;
    bool pendingSpace = false;
    size_t i = 0;
    while (i < s.size()) {
        unsigned cp = decodeUtf8(s, i);
        char c = 0;
        if (cp >= 'a' && cp <= 'z')      c = (char)cp;
        else if (cp >= 'A' && cp <= 'Z') c = (char)(cp - 'A' + 'a');
        else if (cp >= '0' && cp <= '9') c = (char)cp;
        else if (cp >= 0x80)             c = baseLetter(cp);

        if (c == 0) {                       // khoảng trắng, dấu câu, ký tự lạ -> coi là phân cách
            pendingSpace = !out.empty();
            continue;
        }
        if (pendingSpace) out += ' ';
        pendingSpace = false;
        out += c;
    }
    return out;
}

std::string trim(const std::string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

bool isDigits(const std::string& s, int minLen, int maxLen) {
    if ((int)s.size() < minLen || (int)s.size() > maxLen) return false;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') return false;
    }
    return true;
}

bool isValidIsbn(const std::string& s) {
    return isDigits(s, 13, 13);
}

bool isValidCmnd(const std::string& s) {
    return s.size() == 9 || s.size() == 12 ? isDigits(s, 9, 12) : false;
}

bool isValidEmail(const std::string& s) {
    size_t at = s.find('@');
    if (at == std::string::npos || at == 0 || s.find('@', at + 1) != std::string::npos) return false;
    size_t dot = s.find('.', at);
    return dot != std::string::npos && dot > at + 1 && dot < s.size() - 1 && s.find(' ') == std::string::npos;
}

bool isValidPhone(const std::string& s) {
    return isDigits(s, 10, 10) && s[0] == '0';
}

void copyStr(char* dst, const std::string& src, int maxLen) {
    size_t n = src.size() < (size_t)maxLen ? src.size() : (size_t)maxLen;
    // không cắt giữa một ký tự UTF-8 nhiều byte
    while (n > 0 && n < src.size() && ((unsigned char)src[n] & 0xC0) == 0x80) --n;
    memcpy(dst, src.data(), n);
    dst[n] = '\0';
}

int displayWidth(const std::string& s) {
    int w = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        if (((unsigned char)s[i] & 0xC0) != 0x80) ++w;
    }
    return w;
}

int currentYear() {
    time_t now = time(nullptr);
    return localtime(&now)->tm_year + 1900;
}

}
