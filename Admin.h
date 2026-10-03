#ifndef ADMIN_H
#define ADMIN_H

#include "Student.h"
#include "User.h"

#include <string>

// An administrator: authenticates against AdminAccount.txt and owns the
// administrative operations of the system.
class Admin : public User {
public:
    bool login() override;
    void showMenu() override;

private:
    void enrollStudent();
    void createAdminAccount();
    void markAttendance();
    void viewAttendanceList();
    void batchSensorImport();

    // Seeds AdminAccount.txt with the default account on first run.
    void initializeAccountStore() const;

    // Loads the stored credentials for `username` and checks the password.
    bool authenticate(const std::string& username, const std::string& password);

    StudentRoster roster_;
};

#endif
