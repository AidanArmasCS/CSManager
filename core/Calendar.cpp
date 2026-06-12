//
// Calendar.cpp
// Layer 1 - The game clock.
//

#include "Calendar.h"

Calendar::Calendar(int year, int month, int day)
        : year_(year), month_(month), day_(day) {}

bool Calendar::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Calendar::daysInMonth(int month, int year) {
    static const int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && isLeapYear(year)) return 29;
    return days[month - 1];
}

void Calendar::advanceDay() {
    day_++;
    if (day_ > daysInMonth(month_, year_)) {
        day_ = 1;
        month_++;
        if (month_ > 12) {
            month_ = 1;
            year_++;
        }
    }
}

void Calendar::advanceDays(int n) {
    for (int i = 0; i < n; i++) advanceDay();
}

string Calendar::monthName() const {
    static const string names[] = {
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
    };
    return names[month_ - 1];
}

string Calendar::toString() const {
    return to_string(day_) + " " + monthName() + " " + to_string(year_);
}
