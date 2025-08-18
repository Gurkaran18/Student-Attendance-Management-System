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

---

## Project Structure
```bash
.
├── main.cpp # Main source code (menus, logic)
├── AdminAccount.txt # Stores admin credentials
├── studentlist.txt # List of enrolled students
├── UsnFile.txt # Stores sorted student IDs
├── <username>.txt # Individual student attendance files
```

## How It Works
- User logs in with default credentials
- Inputs student name, roll number, and attendance
- Data is saved to a text file for persistence
- Records can be viewed or sorted alphabetically
- Simple CLI menu allows interaction with the system
