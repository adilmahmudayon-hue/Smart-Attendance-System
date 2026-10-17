# 🎓 Smart Attendance System in C

A menu-driven, file-based **Smart Attendance System** developed in C for the CSE 1102 Structured Programming Lab project. The system manages student registration, course information, teacher-course assignments, and attendance records through separate admin, teacher, and student workflows.

The project uses modular C programming and text files to organize information and maintain attendance records course-wise.

## ✨ Features

### 🔐 1. Role-Based Login

The system provides separate login interfaces for:

* **Admin** — manages student registration, courses, and overall attendance reports.
* **Teacher** — accesses assigned courses, takes attendance, and views course records.
* **Student** — views personal course-wise attendance records.

### 🛡️ 2. Admin Panel

The admin menu provides the following options:

* **Manage Students:** access student registration and student record management.
* **Manage Courses:** manage course information and teacher-course assignments.
* **Overall Attendance Record View:** view attendance records organized by course.
* **Logout:** return to the login menu.

Student registration includes duplicate checking, registration record deletion, and sorting functionality.

### 👨‍🏫 3. Teacher Panel

Teachers can access the courses assigned to them.

The workflow is:

1. Log in as a teacher.
2. View the assigned-course list.
3. Select a course.
4. Choose an attendance or record option.
5. Return to the assigned-course list when finished.

Available course options:

* **Present Call:** record attendance for the selected course.
* **Records:** view attendance records for that course.
* **Back to assigned courses:** select another assigned course.

### 📚 4. Course Management and Assignment

The system organizes academic information by course.

* Maintain course codes and course names.
* Associate teachers with their assigned courses.
* Support unassigning a course from a teacher.
* Allow teachers to work with their assigned courses.

Course and assignment information is stored in text files.

### 📝 5. Attendance Management

* Take attendance for a selected course.
* Record student attendance statuses.
* Maintain course-specific attendance information.
* Store session-level attendance data separately where applicable.
* Calculate attendance percentages from attendance counts.

### 📊 6. Course-Wise Attendance Reports

The system provides course-wise attendance reporting rather than relying on a single combined attendance file.

**Student reports** display the student's information alongside course details, including:

| Field       | Description                |
| ----------- | -------------------------- |
| Course Code | Identifier of the course   |
| Course Name | Name of the course         |
| Present     | Number of classes attended |
| Total       | Total classes held         |
| Attendance  | Attendance percentage      |

The admin's overall report and the teacher's course records provide additional ways to review attendance.

### ⚠️ 7. Attendance Percentage and Threshold

Attendance percentage is calculated using:

$$
\text{Attendance Percentage} =
\frac{\text{Classes Attended}}{\text{Total Classes}} \times 100
$$

The project uses a **75% attendance threshold** for identifying low attendance and displaying warnings where implemented.

### 💾 8. File-Based Data Storage

The project uses text files instead of a database. This makes the storage mechanism suitable for demonstrating C file handling, reading, writing, and updating records.

The files store student information, course information, teacher information, course assignments, and course-wise attendance data.

## 🔄 System Workflow

```text
                 SMART ATTENDANCE SYSTEM
                           |
                       Login Menu
                           |
          +----------------+----------------+
          |                |                |
         Admin           Teacher          Student
          |                |                |
    Admin Dashboard   Assigned Courses   Student Menu
          |                |                |
    +-----+------+     Select Course       Records
    |     |      |           |
 Students Courses Overall   +--------+
             |    Records   |        |
         Assign/Unassign Present   Records
             Teachers     Call
          |
       Logout
```

## 🗂️ Project Structure

```text
Smart-Attendance-System/
│
├── .vscode/
│   ├── launch.json
│   └── tasks.json
│
├── data/
│   ├── courses.txt
│   ├── course-assignments.txt
│   ├── teacher.txt
│   ├── student-new.txt
│   ├── logininputs.txt
│   ├── command.txt
│   ├── CSE 1101.txt
│   ├── CSE 1101-session.txt
│   ├── HUM 1108.txt
│   ├── HUM 1108-session.txt
│   └── MATH 1107.txt
│
├── features/
│   ├── course.c
│   ├── course.h
│   ├── managecourses.c
│   ├── managecourses.h
│   ├── present.c
│   ├── present.h
│   ├── RECORD.c
│   ├── record.h
│   ├── REGISTRATION.c
│   └── registration.h
│
├── src/
│   ├── main.c
│   └── previous/
│
├── .gitignore
└── README.md
```

*Note: The tree illustrates the project layout and representative data files. Course-specific attendance and session files may vary as courses are added or updated.*

## 🧩 Module Responsibilities

| File                       | Responsibility                                                                  |
| -------------------------- | ------------------------------------------------------------------------------- |
| `src/main.c`               | Main application flow, login menus, and role-based navigation                   |
| `features/REGISTRATION.c`  | Student registration and registration record operations                         |
| `features/registration.h`  | Registration module declarations                                                |
| `features/present.c`       | Attendance-taking and attendance file operations                                |
| `features/present.h`       | Attendance module declarations                                                  |
| `features/RECORD.c`        | Attendance record viewing and reporting                                         |
| `features/record.h`        | Record module declarations                                                      |
| `features/course.c`        | Course-related operations                                                       |
| `features/course.h`        | Course module declarations                                                      |
| `features/managecourses.c` | Admin-side course management and assignments                                    |
| `features/managecourses.h` | Course management declarations                                                  |
| `data/`                    | Text-based storage for students, teachers, courses, assignments, and attendance |

## 🛠️ Technologies and Concepts

**Language:** C

**Tools:** GCC, Visual Studio Code, Git, and GitHub

Programming concepts used:

* Functions and modular programming
* Conditional statements and loops
* Arrays and strings
* Pointers and function parameters
* File handling
* Header files and multi-file compilation
* Input validation and duplicate checking
* Searching, sorting, and record management
* Percentage calculations

## 🚀 Installation and Execution

### Prerequisites

* GCC compiler installed and available in your terminal.
* Git, if cloning the repository.
* Visual Studio Code, if using the included build configuration.

### 1. Clone the Repository

```bash
git clone https://github.com/adilmahmudayon-hue/Smart-Attendance-System.git
cd Smart-Attendance-System
```

### 2. Compile from the Project Root

Run this command from the root directory of the project:

```powershell
gcc src/main.c features/REGISTRATION.c features/present.c features/RECORD.c features/course.c features/managecourses.c -Ifeatures -o attendance.exe
```

### 3. Run the Application

```powershell
.\attendance.exe
```

If compilation fails, check the error messages before running the executable. If Windows reports that the output executable is locked, close the running application and compile again.

### 4. Run through Visual Studio Code

The repository includes `.vscode/tasks.json` and `.vscode/launch.json` for the VS Code build and debugging workflow. Open the project root folder in VS Code and use the configured build/debug options.

**Important:** Run the program from the project root so that relative paths to the `data/` directory resolve correctly.

## 👥 Contributors

* **Adil Mahmud Ayon** — [@adilmahmudayon-hue](https://github.com/adilmahmudayon-hue)
* **Antora Ghosh** — [@agantoraghosh-prog](https://github.com/agantoraghosh-prog)
* **Mitaly Farzana Oishe** — [@mitalyoyshe](https://github.com/mitalyoyshe)

## 🎯 Project Objective

The objective of this project is to apply structured programming concepts in C to develop a practical attendance management application. It demonstrates modular program design, text-file storage, role-based menus, student registration, course management, and course-wise attendance reporting.

## 🔗 Repository

[Smart Attendance System on GitHub](https://github.com/adilmahmudayon-hue/Smart-Attendance-System)
