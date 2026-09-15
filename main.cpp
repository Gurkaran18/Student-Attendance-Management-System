#include "Admin.h"
#include "Student.h"
#include "User.h"
#include "Utils.h"

#include <iostream>
#include <memory>
#include <string>

// Returns the logged-in Admin, or nullptr if the credentials are wrong.
static std::unique_ptr<User> adminLogin() {
    std::cout << "\n\n------------------------ ADMIN LOGIN ------------------------\n";
    std::string username = readWord("\n\t\t\tEnter username : ");
    std::string password = readWord("\n\t\t\tEnter password : ");

    if (!Admin::authenticate(username, password)) {
        std::cout << "\n\t\tError! Invalid Credentials. Please Try Again\n";
        return nullptr;
    }
    return std::make_unique<Admin>(username, password);
}

// Returns the matching Student, or nullptr if the username is not enrolled.
static std::unique_ptr<User> studentLogin() {
    std::cout << "\n ------------------------ STUDENT MENU ------------------------\n";
    std::string username = readWord("\n\t\t\tEnter username : ");

    std::string name;
    if (!Student::find(username, name)) {
        std::cout << "\n\n\t\t\t\t\tNo student found !!!\n\n";
        return nullptr;
    }
    return std::make_unique<Student>(username, name);
}

int main() {
    Admin::createDefaultAccount();

    while (true) {
        std::cout << "\n ------------------------ MAIN MENU ------------------------\n";
        std::cout << "\n\t\t\t1.ADMIN LOGIN\n\t\t\t2.STUDENT LOGIN\n\t\t\t3.Exit\n";

        std::unique_ptr<User> user;
        switch (readInt("\n\t\t\tEnter your choice...")) {
        case 1:
            user = adminLogin();
            break;
        case 2:
            user = studentLogin();
            break;
        case 3:
            return 0;
        default:
            std::cout << "\n\t\t\tInvalid choice. Please try again.\n";
        }

        if (user) {
            std::cout << "\n\t\t\tLogin successful!!! (" << user->role() << ": " << user->getUsername() << ")\n";
            // Polymorphism: runs Admin::showMenu() or Student::showMenu() depending on the real object
            user->showMenu();
        }
        // 'user' goes out of scope here; unique_ptr deletes it through the virtual destructor
    }
}
