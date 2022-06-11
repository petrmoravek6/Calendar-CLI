#pragma once
#include "../repeat/repeat.h"
#include "../time/clock.h"
#include "eventDayLayout.h"
#include "../time/timePeriod.h"
#include <optional>
#include <set>
#include <memory>
#include <string>
#include <set>
#include <vector>

class CMainBox;

/** Represents an event with all its properties and information */
class CEvent {
public:
    CEvent(std::string name, const STimePeriod & timePeriod, const std::optional<std::string> & place,
           const std::set<std::string> & participants, const std::optional<std::string> & details,
           std::optional<std::unique_ptr<CRepeat>> & repeat, bool movable);
    CEvent(std::string name, const CDate & startDate, const CClock & startClock, const CDate & endDate,
           const CClock & endClock, const std::optional<std::string> & place,
           const std::set<std::string> & participants, const std::optional<std::string> & details,
           std::optional<std::unique_ptr<CRepeat>> & repeat, bool movable);
    CEvent(CEvent && other) noexcept;
    /** Prints all information about the event */
    void viewDetails() const;
    /**
     * @param repEvBegin the date of event (it is parameter, because of multi day events)
     * @param dayInCalendar the date to be checked
     * @return true if the event should repeat on "dayInCalendar" m_Repeat
     */
    bool includeInView(const CDate & repEvBegin, const CDate & dayInCalendar) const;
    void monthViewPrint(size_t idx) const;
    void weekViewPrint(size_t idx) const;
    void dayViewPrint(size_t idx) const;
    void searchViewPrint(size_t idx) const;
    /**
     * Used for inner searching in the database.
     * @return Layout, which start date is the event's start date
     */
    SEventDayLayout getLowerBoundLayout() const;
    /**
     * Used for inner searching in the database.
     * @return Layout, which start date is the event's end date
     */
    SEventDayLayout getUpperBoundLayout() const;
    /**
     * @return TRUE: event repeats over time, FALSE: doesn't
     */
    bool isRepeatable() const;
    std::string getName() const;
    std::string getPlace() const;
    bool hasPlace() const;
    bool isMovable() const;
    /** Divides the event into days it takes and returns the appropriate layouts as a set */
    std::set<SEventDayLayout> generateDayLayouts(size_t id) const;
    /**
     * @return std::string representing the object in JSON format
     */
    std::string toJSON() const;
    /** Creates event with different member value chosen as string "parameter" */
    CEvent makeEventWithNewParameter(const char * parameter) const;
    void deleteParticipant();
    void addParticipant();
    void changeMovable();
    void editDetails();
    /** Determines whether the event completely ends after given time */
    bool takesPlaceAfter(const STimeStamp & time) const;
    /**
     * @param lowLimit Certain point of time from which the method generates the ranges
     * @return Ranges of time that the event overlaps
     */
    std::vector<STimePeriod> getTimePeriods(const STimeStamp & lowLimit) const;
    /** Creates new event with the nearest free date possible from lowLimit parameter */
    CEvent makeEventWithNewNearestDate(const STimeStamp &lowLimit, const std::set<STimePeriod> & intervals) const;
    /** Returnes the number of minutes that the event lasts */
    int durationInMinutes() const;

    /** Gets name for event from user input in the required format */
    static std::string createNameForEvent();
    /** Gets time for event from user input in the required format */
    static STimePeriod createTimeForEvent();
    /** Gets place for event from user input in the required format */
    static std::optional<std::string> createPlaceForEvent();
    /** Gets participants for event from user input in the required format */
    static std::set<std::string> createParticipantsForEvent();
    /** Gets details for event from user input in the required format */
    static std::optional<std::string> createDetailsForEvent();
    /** Gets CRepeat object for event from user input */
    static std::optional<std::unique_ptr<CRepeat>> createRepeatForEvent();
    /** Gets boolean value whether the event should be movable or not from user input */
    static bool createMovingForEvent();
    static void printMenu();
    static void printEditMenu();

    static constexpr long c_MaxLengthName = 40;
    static constexpr long c_MaxLengthNameParticipant = 25;
    static constexpr long c_MaxLengthPlace = 60;
    static constexpr long c_MaxLengthDetails = 200;
private:
    std::string m_Name;
    CDate m_StartDate;
    CClock m_StartClock;
    CDate m_EndDate;
    CClock m_EndClock;
    std::optional<std::string> m_Place;
    std::set<std::string> m_Participants;
    std::optional<std::string> m_Details;
    std::optional<std::unique_ptr<CRepeat>> m_Repeat;
    bool m_IsMovable;

    /** Size of an event view in the specific calendar views */
    static constexpr size_t c_FixLengthMonthView = 14;
    static constexpr int c_DelimBarWidth = 60;
};