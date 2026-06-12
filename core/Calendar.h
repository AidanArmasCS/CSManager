//
// Calendar.h
// Layer 1 - The game clock.
//
// Holds a single in-game date and knows how to advance it.
// Pure data + date math. No printing beyond producing strings.
// Financials (paydays) and the Schedule (event dates) will both
// be driven off this clock.
//

#ifndef CALENDAR_H
#define CALENDAR_H

#include <string>

using namespace std;

class Calendar {
private:
    int year_;
    int month_;   // 1-12
    int day_;     // 1-31

    static bool isLeapYear(int year);
    static int daysInMonth(int month, int year);

public:
    // Defaults to the start of the 2025 competitive season.
    explicit Calendar(int year = 2025, int month = 1, int day = 1);

    // --- Advancing time ---
    void advanceDay();          // step forward one day, rolling month/year over
    void advanceDays(int n);    // step forward n days

    // --- Queries ---
    int getYear() const { return year_; }
    int getMonth() const { return month_; }
    int getDay() const { return day_; }

    // --- Display helpers (UI reads these; engine never prints) ---
    string monthName() const;          // "January"
    string toString() const;           // "1 January 2025"
};

#endif // CALENDAR_H
