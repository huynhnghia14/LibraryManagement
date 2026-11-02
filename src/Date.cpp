#include "Date.h"

#include <ctime>
#include <cstdio>

namespace {
    bool isLeap(int y) {
        return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
    }

    int daysInMonth(int m, int y) {
        static const int DAYS[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        return (m == 2 && isLeap(y)) ? 29 : DAYS[m - 1];
    }
}

bool Date::isValid() const {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    return day >= 1 && day <= daysInMonth(month, year);
}

long Date::toDays() const {
    long days = 0;
    for (int y = 1900; y < year; ++y) days += isLeap(y) ? 366 : 365;
    for (int m = 1; m < month; ++m) days += daysInMonth(m, year);
    return days + day - 1;
}

Date Date::fromDays(long days) {
    Date d = {1, 1, 1900};
    while (days >= (isLeap(d.year) ? 366 : 365)) {
        days -= isLeap(d.year) ? 366 : 365;
        ++d.year;
    }
    while (days >= daysInMonth(d.month, d.year)) {
        days -= daysInMonth(d.month, d.year);
        ++d.month;
    }
    d.day = (int)days + 1;
    return d;
}

Date Date::addDays(int n) const {
    return fromDays(toDays() + n);
}

int Date::daysBetween(const Date& a, const Date& b) {
    return (int)(b.toDays() - a.toDays());
}

Date Date::today() {
    time_t now = time(nullptr);
    tm* t = localtime(&now);
    Date d = {t->tm_mday, t->tm_mon + 1, t->tm_year + 1900};
    return d;
}

std::string Date::toString() const {
    char buf[11];
    snprintf(buf, sizeof buf, "%02d/%02d/%04d", day, month, year);
    return buf;
}

bool Date::parse(const std::string& s, Date& out) {
    Date d;
    char extra;
    if (sscanf(s.c_str(), "%d/%d/%d%c", &d.day, &d.month, &d.year, &extra) != 3) return false;
    if (!d.isValid()) return false;
    out = d;
    return true;
}

bool Date::operator<(const Date& o) const {
    if (year != o.year) return year < o.year;
    if (month != o.month) return month < o.month;
    return day < o.day;
}

bool Date::operator==(const Date& o) const {
    return day == o.day && month == o.month && year == o.year;
}
