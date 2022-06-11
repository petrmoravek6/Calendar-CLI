#include "calendarViewWeekly.h"
#include "../time/date.h"
#include "../time/clock.h"
#include <unordered_set>
#include <map>
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

CCalendarViewWeekly::CCalendarViewWeekly(const CDate & currDay,
                                         const std::unordered_map<size_t, CEvent> & mainDb,
                                         const std::multiset<SEventDayLayout> & dailyEvents,
                                         const std::multiset<SEventDayLayout> & repeatableDailyEvents)
        : CCalendarView(currDay, mainDb) {
    // create beginning and end of calendar
    CDate beginDate = currDay.getFirstDayOfCurrentWeek();
    CClock beginClock(0, 0);
    CDate endDate = currDay.getLastDayOfCurrentWeek();
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
    for (CDate currDate = beginDate; currDate <= m_SelectedTime.getLastDayOfCurrentWeek(); currDate = currDate.getDateShiftedByDays(1)) {
        // for each element from repeatable events:
        for (const auto & ev : repeatableDailyEvents) {
            if (eventsAlreadyIncludedInDay[currDate].count(ev.m_Id) == 0) {
                // get element's ID, get ID's CEvent, call CEvent's method include
                if (m_MainDatabase.at(ev.m_Id).includeInView(ev.m_StartDate, currDate)) {
                    m_EventsOfCalendar[currDate].insert({currDate, ev.m_StartClock, ev.m_EndClock, ev.m_Id});
                    eventsAlreadyIncludedInDay[currDate].insert(ev.m_Id);
                }
            }
        }
    }
}

void CCalendarViewWeekly::fillIndexesAndPrint() {
    cout << endl << setw(c_DelimBarWidth) << setfill('=') << "" << endl;
    size_t idx = 0;
    unordered_map<size_t, size_t> idToIdx;
    for (CDate currDate = m_SelectedTime.getFirstDayOfCurrentWeek(); currDate <= m_SelectedTime.getLastDayOfCurrentWeek(); currDate = currDate.getDateShiftedByDays(1)) {
        cout << currDate.getWeekdayNameShort() << ' ';
        cout << setw(6) << setfill(' ') << currDate.dayAndMonthToString() << "   ";
        if (m_EventsOfCalendar.count(currDate) != 0) {
            auto evIt = m_EventsOfCalendar.at(currDate).begin();
            for (size_t j = 0; j < m_EventsOfCalendar.at(currDate).size(); j++) {
                size_t id = evIt->m_Id;
                if (j != 0) {
                    cout << setw(12) << setfill(' ') << "";
                }
                cout << evIt->m_StartClock << '-' << evIt->m_EndClock << "  ";
                if (idToIdx.count(id) == 0) {
                    m_EventIdxToId[idx] = id;
                    idToIdx[id] = idx;
                    m_MainDatabase.at(id).weekViewPrint(idx++);
                }
                else {
                    m_MainDatabase.at(id).weekViewPrint(idToIdx.at(id));
                }
                if (j != m_EventsOfCalendar.at(currDate).size() - 1) {
                    cout << endl;
                }
                evIt++;
            }
        }
        cout << endl;
        cout << setw(c_DelimBarWidth) << setfill('-') << "" << endl;
    }
}

CDate CCalendarViewWeekly::getPrevDate() const {
    return m_SelectedTime.getDateShiftedByDays(-7);
}
CDate CCalendarViewWeekly::getNextDate() const {
    return m_SelectedTime.getDateShiftedByDays(7);
}