#include "User.h"

#include <cctype>
#include <functional>
#include <iostream>
#include <utility>

User::User(std::string identifier, std::size_t passwordHash)
    : identifier_(std::move(identifier)),
      passwordHash_(passwordHash),
      credentialsLoaded_(true) {}

std::size_t User::hashPassword(const std::string& password) {
    return std::hash<std::string>{}(password);
}

void User::setCredentials(std::string identifier, std::size_t passwordHash) {
    identifier_ = std::move(identifier);
    passwordHash_ = passwordHash;
    credentialsLoaded_ = true;
}

void User::setIdentifier(std::string identifier) {
    identifier_ = std::move(identifier);
}

bool User::verifyPassword(const std::string& candidate) const {
    return credentialsLoaded_ && hashPassword(candidate) == passwordHash_;
}

bool User::isPasswordAcceptable(const std::string& password) {
    const std::size_t length = password.size();

    if (length < 8) {
        std::cout << "\n\t\t\tPassword is too short\n";
        return false;
    }
    if (length > 20) {
        std::cout << "\n\t\t\tPassword is too long\n";
        return false;
    }

    bool hasDigit = false;
    bool hasUpper = false;
    bool hasSpecial = false;

    for (char c : password) {
        if (c >= 'A' && c <= 'Z') hasUpper = true;
        if (c == '@' || c == '&' || c == '_' || c == '*' ||
            c == '(' || c == ')' || c == '#' || c == '$' ||
            c == '^' || c == '.' || c == ',') hasSpecial = true;
        if (std::isdigit(static_cast<unsigned char>(c))) hasDigit = true;
    }

    if (hasUpper && hasSpecial && hasDigit) return true;

    std::cout << "\nPassword should contain at least one uppercase letter, "
                 "one digit, and one special character\n";
    return false;
}
