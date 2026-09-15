#include "Utils.h"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>

int readInt(const std::string& prompt) {
    std::cout << prompt;
    int value;
    if (std::cin >> value) {
        return value;
    }
    if (std::cin.eof()) {
        std::exit(0); // input closed (Ctrl+D): nothing more to read
    }
    // Not a number: reset the stream's error flag and throw away the bad line,
    // otherwise every later read fails immediately and the menu loops forever.
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return -1;
}

std::string readWord(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    if (!(std::cin >> value)) {
        std::exit(0); // input closed (Ctrl+D)
    }
    return value;
}

bool fileExists(const std::string& filename) {
    std::ifstream f(filename);
    return f.good();
}

// Recursive insertion sort: sort the first n-1 elements, then insert element n-1 into place.
void insertionSortRecursive(std::vector<std::string>& arr, std::size_t n) {
    if (n <= 1) {
        return;
    }
    insertionSortRecursive(arr, n - 1);

    std::string last = arr[n - 1];
    std::size_t j = n - 1;
    // shift larger elements one position right to open a slot for 'last'
    while (j > 0 && arr[j - 1] > last) {
        arr[j] = arr[j - 1];
        j--;
    }
    arr[j] = last;
}

// Reads the roll-number index, sorts it, and rewrites the file.
void sortUsnFile() {
    std::vector<std::string> usns;
    std::ifstream in(USN_FILE);
    std::string usn;
    while (in >> usn) {
        usns.push_back(usn);
    }
    in.close();

    insertionSortRecursive(usns, usns.size());

    std::ofstream out(USN_FILE, std::ios::out | std::ios::trunc); // trunc: replace old content
    for (const std::string& u : usns) {
        out << u << "\n";
    }
}

Date today() {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    return {local->tm_mday, local->tm_mon + 1, local->tm_year + 1900};
}
