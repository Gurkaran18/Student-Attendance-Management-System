#include "Admin.h"

#include "Student.h"
#include "Utils.h"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <utility>

Admin::Admin(std::string username, std::string password) : User(std::move(username)), password(std::move(password)) {}

void Admin::createDefaultAccount() {
    if (!fileExists(ADMIN_FILE)) {
        Admin("admin", "admin").saveAccount();
    }
}

bool Admin::authenticate(const std::string& username, const std::string& password) {
    std::ifstream f(ADMIN_FILE);
    std::string fileUsername, filePassword;
    while (f >> fileUsername >> filePassword) {
        if (fileUsername == username && filePassword == password) {
            return true;
        }
    }
    return false;
}

// Rules: length 8-20, at least one uppercase letter, one digit, one special character.
bool Admin::isValidPassword(const std::string& password) {
    if (password.size() < 8) {
        std::cout << "\n\t\t\tPassword is too short (minimum 8 characters)\n";
        return false;
    }
    if (password.size() > 20) {
        std::cout << "\n\t\t\tPassword is too long (maximum 20 characters)\n";
        return false;
    }

    const std::string specialChars = "@&_*()#$^.,";
    bool hasUpper = false, hasDigit = false, hasSpecial = false;
    for (char c : password) {
        // cast to unsigned char: passing a negative char to isupper/isdigit is undefined
        if (std::isupper(static_cast<unsigned char>(c))) hasUpper = true;
        if (std::isdigit(static_cast<unsigned char>(c))) hasDigit = true;
        if (specialChars.find(c) != std::string::npos) hasSpecial = true;
    }

    if (!hasUpper) std::cout << "\n\t\t\tPassword needs at least one uppercase letter\n";
    if (!hasDigit) std::cout << "\n\t\t\tPassword needs at least one digit\n";
    if (!hasSpecial) std::cout << "\n\t\t\tPassword needs at least one special character (@ # $ ^ & * ( ) _ . ,)\n";
    return hasUpper && hasDigit && hasSpecial;
}

void Admin::saveAccount() const {
    std::ofstream f(ADMIN_FILE, std::ios::app);
    f << username << "\t" << password << "\n";
}

void Admin::showMenu() {
    while (true) {
        std::cout << "\n ------------------------ ADMIN MENU ------------------------\n";
        std::cout << "\n\t\t\t1.Mark attendance\n\t\t\t2.Add students\n\t\t\t3.Create new admin account"
                     "\n\t\t\t4.Student attendance list\n\t\t\t5.Main menu\n\t\t\t0.Exit\n";
        switch (readInt("\n\t\t\tEnter your choice...")) {
        case 0:
            std::exit(0); // all files are closed at this point, so exiting directly is safe
        case 1:
            markAttendance();
            break;
        case 2:
            enrollStudent();
            break;
        case 3:
            createAdminAccount();
            break;
        case 4:
            showAttendanceList();
            break;
        case 5:
            return; // back to the main menu loop in main()
        default:
            std::cout << "\n\t\t\tInvalid choice. Please try again.\n";
        }
    }
}

void Admin::createAdminAccount() const {
    std::string newUsername = readWord("\n\t\t\tEnter username: ");
    std::string newPassword;
    // Keep asking until the password follows the rules and is re-entered identically
    while (true) {
        newPassword = readWord("\n\t\t\tEnter password: ");
        if (!isValidPassword(newPassword)) {
            continue;
        }
        std::string confirmPassword = readWord("\n\t\t\tRe-enter password: ");
        if (newPassword == confirmPassword) {
            break;
        }
        std::cout << "\n\t\tPasswords are not matching. Re-enter the password\n";
    }

    Admin(newUsername, newPassword).saveAccount();
    std::cout << "\n\t\t\tAccount created successfully\n";
}

void Admin::enrollStudent() const {
    std::string name = readWord("\n\t\t\tEnter the name: ");
    std::string usn = readWord("\n\t\t\tEnter the username: ");

    Student student(usn, name);
    if (!student.enroll()) {
        std::cout << "\n\t\t\tStudent already enrolled\n";
        return;
    }
    sortUsnFile();
    std::cout << "\n\t\tStudent successfully added to the list\n";
}

void Admin::markAttendance() const {
    std::ifstream list(STUDENT_LIST_FILE);
    if (!list) {
        std::cout << "\n\t\t\tNo students enrolled yet\n";
        return;
    }

    Date date = today();
    std::cout << "\n\t\t\tEnter 1 for present and 0 for absent\n\n";
    std::string usn, name;
    while (list >> usn >> name) {
        int status = readInt("\t\tUniversity Number: " + usn + "\tName: " + name + "\t");
        while (status != Student::ABSENT && status != Student::PRESENT) {
            status = readInt("\t\tPlease enter 1 (present) or 0 (absent): ");
        }
        Student(usn, name).recordAttendance(status, date);
    }
    std::cout << "\n\t\t\tAll attendance marked\n";
}

void Admin::showAttendanceList() const {
    std::ifstream index(USN_FILE);
    if (!index) {
        std::cout << "\n\t\t\tNo students enrolled yet\n";
        return;
    }

    // UsnFile.txt is kept sorted, so the list comes out in roll-number order
    std::string usn, name;
    while (index >> usn) {
        if (Student::find(usn, name)) {
            Student(usn, name).printSummary();
        }
    }
}
