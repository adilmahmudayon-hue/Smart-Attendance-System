# 📚 Smart Attendance System

A **C-based console application** designed to simplify student attendance management for teachers and students.

The system provides separate access for **teachers and students**, allowing teachers to register students, take attendance, and view attendance records, while students can securely log in and view their own attendance information.

---

## 🎯 Project Overview

Managing attendance manually can be time-consuming and can make it difficult to maintain accurate records.

The **Smart Attendance System** provides a simple digital solution where attendance information is stored in files and managed through a structured C program.

The project is being developed with a modular approach so that individual features can be maintained and expanded independently.

---

## ✨ Current Features

### 👨‍🏫 Teacher Module

Teachers can:

- 🔐 Log in using teacher credentials
- 📝 Register new students
- ✅ Take attendance
- 📊 View individual student attendance records
- 📈 Calculate attendance percentages
- ⚠️ Receive attendance-status messages based on attendance percentage

### 👨‍🎓 Student Module

Students can:

- 🔐 Log in using their registered credentials
- 📊 View their own attendance record
- 📈 Check:
  - Classes attended
  - Total classes
  - Attendance percentage

### 💾 Data Management

The system currently uses text files for persistent storage.

| File | Purpose |
|---|---|
| `student-new.txt` | Stores registered student information |
| `attendance.txt` | Stores attendance records |
| `logininputs.txt` | Stores login-related input data |
| `command.txt` | Stores project-related command/input information |

---



## 👥 Our Team



### 🔗 Team Members

- **Adil Mahmud Ayon** — [GitHub Profile](https://github.com/adilmahmudayon-hue)
- **Antora Ghosh** — [GitHub Profile](https://github.com/agantoraghosh-prog)
- **Mitaly Farzana Oishe** — [GitHub Profile](https://github.com/mitalyoyshe)

## 🏗️ Project Structure

```text
Smart-Attendance-System/
│
├── data/
│   ├── attendance.txt
│   ├── command.txt
│   ├── logininputs.txt
│   └── student-new.txt
│
├── features/
│   ├── present.c
│   ├── RECORD.c
│   ├── record.h
│   └── REGISTRATION.c
│
├── src/
│   ├── main.c
│   └── previous/
│       ├── premain.c
│       └── prerecord.c
│
├── .gitignore
├── LICENSE
└── README.md