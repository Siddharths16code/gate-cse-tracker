#include "../include/auth_manager.h"

AuthManager::AuthManager() {
    m_users = DataManager::loadAllUsers();
}

bool AuthManager::registerUser(const std::string& username, const std::string& password,
                                const std::string& fullName, const std::string& email,
                                std::string& errorMsg) {
    if (username.empty() || password.empty() || fullName.empty() || email.empty()) {
        errorMsg = "All fields are required.";
        return false;
    }
    if (username.size() < 3) {
        errorMsg = "Username must be at least 3 characters.";
        return false;
    }
    if (password.size() < 4) {
        errorMsg = "Password must be at least 4 characters.";
        return false;
    }
    for (const auto& u : m_users) {
        if (u.username == username) {
            errorMsg = "Username '" + username + "' is already taken.";
            return false;
        }
    }
    User newUser;
    newUser.username     = username;
    newUser.passwordHash = simpleHash(password);
    newUser.fullName     = fullName;
    newUser.email        = email;
    m_users.push_back(newUser);
    DataManager::saveAllUsers(m_users);
    return true;
}

User* AuthManager::login(const std::string& username, const std::string& password) {
    std::string hash = simpleHash(password);
    for (auto& u : m_users) {
        if (u.username == username && u.passwordHash == hash) {
            m_loggedIn = &u;
            return &u;
        }
    }
    return nullptr;
}

void AuthManager::logout() {
    saveProgress();
    m_loggedIn = nullptr;
}

User* AuthManager::currentUser() { return m_loggedIn; }

void AuthManager::saveProgress() {
    DataManager::saveAllUsers(m_users);
}
