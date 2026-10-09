#include <stdio.h>
#include <string.h>
#include "record.h"
#include "course.h"

#define MAX_STUDENTS 200

typedef struct
{
    char name[100];
    int roll;
    int presentCount;
    int totalClasses;
    float percentage;
} AttendanceRecord;


/* =========================================
   OVERALL CLASS ATTENDANCE REPORT
   ========================================= */
void overallAttendanceReport(void)
{
    FILE *fp;
    char line[500];

    AttendanceRecord students[MAX_STUDENTS];
    AttendanceRecord temp;

    char courseCode[50];
    char courseName[100];
    char filename[100];

    int count = 0;
    int i, j;

    /*
        Select a course first
    */
    printf("\n========================================\n");
    printf("       SELECT COURSE FOR REPORT\n");
    printf("========================================\n");

    if (!selectCourse(courseCode, courseName))
    {
        printf("\nCourse selection cancelled.\n");
        return;
    }

    /*
        Course attendance files are stored like:

        data/CSE1101.txt
        data/CSE1102.txt
        data/CSE1103.txt
    */

    sprintf(filename, "data/%s.txt", courseCode);

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("\nNo attendance record found for %s.\n", courseCode);
        printf("Course: %s\n", courseName);
        return;
    }

    /*
        Read attendance records for the selected course
    */

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (count >= MAX_STUDENTS)
            break;

        /*
            Expected format:

            *Student Name,Roll,Present,Total,Percentage%

            Example:

            *Amina Rahman,101,4,5,80.00%
        */

        if (sscanf(line,
                   "%99[^,],%d,%d,%d,%f%%",
                   students[count].name,
                   &students[count].roll,
                   &students[count].presentCount,
                   &students[count].totalClasses,
                   &students[count].percentage) == 5)
        {
            /*
                Calculate percentage again so that
                the report always shows the correct value.
            */

            if (students[count].totalClasses > 0)
            {
                students[count].percentage =
                    ((float)students[count].presentCount /
                     students[count].totalClasses) * 100.0f;
            }
            else
            {
                students[count].percentage = 0.0f;
            }

            count++;
        }
    }

    fclose(fp);

    /*
        No records found
    */

    if (count == 0)
    {
        printf("\nNo valid attendance records found for this course.\n");
        return;
    }

    /*
        Sort attendance from LOWEST to HIGHEST
    */

    for (i = 0; i < count - 1; i++)
    {
        for (j = 0; j < count - i - 1; j++)
        {
            if (students[j].percentage >
                students[j + 1].percentage)
            {
                temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    /*
        Display course-wise report
    */

    printf("\n");
    printf("====================================================================\n");
    printf("              OVERALL COURSE ATTENDANCE REPORT\n");
    printf("====================================================================\n");

    printf("Course Code : %s\n", courseCode);
    printf("Course Name : %s\n", courseName);

    printf("====================================================================\n");

    printf("%-30s %-8s %-10s %-10s %-12s\n",
           "Student Name",
           "Roll",
           "Present",
           "Classes",
           "Attendance");

    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < count; i++)
    {
        printf("%-30s %-8d %-10d %-10d %8.2f%%\n",
               students[i].name,
               students[i].roll,
               students[i].presentCount,
               students[i].totalClasses,
               students[i].percentage);
    }

    printf("====================================================================\n");

    printf("\nStudents are sorted from lowest to highest attendance.\n");
}


/* =========================================
   SEARCH STUDENT RECORD BY ROLL
   ========================================= */
void recordForCourse(const char courseCode[])
{
    FILE *fp;
    char line[500];

    int searchRoll;
    int found = 0;

    char name[100];
    int roll;
    int presentCount;
    int totalClasses;
    float percentage;

    const float ATTENDANCE_THRESHOLD = 75.0f;

    printf("\n========================================\n");
    printf("              RECORDS\n");
    printf("========================================\n");

    printf("Enter student roll: ");
    scanf("%d", &searchRoll);

    char attendanceFileName[150];

snprintf(attendanceFileName, sizeof(attendanceFileName),
         "data/%s.txt", courseCode);

fp = fopen(attendanceFileName, "r");

    if (fp == NULL)
    {
        printf("\nError: Could not open attendance file.\n");
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line,
                   "%99[^,],%d,%d,%d,%f%%",
                   name,
                   &roll,
                   &presentCount,
                   &totalClasses,
                   &percentage) == 5)
        {
            if (roll == searchRoll)
            {
                printf("\n========================================\n");
                printf("          STUDENT ATTENDANCE\n");
                printf("========================================\n");

                printf("\nStudent Name  : %s\n", name);
                printf("Roll Number   : %d\n", roll);
                printf("Present       : %d\n", presentCount);
                printf("Total Classes : %d\n", totalClasses);
                printf("Attendance    : %.2f%%\n", percentage);

                printf("========================================\n");

                if (percentage < 60.0f)
                {
                    printf("\nAttendance is very low.\n");
                    printf("Please contact your course teacher.\n");
                }
                else if (percentage < 70.0f)
                {
                    printf("\nPlease attend your classes regularly.\n");
                }
                else if (percentage < ATTENDANCE_THRESHOLD)
                {
                    printf("\nWarning: Attendance below %.0f%%!\n",
                           ATTENDANCE_THRESHOLD);
                }
                else
                {
                    printf("\nAttendance status: Good standing.\n");
                }

                found = 1;
                break;
            }
        }
    }

    fclose(fp);

    if (!found)
    {
        printf("\nNo record found for roll %d.\n", searchRoll);
    }
}


/* =========================================
   STUDENT'S OWN ATTENDANCE RECORD
   ========================================= */
void studentRecord(int loggedInRoll)
{
    FILE *courseFile;
    FILE *attendanceFile;
    FILE *studentFile;

    char courseCode[50];
    char courseName[100];
    char filename[150];
    char line[500];

    char studentName[100] = "Unknown";
    char name[100];

    int roll;
    int present;
    int total;
    float percentage;

    int foundAny = 0;

    /* Find the student's name */
    studentFile = fopen("data/student-new.txt", "r");

    if (studentFile != NULL)
    {
        while (fgets(line, sizeof(line), studentFile) != NULL)
        {
            if (sscanf(line, "%99[^,],%d",
                       name, &roll) == 2)
            {
                if (roll == loggedInRoll)
                {
                    strcpy(studentName, name);
                    break;
                }
            }
        }

        fclose(studentFile);
    }

    printf("\n========================================\n");
    printf("       MY COURSEWISE ATTENDANCE\n");
    printf("========================================\n");

    printf("Student Name : %s\n", studentName);
    printf("Roll Number  : %d\n\n", loggedInRoll);

    printf("%-12s %-35s %8s %7s %12s\n",
           "Course Code", "Course Name",
           "Present", "Total", "Attendance");

    printf("--------------------------------------------------------------------------\n");

    /* Read the list of courses */
    courseFile = fopen("data/courses.txt", "r");

    if (courseFile == NULL)
    {
        printf("Error: Could not open data/courses.txt\n");
        return;
    }

    while (fgets(line, sizeof(line), courseFile) != NULL)
    {
        /* Expected format: course code,course name */
        if (sscanf(line, "%49[^,],%99[^\r\n]",
                   courseCode, courseName) != 2)
        {
            continue;
        }

        /* Remove spaces before the course name */
        while (courseName[0] == ' ')
        {
            memmove(courseName, courseName + 1,
                    strlen(courseName));
        }

        /* Build the attendance filename */
        snprintf(filename, sizeof(filename),
                 "data/%s.txt", courseCode);

        attendanceFile = fopen(filename, "r");

        if (attendanceFile == NULL)
        {
            continue;
        }

        /* Search this course for the logged-in student */
        while (fgets(line, sizeof(line), attendanceFile) != NULL)
        {
            if (sscanf(line, "%99[^,],%d,%d,%d,%f%%",
                       name, &roll, &present,
                       &total, &percentage) == 5)
            {
                if (roll == loggedInRoll)
                {
                    if (total > 0)
                    {
                        percentage =
                            (float)present * 100.0f / total;
                    }
                    else
                    {
                        percentage = 0.0f;
                    }

                    printf("%-12s %-35.35s %8d %7d %11.2f%%\n",
                           courseCode, courseName,
                           present, total, percentage);

                    foundAny = 1;
                    break;
                }
            }
        }

        fclose(attendanceFile);
    }

    fclose(courseFile);

    if (!foundAny)
    {
        printf("No course attendance records found for roll %d.\n",
               loggedInRoll);
    }

    printf("==========================================================================\n");
}
