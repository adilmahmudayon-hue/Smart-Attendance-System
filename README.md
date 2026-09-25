# Smart-Attendance-System
A menu-driven Smart Attendance System developed in C for CSE structured programming lab project





## Workflow

The Smart Attendance System follows a simple menu-driven workflow:

1. **New Registration**
   - Enter the student's name, roll number, department, session, and email.
   - The student's information is stored in `data/student-new.txt`.

2. **Present Call**
   - The system loads all registered students.
   - The teacher/instructor marks each student as **Present (P)** or **Absent (A)**.
   - The system updates the total classes, present count, and attendance percentage.
   - Attendance records are stored in `data/attendance.txt`.

3. **Records**
   - Enter a student's roll number.
   - The system searches the attendance records.
   - The student's name, roll number, days present, total classes, and attendance percentage are displayed.

4. **Exit**
   - The system safely exits the program.

### Overall Flow

```text
Start
  ↓
Main Menu
  ↓
┌───────────────────────┐
│ 1. New Registration   │
│ 2. Present Call       │
│ 3. Records            │
│ 4. Exit               │
└───────────────────────┘
  ↓
Registration → Store Student Information
  ↓
Present Call → Mark P/A → Update Attendance
  ↓
Records → Search by Roll → Display Attendance
  ↓
Exit


## 👥 Contributors

This project was developed by:

| Contributor          | GitHub                                                       |

| Adil Mahmud Ayon | [@adilmahmudayon-hue](https://github.com/adilmahmudayon-hue) |
| Agantor Aghosh   | [@agantoraghosh-prog](https://github.com/agantoraghosh-prog) |
| Mitaly Oyshe    | [@mitalyoyshe](https://github.com/mitalyoyshe)               |

