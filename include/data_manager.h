#pragma once
#include "user.h"
#include <vector>
#include <string>

class DataManager {
public:
    static const std::string DATA_FILE;
    static std::vector<User> loadAllUsers();
    static void saveAllUsers(const std::vector<User>& users);
private:
    static std::string escapeStr(const std::string& s);
    static std::string unescapeStr(const std::string& s);
};
