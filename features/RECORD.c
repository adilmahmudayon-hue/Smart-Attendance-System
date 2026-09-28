#include <stdio.h>
#include "record.h"

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

    int count = 0;
    int i, j;

    fp = fopen("data/attendance.txt", "r");

    if (fp == NULL)
    {
        printf("\nError: Could not open attendance file.\n");
        return;
    }

    /* Read all attendance records */
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (count >= MAX_STUDENTS)
            break;

        if (sscanf(line,
                   "%99[^,],%d,%d,%d,%f%%",
                   students[count].name,
                   &students[count].roll,
                   &students[count].presentCount,
                   &students[count].totalClasses,
                   &students[count].percentage) == 5)
        {
            /* Calculate percentage again */
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
        printf("\nNo valid attendance records found.\n");
        return;
    }

    /* Sort from lowest attendance to highest attendance */
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

    /* Display report */
    printf("\n");
    printf("====================================================================\n");
    printf("              OVERALL CLASS ATTENDANCE REPORT\n");
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
void record(void)
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

    fp = fopen("data/attendance.txt", "r");

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
    FILE *fp;
    char line[500];

    char name[100];
    int roll;
    int presentCount;
    int totalClasses;
    float percentage;

    int found = 0;

    fp = fopen("data/attendance.txt", "r");

    if (fp == NULL)
    {
        printf("\nNo attendance records found yet.\n");
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
            if (roll == loggedInRoll)
            {
                printf("\n========================================\n");
                printf("        MY ATTENDANCE RECORD\n");
                printf("========================================\n");

                printf("\nStudent Name  : %s\n", name);
                printf("Roll Number   : %d\n", roll);
                printf("Present       : %d\n", presentCount);
                printf("Total Classes : %d\n", totalClasses);
                printf("Attendance    : %.2f%%\n", percentage);

                printf("========================================\n");

                found = 1;
                break;
            }
        }
    }

    fclose(fp);

    if (!found)
    {
        printf("\nNo attendance record found for your account yet.\n");
    }
}