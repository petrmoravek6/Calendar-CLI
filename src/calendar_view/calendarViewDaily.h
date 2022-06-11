#pragma once
#include "calendarView.h"
#include "../time/date.h"
#include "../time/clock.h"

class CCalendarViewDaily : public CCalendarView {
public:
    CCalendarViewDaily(const CDate & currDay,
                       const std::unordered_map<size_t, CEvent> & mainDb,
                       const std::multiset<SEventDayLayout> & dailyEvents,
                       const std::multiset<SEventDayLayout> & repeatableDailyEvents);
    virtual void fillIndexesAndPrint() override;
    virtual CDate getPrevDate() const override;
    virtual CDate getNextDate() const override;
protected:
    /** Event layouts that are going to be displayed in the current view */
    std::multiset<SEventDayLayout> m_EventsOfCalendar;
};
