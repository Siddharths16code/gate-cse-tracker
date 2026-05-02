#include "../include/data_manager.h"
#include <fstream>
#include <sstream>

const std::string DataManager::DATA_FILE = "gate_tracker_data.txt";

std::string DataManager::escapeStr(const std::string& s) {
    std::string r;
    for (char c : s) {
        if      (c == '|')  r += "\\p";
        else if (c == '\n') r += "\\n";
        else if (c == '\\') r += "\\\\";
        else if (c == ':')  r += "\\c";
        else if (c == ';')  r += "\\s";
        else r += c;
    }
    return r;
}

std::string DataManager::unescapeStr(const std::string& s) {
    std::string r;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            char n = s[i+1];
            if      (n == 'p')  { r += '|';  i++; }
            else if (n == 'n')  { r += '\n'; i++; }
            else if (n == '\\') { r += '\\'; i++; }
            else if (n == 'c')  { r += ':';  i++; }
            else if (n == 's')  { r += ';';  i++; }
            else r += s[i];
        } else {
            r += s[i];
        }
    }
    return r;
}

// Format: username|pwdhash|fullname|email|SUBCODE:topicname:T:P:R:X;topicname:T:P:R:X|SUBCODE:...
std::vector<User> DataManager::loadAllUsers() {
    std::vector<User> users;
    std::ifstream f(DATA_FILE);
    if (!f.is_open()) return users;

    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::vector<std::string> fields;
        std::string tok;
        std::istringstream ss(line);
        while (std::getline(ss, tok, '|'))
            fields.push_back(tok);

        if (fields.size() < 4) continue;
        User u;
        u.username     = unescapeStr(fields[0]);
        u.passwordHash = unescapeStr(fields[1]);
        u.fullName     = unescapeStr(fields[2]);
        u.email        = unescapeStr(fields[3]);

        // fields[4..] = subject blocks
        for (size_t fi = 4; fi < fields.size(); fi++) {
            if (fields[fi].empty()) continue;
            // split by ;
            std::istringstream ts(fields[fi]);
            std::string entry;
            while (std::getline(ts, entry, ';')) {
                if (entry.empty()) continue;
                // SUBCODE:topicname:T:P:R:X
                std::vector<std::string> parts;
                std::istringstream es(entry);
                std::string p;
                while (std::getline(es, p, ':'))
                    parts.push_back(p);
                if (parts.size() < 6) continue;
                std::string sc = unescapeStr(parts[0]);
                std::string tn = unescapeStr(parts[1]);
                TopicProgress tp;
                tp.theory   = (parts[2] == "1");
                tp.pyq      = (parts[3] == "1");
                tp.revision = (parts[4] == "1");
                tp.test     = (parts[5] == "1");
                u.progress[sc].topics[tn] = tp;
            }
        }
        users.push_back(u);
    }
    return users;
}

void DataManager::saveAllUsers(const std::vector<User>& users) {
    std::ofstream f(DATA_FILE, std::ios::trunc);
    for (const auto& u : users) {
        f << escapeStr(u.username) << "|"
          << escapeStr(u.passwordHash) << "|"
          << escapeStr(u.fullName) << "|"
          << escapeStr(u.email);

        for (const auto& [subCode, subProg] : u.progress) {
            f << "|";
            bool first = true;
            for (const auto& [topicName, tp] : subProg.topics) {
                if (!first) f << ";";
                first = false;
                f << escapeStr(subCode) << ":"
                  << escapeStr(topicName) << ":"
                  << (tp.theory   ? "1" : "0") << ":"
                  << (tp.pyq      ? "1" : "0") << ":"
                  << (tp.revision ? "1" : "0") << ":"
                  << (tp.test     ? "1" : "0");
            }
        }
        f << "\n";
    }
}
