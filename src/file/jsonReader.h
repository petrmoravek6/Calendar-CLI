#pragma once
#include "../event/event.h"
#include "../time/date.h"
#include <vector>
#include <stdexcept>
#include <fstream>
#include <optional>

/** Class used for reading CEvent objects from .json file */
class CJSONReader {
public:
    /** Throws FileException if an error during opening the file occurs **/
    CJSONReader(const std::string & filename);
    /** Reads all CEvent objects from "filename" path and stores them into vector which it returns */
    std::vector<CEvent> readAllEvents();
    /** Throws FileException if an error during closing the file occurs **/
    void close();
private:
    std::ifstream m_File;

    /** Reads single event from file */
    CEvent readEvent();
    std::string readEventName();
    CDate readEventDate(const char * title);
    CClock readEventClock(const char * title);
    std::optional<std::string> readOptionalString();
    std::optional<std::string> readEventPlace();
    std::optional<std::string> readEventDetails();
    std::set<std::string> readEventParticipants();
    std::optional<std::unique_ptr<CRepeat>> readEventRepeat();
    bool readEventMoving();
    int readInteger(char delimiter);
    void readTitle(const char * toBeRead);
};


