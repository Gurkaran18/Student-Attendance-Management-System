#include "Student.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <utility>

Student::Student(std::string username, std::string name) : User(std::move(username)), name(std::move(name)) {}

bool Student::enroll() const {
    // The student's own log file existing means they are already enrolled
    if (fileExists(logFile())) {
        return false;
    }

    std::ofstream separateFile(logFile());
    Date d = today();
    separateFile << username << "\t" << name << "\t" << ENROLLED << "\t" << d.day << "\t" << d.month << "\t"
                 << d.year << "\n";

    std::ofstream commonFile(STUDENT_LIST_FILE, std::ios::app);
    commonFile << username << "\t" << name << "\n";

    std::ofstream usnFile(USN_FILE, std::ios::app);
    usnFile << username << "\n";
    return true;
}

void Student::recordAttendance(int status, const Date& date) const {
    std::ofstream f(logFile(), std::ios::app);
    f << username << "\t" << name << "\t" << status << "\t" << date.day << "\t" << date.month << "\t" << date.year
      << "\n";
}

std::vector<Student::Entry> Student::readLog() const {
    std::vector<Entry> entries;
    std::ifstream f(logFile());
    std::string fileUsername, fileName;
    Entry e{};
    while (f >> fileUsername >> fileName >> e.status >> e.date.day >> e.date.month >> e.date.year) {
        if (e.status != ENROLLED) {
            entries.push_back(e);
        }
    }
    return entries;
}

void Student::printTotals(const std::vector<Entry>& entries) {
    int present = 0;
    for (const Entry& e : entries) {
        if (e.status == PRESENT) {
            present++;
        }
    }
    int totalDays = static_cast<int>(entries.size());

    std::cout << "\tPresent:" << present << "\tAbsent:" << totalDays - present << "\tAttendance percentage:";
    if (totalDays == 0) { // newly enrolled: avoid dividing by zero
        std::cout << "0% (No attendance recorded yet)\n";
    } else {
        std::cout << std::fixed << std::setprecision(2) << 100.0 * present / totalDays << "%\n";
    }
}

void Student::printSummary() const {
    std::cout << "\n\n\tUniversity Number:" << username << "\tName: " << name << "\n";
    printTotals(readLog());
}

void Student::showMenu() {
    std::vector<Entry> entries = readLog();
    std::cout << "\n\tName:" << name << "\tUniversity Number:" << username << "\n";
    for (const Entry& e : entries) {
        std::cout << "\t" << e.date.day << "-" << e.date.month << "-" << e.date.year << "\t"
                  << (e.status == PRESENT ? "Present" : "Absent") << "\n";
    }
    std::cout << "\n";
    printTotals(entries);
    std::cout << "\n";
}

bool Student::find(const std::string& username, std::string& nameOut) {
    std::ifstream f(STUDENT_LIST_FILE);
    std::string fileUsername, fileName;
    while (f >> fileUsername >> fileName) {
        if (fileUsername == username) {
            nameOut = fileName;
            return true;
        }
    }
    return false;
}
