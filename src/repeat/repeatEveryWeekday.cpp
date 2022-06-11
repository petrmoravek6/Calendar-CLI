#include "repeatEveryWeekday.h"
#include "../time/date.h"
#include <iostream>

bool CRepeatEveryWeekday::includeInView(const CDate & repEvBegin, const CDate & dayInCalendar) const {
    return dayInCalendar.isWeekDay();
}

void CRepeatEveryWeekday::print(std::ostream & os) const {
    os << "Kazdy vsedni den";
}

int CRepeatEveryWeekday::skippedDaysBetweenEvents() const {
    return 0;
}

std::string CRepeatEveryWeekday::toJSON() const {
    return "\"EVERY_WEEKDAY\"";
}

std::unique_ptr<CRepeat> CRepeatEveryWeekday::clone() const {
    return std::make_unique<CRepeatEveryWeekday>(*this);
}

std::vector<STimePeriod> CRepeatEveryWeekday::generatePeriodsOfEventSinceTime(const STimePeriod & event, const STimeStamp & lowLimit) const {
    std::vector<STimePeriod> res;
    CDate beginOfCounting(lowLimit.m_Date);
    size_t daysSinceLowLimit = 0;
    if (lowLimit.m_Clock > event.m_Start.m_Clock) {
        try {
            beginOfCounting = beginOfCounting.getDateShiftedByDays(1);
        } catch (const InvalidDateException & e) {
            return {};
        }
        ++daysSinceLowLimit;
    }
    auto dayDifferenceCnt = event.m_Start.m_Date.daysBetween(event.m_End.m_Date);
    // we have the point (beginOfCounting) from which we can iterate after one day, check if it's week day and add that to the result
    for (auto currDate = beginOfCounting; daysSinceLowLimit <= c_MaximumDaysToGenerateTimePeriods; ++daysSinceLowLimit) {
            try {
                if (currDate.isWeekDay()) {
                    res.emplace_back(currDate, event.m_Start.m_Clock, currDate.getDateShiftedByDays(dayDifferenceCnt),
                                     event.m_End.m_Clock);
                }
                currDate = currDate.getDateShiftedByDays(1);
            } catch (const InvalidDateException & e) {
                break;
            }
    }
    return res;
}