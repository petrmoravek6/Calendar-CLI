#include <cstring>
#include <vector>
#include "jsonReader.h"
#include "fileException.h"
#include "../repeat/repeat.h"
#include "../repeat/repeatDaily.h"
#include "../repeat/repeatEveryWeekday.h"
#include "../repeat/repeatOnceAWeek.h"
#include "../repeat/repeatOnceTwoWeeks.h"

using namespace std;

CJSONReader::CJSONReader(const std::string &filename) {
    if(m_File.is_open())
        throw FileException("Error during opening the file");
    m_File.open(filename.c_str());
    if (!m_File)
        throw FileException("Error during opening the file");
}

void CJSONReader::close() {
    if (!m_File.is_open())
        throw FileException("Error during closing the file");
    m_File.clear();
    m_File.close();
    if (m_File.fail())
        throw FileException("Error during closing the file");
}

vector<CEvent> CJSONReader::readAllEvents() {
    char c;
    if (!m_File.get(c) || c != '[') {
        throw FileException("Error during reading leading character \"[\" - list of events");
    }
    if (!m_File.get(c)) {
        throw FileException("Error during reading leading character \"{\" or end of events character \"]\"");
    }
    vector<CEvent> res;
    if (c == '{') { // about to read an event
        if (!m_File.unget()) { // it's necessary to put back the character because it could be a part of an event leading character
            throw FileException("Error during putting back \"{\" character");
        }
        while (true) {
            res.push_back(readEvent());
            if (!m_File.get(c)) {
                throw FileException("Error during reading events delimiter character");
            }
            if (c == ']') {
                break;
            }
            else if (c != ',') { // not end ov list nor not about to read another event
                throw FileException("Error during reading events delimiter character - not a \",\" nor \"]\"");
            }
        }
    }
    else if (c != ']') { // not end of list nor an event
        throw FileException("Error during reading events delimiters");
    }
    // If there is no last character (EOF), we are done. If there is newline character and then EOF -> success too.
    // Else: there are more characters. That is bad input
    int last = m_File.get();
    if (last < 0 || (last == 10 && m_File.get() < 0)) {
        return res;
    }
    throw FileException("Not EOF");
}

CEvent CJSONReader::readEvent() {
    char c;
    if(!m_File.get(c) || c != '{') {
        throw FileException("Error during reading leading character \"{\"");
    }
    auto name = readEventName();
    auto startDate = readEventDate(",\"start_date\":{");
    auto startClock = readEventClock(",\"start_clock\":{");
    auto endDate = readEventDate(",\"end_date\":{");
    auto endClock = readEventClock(",\"end_clock\":{");
    if (tie(startDate, startClock) >= tie(endDate, endClock)) {
        throw FileException("Start of the event is not lesser than end");
    }
    auto place = readEventPlace();
    auto participants = readEventParticipants();
    auto details = readEventDetails();
    auto repeat = readEventRepeat();
    if (repeat
        && startDate.daysBetween(endDate) > repeat.value()->skippedDaysBetweenEvents()) {
        throw FileException("Repeating in less interval than the number of days of the event");
    }
    auto isMovable = readEventMoving();
    // ending '}' character doesn't have to be checked/read, because it is discarded in the readEventMoving() method
    return {name, startDate, startClock, endDate, endClock, place, participants, details, repeat, isMovable};
}

std::string CJSONReader::readEventName() {
    readTitle("\"name\":\"");
    string name;
    getline(m_File, name, '\"');
    if (!m_File || name.empty()) {
        throw FileException("Error during reading the event name");
    }
    return name;
}

CDate CJSONReader::readEventDate(const char * title) {
    readTitle(title);
    readTitle("\"year\":");
    int year = readInteger(',');
    readTitle("\"month\":");
    int month = readInteger(',');
    readTitle("\"day\":");
    int day = readInteger('}');
    try {
        return {year, month, day};
    } catch (const InvalidDateException & e) {
        throw FileException("Wrong date format");
    }
}

CClock CJSONReader::readEventClock(const char * title) {
    readTitle(title);
    readTitle("\"hour\":");
    int hour = readInteger(',');
    readTitle("\"minute\":");
    int minute = readInteger('}');
    try {
        return {hour, minute};
    } catch (const InvalidClockException & e) {
        throw FileException("Wrong clock format");
    }
}

optional<string> CJSONReader::readEventPlace() {
    readTitle(",\"place\":");
    return readOptionalString();
}

set<string> CJSONReader::readEventParticipants() {
    readTitle(",\"participants\":");
    char c;
    if(!m_File.get(c) || c != '[') {
        throw FileException("Error during reading leading participants character \"[\"");
    }
    if (!m_File.get(c)) {
        throw FileException("Error during reading leading specific participant character \" \" or ]\" ");
    }
    set<string> res;
    if (c == '\"') {
        string person;
        while(true) {
            if (!getline(m_File, person, '\"') || person.empty()) {
                throw FileException("Error during reading a participant of event");
            }
            res.insert(person);
            // read next character If it's ',' than check whether the next character is '"' (another person ia about to be read).
            // If it's ']' we have reached an end of list of participants.
            if (!m_File.get(c)) {
                throw FileException("Error during reading delimiter participants character \"[ or ,\"");
            }
            if (c == ',') {
                if (!m_File.get(c) || c != '"') {
                    throw FileException("Error during reading next leading participants character \"");
                }
            }
            else if (c == ']') {
                break;
            }
            else
                throw FileException("Error during reading delimiter participants character, wrong format");
        }
    } else if (c != ']') {
        throw FileException("Error during reading participants of event, wrong format");
    }
    return res;
}

optional<string> CJSONReader::readEventDetails() {
    readTitle(",\"details\":");
    return readOptionalString();
}

optional<unique_ptr<CRepeat>> CJSONReader::readEventRepeat() {
    readTitle(",\"repeat\":");
    auto repeat = readOptionalString();
    if (!repeat) {
        return nullopt;
    }
    else if (repeat.value() == "DAILY") {
        return {make_unique<CRepeatDaily>()};
    }
    else if (repeat.value() == "EVERY_WEEKDAY") {
        return {make_unique<CRepeatEveryWeekday>()};
    }
    else if (repeat.value() == "ONCE_A_WEEK") {
        return {make_unique<CRepeatOnceAWeek>()};
    }
    else if (repeat.value() == "ONCE_TWO_WEEKS") {
        return {make_unique<CRepeatOnceTwoWeeks>()};
    }
    else {
        throw FileException("Unknown event repeat");
    }
}

bool CJSONReader::readEventMoving() {
    readTitle(",\"movable\":");
    string value;
    if (!getline(m_File, value, '}')) {
        throw FileException("Error during reading a participant of event");
    }
    if (value == "true") {
        return true;
    }
    else if (value == "false") {
        return false;
    }
    else {
        throw FileException("Error during reading isMovable boolean value, wrong format");
    }
}

int CJSONReader::readInteger(char delimiter) {
    string str;
    if (!getline(m_File, str, delimiter)) {
        throw FileException("Error during reading an integer, bad file");
    }
    try {
        int res = stoi(str);
        return res;
    }
    catch (...) {
        throw FileException("Error during reading an integer, wrong int format");
    }
}

void CJSONReader::readTitle(const char * toBeRead) {
    size_t toBeReadSize = strlen(toBeRead);
    unique_ptr<char[]> title(new char[toBeReadSize + 1]);
    if (!m_File.read(title.get(), toBeReadSize)) {
        throw FileException("Error during reading the file");
    }
    title.get()[toBeReadSize] = '\0';
    if (strcmp(toBeRead, title.get()) != 0) {
        throw FileException("Wrong title");
    }
}

std::optional<std::string> CJSONReader::readOptionalString() {
    char c;
    if(!m_File.get(c)) {
        throw FileException("Error during reading leading optional character");
    }
    if (c == '\"') {
        string value;
        // value cannot be an empty string
        if (!getline(m_File, value, '\"') || value.empty()) {
            throw FileException("Error during reading place of event");
        }
        return {value};
    } else if (c == 'n') {
        readTitle("ull");
        return nullopt;
    }
    else {
        throw FileException("Error during reading optional, wrong leading character");
    }
}
