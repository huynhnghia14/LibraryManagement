#ifndef DATE_H
#define DATE_H

#include <string>

// Ngày tháng dùng cho phiếu mượn, độc giả, giao dịch (POD, ghi thẳng ra file nhị phân)
struct Date {
    int day, month, year;

    bool isValid() const;                           // 1..31, 1..12, 1900..2100, kiểm tra năm nhuận
    long toDays() const;                            // số ngày kể từ 01/01/1900
    static Date fromDays(long days);
    Date addDays(int n) const;                      // ngày hẹn trả = ngày mượn + n
    static int daysBetween(const Date& a, const Date& b);   // b - a
    static Date today();
    std::string toString() const;                   // "dd/mm/yyyy"
    static bool parse(const std::string& s, Date& out);

    bool operator<(const Date& o) const;
    bool operator==(const Date& o) const;
};

#endif
