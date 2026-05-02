#pragma once
#include <string>
#include <map>

struct TopicProgress {
    bool theory   = false;
    bool pyq      = false;
    bool revision = false;
    bool test     = false;
};

struct SubjectProgress {
    std::map<std::string, TopicProgress> topics;
};

struct User {
    std::string username;
    std::string passwordHash;
    std::string fullName;
    std::string email;
    std::map<std::string, SubjectProgress> progress;
};

inline std::string simpleHash(const std::string& s) {
    unsigned long long h = 5381;
    for (unsigned char c : s)
        h = ((h << 5) + h) ^ c;
    return std::to_string(h);
}
