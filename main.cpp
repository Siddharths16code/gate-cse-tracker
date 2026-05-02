#include "include/tracker_app.h"
#include <iostream>

int main() {
    try {
        TrackerApp app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
