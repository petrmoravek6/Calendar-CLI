#include "eventDayLayout.h"
#include "../time/date.h"
#include "../time/clock.h"
#include <tuple>
using namespace std;

SEventDayLayout::SEventDayLayout(const CDate & startDate, const CClock & startClock, const CClock & endClock, size_t id)
    : m_StartDate(startDate), m_StartClock(startClock), m_EndClock(endClock), m_Id(id) {
}

bool SEventDayLayout::operator < (const SEventDayLayout & b) const {
    return tie(m_StartDate, m_StartClock) < tie(b.m_StartDate, b.m_StartClock);
}

bool SEventDayLayout::hasSameParameters(const SEventDayLayout &b) const {
    return m_StartClock == b.m_StartClock && m_StartDate == b.m_StartDate && m_Id == b.m_Id && m_EndClock == b.m_EndClock;
}
