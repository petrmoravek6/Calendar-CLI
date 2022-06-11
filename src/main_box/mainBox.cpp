#include "mainBox.h"
#include "../calendar_view/calendarView.h"
#include "../calendar_view/calendarViewDaily.h"
#include "../calendar_view/calendarViewWeekly.h"
#include "../calendar_view/calendarViewMonthly.h"
#include "../util/util.h"
#include "../event/event.h"
#include "../repeat/repeat.h"
#include "../repeat/repeatDaily.h"
#include "../repeat/repeatEveryWeekday.h"
#include "../repeat/repeatOnceAWeek.h"
#include "../repeat/repeatOnceTwoWeeks.h"
#include "../util/id.h"
#include "../file/writer.h"
#include "../file/fileException.h"
#include "../file/jsonReader.h"
#include <iostream>
#include <forward_list>
#include <set>
#include <map>
#include <chrono>
#include <thread>
#include <tuple>
#include <iomanip>
#include <algorithm>

using namespace std;

void CMainBox::printMainMenu() {
    cout << endl << setw(c_DelimBarWidth) << setfill('=') << "" << endl;
    cout << "HLAVNI MENU" << endl;
    cout << "\t1) Zobrazit denni kalendar" << endl;
    cout << "\t2) Zobrazit tydenni kalendar" << endl;
    cout << "\t3) Zobrazit mesicni kalendar" << endl;
    cout << "\t4) Vyhledat udalosti" << endl;
    cout << "\t5) Zadat udalost" << endl;
    cout << "\t6) Ulozit" << endl;
    cout << "\t0) Konec" << endl;
    cout << setw(c_DelimBarWidth) << setfill('=') << "" << endl;
    cout << endl;
    cout << "Zadejte jednu z moznosti a enterem potvrdte:" << endl;
}

void CMainBox::mainMenu() {
    loadEvents();
    while (true) {
        printMainMenu();
        int choice = getNumber(0, 6);
        if (choice == 1 || choice == 2 || choice == 3) {
            displayCalendar(choice);
        }
        else if (choice == 4) {
            searchEvent();
        }
        else if (choice == 5) {
            createEvent();
        }
        else if (choice == 6) {
            saveEvents();
        }
        else if (choice == 0) {
            exitMenu();
            return;
        }
    }
}

void CMainBox::displayCalendar(int type) {
    time_t t = time(0);   // get time now
    tm * t_tm = localtime(&t);
    CDate selectedDate(*t_tm);
    unique_ptr<CCalendarView> calendar = nullptr;
    while (true) {
        switch (type) {
            case 1:
                calendar = make_unique<CCalendarViewDaily>(selectedDate, m_EventsMainDatabase, m_DailyEvents, m_RepeatableDailyEvents);
                break;
            case 2:
                calendar = make_unique<CCalendarViewWeekly>(selectedDate, m_EventsMainDatabase, m_DailyEvents, m_RepeatableDailyEvents);
                break;
            case 3:
                calendar = make_unique<CCalendarViewMonthly>(selectedDate, m_EventsMainDatabase, m_DailyEvents, m_RepeatableDailyEvents);
                break;
        }
        calendar->fillIndexesAndPrint();
        switch (calendar->menu()) {
            case 0:
                return;
            case 1:
                selectedDate = calendar->getPrevDate();
                break;
            case 2:
                selectedDate = calendar->getNextDate();
                break;
            case 3: {
                if (calendar->isEmpty()) {
                    printMsgAndSleep("V aktualnim kalendari nejsou zadne udalosti k vyberu.", 2);
                    break;
                }
                size_t id = calendar->selectEvent();
                m_EventsMainDatabase.at(id).viewDetails();
                eventDetailsMenu(id);
                break;
            }
            case 4:
                createEvent();
                break;
            case 5:
                if (calendar->isEmpty()) {
                    printMsgAndSleep("V aktualnim kalendari nejsou zadne udalosti k uprave.", 2);
                    break;
                }
                editEvent(calendar->selectEvent());
                break;
            case 6:
                if (calendar->isEmpty()) {
                    printMsgAndSleep("V aktualnim kalendari nejsou zadne udalosti k mazani.", 2);
                    break;
                }
                deleteEvent(calendar->selectEvent());
        }
    }
}

void CMainBox::deleteEvent(size_t id) {
    const auto & event = m_EventsMainDatabase.at(id);
    searchThroughAndDeleteLayoutDatabase(id, m_DailyEvents);
    if (event.isRepeatable()) {
        searchThroughAndDeleteLayoutDatabase(id, m_RepeatableDailyEvents);
    }
    searchThroughAndDeleteDatabase(id, m_EventsSortedByName);
    if (event.hasPlace()) {
        searchThroughAndDeleteDatabase(id, m_EventsSortedByPlace);
    }
    m_EventsMainDatabase.erase(id);
}

void CMainBox::searchThroughAndDeleteLayoutDatabase(size_t id, multiset<SEventDayLayout> & db) {
    const auto & event = m_EventsMainDatabase.at(id);
    forward_list<multiset<SEventDayLayout>::iterator> toDel;
    auto lb = db.lower_bound(event.getLowerBoundLayout());
    auto ub = db.upper_bound(event.getUpperBoundLayout());
    for (auto it = lb; it != ub; it++) {
        if (it->m_Id == id) {
            toDel.push_front(it);
        }
    }
    // delete all elements from toDel
    for (const auto & it : toDel) {
        db.erase(it);
    }
}

void CMainBox::searchThroughAndDeleteDatabase(size_t id, multimap<string, size_t> &db) {
    const auto & event = m_EventsMainDatabase.at(id);
    for (auto [begin, end] = db.equal_range(event.getName()); begin != end; begin++) {
        if (begin->second == id) {
            db.erase(begin);
            break;
        }
    }
}

void CMainBox::createEvent() {
    auto name = CEvent::createNameForEvent();
    auto timePeriod = CEvent::createTimeForEvent();
    auto place = CEvent::createPlaceForEvent();
    auto participants = CEvent::createParticipantsForEvent();
    auto details = CEvent::createDetailsForEvent();
    auto repeat = CEvent::createRepeatForEvent();
    auto movable = CEvent::createMovingForEvent();

    if (repeat
        && timePeriod.m_Start.m_Date.daysBetween(timePeriod.m_End.m_Date) > repeat.value()->skippedDaysBetweenEvents()) {
        printMsgAndSleep("Chyba vytvoreni udalosti. Nelze vytvorit udalost, ktera se opakuje v mensich"
                         " intervalech nez je pocet dnu, ktere zabira. "
                         "Mohlo by dojit k prekryvani udalosti, ktere v tomto pripade logicky nedava smysl.", 4);
        return;
    }
    addEvent(name, timePeriod, place, participants, details, repeat, movable);
}

void CMainBox::addEvent(const string & name, const STimePeriod & timePeriod, const optional<string> & place,
                        const set<string> & participants, const optional<string> & details,
                        optional<unique_ptr<CRepeat>> & repeat, bool movable) {
    size_t id = ID::nextId();
    bool repeatHasValue = repeat.has_value();
    m_EventsMainDatabase.emplace(id, move(CEvent(name, timePeriod, place, participants, details, repeat, movable)));
    auto dayLayouts = m_EventsMainDatabase.at(id).generateDayLayouts(id);
    m_DailyEvents.insert(dayLayouts.begin(), dayLayouts.end());
    if (repeatHasValue) {
        m_RepeatableDailyEvents.insert(dayLayouts.begin(), dayLayouts.end());
    }
    m_EventsSortedByName.emplace(name, id);
    if (place.has_value()) {
        m_EventsSortedByPlace.emplace(place.value(), id);
    }
}

void CMainBox::addEvent(CEvent & event) {
    size_t id = ID::nextId();
    bool repeatHasValue = event.isRepeatable();
    m_EventsSortedByName.emplace(event.getName(), id);
    if (event.hasPlace()) {
        m_EventsSortedByPlace.emplace(event.getPlace(), id);
    }
    m_EventsMainDatabase.emplace(id, move(event));
    auto dayLayouts = m_EventsMainDatabase.at(id).generateDayLayouts(id);
    m_DailyEvents.insert(dayLayouts.begin(), dayLayouts.end());
    if (repeatHasValue) {
        m_RepeatableDailyEvents.insert(dayLayouts.begin(), dayLayouts.end());
    }

}

bool CMainBox::saveEvents() const {
    if (m_EventsMainDatabase.empty()) {
        printMsgAndSleep("V aplikaci nejsou zadne udalosti k ulozeni", 2);
        return true;
    }
    cout << "Zadejte cilovou cestu k ulozeni udalosti ve formatu .json" << endl;
    auto path = getLine();
    string errorMsg = "Behem ukladani doslo k chybe, data nebyla ulozena.";
    try {
        CWriter writer(path);
        if(!writer.write("[")) {
            printMsgAndSleep(errorMsg, 2);
            return false;
        }
        size_t i = 0;
        for (const auto & ev : m_EventsMainDatabase) {
            if(!writer.write(ev.second.toJSON())) {
                printMsgAndSleep(errorMsg, 2);
                return false;
            }
            if (i != m_EventsMainDatabase.size() - 1) {
                if(!writer.write(",")) {
                    printMsgAndSleep(errorMsg, 2);
                    return false;
                }
            }
            ++i;
        }
        if(!writer.write("]") || !writer.close()) {
            printMsgAndSleep(errorMsg, 2);
            return false;
        }
    } catch (const FileException & e) {
        printMsgAndSleep(errorMsg, 2);
        return false;
    }
    printMsgAndSleep("Ukladani souboru probehlo v poradku", 2);
    return true;
}

void CMainBox::exitMenu() const {
    cout << "Prejete si pred ukoncenim aplikace ulozit aktualni udalosti z kalendare?" << endl;
    cout << "Zadejte 1 pro ANO, 0 pro NE." << endl;
    switch (getNumber(0, 1)) {
        case 0:
            return;
        case 1:
            while(!saveEvents()) {
                printMsgAndSleep("Zadejte 1 pro opakovani akce, 0 pro ukonceni aplikace (data nebudou ulozena).", 1);
                switch (getNumber(0, 1)) {
                    case 0:
                        return;
                    case 1:
                        break;
                }
            }
    }
}

void CMainBox::loadEvents() {
    cout << endl << "Zadejte cestu k souboru ve formatu .json pro nacteni udalosti nebo zadejte nulu pro vytvoreni prazdneho kalendare." << endl;
    while (true) {
        string path = getLine();
        if (path == "0") {
            return;
        }
        try {
            CJSONReader reader(path);
            auto events = reader.readAllEvents();
            reader.close();
            for (auto & ev : events) {
                addEvent(ev);
            }
            return;
        } catch (const FileException & e) {
            cout << "Pri nacitani doslo k chybe. Zkontrolujte, zda dany soubor existuje a je ve spravnem formatu."
                    " Zadejte cestu k souboru znovu, popr. zadejte nulu pro vytvoreni prazdneho kalendare." << endl;
        }
    }
}

set<size_t> CMainBox::searchEventsByName() const {
    set<size_t> res;
    cout << "Zadej hledany nazev udalosti:" << endl;
    string name = getLine(CEvent::c_MaxLengthName);
    for (auto [begin, end] = m_EventsSortedByName.equal_range(name); begin != end; begin++) {
        res.insert(begin->second);
    }
    return res;
}

set<size_t> CMainBox::searchEventsByPlace() const {
    set<size_t> res;
    cout << "Zadej hledane misto udalosti:" << endl;
    string place = getLine(CEvent::c_MaxLengthPlace);
    for (auto [begin, end] = m_EventsSortedByPlace.equal_range(place); begin != end; begin++) {
        res.insert(begin->second);
    }
    return res;
}

void CMainBox::displaySearchResults(const set<size_t> & results) {
    if (results.empty()) {
        printMsgAndSleep("V kalendari neexistuji zadne udalosti vyhovujici vasim pozadavkum.", 2);
        return;
    }
    unordered_map<size_t, size_t> idxToId;
    size_t idx = 0;
    cout << endl << setw(c_SearchDelimBarWidth) << setfill('-') << "" << endl;
    for (auto id: results) {
        m_EventsMainDatabase.at(id).searchViewPrint(idx);
        idxToId[idx++] = id;
        cout << endl;
    }
    cout << setw(c_SearchDelimBarWidth) << setfill('-') << "" << endl;
    cout << endl << "Zadejte jednu z moznosti" << endl;
    cout << "0)Zpet do hl. menu\t1)Vybrat udalost\t2)Odfiltrovat podle nazvu udalosti\t3)Odfiltrovat podle mista udalosti" << endl;
    int choice = getNumber(0, 3);
    if (choice == 0) {
        return;
    } else if (choice == 1) {
        cout << "Vyberte pomoci indexu udalosti." << endl;
        size_t id = idxToId.at(getNumber(0, idx - 1));
        m_EventsMainDatabase.at(id).viewDetails();
        eventDetailsMenu(id);
        return;
    }
    set<size_t> filtered;
    if (choice == 2) {
        filtered = searchEventsByName();
    }
    else if (choice == 3) {
        filtered = searchEventsByPlace();
    }
    set<size_t> common;
    set_intersection(filtered.begin(), filtered.end(), results.begin(), results.end(),
                     inserter(common, common.begin()));
    displaySearchResults(common);
}

void CMainBox::editEvent(size_t id) {
    CEvent::printEditMenu();
    CEvent & event = m_EventsMainDatabase.at(id);
    int choice = getNumber(1,9);
    if (choice == 4) {
        event.addParticipant();
        return;
    } else if (choice == 5) {
        event.deleteParticipant();
        return;
    } else if (choice == 6) {
        event.editDetails();
        return;
    } else if (choice == 8) {
        event.changeMovable();
        return;
    } else if (choice == 9) {
        moveEventToEarliestTimePossible(id);
        return;
    }
    const char * parameter;
    if (choice == 1) {
        parameter = "NAME";
    } else if (choice == 2) {
        if (!event.isMovable()) {
            printMsgAndSleep("Nelze zmenit cas, udalost neni presunutelna.", 2);
            return;
        }
        parameter = "TIME";
    } else if (choice == 3) {
        parameter = "PLACE";
    } else if (choice == 7) {
        parameter = "REPEAT";
    }
    auto temp = event.makeEventWithNewParameter(parameter);
    addEvent(temp);
    deleteEvent(id);
}

void CMainBox::eventDetailsMenu(size_t id) {
    CEvent::printMenu();
    switch (getNumber(0, 2)) {
        case 0:
            return;
        case 1:
            editEvent(id);
            return;
        case 2:
            deleteEvent(id);
    }
}

void CMainBox::searchEvent() {
    cout << "Zadejte kriteria podle kterych chcete vyledavat." << endl;
    cout << "1)Nazev udalosti\t2)Misto udalosti" << endl;
    switch (getNumber(1, 2)) {
        case 1:
            displaySearchResults(searchEventsByName());
            return;
        case 2:
            displaySearchResults(searchEventsByPlace());
    }
}
/**
 *
 * @param id id of event to be moved
 */
void CMainBox::moveEventToEarliestTimePossible(size_t id) {
    const CEvent & event = m_EventsMainDatabase.at(id);
    if (!event.isMovable()) {
        printMsgAndSleep("Vybrana udalost neni presunutelna.", 2);
        return;
    }
    if (event.isRepeatable()) {
        printMsgAndSleep("Pro udalost, ktera se pravidelne opakuje, nelze nalezt nejblizsi mozny termin. Tato akce nedava smysl.", 3);
        return;
    }
    cout << endl << "Kalendar najde nejblizsi termin behem ktereho se nekona zadna udalost. V hledani se neuvazuje ani cela"
                      " dosavadni doba trvani udalosti, ktera po presunuti zustane volna." << endl;

    cout << "Zadej datum a cas (ve formatu DD/MM/YYYY hh:mm) odkdy nejdrive prichazi novy termin v uvahu:" << endl;
    STimeStamp lowLimit = getTime();
    set<STimePeriod> intervals = makeEventIntervals(lowLimit);
    try {
        auto temp = event.makeEventWithNewNearestDate(lowLimit, intervals);
        addEvent(temp);
        deleteEvent(id);
    } catch (const range_error & e) {
        printMsgAndSleep("Pro vybranou udalost nelze v nejblizsich dvou letech najit nahradni termin", 2);
    }
}

set<STimePeriod> CMainBox::makeEventIntervals(const STimeStamp & lowLimit) const {
    set<STimePeriod> res;
    for (const auto & [id, event] : m_EventsMainDatabase) {
        auto periods = event.getTimePeriods(lowLimit);
        res.insert(periods.begin(), periods.end());
    }
    // ading upper boundary, aka time stamp two years from lowLimit or maximal time stamp possible (31.12.2037 23:59)
    try {
        CDate dateUpperLimit = lowLimit.m_Date.getDateShiftedByDays(CRepeat::c_MaximumDaysToGenerateTimePeriods);
        res.emplace(dateUpperLimit, lowLimit.m_Clock, dateUpperLimit, lowLimit.m_Clock);
        return res;
    } catch (const InvalidDateException & e) {
        res.emplace(CDate(2037, 12, 31), CClock(23, 59), CDate(2037, 12, 31), CClock(23, 59));
        return res;
    }
}
