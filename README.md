# Student Attendance Management System (C++ OOP)

A console-based attendance management system built using **Object-Oriented Programming in C++**.  
This project allows **Admins** to manage students, mark attendance, and view reports, while **Students** can log in to view their attendance records.

---

## Features

### Admin
- Admin login (with secure username & password validation)
- Add new students
- Create new admin accounts
- Mark attendance (present/absent for each student)
- View complete student attendance list

### Student
- Student login with username
- Check attendance history (present/absent per date)
- View attendance percentage

---

## Tech Stack
- **Language:** C++ (Object-Oriented Programming)
- **Concepts Used:**  
  - Classes & Objects  
  - File Handling (read/write student & attendance records)  
  - Data Validation (password rules, duplicate student check)  
  - Sorting (Insertion Sort for student records)  
  - Modular Programming  

## How to Run

### 1. Compile the Code
```bash
g++ main.cpp -o attendance
```
### 2. Run the executable
```bash
./OOPS
```
### 3. Login with credentials
```bash
Username: admin
Password: admin
```
---

## How It Works
- User logs in with default credentials
- Inputs student name, roll number, and attendance
- Data is saved to a text file for persistence
- Records can be viewed or sorted alphabetically
- Simple CLI menu allows interaction with the system
