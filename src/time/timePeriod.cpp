#include "timePeriod.h"
#include <tuple>

STimePeriod::STimePeriod (const CDate & startDate, const CClock & startClock, const CDate & endDate, const CClock & endClock)
    : m_Start(STimeStamp(startDate, startClock)), m_End(STimeStamp(endDate, endClock)) {
}

STimePeriod::STimePeriod (const STimeStamp & start, const STimeStamp & end)
    : m_Start(start), m_End(end) {
}

bool STimePeriod::operator < (const STimePeriod & b) const {
    return std::tie(m_Start, m_End) < std::tie(b.m_Start, b.m_End);
}
