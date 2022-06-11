#include "calendarViewMonthly.h"
#include "../time/date.h"
#include "../time/clock.h"
#include <unordered_set>
#include <map>
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

CCalendarViewMonthly::CCalendarViewMonthly(const CDate & currDay,
                                           const std::unordered_map<size_t, CEvent> & mainDb,
                                           const std::multiset<SEventDayLayout> & dailyEvents,
                                           const std::multiset<SEventDayLayout> & repeatableDailyEvents)
       : CCalendarView(currDay, mainDb) {
    // create beginning and end of calendar
    CDate beginDate = currDay.getFirstDayOfCurrentMonth();
    CClock beginClock(0, 0);
    CDate endDate = currDay.getLastDayOfCurrentMonth();
    CClock endClock(23, 59);
    // from beginning of calendar to end add all events that take place in this range
    auto beginIt = dailyEvents.lower_bound(SEventDayLayout(beginDate, beginClock, beginClock, 0));
    auto endIt = dailyEvents.upper_bound(SEventDayLayout(endDate, endClock, endClock, 0));
    map<CDate, unordered_set<size_t>> eventsAlreadyIncludedInDay;
    for (auto it = beginIt; it != endIt; it++) {
        m_EventsOfCalendar[it->m_StartDate].insert(*it);
        eventsAlreadyIncludedInDay[it->m_StartDate].insert(it->m_Id);
    }
    // adding repeatable events that don't have to have common date, but should appear in the calendar
    for (CDate currDate = beginDate; currDate.monthEqual(beginDate); currDate = currDate.getDateShiftedByDays(1)) {
        // for each element from repeatable events:
        for (const auto & ev : repeatableDailyEvents) {
            if (eventsAlreadyIncludedInDay[currDate].count(ev.m_Id) == 0) {
                // get element's ID, get ID's CEvent, call CEvent's method include
                if (m_MainDatabase.at(ev.m_Id).includeInView(ev.m_StartDate, currDate)) {
                    m_EventsOfCalendar[currDate].emplace(currDate, ev.m_StartClock, ev.m_EndClock, ev.m_Id);
                    eventsAlreadyIncludedInDay[currDate].insert(ev.m_Id);
                }
            }
        }
    }
}

void CCalendarViewMonthly::fillIndexesAndPrint() {
    cout << endl << setw(c_DelimBarWidth) << setfill('=') << "" << endl;
    string heading = m_SelectedTime.getMonthName();
    transform(heading.begin(), heading.end(), heading.begin(), ::toupper);
    cout << heading << endl;

    size_t idx = 0;
    unordered_map<size_t, size_t> idToIdx;
    for (CDate currDate(m_SelectedTime.getFirstDayOfCurrentMonth()); currDate.monthEqual(m_SelectedTime); currDate = currDate.getDateShiftedByDays(1)) {
        cout << setw(3) << setfill(' ') << to_string(currDate.m_Day) + '.';
        cout << ' ' << currDate.getWeekdayNameShort() << "  ";
        if (m_EventsOfCalendar.count(currDate) != 0) {
            for (const auto & eventOfDate : m_EventsOfCalendar.at(currDate)) {
                size_t id = eventOfDate.m_Id;
                if (idToIdx.count(id) == 0) {
                    m_EventIdxToId[idx] = id;
                    idToIdx[id] = idx;
                    m_MainDatabase.at(id).monthViewPrint(idx++);
                }
                else {
                    m_MainDatabase.at(id).monthViewPrint(idToIdx.at(id));
                }
            }
        }
        cout << endl;
    }
}

CDate CCalendarViewMonthly::getPrevDate() const {
    return m_SelectedTime.getDateShiftedByOneMonth(-1);
}
CDate CCalendarViewMonthly::getNextDate() const {
    return m_SelectedTime.getDateShiftedByOneMonth(1);
}