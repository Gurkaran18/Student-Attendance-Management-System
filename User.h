#ifndef USER_H
#define USER_H

#include <cstddef>
#include <string>

// Abstract base for every account that can sign in to the system.
// Credentials are private: subclasses set them through setCredentials()
// and check them through verifyPassword(), never by touching the hash.
class User {
public:
    virtual ~User() = default;

    User(const User&) = delete;
    User& operator=(const User&) = delete;

    // Authenticate against this role's own persistent store.
    virtual bool login() = 0;
    // Role-specific menu, entered only after login() succeeded.
    virtual void showMenu() = 0;

    // True only when credentials have been loaded and the hash matches.
    bool verifyPassword(const std::string& candidate) const;

    const std::string& identifier() const { return identifier_; }
    bool quitRequested() const { return quitRequested_; }

    // Password policy: 8-20 characters, one uppercase, one digit,
    // one special character. Reports the reason it rejected the password.
    static bool isPasswordAcceptable(const std::string& password);

protected:
    User() = default;
    User(std::string identifier, std::size_t passwordHash);

    static std::size_t hashPassword(const std::string& password);

    void setCredentials(std::string identifier, std::size_t passwordHash);
    void setIdentifier(std::string identifier);
    void requestQuit() { quitRequested_ = true; }

private:
    std::string identifier_;
    std::size_t passwordHash_ = 0;
    bool credentialsLoaded_ = false;
    bool quitRequested_ = false;
};

#endif
