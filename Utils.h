#ifndef UTILS_H
#define UTILS_H

#include <cstddef>
#include <string>
#include <vector>

// ---------- Data files ----------
inline constexpr const char* ADMIN_FILE = "AdminAccount.txt";   // username  password
inline constexpr const char* STUDENT_LIST_FILE = "studentlist.txt"; // username  name
inline constexpr const char* USN_FILE = "UsnFile.txt";          // sorted roll-number index
// Each student also has <username>.txt: username name status day month year

// ---------- Console input ----------
int readInt(const std::string& prompt);          // returns -1 if the input is not a number
std::string readWord(const std::string& prompt); // reads one whitespace-separated word

// ---------- Files and sorting ----------
bool fileExists(const std::string& filename);
void insertionSortRecursive(std::vector<std::string>& arr, std::size_t n);
void sortUsnFile();

// ---------- Dates ----------
struct Date {
    int day;
    int month;
    int year;
};
Date today();

#endif
