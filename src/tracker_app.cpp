// ============================================================
//  GATE CSE 2026 Syllabus Tracker  –  Main Application Logic
// ============================================================
#include "../include/tracker_app.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <limits>

#ifdef _WIN32
  #include <windows.h>
  #include <conio.h>
  #define CLEAR_CMD "cls"
#else
  #include <termios.h>
  #include <unistd.h>
  #define CLEAR_CMD "clear"
#endif

// ─── ANSI colour codes ───────────────────────────────────────
#define RST   "\033[0m"
#define BOLD  "\033[1m"
#define DIM   "\033[2m"
#define RED   "\033[31m"
#define GRN   "\033[32m"
#define YEL   "\033[33m"
#define CYN   "\033[36m"
#define BGRN  "\033[92m"
#define BYEL  "\033[93m"
#define BCYN  "\033[96m"
#define BWHT  "\033[97m"
#define MAG   "\033[35m"

// ════════════════════════════════════════════════════════════
//  Constructor
// ════════════════════════════════════════════════════════════
TrackerApp::TrackerApp() {
    syllabus = getGateSyllabus();
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD m = 0;
    GetConsoleMode(h, &m);
    SetConsoleMode(h, m | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

void TrackerApp::run() {
    clearScreen();
    printBanner();
    showAuthMenu();
}

// ════════════════════════════════════════════════════════════
//  UI Helpers
// ════════════════════════════════════════════════════════════
void TrackerApp::clearScreen() {
    system(CLEAR_CMD);
}

void TrackerApp::printLine(char c, int len) {
    std::cout << CYN;
    for (int i = 0; i < len; i++) std::cout << c;
    std::cout << RST << "\n";
}

void TrackerApp::printBanner() {
    printLine('=');
    std::cout << BOLD << BCYN;
    std::cout << "  ######   #######  #######  ######     ######  #####  ######\n";
    std::cout << "  ##       ##   ##    ##      ##        ##     ##      ##    \n";
    std::cout << "  ## ###   #######    ##      #####     ##      ###    ######\n";
    std::cout << "  ##  ##   ##   ##    ##      ##        ##        ##   ##    \n";
    std::cout << "  ######   ##   ##    ##      ######    ######  #####  ######\n";
    std::cout << RST;
    std::cout << BOLD << BYEL;
    std::cout << "          GATE CSE 2027 Syllabus Tracker  |  IIT MADRAS\n";
    std::cout << RST;
    printLine('=');
    std::cout << "\n";
}

void TrackerApp::printHeader(const std::string& title) {
    clearScreen();
    printLine('=');
    int pad = (int)(70 - title.size()) / 2;
    if (pad < 0) pad = 0;
    std::cout << BOLD << BCYN
              << std::string(pad, ' ') << title << "\n"
              << RST;
    printLine('=');
    std::cout << "\n";
}

void TrackerApp::pressEnterToContinue() {
    std::cout << "\n" << DIM << "  Press Enter to continue..." << RST;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int TrackerApp::getChoice(int minVal, int maxVal) {
    int ch;
    while (true) {
        std::cout << BYEL << "  Enter choice [" << minVal << "-" << maxVal << "]: " << RST;
        if (std::cin >> ch && ch >= minVal && ch <= maxVal) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return ch;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << RED << "  Invalid. Try again.\n" << RST;
    }
}

std::string TrackerApp::getHiddenInput(const std::string& prompt) {
    std::cout << prompt;
    std::string pass;
#ifdef _WIN32
    char c;
    while ((c = (char)_getch()) != '\r') {
        if (c == '\b') {
            if (!pass.empty()) { pass.pop_back(); std::cout << "\b \b"; }
        } else { pass += c; std::cout << '*'; }
    }
    std::cout << "\n";
#else
    struct termios old_t, new_t;
    tcgetattr(STDIN_FILENO, &old_t);
    new_t = old_t;
    new_t.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_t);
    char c;
    while (read(STDIN_FILENO, &c, 1) && c != '\n') {
        if (c == 127 || c == '\b') {
            if (!pass.empty()) { pass.pop_back(); std::cout << "\b \b" << std::flush; }
        } else { pass += c; std::cout << '*' << std::flush; }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old_t);
    std::cout << "\n";
#endif
    return pass;
}

void TrackerApp::printProgressBar(int done, int total, int width) {
    if (total == 0) { std::cout << DIM << "[No topics]" << RST; return; }
    int filled = done * width / total;
    float pct  = 100.0f * done / total;
    std::string bar = "[";
    for (int i = 0; i < width; i++)
        bar += (i < filled) ? "#" : ".";
    bar += "]";
    if (pct >= 80)      std::cout << BGRN;
    else if (pct >= 50) std::cout << BYEL;
    else                std::cout << RED;
    std::cout << bar << " " << std::fixed << std::setprecision(0)
              << pct << "% (" << done << "/" << total << ")" << RST;
}

std::string TrackerApp::modeLabel(int m) {
    switch (m) {
        case 1: return "Theory";
        case 2: return "PYQ";
        case 3: return "Revision";
        case 4: return "Test";
        default: return "";
    }
}

int TrackerApp::totalTopics(const std::string& subCode) {
    for (const auto& s : syllabus)
        if (s.code == subCode) return (int)s.topics.size();
    return 0;
}

int TrackerApp::countCompleted(const std::string& subCode, const std::string& mode) {
    auto* u = auth.currentUser();
    if (!u) return 0;
    auto it = u->progress.find(subCode);
    if (it == u->progress.end()) return 0;
    int cnt = 0;
    for (const auto& [tn, tp] : it->second.topics) {
        if      (mode == "theory"   && tp.theory)   cnt++;
        else if (mode == "pyq"      && tp.pyq)      cnt++;
        else if (mode == "revision" && tp.revision) cnt++;
        else if (mode == "test"     && tp.test)     cnt++;
    }
    return cnt;
}

void TrackerApp::toggleTopic(const Subject& sub, const Topic& topic, int mode) {
    auto* u = auth.currentUser();
    if (!u) return;
    auto& tp = u->progress[sub.code].topics[topic.name];
    switch (mode) {
        case 1: tp.theory   = !tp.theory;   break;
        case 2: tp.pyq      = !tp.pyq;      break;
        case 3: tp.revision = !tp.revision; break;
        case 4: tp.test     = !tp.test;     break;
    }
    auth.saveProgress();
}

// ════════════════════════════════════════════════════════════
//  AUTH SCREENS
// ════════════════════════════════════════════════════════════
void TrackerApp::showAuthMenu() {
    while (true) {
        clearScreen();
        printBanner();
        std::cout << BCYN << "  Welcome to GATE CSE Syllabus Tracker!\n\n" << RST;
        std::cout << BOLD
                  << "  +-----------------------------+\n"
                  << "  |    AUTHENTICATION MENU      |\n"
                  << "  +-----------------------------+\n"
                  << "  |  1.  Sign In                |\n"
                  << "  |  2.  Sign Up (Register)     |\n"
                  << "  |  0.  Exit                   |\n"
                  << "  +-----------------------------+\n"
                  << RST << "\n";

        int ch = getChoice(0, 2);
        if      (ch == 1) showLoginScreen();
        else if (ch == 2) showRegisterScreen();
        else {
            std::cout << BYEL << "\n  Goodbye! All the best for GATE 2026!\n\n" << RST;
            return;
        }
    }
}

void TrackerApp::showLoginScreen() {
    printHeader("SIGN IN");
    std::cout << CYN << "  Enter your credentials.\n\n" << RST;

    std::string username, password;
    std::cout << BWHT << "  Username : " << RST;
    std::getline(std::cin, username);
    password = getHiddenInput(std::string(BWHT) + "  Password : " + std::string(RST));

    auto* user = auth.login(username, password);
    if (user) {
        std::cout << "\n" << BGRN << "  Login successful! Welcome back, "
                  << BOLD << user->fullName << RST << BGRN << "!\n" << RST;
        pressEnterToContinue();
        showMainDashboard();
    } else {
        std::cout << "\n" << RED << "  Invalid username or password.\n" << RST;
        pressEnterToContinue();
    }
}

void TrackerApp::showRegisterScreen() {
    printHeader("CREATE NEW ACCOUNT");
    std::cout << CYN << "  Fill in the details below.\n\n" << RST;

    std::string username, password, confirmPass, fullName, email;
    std::cout << BWHT << "  Full Name    : " << RST; std::getline(std::cin, fullName);
    std::cout << BWHT << "  Email        : " << RST; std::getline(std::cin, email);
    std::cout << BWHT << "  Username     : " << RST; std::getline(std::cin, username);
    password    = getHiddenInput(std::string(BWHT) + "  Password     : " + RST);
    confirmPass = getHiddenInput(std::string(BWHT) + "  Confirm Pass : " + RST);

    if (password != confirmPass) {
        std::cout << "\n" << RED << "  Passwords do not match!\n" << RST;
        pressEnterToContinue();
        return;
    }
    std::string err;
    if (auth.registerUser(username, password, fullName, email, err)) {
        std::cout << "\n" << BGRN << "  Account created! You can now sign in.\n" << RST;
    } else {
        std::cout << "\n" << RED << "  Registration failed: " << err << "\n" << RST;
    }
    pressEnterToContinue();
}

// ════════════════════════════════════════════════════════════
//  MAIN DASHBOARD
// ════════════════════════════════════════════════════════════
void TrackerApp::showMainDashboard() {
    auto* u = auth.currentUser();
    if (!u) return;

    while (true) {
        printHeader("MAIN DASHBOARD");

        std::cout << BOLD << BCYN << "  Student : " << RST
                  << BOLD << u->fullName << RST
                  << DIM  << "  (@" << u->username << ")\n\n" << RST;

        // Overall stats
        int totalAll = 0, dTh = 0, dPy = 0, dRv = 0, dTe = 0;
        for (const auto& sub : syllabus) {
            int t = (int)sub.topics.size();
            totalAll += t;
            dTh += countCompleted(sub.code, "theory");
            dPy += countCompleted(sub.code, "pyq");
            dRv += countCompleted(sub.code, "revision");
            dTe += countCompleted(sub.code, "test");
        }

        std::cout << BOLD << "  OVERALL PROGRESS  (total topics: " << totalAll << ")\n" << RST;
        std::cout << "  " << std::string(66, '-') << "\n";

        struct ModeRow { const char* label; int done; const char* col; };
        ModeRow rows[] = {
            {"Theory",   dTh, BCYN},
            {"PYQ",      dPy, BYEL},
            {"Revision", dRv, BGRN},
            {"Test",     dTe, MAG }
        };
        for (auto& r : rows) {
            std::cout << "  " << r.col << BOLD << std::left << std::setw(12) << r.label << RST << " ";
            printProgressBar(r.done, totalAll, 32);
            std::cout << "\n";
        }

        std::cout << "\n" << BOLD << "  SUBJECTS\n" << RST;
        std::cout << "  " << std::string(66, '-') << "\n";

        for (int i = 0; i < (int)syllabus.size(); i++) {
            const auto& sub = syllabus[i];
            int total = (int)sub.topics.size();
            int th = countCompleted(sub.code, "theory");
            int py = countCompleted(sub.code, "pyq");
            int rv = countCompleted(sub.code, "revision");
            int te = countCompleted(sub.code, "test");
            float avg = total > 0 ? (th+py+rv+te)*25.0f/(total*4) : 0;
            const char* col = avg >= 80 ? BGRN : (avg >= 40 ? BYEL : RED);

            std::cout << "  " << BOLD << BCYN << std::setw(2) << (i+1) << RST
                      << ". " << BOLD << std::left << std::setw(42) << sub.name << RST
                      << col << "[T:" << th << " P:" << py
                      << " R:" << rv << " X:" << te << "/" << total << "]" << RST << "\n";
        }

        std::cout << "\n";
        printLine('-');
        std::cout << BOLD
                  << "  1-10  Open a subject        "
                  << "11  Overall Progress Report\n"
                  << "  12    Weak Areas Analysis    "
                  << " 0  Logout\n"
                  << RST;
        printLine('-');

        int ch = getChoice(0, 12);
        if (ch == 0) {
            auth.logout();
            std::cout << BYEL << "\n  Logged out. Keep grinding!\n\n" << RST;
            pressEnterToContinue();
            return;
        } else if (ch >= 1 && ch <= (int)syllabus.size()) {
            showSubjectTopics(syllabus[ch - 1]);
        } else if (ch == 11) {
            showOverallProgress();
        } else if (ch == 12) {
            showWeakAreas();
        }
    }
}

// ════════════════════════════════════════════════════════════
//  SUBJECT TOPICS SCREEN
// ════════════════════════════════════════════════════════════
void TrackerApp::showSubjectTopics(const Subject& subject) {
    auto* u = auth.currentUser();
    if (!u) return;

    while (true) {
        printHeader("SUBJECT: " + subject.name);
        int total = (int)subject.topics.size();

        // Mode progress bars
        struct MR { const char* label; std::string key; const char* col; };
        MR mrs[] = {
            {"Theory",   "theory",   BCYN},
            {"PYQ",      "pyq",      BYEL},
            {"Revision", "revision", BGRN},
            {"Test",     "test",     MAG }
        };
        std::cout << BOLD << "  Progress:\n" << RST;
        for (auto& mr : mrs) {
            int done = countCompleted(subject.code, mr.key);
            std::cout << "  " << mr.col << BOLD << std::left << std::setw(12) << mr.label << RST << " ";
            printProgressBar(done, total, 28);
            std::cout << "\n";
        }
        std::cout << "\n";

        // Column headers
        std::cout << BOLD
                  << "  #   Topic" << std::string(39, ' ')
                  << BCYN  << "[T]" << RST << BOLD << " "
                  << BYEL  << "[P]" << RST << BOLD << " "
                  << BGRN  << "[R]" << RST << BOLD << " "
                  << MAG   << "[X]\n" << RST;
        std::cout << "  " << std::string(66, '-') << "\n";

        for (int i = 0; i < (int)subject.topics.size(); i++) {
            const auto& topic = subject.topics[i];
            const auto& tp = u->progress[subject.code].topics[topic.name];

            auto mark = [](bool v, const char* col) -> std::string {
                if (v) return std::string(col) + "  Y  " + RST;
                return std::string(DIM)         + "  -  " + RST;
            };

            std::cout << "  " << BOLD << BCYN << std::setw(2) << (i+1) << RST
                      << ". " << std::left << std::setw(42) << topic.name.substr(0, 41)
                      << mark(tp.theory,   BCYN)
                      << mark(tp.pyq,      BYEL)
                      << mark(tp.revision, BGRN)
                      << mark(tp.test,     MAG)
                      << "\n";
        }

        // Help
        std::cout << "\n";
        printLine('-');
        std::cout << BOLD << "  HOW TO MARK:\n" << RST;
        std::cout << "  Type  topicNumber + mode letter\n";
        std::cout << "  e.g.  3t  = Topic 3 Theory      5p  = Topic 5 PYQ\n";
        std::cout << "        2r  = Topic 2 Revision     7x  = Topic 7 Test\n";
        std::cout << "  Bulk: mat = Mark ALL Theory      map/mar/max  similarly\n";
        std::cout << "  Type  0   to go back\n";
        printLine('-');

        std::cout << BYEL << "  Command: " << RST;
        std::string cmd;
        std::getline(std::cin, cmd);

        // Remove spaces
        cmd.erase(std::remove(cmd.begin(), cmd.end(), ' '), cmd.end());

        if (cmd == "0" || cmd == "back") return;

        // Bulk mark: "mat" / "map" / "mar" / "max"
        if (cmd.size() == 3 && (cmd[0]=='m' || cmd[0]=='M') && (cmd[1]=='a' || cmd[1]=='A')) {
            char mc = (char)tolower(cmd[2]);
            int mode = (mc=='t') ? 1 : (mc=='p') ? 2 : (mc=='r') ? 3 : (mc=='x') ? 4 : 0;
            if (mode == 0) { std::cout << RED << "  Invalid mode (t/p/r/x).\n" << RST; pressEnterToContinue(); continue; }
            std::string modeKey = (mode==1)?"theory":(mode==2)?"pyq":(mode==3)?"revision":"test";
            int done = countCompleted(subject.code, modeKey);
            bool markAll = (done < total);
            for (const auto& t2 : subject.topics) {
                auto& tp2 = u->progress[subject.code].topics[t2.name];
                switch (mode) {
                    case 1: tp2.theory   = markAll; break;
                    case 2: tp2.pyq      = markAll; break;
                    case 3: tp2.revision = markAll; break;
                    case 4: tp2.test     = markAll; break;
                }
            }
            auth.saveProgress();
            std::cout << BGRN << "  " << (markAll ? "Marked" : "Unmarked")
                      << " ALL topics for " << modeLabel(mode) << "!\n" << RST;
            pressEnterToContinue();
            continue;
        }

        // Single: "3t", "12r", etc.
        if (!cmd.empty()) {
            char mc = (char)tolower(cmd.back());
            int mode = (mc=='t') ? 1 : (mc=='p') ? 2 : (mc=='r') ? 3 : (mc=='x') ? 4 : 0;
            if (mode == 0) { std::cout << RED << "  Unknown command.\n" << RST; pressEnterToContinue(); continue; }
            try {
                int idx = std::stoi(cmd.substr(0, cmd.size()-1));
                if (idx < 1 || idx > (int)subject.topics.size()) {
                    std::cout << RED << "  Topic number out of range.\n" << RST;
                    pressEnterToContinue(); continue;
                }
                const auto& topic = subject.topics[idx - 1];
                toggleTopic(subject, topic, mode);
                const auto& tp2 = u->progress[subject.code].topics[topic.name];
                bool val = (mode==1)?tp2.theory:(mode==2)?tp2.pyq:(mode==3)?tp2.revision:tp2.test;
                std::cout << BGRN << "  " << topic.name << "  ->  "
                          << modeLabel(mode) << " " << (val ? "[Y] marked!" : "[-] unmarked") << "\n" << RST;
                pressEnterToContinue();
            } catch (...) {
                std::cout << RED << "  Invalid command. Example: 3t or 5p\n" << RST;
                pressEnterToContinue();
            }
        }
    }
}

// ════════════════════════════════════════════════════════════
//  OVERALL PROGRESS REPORT
// ════════════════════════════════════════════════════════════
void TrackerApp::showOverallProgress() {
    printHeader("OVERALL PROGRESS REPORT");
    auto* u = auth.currentUser();
    if (!u) return;

    std::cout << BOLD << BCYN << "  Student: " << RST << BOLD << u->fullName << "\n\n" << RST;

    // Table header
    std::cout << BOLD
              << "  " << std::left << std::setw(36) << "Subject"
              << std::setw(7)  << "Topics"
              << std::setw(10) << "Theory"
              << std::setw(10) << "PYQ"
              << std::setw(10) << "Revision"
              << std::setw(8)  << "Test" << "\n" << RST;
    std::cout << "  " << std::string(76, '-') << "\n";

    int gTotal = 0, gTh = 0, gPy = 0, gRv = 0, gTe = 0;

    for (const auto& sub : syllabus) {
        int total = (int)sub.topics.size();
        int th = countCompleted(sub.code, "theory");
        int py = countCompleted(sub.code, "pyq");
        int rv = countCompleted(sub.code, "revision");
        int te = countCompleted(sub.code, "test");
        gTotal += total; gTh += th; gPy += py; gRv += rv; gTe += te;

        auto pctStr = [&](int d) -> std::string {
            float p = total > 0 ? 100.0f*d/total : 0;
            char buf[32];
            const char* c = (p>=80)?BGRN:(p>=50)?BYEL:RED;
            snprintf(buf, sizeof(buf), "%s%3.0f%%%s", c, p, RST);
            return buf;
        };

        float avg = total > 0 ? (th+py+rv+te)*25.0f/(total*4) : 0;
        const char* rc = avg>=80?BGRN:(avg>=40?BYEL:RED);

        std::cout << "  " << rc << std::left << std::setw(36) << sub.name.substr(0,35) << RST
                  << DIM << std::setw(7) << total << RST
                  << std::setw(20) << pctStr(th)
                  << std::setw(20) << pctStr(py)
                  << std::setw(20) << pctStr(rv)
                  << std::setw(20) << pctStr(te)
                  << "\n";
    }

    std::cout << "  " << BOLD << std::string(76, '=') << "\n";
    auto gpct = [&](int d) {
        float p = gTotal>0?100.0f*d/gTotal:0;
        const char* c = (p>=80)?BGRN:(p>=50)?BYEL:RED;
        std::cout << c << BOLD << std::setw(20)
                  << (std::to_string((int)p)+"%") << RST;
    };
    std::cout << "  " << BOLD << BCYN << std::left << std::setw(36) << "TOTAL"
              << RST << BOLD << std::setw(7) << gTotal << RST;
    gpct(gTh); gpct(gPy); gpct(gRv); gpct(gTe);
    std::cout << "\n\n";

    // Per-subject theory bar chart
    std::cout << BOLD << "  THEORY PROGRESS BY SUBJECT:\n" << RST;
    std::cout << "  " << std::string(66, '-') << "\n";
    for (const auto& sub : syllabus) {
        int total = (int)sub.topics.size();
        int th = countCompleted(sub.code, "theory");
        std::cout << "  " << BCYN << std::left << std::setw(36) << sub.name.substr(0,35) << RST << " ";
        printProgressBar(th, total, 22);
        std::cout << "\n";
    }

    pressEnterToContinue();
}

// ════════════════════════════════════════════════════════════
//  WEAK AREAS
// ════════════════════════════════════════════════════════════
void TrackerApp::showWeakAreas() {
    printHeader("WEAK AREAS ANALYSIS");
    auto* u = auth.currentUser();
    if (!u) return;

    struct WeakTopic { std::string subject; std::string topic; int modesLeft; };
    std::vector<WeakTopic> weak;

    for (const auto& sub : syllabus) {
        for (const auto& topic : sub.topics) {
            const auto& tp = u->progress[sub.code].topics[topic.name];
            int ml = (!tp.theory)+(!tp.pyq)+(!tp.revision)+(!tp.test);
            if (ml > 0) weak.push_back({sub.name, topic.name, ml});
        }
    }

    std::sort(weak.begin(), weak.end(),
        [](const WeakTopic& a, const WeakTopic& b){ return a.modesLeft > b.modesLeft; });

    if (weak.empty()) {
        std::cout << BGRN << "  Congratulations! All topics are fully covered!\n" << RST;
    } else {
        std::cout << BYEL << "  Topics with pending modes (sorted by most incomplete):\n\n" << RST;
        std::cout << BOLD
                  << "  " << std::left << std::setw(34) << "Subject"
                  << std::setw(36) << "Topic"
                  << "Pending\n" << RST;
        std::cout << "  " << std::string(76, '-') << "\n";

        int limit = std::min((int)weak.size(), 35);
        for (int i = 0; i < limit; i++) {
            const char* col = (weak[i].modesLeft==4)?RED:(weak[i].modesLeft>=2)?BYEL:BGRN;
            std::cout << "  " << col << std::left
                      << std::setw(34) << weak[i].subject.substr(0,33)
                      << std::setw(36) << weak[i].topic.substr(0,35)
                      << weak[i].modesLeft << "/4\n" << RST;
        }
        if ((int)weak.size() > limit)
            std::cout << DIM << "  ... and " << (weak.size()-limit) << " more.\n" << RST;

        std::cout << "\n" << BOLD << "  Incomplete topics: "
                  << RED << weak.size() << RST << "\n";
    }
    pressEnterToContinue();
}
