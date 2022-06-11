#include "date.h"
#include <tuple>
#include <ctime>
#include <iostream>
using namespace std;

CDate::CDate (int y, int m, int d) {
    if (!isValidDate(y, m, d)) {
        throw InvalidDateException();
    }
    m_Year = y;
    m_Month = m;
    m_Day = d;
}

CDate::CDate(const tm & t) : m_Year(t.tm_year + 1900), m_Month(t.tm_mon + 1), m_Day(t.tm_mday) {
}

CDate CDate::getDateShiftedByDays (int days) const {
    tm newTM = {0, 0, 12, m_Day + days, m_Month - 1, m_Year - 1900};
    newTM.tm_isdst = -1;
    mktime(&newTM);
    return {newTM};
}

CDate CDate::getDateShiftedByOneMonth (int step) const {
    int mon, year;
    if (step == 1) {
        if (m_Month == 12) {
            mon = 1;
            year = m_Year + 1;
        }
        else {
            mon = m_Month + 1;
            year = m_Year;
        }
    }
    else if (step == -1) {
        if (m_Month == 1) {
            mon = 12;
            year = m_Year - 1;
        }
        else {
            mon = m_Month - 1;
            year = m_Year;
        }
    }
    else {
        throw invalid_argument("argument must be either 1 or -1");
    }
    tm newTM = {0, 0, 12, min(daysInMonth(mon, year), m_Day), mon - 1, year - 1900};
    newTM.tm_isdst = -1;
    mktime(&newTM);
    return {newTM};
}

bool CDate::operator < (const CDate & b) const {
    return tie(m_Year, m_Month, m_Day) < tie(b.m_Year, b.m_Month, b.m_Day);
}

bool CDate::monthEqual(const CDate &b) const {
    return m_Month == b.m_Month;
}

const char *CDate::getMonthName() const {
    return monthName[m_Month - 1];
}

const char *CDate::getWeekdayName() const {
    tm t = {0, 0, 12, m_Day, m_Month - 1, m_Year - 1900};
    t.tm_isdst = -1;
    mktime(&t);
    return weekdayName[t.tm_wday];
}

const char *CDate::getWeekdayNameShort() const {
    tm t = {0, 0, 12, m_Day, m_Month - 1, m_Year - 1900};
    t.tm_isdst = -1;
    mktime(&t);
    return weekdayNameShort[t.tm_wday];
}

CDate CDate::getFirstDayOfCurrentWeek() const {
    tm t = {0, 0, 12, m_Day, m_Month - 1, m_Year - 1900};
    t.tm_isdst = -1;
    mktime(&t);
    if (t.tm_wday == 1) {
        return *this;
    }
    else if (t.tm_wday > 1) {
        return this->getDateShiftedByDays(-(t.tm_wday - 1));
    }
    else {
        return this->getDateShiftedByDays(-6);
    }
}

CDate CDate::getLastDayOfCurrentWeek() const {
    tm t = {0, 0, 12, m_Day, m_Month - 1, m_Year - 1900};
    t.tm_isdst = -1;
    mktime(&t);
    if (t.tm_wday == 0) {
        return *this;
    }
    else {
        return this->getDateShiftedByDays(7 - t.tm_wday);
    }
}

long CDate::daysBetween(const CDate &b) const {
    // we don't need a value for secs, minutes and hours, so we use "dummy" parameters 0, 0, 12 here
    struct tm x = {0, 0, 12, m_Day, m_Month - 1, m_Year - 1900};
    struct tm y = {0, 0, 12, b.m_Day, b.m_Month - 1, b.m_Year - 1900};
    x.tm_isdst = -1;
    y.tm_isdst = -1;
    return abs(static_cast<long>(difftime(mktime(&x), mktime(&y)) / (60 * 60 * 24)));
}

CDate CDate::getFirstDayOfCurrentMonth() const {
    return {m_Year, m_Month, 1};
}

CDate CDate::getLastDayOfCurrentMonth() const {
    return {m_Year, m_Month, daysInMonth(m_Month, m_Year)};
}

string CDate::dayAndMonthToString() const {
    return to_string(m_Day) + '.' + to_string(m_Month) + '.';
}

std::ostream &operator << (ostream & os, const CDate & d) {
    os << d.m_Day << '.' << d.m_Month << '.' << d.m_Year;
    return os;
}

int CDate::daysInMonth(int month, int year) {
    if (month == 1 ||
        month == 3 ||
        month == 5 ||
        month == 7 ||
        month == 8 ||
        month == 10 ||
        month == 12)
        return 31;
    else if (month == 4 ||
             month == 6 ||
             month == 9 ||
             month == 11)
        return 30;
    else {
        if (isLeapYear(year))
            return 29;
        else
            return 28;
    }
}

bool CDate::isValidDate (int year, int month, int day) {
    return !(month < 1 || month > 12 || day < 1 || day > daysInMonth(month, year)
             || year < 1970 || year > 2037);
}

bool CDate::isLeapYear (int year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0) && year % 4000 != 0;
}

const char* CDate::monthName[] = {"Leden", "Unor", "Brezen", "Duben", "Kveten", "Cerven", "Cervenec",
                                     "Srpen", "Zari", "Rijen", "Listopad", "Prosinec"};

const char* CDate::weekdayName[] = {"Nedele", "Pondeli", "Utery", "Streda", "Ctvrtek", "Patek", "Sobota"};

const char* CDate::weekdayNameShort[] = {"NE", "PO", "UT", "ST", "CT", "PA", "SO"};

bool CDate::operator==(const CDate &b) const {
    return !(*this < b) && !(b < *this);
}

bool CDate::operator!=(const CDate &b) const {
    return !(*this == b);
}

bool CDate::operator<=(const CDate &b) const {
    return (*this < b || *this == b);
}

std::string CDate::toJSON() const {
    string res("{");
    res += "\"year\":" + to_string(m_Year);
    res += ",\"month\":" + to_string(m_Month);
    res += ",\"day\":" + to_string(m_Day);
    res += "}";
    return res;
}

bool CDate::isWeekDay() const {
    struct tm x = {0, 0, 12, m_Day, m_Month - 1, m_Year - 1900};
    x.tm_isdst = -1;
    mktime(&x);
    return x.tm_wday > 0 && x.tm_wday < 6;
}
