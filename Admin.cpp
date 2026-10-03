#include "Admin.h"

#include "Util.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace {

const char* const kAdminFile = "AdminAccount.txt";
const char* const kSensorFile = "rfid_sensor_logs.txt";
const char* const kErrorLogFile = "error_log.txt";

} // namespace

void Admin::initializeAccountStore() const {
    if (util::fileExists(kAdminFile)) {
        return;
    }

    std::ofstream file(kAdminFile);
    if (!file.is_open()) {
        std::cerr << "\n\t\t[Error] Failed to initialize Admin database.\n";
        return;
    }
    file << "admin\t" << hashPassword("admin") << "\n";
}

bool Admin::authenticate(const std::string& username, const std::string& password) {
    std::ifstream file(kAdminFile);
    if (!file.is_open()) {
        std::cerr << "\n\t\t[Error] Unable to access Admin database.\n";
        return false;
    }

    std::string storedUsername;
    std::size_t storedHash = 0;

    while (file >> storedUsername >> storedHash) {
        if (storedUsername != username) {
            continue;
        }
        // Adopt the stored credentials, then let the base class compare.
        setCredentials(storedUsername, storedHash);
        if (verifyPassword(password)) {
            return true;
        }
    }
    return false;
}

bool Admin::login() {
    initializeAccountStore();
    std::cout << "\n\n------------------------ ADMIN LOGIN ------------------------\n";

    const std::string username = util::readWord("\n\t\t\tEnter username: ");
    const std::string password = util::readWord("\n\t\t\tEnter password: ");

    if (authenticate(username, password)) {
        std::cout << "\n\t\t\tLogin successful!!!\n";
        return true;
    }

    std::cout << "\n\t\tError! Invalid Credentials. Please Try Again\n";
    return false;
}

void Admin::showMenu() {
    while (true) {
        std::cout << "\n ------------------------ ADMIN MENU ------------------------\n";
        std::cout << "\n\t\t\t1.Mark attendance"
                  << "\n\t\t\t2.Add students"
                  << "\n\t\t\t3.Create new admin account"
                  << "\n\t\t\t4.Student attendance list"
                  << "\n\t\t\t5.Batch Sensor Import"
                  << "\n\t\t\t6.Main menu"
                  << "\n\t\t\t0.Exit\n";

        const int choice = util::readInt("\n\t\t\tEnter your choice... ");

        switch (choice) {
            case 0: requestQuit(); return;
            case 1: markAttendance(); break;
            case 2: enrollStudent(); break;
            case 3: createAdminAccount(); break;
            case 4: viewAttendanceList(); break;
            case 5: batchSensorImport(); break;
            case 6: return;
            default: std::cout << "\n\t\tInvalid Choice!\n";
        }
    }
}

void Admin::createAdminAccount() {
    const std::string username = util::readWord("\n\t\t\tEnter username: ");

    std::string password;
    while (true) {
        password = util::readWord("\n\t\t\tEnter password: ");
        if (!isPasswordAcceptable(password)) {
            continue;
        }
        const std::string repeated = util::readWord("\n\t\t\tRe-enter password: ");
        if (password == repeated) {
            break;
        }
        std::cout << "\n\t\tPasswords do not match. Please try again.\n";
    }

    std::ofstream file(kAdminFile, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "\n\t\t[Error] Failed to securely write to Admin database.\n";
        return;
    }

    file << username << "\t" << hashPassword(password) << "\n";
    std::cout << "\n\t\t\tAccount created successfully\n";
}

void Admin::enrollStudent() {
    const std::string name = util::readWord("\n\t\t\tEnter the name: ");
    const std::string usn = util::readWord("\n\t\t\tEnter the username: ");

    Student student(usn, name);

    if (util::fileExists(usn + ".txt")) {
        std::cout << "\n\t\t\tStudent already enrolled\n";
        return;
    }

    if (!student.createRecordFile(util::today())) {
        std::cerr << "\n\t\t[Error] Failed to initialize student record files properly.\n";
        return;
    }

    if (roster_.add(usn, name)) {
        std::cout << "\n\t\tStudent successfully added to the list\n";
    }
}

void Admin::markAttendance() {
    const util::Date date = util::today();
    std::cout << "\n\t\t\tEnter 1 for present and 0 for absent\n\n";

    const auto students = roster_.all();
    for (const auto& entry : students) {
        Student student(entry.first, entry.second);

        std::cout << "\t\tUniversity Number: " << entry.first
                  << "\tName: " << entry.second << "\t";

        int status = util::readInt("");
        while (status != 0 && status != 1) {
            std::cout << "\t\t[Error] Input 1 for Present, 0 for Absent: ";
            status = util::readInt("");
        }

        if (!student.appendAttendance(status, date)) {
            std::cerr << "\n\t\t[Error] Failed to open database segment for "
                      << entry.first << "\n";
        }
    }

    std::cout << "\n\t\t\tAll attendance marked\n";
}

void Admin::viewAttendanceList() {
    for (const std::string& usn : roster_.sortedUsns()) {
        Student student(usn, "");
        if (!student.loadRecords()) {
            std::cerr << "\n\t\t[Error] Cannot read log for " << usn << "\n";
            continue;
        }
        student.printSummary();
    }
}

void Admin::batchSensorImport() {
    std::ifstream sensorFile(kSensorFile);
    if (!sensorFile.is_open()) {
        std::cerr << "\n\t\t[Error] Unable to locate or open '" << kSensorFile << "'.\n";
        return;
    }

    std::ofstream errorLog(kErrorLogFile, std::ios::app);
    if (!errorLog.is_open()) {
        std::cerr << "\n\t\t[Error] Unable to open 'error_log.txt'. "
                     "Continuing without logging errors...\n";
    }

    std::string line;
    int lineNum = 0;
    int successCount = 0;

    while (std::getline(sensorFile, line)) {
        lineNum++;
        if (line.empty()) continue;

        std::istringstream parser(line);
        std::string usn;
        std::string name;
        int status = -1;
        util::Date date;
        date.day = -1;
        date.month = -1;
        date.year = -1;

        if (!(parser >> usn >> name >> status >> date.day >> date.month >> date.year)) {
            if (errorLog.is_open()) {
                errorLog << "Line " << lineNum
                         << ": Skipping due to missing or malformed fields => "
                         << line << "\n";
            }
            continue;
        }

        if (status < 0 || status > 1 || date.day < 1 || date.day > 31 ||
            date.month < 1 || date.month > 12 || date.year < 2000) {
            if (errorLog.is_open()) {
                errorLog << "Line " << lineNum
                         << ": Impossible sensor date/value => " << line << "\n";
            }
            continue;
        }

        Student student(usn, name);
        if (student.appendAttendance(status, date)) {
            successCount++;
        } else if (errorLog.is_open()) {
            errorLog << "Line " << lineNum
                     << ": Failed to write to student DB for USN " << usn << "\n";
        }
    }

    std::cout << "\n\t\tBatch Sensor Import completed. Successfully processed "
              << successCount << " valid logs.\n";
    std::cout << "\t\tCheck 'error_log.txt' for any invalid sensor lines skipped.\n";
}
