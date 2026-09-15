#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include "Utils.h"

#include <string>
#include <vector>

class Student : public User {
public:
    // Status codes stored in <username>.txt
    static constexpr int ABSENT = 0;
    static constexpr int PRESENT = 1;
    static constexpr int ENROLLED = 2; // first line, written at enrollment; not a school day

    Student(std::string username, std::string name);

    void showMenu() override; // shows this student's date-wise history and percentage
    std::string role() const override { return "Student"; }

    const std::string& getName() const { return name; }

    bool enroll() const; // creates the student's records; false if already enrolled
    void recordAttendance(int status, const Date& date) const;
    void printSummary() const; // one entry in the admin's attendance list

    // Looks the username up in studentlist.txt and fills in the name if found.
    static bool find(const std::string& username, std::string& nameOut);

private:
    struct Entry {
        int status;
        Date date;
    };

    std::string name;

    std::string logFile() const { return username + ".txt"; }
    std::vector<Entry> readLog() const;
    static void printTotals(const std::vector<Entry>& entries);
};

#endif
