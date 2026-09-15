#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

#include <string>

class Admin : public User {
public:
    Admin(std::string username, std::string password);

    void showMenu() override;
    std::string role() const override { return "Admin"; }

    static void createDefaultAccount(); // writes admin/admin if AdminAccount.txt is missing
    static bool authenticate(const std::string& username, const std::string& password);
    static bool isValidPassword(const std::string& password); // prints which rule failed

private:
    std::string password;

    void saveAccount() const;
    void createAdminAccount() const;
    void enrollStudent() const;
    void markAttendance() const;
    void showAttendanceList() const;
};

#endif
