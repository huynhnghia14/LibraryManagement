#include "Date.h"

#include <cassert>
#include <iostream>

void testIsValid() {
    Date leap = {29, 2, 2024}, notLeap = {29, 2, 2026}, bad = {31, 4, 2026};
    assert(leap.isValid());
    assert(!notLeap.isValid());
    assert(!bad.isValid());
}

void testAddDaysAcrossMonthAndYear() {
    Date d = {25, 12, 2026};
    Date due = d.addDays(14);
    assert(due.day == 8 && due.month == 1 && due.year == 2027);
}

void testDaysBetween() {
    Date a = {19, 10, 2026}, b = {2, 11, 2026};
    assert(Date::daysBetween(a, b) == 14);
    assert(Date::daysBetween(b, a) == -14);
}

void testParseAndToString() {
    Date d;
    assert(Date::parse("05/03/2026", d));
    assert(d.toString() == "05/03/2026");
    assert(!Date::parse("32/01/2026", d));
    assert(!Date::parse("abc", d));
}

int main() {
    testIsValid();
    testAddDaysAcrossMonthAndYear();
    testDaysBetween();
    testParseAndToString();
    std::cout << "test_date: passed" << std::endl;
    return 0;
}
