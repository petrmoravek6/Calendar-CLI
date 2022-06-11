#pragma once
#include "../time/date.h"
#include "../time/timePeriod.h"
#include "../time/timeStamp.h"
#include <iostream>
#include <memory>
#include <vector>

/** Abstract class representing repeating of an event */
class CRepeat {
public:
    virtual ~CRepeat() = default;
    /** Decides whether to display the event that is taking place on "repEvBegin" for the given date "dayInCalendar" */
    virtual bool includeInView(const CDate & repEvBegin, const CDate & dayInCalendar) const = 0;
    friend std::ostream& operator << (std::ostream & os, const CRepeat & repeat);
    virtual void print(std::ostream & os) const = 0;
    /**
     * @return The number of days that doesn't contain event with this repeat between repeating, e.g:
     * 6 for once a week repeating
     */
    virtual int skippedDaysBetweenEvents() const = 0;
    /**
     * @return std::string representing the object in JSON format
     */
    virtual std::string toJSON() const = 0;
    virtual std::unique_ptr<CRepeat> clone() const = 0;
    /**
     * Generates STimePeriods from lowLimit to 2 years after it or until end of possible calendar high boundary
     * @param event The event's time period that is to be repeated. We have to have this information for multi day events and begin and end time
     * @param lowLimit Point from which we are going to build the output list.
     * @return vector of STimePeriods of the event, that uses this repeat.
     */
    virtual std::vector<STimePeriod> generatePeriodsOfEventSinceTime(const STimePeriod & event, const STimeStamp & lowLimit) const = 0;

    /** Maximum time period that is going to be searched */
    static constexpr size_t c_MaximumDaysToGenerateTimePeriods = 731; // (two years)
};