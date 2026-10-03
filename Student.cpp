#include "Student.h"

#include <fstream>
#include <iostream>
#include <utility>

namespace {

const char* const kRosterFile = "studentlist.txt";
const char* const kIndexFile = "UsnFile.txt";

} // namespace

// ---------------------------------------------------------------- Student --

Student::Student(std::string usn, std::string name)
    : name_(std::move(name)) {
    setIdentifier(std::move(usn));
}

bool Student::loadRecords() {
    records_.clear();
    hasHeader_ = false;

    std::ifstream file(recordFilename());
    if (!file.is_open()) {
        return false;
    }

    std::string fileUsn;
    std::string fileName;
    int status = 0;
    util::Date date;
    while (file >> fileUsn >> fileName >> status >> date.day >> date.month >> date.year) {
        if (!hasHeader_) {
            // Enrollment marker: it carries the authoritative name.
            name_ = fileName;
            hasHeader_ = true;
            continue;
        }
        records_.push_back(AttendanceRecord{status, date});
    }

    return true;
}

bool Student::appendAttendance(int status, const util::Date& date) {
    std::ofstream file(recordFilename(), std::ios::app);
    if (!file.is_open()) {
        return false;
    }
    file << usn() << "\t" << name_ << "\t" << status << "\t"
         << date.day << "\t" << date.month << "\t" << date.year << "\n";
    return true;
}

bool Student::createRecordFile(const util::Date& date) {
    std::ofstream file(recordFilename());
    if (!file.is_open()) {
        return false;
    }
    // Status 2 marks the enrollment row; it is never counted as attendance.
    file << usn() << "\t" << name_ << "\t" << 2 << "\t"
         << date.day << "\t" << date.month << "\t" << date.year << "\n";
    return true;
}

int Student::presentCount() const {
    int present = 0;
    for (const AttendanceRecord& record : records_) {
        if (record.status == 1) present++;
    }
    return present;
}

int Student::absentCount() const {
    int absent = 0;
    for (const AttendanceRecord& record : records_) {
        if (record.status == 0) absent++;
    }
    return absent;
}

double Student::attendancePercentage() const {
    const int total = markedDays();
    if (total <= 0) return 0.0;
    return (static_cast<double>(presentCount()) / total) * 100.0;
}

void Student::printLog() const {
    if (hasHeader_) {
        std::cout << "\n\tName:" << name_ << "\tUniversity Number:" << usn() << "\n";
    }

    for (const AttendanceRecord& record : records_) {
        if (record.status == 1) {
            std::cout << "\t" << record.date.day << "-" << record.date.month
                      << "-" << record.date.year << "\tPresent\n";
        } else if (record.status == 0) {
            std::cout << "\t" << record.date.day << "-" << record.date.month
                      << "-" << record.date.year << "\tAbsent\n";
        }
    }

    std::cout << "\n\n\tPresent:" << presentCount()
              << "\tAbsent:" << absentCount()
              << "\tAttendance percentage:" << attendancePercentage() << "%\n\n";
}

void Student::printSummary() const {
    if (hasHeader_) {
        std::cout << "\n\n\tUniversity Number:" << usn() << "\tName: " << name_ << "\n";
    }
    std::cout << "\tPresent:" << presentCount()
              << "\tAbsent:" << absentCount()
              << "\tAttendance percentage:" << attendancePercentage() << "%\n";
}

bool Student::login() {
    std::cout << "\n ------------------------ STUDENT MENU ------------------------\n";
    const std::string entered = util::readWord("\n\t\t\tEnter username: ");

    StudentRoster roster;
    std::string rosterName;
    if (!roster.find(entered, rosterName)) {
        std::cout << "\n\n\t\t\t\t\tNo student found !!!\n\n";
        return false;
    }

    setIdentifier(entered);
    name_ = rosterName;
    std::cout << "\n\t\t\tStudent Record Found !!!\n\n";

    if (!loadRecords()) {
        std::cerr << "\n\t\t[Error] Could not retrieve data from "
                  << usn() << ".txt\n";
        return false;
    }
    return true;
}

void Student::showMenu() {
    printLog();
}

// ---------------------------------------------------------- StudentRoster --

std::vector<std::pair<std::string, std::string>> StudentRoster::all() const {
    std::vector<std::pair<std::string, std::string>> students;

    std::ifstream file(kRosterFile);
    if (!file.is_open()) {
        std::cerr << "\n\t\t[Error] Missing studentlist.txt database!\n";
        return students;
    }

    std::string usn;
    std::string name;
    while (file >> usn >> name) {
        students.emplace_back(usn, name);
    }
    return students;
}

bool StudentRoster::find(const std::string& usn, std::string& nameOut) const {
    for (const auto& entry : all()) {
        if (entry.first == usn) {
            nameOut = entry.second;
            return true;
        }
    }
    return false;
}

bool StudentRoster::contains(const std::string& usn) const {
    std::string ignored;
    return find(usn, ignored);
}

std::vector<std::string> StudentRoster::sortedUsns() const {
    std::vector<std::string> usns;

    std::ifstream file(kIndexFile);
    if (!file.is_open()) {
        std::cerr << "\n\t\t[Error] Cannot read UsnFile.txt\n";
        return usns;
    }

    std::string usn;
    while (file >> usn) {
        usns.push_back(usn);
    }

    recursiveInsertionSort(usns);
    return usns;
}

bool StudentRoster::rewriteIndex(const std::vector<std::string>& usns) const {
    std::ofstream file(kIndexFile);
    if (!file.is_open()) {
        std::cerr << "\n\t\t[Error] Unable to update UsnFile.txt mapping.\n";
        return false;
    }
    for (const std::string& usn : usns) {
        file << usn << "\n";
    }
    return true;
}

bool StudentRoster::add(const std::string& usn, const std::string& name) {
    std::ofstream rosterFile(kRosterFile, std::ios::app);
    std::ofstream indexFile(kIndexFile, std::ios::app);

    if (!rosterFile.is_open() || !indexFile.is_open()) {
        std::cerr << "\n\t\t[Error] Failed to initialize student record files properly.\n";
        return false;
    }

    rosterFile << usn << "\t" << name << "\n";
    indexFile << usn << "\n";
    rosterFile.close();
    indexFile.close();

    // Re-sort the roll-number index after the append.
    return rewriteIndex(sortedUsns());
}

// --------------------------------------------------------- insertion sort --

void recursiveInsertionSort(std::vector<std::string>& items, std::size_t n) {
    if (n <= 1) {
        return;
    }

    // Sort everything before the last element first.
    recursiveInsertionSort(items, n - 1);

    // Then slide the nth element down into its sorted position.
    std::string key = std::move(items[n - 1]);
    std::size_t i = n - 1;
    while (i > 0 && items[i - 1] > key) {
        items[i] = std::move(items[i - 1]);
        i--;
    }
    items[i] = std::move(key);
}

void recursiveInsertionSort(std::vector<std::string>& items) {
    recursiveInsertionSort(items, items.size());
}
