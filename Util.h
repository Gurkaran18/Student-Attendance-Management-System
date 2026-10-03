#ifndef UTIL_H
#define UTIL_H

#include <string>

namespace util {

// Plain calendar date as it is stored in the attendance logs.
struct Date {
    int day = 0;
    int month = 0;
    int year = 0;
};

// Today's date from the system clock.
Date today();

// Console input helpers: they re-prompt until the input is usable.
int readInt(const std::string& prompt);
std::string readWord(const std::string& prompt);

bool fileExists(const std::string& filename);

} // namespace util

#endif
