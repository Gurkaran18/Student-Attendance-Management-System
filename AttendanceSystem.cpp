#include "AttendanceSystem.h"

#include "Admin.h"
#include "Student.h"
#include "User.h"
#include "Util.h"

#include <iostream>
#include <memory>

bool AttendanceSystem::runSession(User& user) {
    if (user.login()) {
        user.showMenu();
    }
    return !user.quitRequested();
}

void AttendanceSystem::run() {
    while (true) {
        std::cout << "\n ------------------------ MAIN MENU ------------------------\n";
        std::cout << "\n\t\t\t1.ADMIN LOGIN"
                  << "\n\t\t\t2.STUDENT LOGIN"
                  << "\n\t\t\t3.Exit\n";

        const int choice = util::readInt("\n\t\t\tEnter your choice... ");

        std::unique_ptr<User> user;
        switch (choice) {
            case 1: user = std::make_unique<Admin>(); break;
            case 2: user = std::make_unique<Student>(); break;
            case 3: return;
            default: std::cout << "\n\t\tInvalid Choice!\n"; continue;
        }

        if (!runSession(*user)) {
            return;  // the session asked to quit the program
        }
    }
}
