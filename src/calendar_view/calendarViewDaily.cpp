#include "calendarViewDaily.h"
#include "../time/date.h"
#include "../time/clock.h"
#include <unordered_set>
#include <map>
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

CCalendarViewDaily::CCalendarViewDaily(const CDate & currDay,
                                       const std::unordered_map<size_t, CEvent> & mainDb,
                                       const std::multiset<SEventDayLayout> & dailyEvents,
                                       const std::multiset<SEventDayLayout> & repeatableDailyEvents)
: CCalendarView(currDay, mainDb) {
    // create CClock for beginning and end of calendar
    CClock beginClock(0, 0);
    CClock endClock(23, 59);
    // from beginning of calendar (day 0:00 here) to end (day 23:59) add all events that take place in this range
    auto beginIt = dailyEvents.lower_bound(SEventDayLayout(currDay, beginClock, beginClock, 0));
    auto endIt = dailyEvents.upper_bound(SEventDayLayout(currDay, endClock, endClock, 0));
    unordered_set<size_t> eventsAlreadyIncluded;
    for (auto it = beginIt; it != endIt; it++) {
        m_EventsOfCalendar.insert(*it);
        eventsAlreadyIncluded.insert(it->m_Id);
    }
    for (const auto & ev : repeatableDailyEvents) {
        if (eventsAlreadyIncluded.count(ev.m_Id) == 0) {
            if (m_MainDatabase.at(ev.m_Id).includeInView(ev.m_StartDate, currDay)) {
                m_EventsOfCalendar.insert({currDay, ev.m_StartClock, ev.m_EndClock, ev.m_Id});
                eventsAlreadyIncluded.insert(ev.m_Id);
            }
        }
    }
}

void CCalendarViewDaily::fillIndexesAndPrint() {
    cout << endl << setw(c_DelimBarWidth) << setfill('=') << "" << endl;
    size_t idx = 0;
    cout << m_SelectedTime.getWeekdayName() << ' ' << m_SelectedTime.dayAndMonthToString() << ':' << endl << endl;
    if (m_EventsOfCalendar.empty()) {
        cout << "-Tento den se nekonaji zadne udalosti-" << endl;
    }
    for (const auto & ev : m_EventsOfCalendar) {
        m_EventIdxToId[idx] = ev.m_Id;
        cout << ev.m_StartClock << '-' << ev.m_EndClock;
        cout << "   ";
        m_MainDatabase.at(ev.m_Id).dayViewPrint(idx++);
        cout << endl;
    }
    cout << endl;
}

CDate CCalendarViewDaily::getPrevDate() const {
    return m_SelectedTime.getDateShiftedByDays(-1);
}
CDate CCalendarViewDaily::getNextDate() const {
return m_SelectedTime.getDateShiftedByDays(1);
}