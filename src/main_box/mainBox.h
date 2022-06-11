#pragma once
#include "../event/eventDayLayout.h"
#include "../event/event.h"
#include "../time/timePeriod.h"
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <memory>

/** Main class that manages the database of events, provides main menu and searching */
class CMainBox {
public:
    /** Prints actions that user can do from main menu and evaluates what to do next */
    void mainMenu();
    /** Displays calendar and its menu. Manages the user's choices until the user exits the calendar view */
    void displayCalendar(int type);
    /** Loads event information from standard input and adds it to database */
    void createEvent();
    void addEvent(CEvent & event);
    void addEvent(const std::string & name, const STimePeriod & timePeriod, const std::optional<std::string> & place,
                  const std::set<std::string> & participants, const std::optional<std::string> & details,
                  std::optional<std::unique_ptr<CRepeat>> & repeat, bool movable);
    void deleteEvent(size_t id);
    /** User can edit a specific event from database. He can choose what parameter to edit from menu */
    void editEvent(size_t id);
    /** Asks user to save the database of events and exits the application */
    void exitMenu() const;
    bool saveEvents() const;
    /** Searches for event from database by name or/and place of the event */
    void searchEvent();
    /** This method tries to move a chosen event to the closest free date possible. The user is asked to enter
     *  the earliest possible term that the algorithm will consider. If not success, the method will print out fail
     *  message and doesn't move the event.
     */
    void moveEventToEarliestTimePossible(size_t id);
private:
    /** key: unique id, value: event full information */
    std::unordered_map<size_t, CEvent> m_EventsMainDatabase;
    /** stores IDs of events that repeat */
    std::multiset<SEventDayLayout> m_RepeatableDailyEvents;
    /** used for displaying calendar views */
    std::multiset<SEventDayLayout> m_DailyEvents;
    /** key: name, value: id of event */
    std::multimap<std::string, size_t> m_EventsSortedByName;
    /** key: place, value: id of event,
        note: not all events have places, so it doesn't contain all events of database */
    std::multimap<std::string, size_t> m_EventsSortedByPlace;

    void eventDetailsMenu(size_t id);
    void loadEvents();
    void searchThroughAndDeleteLayoutDatabase(size_t id, std::multiset<SEventDayLayout> & db);
    void searchThroughAndDeleteDatabase(size_t id, std::multimap<std::string, size_t> & db);
    void displaySearchResults(const std::set<size_t> &results);
    std::set<size_t> searchEventsByName() const;
    std::set<size_t> searchEventsByPlace() const;
    /** Creates intervals for moveEventToEarliestTimePossible method */
    std::set<STimePeriod> makeEventIntervals(const STimeStamp &lowLimit) const;

    static void printMainMenu();
    static constexpr int c_DelimBarWidth = 46;
    static constexpr int c_SearchDelimBarWidth = 100;
};
