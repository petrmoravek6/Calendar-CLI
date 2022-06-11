#pragma once
#include "../time/date.h"
#include "../time/clock.h"

/** Structure used for representing a day and a specific time at that day (begin clock and end clock) that an event occupies
 * in time. Reference to the event is represented by it's ID.
 */
struct SEventDayLayout {
    SEventDayLayout(const CDate & startDate, const CClock & startClock, const CClock & endClock, size_t id);
    CDate m_StartDate;
    CClock m_StartClock;
    CClock m_EndClock;
    size_t m_Id;
    bool operator < (const SEventDayLayout & b) const;
    /** Tests whether the instance has completely same member values as the other one. Used for testing. **/
    bool hasSameParameters (const SEventDayLayout & b) const;
};
