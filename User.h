#ifndef USER_H
#define USER_H

#include <string>
#include <utility>

// Abstract base class for anyone who can log in (Admin or Student).
// It cannot be instantiated; each role must provide its own menu and role name.
class User {
public:
    explicit User(std::string name) : username(std::move(name)) {}
    virtual ~User() = default; // virtual so deleting through a User* runs the derived destructor

    virtual void showMenu() = 0;
    virtual std::string role() const = 0;

    const std::string& getUsername() const { return username; }

protected:
    std::string username;
};

#endif
