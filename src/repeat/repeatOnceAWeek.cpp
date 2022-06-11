#include "repeatOnceAWeek.h"
#include "../time/date.h"
#include <iostream>

bool CRepeatOnceAWeek::includeInView(const CDate & repEvBegin, const CDate & dayInCalendar) const {
    return repEvBegin.daysBetween(dayInCalendar) % 7 == 0;
}

void CRepeatOnceAWeek::print(std::ostream & os) const {
    os << "Jednou tydne";
}

int CRepeatOnceAWeek::skippedDaysBetweenEvents() const {
    return 6;
}

std::string CRepeatOnceAWeek::toJSON() const {
    return "\"ONCE_A_WEEK\"";
}

std::unique_ptr<CRepeat> CRepeatOnceAWeek::clone() const {
    return std::make_unique<CRepeatOnceAWeek>(*this);
}

std::vector<STimePeriod> CRepeatOnceAWeek::generatePeriodsOfEventSinceTime(const STimePeriod & event, const STimeStamp & lowLimit) const {
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
    // finding checkpoint from which we are going to make 7days steps
    while(!this->includeInView(beginOfCounting, event.m_Start.m_Date)) {
        try {
            beginOfCounting = beginOfCounting.getDateShiftedByDays(1);
        } catch (const InvalidDateException & e) {
            return {};
        }
        ++daysSinceLowLimit;
    }
    auto dayDifferenceCnt = event.m_Start.m_Date.daysBetween(event.m_End.m_Date);
    // we have the point from which we can iterate after 7 days and add that to the result
    for (auto currDate = beginOfCounting; daysSinceLowLimit <= c_MaximumDaysToGenerateTimePeriods; daysSinceLowLimit += 7) {
        try {
            res.emplace_back(currDate, event.m_Start.m_Clock, currDate.getDateShiftedByDays(dayDifferenceCnt), event.m_End.m_Clock);
            currDate = currDate.getDateShiftedByDays(7);
        } catch (const InvalidDateException & e) {
            break;
        }
    }
    return res;
}