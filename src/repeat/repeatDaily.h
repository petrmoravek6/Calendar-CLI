#pragma once
#include "repeat.h"
#include "../time/date.h"
#include <iostream>

class CRepeatDaily : public CRepeat {
public:
    virtual bool includeInView(const CDate & repEvBegin, const CDate & dayInCalendar) const override;
    virtual void print(std::ostream & os) const override;
    virtual int skippedDaysBetweenEvents() const override;
    virtual std::string toJSON() const override;
    virtual std::unique_ptr<CRepeat> clone() const override;
    virtual std::vector<STimePeriod> generatePeriodsOfEventSinceTime(const STimePeriod & event, const STimeStamp & lowLimit) const override;
};