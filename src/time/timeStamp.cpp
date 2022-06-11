#include "timeStamp.h"
#include <tuple>

STimeStamp::STimeStamp(const CDate & date, const CClock & clock)
    : m_Date(date), m_Clock(clock) {
}

bool STimeStamp::operator < (const STimeStamp & b) const {
    return std::tie(m_Date, m_Clock) < std::tie(b.m_Date, b.m_Clock);
}

bool STimeStamp::operator > (const STimeStamp & b) const {
    return std::tie(m_Date, m_Clock) > std::tie(b.m_Date, b.m_Clock);
}

bool STimeStamp::operator == (const STimeStamp & b) const {
    return !(*this < b) && !(b < *this);
}

bool STimeStamp::operator >= (const STimeStamp & b) const {
    return *this > b || *this == b;
}

int STimeStamp::minutesBetween (const STimeStamp &other) const {
    struct tm x = {0, m_Clock.m_Minute, m_Clock.m_Hour, m_Date.m_Day, m_Date.m_Month - 1, m_Date.m_Year - 1900};
    struct tm y = {0, other.m_Clock.m_Minute, other.m_Clock.m_Hour, other.m_Date.m_Day, other.m_Date.m_Month - 1, other.m_Date.m_Year - 1900};
    x.tm_isdst = -1;
    y.tm_isdst = -1;
    return static_cast<int>(std::abs(difftime(mktime(&x), mktime(&y)) / 60));
}

STimeStamp STimeStamp::getTimeStampShiftedByMinutes(int minutes) const {
    tm newTM = {0, m_Clock.m_Minute + minutes, m_Clock.m_Hour, m_Date.m_Day, m_Date.m_Month - 1, m_Date.m_Year - 1900};
    newTM.tm_isdst = -1;
    mktime(&newTM);
    return {CDate(newTM.tm_year + 1900, newTM.tm_mon + 1, newTM.tm_mday), CClock(newTM.tm_hour, newTM.tm_min)};
}
