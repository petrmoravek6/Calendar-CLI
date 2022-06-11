#include "clock.h"
#include <tuple>
#include <iostream>
#include <iomanip>
using namespace std;

CClock::CClock (int h, int m) {
    if (!isValidClock(h, m)) {
        throw InvalidClockException();
    }
    m_Hour = h;
    m_Minute = m;
}

bool CClock::operator < (const CClock & b) const {
    return tie(m_Hour, m_Minute) < tie(b.m_Hour, b.m_Minute);
}

ostream& operator << (ostream & os, const CClock & clock) {
    os << setw(2) << setfill('0') << clock.m_Hour << ':' << setw(2) << setfill('0') << clock.m_Minute;
    return os;
}

bool CClock::isValidClock (int hour, int minute) {
    return !(hour < 0 || hour > 23 || minute < 0 || minute > 59);
}

bool CClock::operator == (const CClock & b) const {
    return !(*this < b) && !(b < *this);
}

bool CClock::operator != (const CClock & b) const {
    return !(*this == b);
}

bool CClock::operator > (const CClock & b) const {
    return tie(m_Hour, m_Minute) > tie(b.m_Hour, b.m_Minute);
}

std::string CClock::toJSON() const {
    string res("{");
    res += "\"hour\":" + to_string(m_Hour);
    res += ",\"minute\":" + to_string(m_Minute);
    res += "}";
    return res;
}
