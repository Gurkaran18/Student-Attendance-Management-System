#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include "Util.h"

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

// One line of a per-student attendance log.
struct AttendanceRecord {
    int status = 0;          // 1 = present, 0 = absent
    util::Date date;
};

// A student owns its USN (the inherited identifier), its name and its
// attendance records, and is the only class that reads or writes "<USN>.txt".
class Student : public User {
public:
    Student() = default;
    Student(std::string usn, std::string name);

    bool login() override;
    void showMenu() override;

    // Reads "<USN>.txt" into records_. The first row of the file is the
    // enrollment marker: it supplies the name and is not counted.
    // Returns false only when the file could not be opened; an empty file
    // loads successfully with no records. Callers report their own errors.
    bool loadRecords();

    // Appends one dated attendance row to this student's own log.
    bool appendAttendance(int status, const util::Date& date);

    // Creates "<USN>.txt" with the enrollment marker row (status 2).
    bool createRecordFile(const util::Date& date);

    const std::string& usn() const { return identifier(); }
    const std::string& name() const { return name_; }

    int presentCount() const;
    int absentCount() const;
    int markedDays() const { return static_cast<int>(records_.size()); }
    double attendancePercentage() const;

    void printLog() const;      // dated log followed by the summary
    void printSummary() const;  // header line plus present/absent/percentage

private:
    std::string recordFilename() const { return usn() + ".txt"; }

    std::string name_;
    std::vector<AttendanceRecord> records_;
    // True once the enrollment marker row has been read; the name header is
    // only printed when a row was actually present.
    bool hasHeader_ = false;
};

// Owns the two roster files: studentlist.txt (USN + name) and
// UsnFile.txt (the roll-number index, kept sorted).
class StudentRoster {
public:
    bool contains(const std::string& usn) const;
    bool find(const std::string& usn, std::string& nameOut) const;
    std::vector<std::pair<std::string, std::string>> all() const;
    std::vector<std::string> sortedUsns() const;

    // Appends the student to both roster files and re-sorts the index.
    bool add(const std::string& usn, const std::string& name);

private:
    bool rewriteIndex(const std::vector<std::string>& usns) const;
};

// Recursive insertion sort: sort the first n-1 elements, then insert the nth
// into its place. Ascending order over an STL vector.
void recursiveInsertionSort(std::vector<std::string>& items, std::size_t n);
void recursiveInsertionSort(std::vector<std::string>& items);

#endif
