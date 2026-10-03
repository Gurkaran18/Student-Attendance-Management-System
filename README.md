# Student Attendance Management System (C++ OOP)

A menu-driven console attendance system built with **Object-Oriented Programming in C++17**.
**Admins** enroll students, create admin accounts, and mark attendance, while **Students**
log in to view their attendance history and percentage. Standard library only — no third-party
dependencies.

---

## Features

### Admin
- Admin login with username and hashed password
- Add (enroll) new students, with duplicate enrollments blocked
- Create new admin accounts, with password rules enforced:
  8 to 20 characters, at least one uppercase letter, one digit, and one special character (`@ & _ * ( ) # $ ^ . ,`)
- Mark attendance (present/absent for each student, stamped with today's date)
- View the attendance list for all students, in roll-number order
- Batch import attendance from an RFID sensor log, with malformed rows rejected
  to a line-numbered `error_log.txt`

### Student
- Student login with university number (USN)
- Check attendance history (present/absent per date)
- View attendance percentage

---

## Tech Stack

- **Language:** C++17 (standard library only)
- **Concepts used:**
  - **Classes & Objects, Encapsulation** — all data members are private; each class
    reads and writes only the files it owns
  - **Abstraction** — abstract `User` base class with pure virtual `login()` and
    `showMenu()`, and a virtual destructor
  - **Inheritance & Polymorphism** — `Admin` and `Student` derive from `User`;
    `AttendanceSystem` drives them through a `User&`, so dispatch is resolved at run time
  - **Credential encapsulation** — the password hash is private to `User` and only
    reachable through `verifyPassword()`; passwords are stored as a
    `std::hash<std::string>` digest, never in plaintext
  - **STL** — `std::vector`, `std::string`, `std::pair`, `std::unique_ptr`
  - **File Handling** — `fstream` for accounts, roster, roll-number index and
    per-student date-stamped attendance logs
  - **Data Validation** — password rules, duplicate student check, menu and
    attendance input re-prompted until valid
  - **Sorting** — a hand-written **recursive insertion sort** over a `std::vector`
    keeps the roll-number index sorted
  - **Modular Programming** — separate headers and source files, built with a Makefile

---

## Project Structure

| File | Purpose |
|------|---------|
| `User.h` / `User.cpp` | Abstract base class for anyone who can log in. Private identifier and password hash, `verifyPassword()`, shared password policy |
| `Admin.h` / `Admin.cpp` | Admin login, admin menu, enrolling students, creating admin accounts, marking attendance, attendance list, batch sensor import |
| `Student.h` / `Student.cpp` | A student's USN, name and attendance records; attendance percentage and history. Also `StudentRoster` and the recursive insertion sort |
| `AttendanceSystem.h` / `AttendanceSystem.cpp` | Thin controller: the main menu, which builds the right `User` subclass and runs it |
| `Util.h` / `Util.cpp` | Validated console input, today's date, file-existence check |
| `main.cpp` | Entry point |
| `Makefile` | Build script |

---

## How to Run

### 1. Compile the code

```bash
make
```

or directly:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -o main \
    main.cpp AttendanceSystem.cpp Admin.cpp Student.cpp User.cpp Util.cpp
```

### 2. Run the executable

```bash
./main
```

### 3. Log in with the default admin credentials

```
Username: admin
Password: admin
```

The repository ships no accounts or attendance data — every data file is created at
runtime. The default admin account is seeded automatically the first time you choose
**1. ADMIN LOGIN**, so a fresh clone can be logged into straight away. Use it to create
your own admin account from the menu. Deleting `AdminAccount.txt` restores the default
login; student records live in separate files and are not affected.

Students have no password: an admin creates them with **2. Add students**, and a student
then signs in from the main menu with their USN.

Clean the build with `make clean`.

---

## How It Works

- The admin logs in and picks an option from the menu
- Enrolling a student creates their attendance log and adds them to the student list
  and the roll-number index, which is re-sorted by the recursive insertion sort
- Marking attendance appends a dated present/absent entry to each student's log
- The attendance list and student view calculate the percentage from the log
  (0% when nothing is recorded yet)
- Batch import reads `rfid_sensor_logs.txt`, appends valid records to the matching
  student logs, and writes every rejected row to `error_log.txt` with its line number
- A student logs in with their USN to see their date-wise history and percentage

---

## Data Files

All data is stored as tab-separated text files in the directory the program runs from.

| File | Contents |
|------|----------|
| `AdminAccount.txt` | `username  passwordHash` for each admin |
| `studentlist.txt` | `USN  name` for each enrolled student |
| `UsnFile.txt` | Sorted list of student USNs (the roll-number index) |
| `<USN>.txt` | One line per entry: `USN  name  status  day  month  year`. The first row is the enrollment marker (`status 2`) and is not counted toward attendance; `1` = present, `0` = absent |
| `rfid_sensor_logs.txt` | Sensor input for batch import: `USN name status day month year`, one record per line |
| `error_log.txt` | Rows rejected by batch import, with the source line number and reason |

---

<details>
<summary><b>macOS build note</b> (click to expand)</summary>

Some Command Line Tools installs ship a stale, partial
`/Library/Developer/CommandLineTools/usr/include/c++/v1` that shadows the complete
libc++ inside the SDK, making every `#include <string>` fail. The Makefile detects this
and falls back to the SDK's libc++. The durable fix is to reinstall the tools:

```bash
sudo rm -rf /Library/Developer/CommandLineTools && xcode-select --install
```

</details>
