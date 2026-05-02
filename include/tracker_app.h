#pragma once
#include "auth_manager.h"
#include "syllabus_data.h"
#include <string>
#include <vector>

class TrackerApp {
public:
    TrackerApp();
    void run();

private:
    AuthManager auth;
    std::vector<Subject> syllabus;

    void showAuthMenu();
    void showLoginScreen();
    void showRegisterScreen();
    void showMainDashboard();
    void showSubjectTopics(const Subject& subject);
    void showOverallProgress();
    void showWeakAreas();

    void        printBanner();
    void        printLine(char c = '-', int len = 70);
    void        printHeader(const std::string& title);
    void        clearScreen();
    std::string getHiddenInput(const std::string& prompt);
    void        pressEnterToContinue();
    int         getChoice(int minVal, int maxVal);
    void        printProgressBar(int done, int total, int width = 30);
    std::string modeLabel(int m);
    int         countCompleted(const std::string& subCode, const std::string& mode);
    int         totalTopics(const std::string& subCode);
    void        toggleTopic(const Subject& sub, const Topic& topic, int mode);
};
