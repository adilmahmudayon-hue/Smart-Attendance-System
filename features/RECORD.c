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
    char comment[300];
} AttendanceRecord;
 void getTeacherComment(const char line[], char comment[], int size)
{
    const char *p = line;
    int commas = 0;

    comment[0] = '\0';

    while (*p != '\0' && commas < 5)
    {
        if (*p == ',')
        {
            commas++;
        }

        p++;
    }

    if (commas == 5)
    {
        snprintf(comment, size, "%s", p);

        comment[strcspn(comment, "\r\n")] = '\0';
    }
}


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

    
    printf("\n\n");
    printf("\tSELECT COURSE FOR REPORT\n");
    printf("\n");

    if (!selectCourse(courseCode, courseName))
    {
        printf("\nCourse selection cancelled.\n");
        return;
    }


    sprintf(filename, "data/%s.txt", courseCode);

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("\nNo attendance record found for %s.\n", courseCode);
        printf("Course: %s\n", courseName);
        return;
    }


    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (count >= MAX_STUDENTS)
            break;

        //Student Name,Roll,Present,Total,Percentage%

    

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

   

    if (count == 0)
    {
        printf("\nNo valid attendance records found for this course.\n");
        return;
    }

    
       // Sort attendance from LOWEST to HIGHEST
    

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

    
       // Display course-wise report
    

    printf("\n\n");
    printf("\t\tOVERALL COURSE ATTENDANCE REPORT\n");
    printf("\n\n");

    printf("Course Code : %s\n", courseCode);
    printf("Course Name : %s\n", courseName);

    printf("\n\n");

    printf("%-30s %-8s %-10s %-10s %-12s\n\n",
           "Student Name",
           "Roll",
           "Present",
           "Classes",
           "Attendance");

    printf("\n");

    for (i = 0; i < count; i++)
    {
        printf("%-30s %-8d %-10d %-10d %8.2f%%\n",
               students[i].name,
               students[i].roll,
               students[i].presentCount,
               students[i].totalClasses,
               students[i].percentage);
    }

    printf("\n\n");

    printf("\nStudents are sorted from lowest to highest attendance.\n");
}

//student record by roll search

void recordForCourse(const char courseCode[])
{
    FILE *fp;
    char filename[150];
    char line[600];
    char lines[MAX_STUDENTS][600];
    char comment[300];
    char name[100];

    int roll;
    int present;
    int total;
    float percentage;
    int searchRoll;
    int count = 0;
    int found = -1;
    int i;

    snprintf(filename, sizeof(filename), "data/%s.txt", courseCode);

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("\nError: Could not open attendance file.\n");
        return;
    }

    while (count < MAX_STUDENTS &&
           fgets(lines[count], sizeof(lines[count]), fp) != NULL)
    {
        count++;
    }

    fclose(fp);

    printf("\n\tSTUDENT ATTENDANCE RECORDS\n");
    printf("Enter student roll: ");

    if (scanf("%d", &searchRoll) != 1)
    {
        while (getchar() != '\n' && !feof(stdin))
        {
        }

        printf("Invalid roll number.\n");
        return;
    }

    while (getchar() != '\n' && !feof(stdin))
    {
    }

    for (i = 0; i < count; i++)
    {
        if (sscanf(lines[i], "%99[^,],%d,%d,%d,%f%%",
                   name, &roll, &present, &total, &percentage) == 5)
        {
            if (roll == searchRoll)
            {
                found = i;
                break;
            }
        }
    }

    if (found == -1)
    {
        printf("\nNo record found for roll %d.\n", searchRoll);
        return;
    }

    getTeacherComment(lines[found], comment, sizeof(comment));

    printf("\nStudent Name  : %s\n", name);
    printf("Roll Number   : %d\n", searchRoll);
    printf("Present       : %d\n", present);
    printf("Total Classes : %d\n", total);
    printf("Attendance    : %.2f%%\n", percentage);
    printf("Current Comment: %s\n",
           comment[0] ? comment : "No comment yet.");

    printf("\nEnter teacher comment: ");

    if (fgets(comment, sizeof(comment), stdin) == NULL)
    {
        printf("Could not read the comment.\n");
        return;
    }

    comment[strcspn(comment, "\r\n")] = '\0';

    /*
       Update the matching record in memory.
       No separate comment file is created.
    */
    if (total > 0)
    {
        percentage = (float)present * 100.0f / total;
    }
    else
    {
        percentage = 0.0f;
    }

    snprintf(lines[found], sizeof(lines[found]),
             "%s,%d,%d,%d,%.2f%%,%s\n",
             name, roll, present, total, percentage, comment);

    /*
       Rewrite the existing course attendance file.
    */
    fp = fopen(filename, "w");

    if (fp == NULL)
    {
        printf("\nError: Could not save the comment.\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        fputs(lines[i], fp);
    }

    fclose(fp);

    printf("\nTeacher comment saved successfully.\n");
}


//student's own record


void studentRecord(int loggedInRoll)
{
    FILE *courseFile;
    FILE *attendanceFile;
    FILE *studentFile;

    char courseCode[50];
    char courseName[100];
    char filename[150];
    char line[600];

    char studentName[100] = "Unknown";
    char name[100];
    char comment[300];

    int roll;
    int present;
    int total;
    float percentage;
    int foundAny = 0;

    studentFile = fopen("data/student-new.txt", "r");

    if (studentFile != NULL)
    {
        while (fgets(line, sizeof(line), studentFile) != NULL)
        {
            if (sscanf(line, "%99[^,],%d", name, &roll) == 2)
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

    printf("\n\tMY COURSEWISE ATTENDANCE\n\n");
    printf("Student Name : %s\n", studentName);
    printf("Roll Number  : %d\n\n", loggedInRoll);

    printf("%-12s %-30s %7s %7s %11s  %s\n",
           "Course Code", "Course Name",
           "Present", "Total", "Attendance", "Teacher Comment");

    printf("\n");

    courseFile = fopen("data/courses.txt", "r");

    if (courseFile == NULL)
    {
        printf("Error: Could not open data/courses.txt\n");
        return;
    }

    while (fgets(line, sizeof(line), courseFile) != NULL)
    {
        if (sscanf(line, "%49[^,],%99[^\r\n]",
                   courseCode, courseName) != 2)
        {
            continue;
        }

        while (courseName[0] == ' ')
        {
            memmove(courseName, courseName + 1, strlen(courseName));
        }

        snprintf(filename, sizeof(filename),
                 "data/%s.txt", courseCode);

        attendanceFile = fopen(filename, "r");

        if (attendanceFile == NULL)
        {
            continue;
        }

        while (fgets(line, sizeof(line), attendanceFile) != NULL)
        {
            if (sscanf(line, "%99[^,],%d,%d,%d,%f%%",
                       name, &roll, &present,
                       &total, &percentage) == 5)
            {
                if (roll == loggedInRoll)
                {
                    getTeacherComment(line, comment, sizeof(comment));

                    if (total > 0)
                    {
                        percentage = (float)present * 100.0f / total;
                    }
                    else
                    {
                        percentage = 0.0f;
                    }

                    printf("%-12s %-30.30s %7d %7d %10.2f%%  %s\n",
                           courseCode, courseName,
                           present, total, percentage,
                           comment[0] ? comment : "No comment yet");

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

    printf("\n");
}
