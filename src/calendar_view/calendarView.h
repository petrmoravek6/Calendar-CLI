#pragma once
#include "../time/date.h"
#include "../event/event.h"
#include "../event/eventDayLayout.h"
#include <vector>
#include <unordered_map>

/** Abstract class representing specific view that is going to be printed */
class CCalendarView {
public:
    CCalendarView(const CDate & currDate,
                  const std::unordered_map<size_t, CEvent> & mainDb);
    virtual ~CCalendarView() = default;
    /** Fills m_EventIdxToId and prints the view */
    virtual void fillIndexesAndPrint() = 0;
    int menu() const;
    /** Returns CDate corresponding to the previous view in calendar*/
    virtual CDate getPrevDate() const = 0;
    /** Returns CDate corresponding to the next view in calendar*/
    virtual CDate getNextDate() const = 0;
    /** Returns event's ID from user input */
    size_t selectEvent() const;
    bool isEmpty() const;
protected:
    /** A key date for which the view is going to be build */
    CDate m_SelectedTime;
    /** Reference to main database of all events */
    const std::unordered_map<size_t, CEvent> & m_MainDatabase;
    /** Stores unique indexes of events presented in current view and maps them to the IDs of the events */
    std::unordered_map<size_t, size_t> m_EventIdxToId;
    void printMenu() const;
    static constexpr int c_DelimBarWidth = 88;
};


