#pragma once
#include <string>
#include "../time/timeStamp.h"

static const long DEFAULT_MAX_STRING_INPUT = 600;
/**
 *
 * @param min Minimum number that can be read (included)
 * @param max Maximum number that can be read (included)
 * @return Number from user input. Reads until the input is not in correct format.
 */
int getNumber(long min, long max);
/*
 * Returns a string from user input. Cannot exceed maximum size maxLength. Reads until the input is not in correct format.
 */
std::string getLine(long maxLength = DEFAULT_MAX_STRING_INPUT);
/** Returns STimeStamp from user input. Reads until the input is not in correct format. */
STimeStamp getTime();
/** Prints message to standard output and freezes the thread for a specific amount of time. */
void printMsgAndSleep(const std::string & msg, int64_t sec);


