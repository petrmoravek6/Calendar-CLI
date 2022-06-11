#include "repeatOnceTwoWeeks.h"
#include "../time/date.h"
#include <iostream>
#include <vector>

bool CRepeatOnceTwoWeeks::includeInView(const CDate & repEvBegin, const CDate & dayInCalendar) const {
    return repEvBegin.daysBetween(dayInCalendar) % 14 == 0;
}

void CRepeatOnceTwoWeeks::print(std::ostream & os) const {
    os << "Jednou za 14 dni";
}

int CRepeatOnceTwoWeeks::skippedDaysBetweenEvents() const {
    return 13;
}

std::string CRepeatOnceTwoWeeks::toJSON() const {
    return "\"ONCE_TWO_WEEKS\"";
}

std::unique_ptr<CRepeat> CRepeatOnceTwoWeeks::clone() const {
    return std::make_unique<CRepeatOnceTwoWeeks>(*this);
}

std::vector<STimePeriod> CRepeatOnceTwoWeeks::generatePeriodsOfEventSinceTime(const STimePeriod & event, const STimeStamp & lowLimit) const {
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
    // finding checkpoint from which we are going to make 14days steps
    while(!this->includeInView(beginOfCounting, event.m_Start.m_Date)) {
        try {
            beginOfCounting = beginOfCounting.getDateShiftedByDays(1);
        } catch (const InvalidDateException & e) {
            return {};
        }
        ++daysSinceLowLimit;
    }
    auto dayDifferenceCnt = event.m_Start.m_Date.daysBetween(event.m_End.m_Date);
    // we have the point from which we can iterate after 14 days and add that to the result
    for (auto currDate = beginOfCounting; daysSinceLowLimit <= c_MaximumDaysToGenerateTimePeriods; daysSinceLowLimit += 14) {
        try {
            res.emplace_back(currDate, event.m_Start.m_Clock, currDate.getDateShiftedByDays(dayDifferenceCnt), event.m_End.m_Clock);
            currDate = currDate.getDateShiftedByDays(14);
        } catch (const InvalidDateException & e) {
            break;
        }
    }
    return res;
}
