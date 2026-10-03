#include "Util.h"

#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>

namespace util {

Date today() {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    Date date;
    if (local != nullptr) {
        date.day = local->tm_mday;
        date.month = local->tm_mon + 1;
        date.year = local->tm_year + 1900;
    }
    return date;
}

int readInt(const std::string& prompt) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        if (std::cin.eof()) {
            return 0;
        }
        std::cout << "\n\t\t[Error] Invalid input. Please enter a valid integer.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string readWord(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::cin >> value;
    return value;
}

bool fileExists(const std::string& filename) {
    std::ifstream file(filename.c_str());
    return file.good();
}

} // namespace util
