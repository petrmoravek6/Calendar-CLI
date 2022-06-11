#pragma once
#include "date.h"
#include "clock.h"
#include "timeStamp.h"

/** Basic structure representing time interval - beginning and end */
struct STimePeriod {
    STimePeriod(const CDate & startDate, const CClock & startClock, const CDate & endDate, const CClock & endClock);
    STimePeriod(const STimeStamp & start, const STimeStamp & end);
    bool operator < (const STimePeriod & b) const;

    STimeStamp m_Start;
    STimeStamp m_End;
};


