#pragma once
#include <stdexcept>

class InvalidClockException : public std::invalid_argument {
public:
    InvalidClockException ()
            : invalid_argument ("invalid time or format") {
    }
};

/** Class representing hours and minutes */
class CClock {
public:
    /** Constructor throws an InvalidClockExcepiton if the given parameters "h" and "m" are out of time range */
    CClock(int h, int m);
    /**
     * @return std::string representing the object in JSON format
     */
    std::string toJSON() const;
    friend std::ostream& operator << (std::ostream & os, const CClock & clock);
    bool operator < (const CClock & b) const;
    bool operator == (const CClock & b) const;
    bool operator != (const CClock & b) const;
    bool operator > (const CClock & b) const;
    int m_Hour;
    int m_Minute;
private:
    static bool isValidClock(int hour, int minute);
};


