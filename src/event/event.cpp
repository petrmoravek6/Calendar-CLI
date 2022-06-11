#include "event.h"
#include "../repeat/repeat.h"
#include "../repeat/repeatDaily.h"
#include "../repeat/repeatEveryWeekday.h"
#include "../repeat/repeatOnceAWeek.h"
#include "../repeat/repeatOnceTwoWeeks.h"
#include "../util/util.h"
#include "eventDayLayout.h"
#include "../main_box/mainBox.h"
#include <iomanip>
#include <iostream>
#include <memory>
#include <cstring>
#include <tuple>
#include <optional>

using namespace std;

CEvent::CEvent(string name, const STimePeriod & timePeriod, const optional<string> & place,
               const set<string> & participants, const optional<string> & details,
               optional<unique_ptr<CRepeat>> & repeat, bool movable)
        : m_Name(move(name)), m_StartDate(timePeriod.m_Start.m_Date), m_StartClock(timePeriod.m_Start.m_Clock),
          m_EndDate(timePeriod.m_End.m_Date), m_EndClock(timePeriod.m_End.m_Clock), m_Place(place),
          m_Participants(participants), m_Details(details), m_Repeat(move(repeat)), m_IsMovable(movable) {
}

CEvent::CEvent(string name, const CDate & startDate, const CClock & startClock, const CDate & endDate,
               const CClock & endClock, const optional<string> & place,
               const set<string> & participants, const optional<string> & details,
               optional<unique_ptr<CRepeat>> & repeat, bool movable)
        : m_Name(move(name)), m_StartDate(startDate), m_StartClock(startClock),
          m_EndDate(endDate), m_EndClock(endClock), m_Place(place),
          m_Participants(participants), m_Details(details), m_Repeat(move(repeat)), m_IsMovable(movable) {
}

CEvent::CEvent(CEvent &&other) noexcept
        : m_Name(move(other.m_Name)), m_StartDate(other.m_StartDate), m_StartClock(other.m_StartClock), m_EndDate(other.m_EndDate),
          m_EndClock(other.m_EndClock), m_Place(move(other.m_Place)), m_Participants(move(other.m_Participants)), m_Details(move(other.m_Details)),
          m_Repeat(move(other.m_Repeat)), m_IsMovable(other.m_IsMovable) {
}

bool CEvent::includeInView(const CDate & repEvBegin, const CDate & dayInCalendar) const {
    if (m_Repeat.has_value()) {
        return m_Repeat.value()->includeInView(repEvBegin, dayInCalendar);
    }
    return false;
}

void CEvent::monthViewPrint(size_t idx) const {
    cout << '|';
    cout << setw(2) << setfill('0') << idx << ") ";
    if (m_Name.length() > c_FixLengthMonthView) {
        cout << m_Name.substr(0, c_FixLengthMonthView - 2) + "..";
    }
    else {
        cout << left << setw(c_FixLengthMonthView) << setfill(' ') << m_Name << right;
    }
    cout << '|';
}

void CEvent::weekViewPrint(size_t idx) const {
    cout << setw(2) << setfill('0') << idx << ") ";
    cout << m_Name;
}

void CEvent::dayViewPrint(size_t idx) const {
    cout << setw(2) << setfill('0') << idx << ") ";
    cout << m_Name;
    if (m_Place.has_value()) {
        cout << " (" << m_Place.value() << ')';
    }
}

void CEvent::searchViewPrint(size_t idx) const {
    cout << setw(3) << setfill('0') << idx << ")  ";
    cout << "Nazev: " << m_Name;
    cout << "\tMisto: " << m_Place.value_or("-zadne-");
    cout << "\tCas: " << m_StartDate << ' ' << m_StartClock << " - " << m_EndDate << ' ' << m_EndClock;
}

void CEvent::viewDetails() const {
    cout << endl << setw(c_DelimBarWidth) << setfill('-') << "" << endl;
    cout << "UDALOST" << endl;
    cout << "Nazev:\t\t\t" << m_Name << endl;
    cout << "Od:\t\t\t" << m_StartDate << ' ' << m_StartClock << endl;
    cout << "Do:\t\t\t" << m_EndDate << ' ' << m_EndClock << endl;
    if (m_Place.has_value()) {
        cout << "Misto:\t\t\t" << m_Place.value() << endl;
    }
    if (!m_Participants.empty()) {
        cout << "Ucastnici:\t\t";
        size_t i = 0;
        for (const auto & name : m_Participants) {
            cout << name;
            if (i != m_Participants.size() - 1) {
                cout << ", ";
            }
            i++;
        }
        cout << endl;
    }
    if (m_Details.has_value()) {
        cout << "Detaily:\t\t" << m_Details.value() << endl;
    }
    if (m_Repeat.has_value()) {
        cout << "Opakovani udalosti:\t" << *(m_Repeat.value()) << endl;
    }
    if (m_IsMovable) {
        cout << "Udalost je presunutelna" << endl;
    }
    else {
        cout << "Udalost neni presunutelna" << endl;
    }
    cout << setw(c_DelimBarWidth) << setfill('-') << "" << endl;
}

void CEvent::printMenu() {
    cout << endl;
    cout << "0) Odejit\t1)Upravit\t2)Smazat" << endl;
    cout << "Zadejte jednu z moznosti a potvrdte enterem" << endl;
}

SEventDayLayout CEvent::getLowerBoundLayout() const {
    // 3. - 4. parameters are dummy, they won't be used in binary searching
    return {m_StartDate, m_StartClock, m_StartClock, 0};
}

SEventDayLayout CEvent::getUpperBoundLayout() const {
    // 3. - 4. parameters are dummy
    return {m_EndDate, m_StartClock, m_EndClock, 0};
}

bool CEvent::isRepeatable() const {
    return m_Repeat.has_value();
}

string CEvent::getName() const {
    return m_Name;
}

string CEvent::getPlace() const {
    if (m_Place.has_value()) {
        return m_Place.value();
    }
    throw runtime_error("the event has no venue");
}

bool CEvent::hasPlace() const {
    return m_Place.has_value();
}

set<SEventDayLayout> CEvent::generateDayLayouts(size_t id) const {
    set<SEventDayLayout> res;
    if (m_StartDate == m_EndDate) {
        res.emplace(m_StartDate, m_StartClock, m_EndClock, id);
    }
    else {
        res.emplace(m_StartDate, m_StartClock, CClock(23, 59), id);
        CDate curr = m_StartDate.getDateShiftedByDays(1);
        while (curr != m_EndDate) {
            res.emplace(curr, CClock(0, 0), CClock(23, 59), id);
            curr = curr.getDateShiftedByDays(1);
        }
        res.emplace(m_EndDate, CClock(0, 0), m_EndClock, id);
    }
    return res;
}

string CEvent::toJSON() const {
    // name
    string res("{");
    res += "\"name\":\"" + m_Name + '\"';
    // time
    res += ",\"start_date\":" + m_StartDate.toJSON();
    res += ",\"start_clock\":" + m_StartClock.toJSON();
    res += ",\"end_date\":" + m_EndDate.toJSON();
    res += ",\"end_clock\":" + m_EndClock.toJSON();
    // place
    res += ",\"place\":";
    if (m_Place.has_value())
        res += '\"' + m_Place.value() + '\"';
    else
        res += "null";
    //participants
    res += ",\"participants\":[";
    auto it = m_Participants.begin();
    for (size_t i = 0; i < m_Participants.size(); i++) {
        res += '\"' + *it + '\"';
        if (i != m_Participants.size() - 1)
            res += ',';
        ++it;
    }
    // details
    res += "],\"details\":";
    if (m_Details.has_value())
        res += '\"' + m_Details.value() + '\"';
    else
        res += "null";
    // repeat
    res += ",\"repeat\":";
    if (m_Repeat.has_value())
        res += m_Repeat.value()->toJSON();
    else
        res += "null";
    // isMovable
    res += ",\"movable\":";
    ostringstream converter;
    converter << boolalpha << m_IsMovable;
    res += converter.str() + "}";
    return res;
}

void CEvent::printEditMenu() {
    cout << endl;
    cout << "Zadejte jednu z moznosti:" << endl;
    cout << "1) Upravit nazev" << "\t2) Zmenit cas" << "\t3) Zmenit misto" << endl;
    cout << "4) Pridat ucastniky" << "\t5) Odebrat ucastniky" << "\t6) Zmenit detail udalosti" << endl;
    cout << "7) Upravit opakovani udalosti" << "\t8) Zmenit presunutelnost udalosti" << "\t9) Presunout na nejblizsi volny termin" << endl;
}

string CEvent::createNameForEvent() {
    cout << endl << "Zadejte nazev udalosti:" << endl;
    string res;
    while (true) {
        res = getLine(CEvent::c_MaxLengthName);
        if (res.empty()) {
            printMsgAndSleep("Jmeno nemuze byt prazdne. Zadejte znovu.", 2);
        }
        else {
            return res;
        }
    }
}
STimePeriod CEvent::createTimeForEvent() {
    cout << endl;
    while (true) {
        cout << "Zadejte zacatek udalosti ve formatu: DD/MM/YYYY hh:mm" << endl;
        auto beginTime = getTime();
        cout << "Zadejte konec udalosti ve formatu: DD/MM/YYYY hh:mm" << endl;
        auto endTime = getTime();
        if (!(beginTime < endTime)) {
            printMsgAndSleep("Zacatek udalosti musi predchazet konci. Zadejte casy znovu.", 2);
        }
        else {
            return {beginTime, endTime};
        }
    }
}
optional<string> CEvent::createPlaceForEvent() {
    cout << endl << "Zadejte misto udalosti (pro vynechani pouze odentrujte)" << endl;
    string placeName = getLine(CEvent::c_MaxLengthPlace);
    if (placeName.empty()) {
        return nullopt;
    }
    return {placeName};
}
set<string> CEvent::createParticipantsForEvent() {
    cout << endl << "Zadejte ucastniky udalosti. Pro ukonceni zadavani ucastniku odentrujte prazdny vstup." << endl;
    set<string> participants;
    string person;
    while (true) {
        person = getLine(CEvent::c_MaxLengthNameParticipant);
        if (person.empty()) {
            break;
        }
        participants.insert(person);
    }
    return participants;
}
optional<string> CEvent::createDetailsForEvent() {
    cout << endl << "Zadejte detaily udalosti (pro vynechani pouze odentrujte)" << endl;
    string detailsStr = getLine(CEvent::c_MaxLengthDetails);
    if (detailsStr.empty()) {
        return nullopt;
    }
    return {detailsStr};
}
optional<unique_ptr<CRepeat>> CEvent::createRepeatForEvent() {
    cout << endl << "Zadejte opakovani udalosti:" << endl;
    cout << "0)Zadne  1)Denne  2)Kazdy vsedni den  3)Jednou tydne  4)Jednou za 14 dni" << endl;
    switch (getNumber(0, 4)) {
        case 1:
            return {make_unique<CRepeatDaily>()};
        case 2:
            return {make_unique<CRepeatEveryWeekday>()};
        case 3:
            return {make_unique<CRepeatOnceAWeek>()};
        case 4:
            return {make_unique<CRepeatOnceTwoWeeks>()};
        default:
            return nullopt;
    }
}

bool CEvent::createMovingForEvent() {
    cout << endl << "Chcete, aby byla udalost presunutelna? Zadejte 1 pro ANO, 0 pro NE." << endl;
    return getNumber(0, 1) == 1;
}

CEvent CEvent::makeEventWithNewParameter(const char * parameter) const {
    optional<unique_ptr<CRepeat>> repeat;
    if (m_Repeat.has_value()) {
        repeat = {m_Repeat.value()->clone()};
    }

    if (strcmp(parameter, "NAME") == 0) {
        return {createNameForEvent(), m_StartDate, m_StartClock, m_EndDate, m_EndClock, m_Place, m_Participants, m_Details,
                repeat, m_IsMovable};
    } else if (strcmp(parameter, "TIME") == 0) {
        while (true) {
            auto time = createTimeForEvent();
            if (repeat
                && time.m_Start.m_Date.daysBetween(time.m_End.m_Date) > repeat.value()->skippedDaysBetweenEvents()) {
                printMsgAndSleep("Chyba vytvoreni udalosti. Nelze vytvorit udalost, ktera se opakuje v mensich"
                                 " intervalech nez je pocet dnu, ktere zabira. "
                                 "Mohlo by dojit k prekryvani udalosti, ktere v tomto pripade logicky nedava smysl."
                                 "Zadejte cas znovu.", 4);
                continue;
            }
            return {m_Name, time, m_Place, m_Participants, m_Details, repeat, m_IsMovable};
        }
    } else if (strcmp(parameter, "PLACE") == 0) {
        return {m_Name, m_StartDate, m_StartClock, m_EndDate, m_EndClock, createPlaceForEvent(), m_Participants, m_Details,
                repeat, m_IsMovable};
    } else { // "REPEAT"
        while (true) {
            repeat = createRepeatForEvent();
            if (repeat
                && m_StartDate.daysBetween(m_EndDate) > repeat.value()->skippedDaysBetweenEvents()) {
                printMsgAndSleep("Chyba vytvoreni udalosti. Nelze vytvorit udalost, ktera se opakuje v mensich"
                                 " intervalech nez je pocet dnu, ktere zabira. "
                                 "Mohlo by dojit k prekryvani udalosti, ktere v tomto pripade logicky nedava smysl."
                                 "Zmente opakovani udalosti.", 4);
                continue;
            }
            return {m_Name, m_StartDate, m_StartClock, m_EndDate, m_EndClock, m_Place, m_Participants, m_Details,
                    repeat, m_IsMovable};
        }
    }
}

void CEvent::deleteParticipant() {
    cout << endl << "Zadejte jmeno ucastnika:" << endl;
    auto participant = getLine(c_MaxLengthNameParticipant);
    if (m_Participants.erase(participant) == 1) {
        printMsgAndSleep("Ucastnik byl uspesne odebran.", 2);
    }
    else {
        printMsgAndSleep("Zadany ucastnik nebyl soucasti udalosti.", 2);
    }
}

void CEvent::addParticipant() {
    cout << endl << "Zadejte jmeno ucastnika:" << endl;
    auto participant = getLine(c_MaxLengthNameParticipant);
    if (m_Participants.count(participant) == 1) {
        printMsgAndSleep("Zadany ucastnik je jiz soucasti udalosti.", 2);
    }
    else {
        m_Participants.insert(participant);
        printMsgAndSleep("Ucastnik byl uspesne pridan.", 2);
    }
}

void CEvent::editDetails() {
    m_Details = createDetailsForEvent();
    printMsgAndSleep("Zmena detailu udalosti byla uspesne provedena.", 2);
}

void CEvent::changeMovable() {
    m_IsMovable = createMovingForEvent();
    printMsgAndSleep("Zmena presunutelnosti udalosti byla usepsne provedena.", 2);
}

bool CEvent::isMovable() const {
    return m_IsMovable;
}

bool CEvent::takesPlaceAfter(const STimeStamp & time) const {
    return tie(m_EndDate, m_EndClock) > tie(time.m_Date, time.m_Clock);
}

vector<STimePeriod> CEvent::getTimePeriods(const STimeStamp & lowLimit) const {
    vector<STimePeriod> res;
    STimePeriod periodOfThisEvent(m_StartDate, m_StartClock, m_EndDate, m_EndClock);
    if (this->takesPlaceAfter(lowLimit)) {
        res.push_back(periodOfThisEvent);
    }
    if (m_Repeat.has_value()) {
        auto periodsFromRepeat = m_Repeat.value()->generatePeriodsOfEventSinceTime(periodOfThisEvent, lowLimit);
        res.insert(res.end(), periodsFromRepeat.begin(), periodsFromRepeat.end());
    }
    return res;
}

// I was inspired by solution at: https://cs.stackexchange.com/questions/133276/algorithm-to-compute-the-gaps-between-a-set-of-intervals
CEvent CEvent::makeEventWithNewNearestDate(const STimeStamp & lowLimit, const set<STimePeriod> & intervals) const {
    STimeStamp lastCoveredPoint = lowLimit;
    for (const auto & interval : intervals) {
        // we have a gap
        if (interval.m_Start > lastCoveredPoint) {
            // we have a gap big enough to put the event there
            int durationOfThisEvent = this->durationInMinutes();
            if (lastCoveredPoint.minutesBetween(interval.m_Start) >= durationOfThisEvent) {
                auto end = lastCoveredPoint.getTimeStampShiftedByMinutes(durationOfThisEvent);
                optional<unique_ptr<CRepeat>> newRepeat;
                if (m_Repeat.has_value()) {
                    newRepeat = {m_Repeat.value()->clone()};
                }
                return {m_Name, lastCoveredPoint.m_Date, lastCoveredPoint.m_Clock, end.m_Date, end.m_Clock, m_Place,
                        m_Participants, m_Details, newRepeat, m_IsMovable};
            }
        }
        lastCoveredPoint = max(lastCoveredPoint, interval.m_End);
    }
    throw range_error("Max year reached or couldn't find gap for event");
}

int CEvent::durationInMinutes() const {
    struct tm start = {0, m_StartClock.m_Minute, m_StartClock.m_Hour, m_StartDate.m_Day, m_StartDate.m_Month - 1, m_StartDate.m_Year - 1900};
    struct tm end = {0, m_EndClock.m_Minute, m_EndClock.m_Hour, m_EndDate.m_Day, m_EndDate.m_Month - 1, m_EndDate.m_Year - 1900};
    start.tm_isdst = -1;
    end.tm_isdst = -1;
    return static_cast<int>(difftime(mktime(&end), mktime(&start)) / 60);
}
