#include "repeatDaily.h"
#include "../time/date.h"
#include <iostream>

bool CRepeatDaily::includeInView(const CDate & repEvBegin, const CDate & dayInCalendar) const {
    return true;
}

void CRepeatDaily::print(std::ostream & os) const {
    os << "Denne";
}

int CRepeatDaily::skippedDaysBetweenEvents() const {
    return 0;
}

std::string CRepeatDaily::toJSON() const {
    return "\"DAILY\"";
}

std::unique_ptr<CRepeat> CRepeatDaily::clone() const {
    return std::make_unique<CRepeatDaily>(*this);
}

std::vector<STimePeriod> CRepeatDaily::generatePeriodsOfEventSinceTime(const STimePeriod & event, const STimeStamp & lowLimit) const {
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
    // we have the point (beginOfCounting) from which we can iterate after 1 day and add that to the result
    for (auto currDate = beginOfCounting; daysSinceLowLimit <= c_MaximumDaysToGenerateTimePeriods; ++daysSinceLowLimit) {
        try {
            res.emplace_back(currDate, event.m_Start.m_Clock, currDate.getDateShiftedByDays(dayDifferenceCnt), event.m_End.m_Clock);
            currDate = currDate.getDateShiftedByDays(1);
        } catch (const InvalidDateException & e) {
            break;
        }
    }
    return res;
}