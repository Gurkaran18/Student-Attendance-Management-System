# Student Attendance Management System

A menu-driven C++17 attendance system with Admin and Student roles, file-based
persistence, and no third-party dependencies.

## Build

```
make
```

Or directly:

```
g++ -std=c++17 -Wall -Wextra -pedantic -o main \
    main.cpp AttendanceSystem.cpp Admin.cpp Student.cpp User.cpp Util.cpp
```

Run with `./main`. Clean with `make clean`.

> **macOS note.** Some Command Line Tools installs ship a stale, partial
> `/Library/Developer/CommandLineTools/usr/include/c++/v1` that shadows the
> complete libc++ inside the SDK, making every `#include <string>` fail. The
> Makefile detects this case and falls back to the SDK's libc++
> (`-nostdinc++ -isystem "$(xcrun --show-sdk-path)/usr/include/c++/v1"`).
> The durable fix is to reinstall the tools:
> `sudo rm -rf /Library/Developer/CommandLineTools && xcode-select --install`.

## Design

| File | Contents |
| --- | --- |
| `User.h/.cpp` | Abstract base. Private identifier and password hash, `verifyPassword()`, shared password policy, pure virtual `login()` / `showMenu()`, virtual destructor. |
| `Admin.h/.cpp` | `Admin : public User`. Enrollment, admin account creation, attendance marking, attendance list, batch sensor import. Owns `AdminAccount.txt`. |
| `Student.h/.cpp` | `Student : public User` holding its USN, name and attendance records; owns `<USN>.txt`. Also `StudentRoster` (owns `studentlist.txt` and `UsnFile.txt`) and `recursiveInsertionSort`. |
| `AttendanceSystem.h/.cpp` | Thin controller: top-level menu, constructs the right `User` subclass and drives it through a `User&`. |
| `Util.h/.cpp` | Validated console input, `Date`, file-existence check. |

Each class reads and writes only the files it owns. There are no public data
members.

## Data files

On-disk formats are unchanged and backward compatible.

| File | Format |
| --- | --- |
| `AdminAccount.txt` | `username<TAB>passwordHash` |
| `studentlist.txt` | `USN<TAB>name` |
| `UsnFile.txt` | One USN per line, kept sorted |
| `<USN>.txt` | `USN<TAB>name<TAB>status<TAB>day<TAB>month<TAB>year`; the first row is the enrollment marker (status `2`) and is not counted toward attendance; `1` = present, `0` = absent |
| `rfid_sensor_logs.txt` | `USN name status day month year`, one record per line |
| `error_log.txt` | Appended rejection reasons, with source line numbers |

## Password rules

8 to 20 characters, with at least one uppercase letter, one digit, and one
special character from ``@ & _ * ( ) # $ ^ . ,``. Passwords are stored as a
`std::hash<std::string>` digest, never in plaintext.
