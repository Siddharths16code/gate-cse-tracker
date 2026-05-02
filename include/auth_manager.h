#pragma once
#include "user.h"
#include "data_manager.h"
#include <vector>
#include <string>

class AuthManager {
public:
    AuthManager();
    bool registerUser(const std::string& username, const std::string& password,
                      const std::string& fullName, const std::string& email,
                      std::string& errorMsg);
    User* login(const std::string& username, const std::string& password);
    void  logout();
    User* currentUser();
    void  saveProgress();
    std::vector<User>& users() { return m_users; }
private:
    std::vector<User> m_users;
    User* m_loggedIn = nullptr;
};
