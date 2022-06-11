#pragma once
#include <ctime>
#include <string>
#include <iostream>
#include <stdexcept>

class InvalidDateException : public std::invalid_argument {
public:
    InvalidDateException ()
            : invalid_argument ("invalid date or format") {
    }
};

/** Class representing year, month and day of the month */
class CDate {
public:
    /** Constructor throws an InvalidDateException if the given parameters "y", "m" and "d" are out of time range
     * or out of the aplication date range (1970 - 2037) */
    CDate(int y, int m, int d);
    CDate(const tm & t);
    CDate getDateShiftedByDays (int days) const;
    CDate getDateShiftedByOneMonth (int step) const;
    /** Returns true if this.month == parameter_date.month */
    bool monthEqual(const CDate &b) const;
    const char* getMonthName() const;
    const char* getWeekdayName() const;
    const char* getWeekdayNameShort() const;
    CDate getFirstDayOfCurrentWeek() const;
    CDate getLastDayOfCurrentWeek() const;
    CDate getFirstDayOfCurrentMonth() const;
    CDate getLastDayOfCurrentMonth() const;
    /** Computes the distance between this and the other date */
    long daysBetween(const CDate & b) const;
    bool isWeekDay() const;
    std::string dayAndMonthToString() const;
    /**
     * @return std::string representing the object in JSON format
     */
    std::string toJSON() const;
    friend std::ostream& operator << (std::ostream & os, const CDate & d);
    bool operator < (const CDate & b) const;
    bool operator == (const CDate & b) const;
    bool operator != (const CDate & b) const;
    bool operator <= (const CDate & b) const;
    int m_Year;
    int m_Month;
    int m_Day;
private:
    static int daysInMonth (int month, int year);
    static bool isValidDate (int year, int month, int day);
    static bool isLeapYear (int year);
    static const char* monthName[];
    static const char* weekdayName[];
    static const char* weekdayNameShort[];
};


