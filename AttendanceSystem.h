#ifndef ATTENDANCE_SYSTEM_H
#define ATTENDANCE_SYSTEM_H

class User;

// Thin controller: shows the top-level menu, builds the right User subclass
// and drives it through the base-class interface.
class AttendanceSystem {
public:
    void run();

private:
    // Runs one session polymorphically: login() and showMenu() are resolved
    // at run time through this User reference.
    bool runSession(User& user);
};

#endif
