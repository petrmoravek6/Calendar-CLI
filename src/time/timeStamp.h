#pragma once
#include "date.h"
#include "clock.h"

/** Basic structure representing a point in the time - date and clock */
struct STimeStamp {
    STimeStamp(const CDate & date, const CClock & clock);
    bool operator < (const STimeStamp & b) const;
    bool operator > (const STimeStamp & b) const;
    bool operator == (const STimeStamp & b) const;
    bool operator >= (const STimeStamp & b) const;
    /**
     * Returnes number of minutes between two time stamps
     */
    int minutesBetween (const STimeStamp & other) const;
    /** Shifts the time stamp by given number of minutes */
    STimeStamp getTimeStampShiftedByMinutes (int minutes) const;

    CDate m_Date;
    CClock m_Clock;
};


