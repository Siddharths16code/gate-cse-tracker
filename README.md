# GATE CSE 2026 - Syllabus Tracker

A complete C++ console application to track your GATE CSE 2026 preparation.

---

## Features

- User Registration and Login (with hidden password input)
- All 10 GATE CSE sections pre-loaded (114 topics total)
- 4 tracking modes per topic: Theory, PYQ, Revision, Test
- Color-coded progress bars on dashboard
- Overall Progress Report (per subject, per mode)
- Weak Areas Analysis
- Auto-save to local file (gate_tracker_data.txt)

---

## Project Structure

```
gate_tracker/
|-- main.cpp
|-- CMakeLists.txt
|-- README.md
|-- include/
|   |-- syllabus_data.h    (all 114 topics pre-loaded)
|   |-- user.h             (User and progress structs)
|   |-- data_manager.h     (file I/O)
|   |-- auth_manager.h     (auth logic)
|   |-- tracker_app.h      (main app class)
|-- src/
    |-- data_manager.cpp
    |-- auth_manager.cpp
    |-- tracker_app.cpp    (all UI screens)
```

---

## Build in VS Code

### Requirements
- CMake >= 3.14
- g++ or clang++ with C++17, OR Visual Studio with MSVC

### Option 1 - CMake Tools Extension (Easiest)
1. Install "CMake Tools" extension in VS Code
2. Open the `gate_tracker` folder: File > Open Folder
3. Ctrl+Shift+P > "CMake: Select a Kit" (choose GCC or MSVC)
4. Ctrl+Shift+P > "CMake: Build"
5. Ctrl+Shift+P > "CMake: Run Without Debugging"

### Option 2 - Integrated Terminal (Linux / macOS / WSL)
Open terminal inside VS Code (Ctrl+`) and run:

```bash
cd gate_tracker
mkdir build
cd build
cmake ..
make
./bin/GATETracker
```

### Option 3 - Direct g++ (Simplest)
Open terminal in the gate_tracker folder and run:

```bash
# Linux / macOS / WSL
g++ -std=c++17 -O2 -Iinclude -o GATETracker main.cpp src/data_manager.cpp src/auth_manager.cpp src/tracker_app.cpp
./GATETracker

# Windows (MinGW)
g++ -std=c++17 -O2 -Iinclude -o GATETracker.exe main.cpp src/data_manager.cpp src/auth_manager.cpp src/tracker_app.cpp
GATETracker.exe
```

### Option 4 - Windows (MinGW + CMake)
```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
bin\GATETracker.exe
```

---

## How to Use

### First Time
1. Launch the program
2. Choose option 2 (Sign Up)
3. Enter Full Name, Email, Username, Password

### Tracking Topics
1. Sign in
2. From Dashboard, enter 1-10 to open a subject
3. Type a command to mark topics:
   - `3t`  = Toggle Topic 3 Theory
   - `5p`  = Toggle Topic 5 PYQ
   - `2r`  = Toggle Topic 2 Revision
   - `7x`  = Toggle Topic 7 Test
4. Bulk commands:
   - `mat` = Mark ALL topics Theory (toggle)
   - `map` = Mark ALL topics PYQ
   - `mar` = Mark ALL topics Revision
   - `max` = Mark ALL topics Test
5. Type `0` to go back to dashboard

---

## Subjects Covered

| #  | Section                             | Topics |
|----|-------------------------------------|--------|
| 1  | Engineering Mathematics             | 23     |
| 2  | Digital Logic                       | 7      |
| 3  | Computer Organization & Architecture| 10     |
| 4  | Programming and Data Structures     | 10     |
| 5  | Algorithms                          | 11     |
| 6  | Theory of Computation               | 9      |
| 7  | Compiler Design                     | 9      |
| 8  | Operating System                    | 10     |
| 9  | Databases                           | 10     |
| 10 | Computer Networks                   | 15     |

Total: 114 topics

---

## Progress Legend

- [T] = Theory done
- [P] = PYQ done
- [R] = Revision done
- [X] = Test done
- Y   = Completed
- -   = Not done yet

Progress bar colors:
- Green  = 80% or above
- Yellow = 50-79%
- Red    = below 50%

---

Good luck for GATE 2026!
