# Student Attendance Management System (C++ OOP)

A menu-driven console attendance system built using **Object-Oriented Programming in C++17**.  
**Admins** enroll students, create admin accounts, and mark attendance, while **Students** log in to view their attendance history and percentage.

---

## Features

### Admin
- Admin login with username and password
- Add (enroll) new students, with duplicate enrollments blocked
- Create new admin accounts, with password rules enforced:
  8 to 20 characters, at least one uppercase letter, one digit, and one special character (`@ # $ ^ & * ( ) _ . ,`)
- Mark attendance (present/absent for each student, stamped with today's date)
- View the attendance list for all students, in roll-number order

### Student
- Student login with username
- Check attendance history (present/absent per date)
- View attendance percentage

---

## Tech Stack
- **Language:** C++17 (standard library only)
- **Concepts Used:**
  - Classes & Objects, Encapsulation (private data, public methods)
  - Inheritance & Polymorphism (abstract `User` base class, `Admin` and `Student` override `showMenu()`)
  - Smart pointers (`std::unique_ptr<User>`)
  - File Handling with `fstream` (accounts and per-student attendance logs)
  - Data Validation (password rules, duplicate student check, menu input)
  - Sorting (recursive insertion sort over a `std::vector` keeps the roll-number index sorted)
  - Modular Programming (headers and source files, Makefile)

## Project Structure
| File | Purpose |
|------|---------|
| `User.h` | Abstract base class for anyone who can log in |
| `Admin.h` / `Admin.cpp` | Admin login check, password rules, admin menu, enrolling students, marking attendance |
| `Student.h` / `Student.cpp` | Student records, attendance history and percentage |
| `Utils.h` / `Utils.cpp` | Safe console input, file helpers, recursive insertion sort, today's date |
| `main.cpp` | Main menu and login flow |
| `Makefile` | Build script |

## How to Run

### 1. Compile the code
```bash
g++ -std=c++17 -Wall -Wextra -o attendance main.cpp Admin.cpp Student.cpp Utils.cpp
```
or simply:
```bash
make
```
### 2. Run the executable
```bash
./attendance
```
### 3. Log in with the default admin credentials
```
Username: admin
Password: admin
```
The default account is created automatically on first run. Use it to create your own admin accounts.

---

## Data Files
All data is stored as tab-separated text files in the directory the program runs from:

| File | Contents |
|------|----------|
| `AdminAccount.txt` | `username  password` for each admin |
| `studentlist.txt` | `username  name` for each enrolled student |
| `UsnFile.txt` | Sorted list of student usernames (roll-number index) |
| `<username>.txt` | One line per entry: `username  name  status  day  month  year` (status: `2` = enrolled, `1` = present, `0` = absent) |

## How It Works
- The admin logs in (default credentials above) and picks an option from the menu
- Enrolling a student creates their attendance log and adds them to the student list and roll-number index
- Marking attendance appends a dated present/absent entry to each student's log
- The attendance list and student view calculate the percentage from the log (0% when nothing is recorded yet)
- A student logs in with their username to see their date-wise history and percentage
