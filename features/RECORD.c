#include <stdio.h>
#include "record.h"

/*
    Reads and displays attendance information
    from data/attendance.txt.

    File format:
    Name,Roll,Present,TotalClasses,Percentage%
*/

/* =========================================
   TEACHER RECORD
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

    const float ATTENDANCE_THRESHOLD = 75.0;

    printf("\n========================================\n");
    printf("              RECORDS\n");
    printf("========================================\n");

    printf("Enter student roll: ");
    scanf("%d", &searchRoll);

    fp = fopen("../data/attendance.txt", "r");

    if (fp == NULL)
    {
        printf("\nNo attendance records found.\n");
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        /*
            Expected format:
            Name,Roll,Present,TotalClasses,Percentage%
        */

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

                if (percentage < 60.0)
                {
                    printf("\nAttendance is very low.\n");
                    printf("Please contact your course teacher.\n");
                }
                else if (percentage < 70.0)
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
   STUDENT OWN RECORD
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

    fp = fopen("../data/attendance.txt", "r");

    if (fp == NULL)
    {
        printf("\nNo attendance records found yet.\n");
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        /*
            Expected format:
            Name,Roll,Present,TotalClasses,Percentage%
        */

        if (sscanf(line,
                   "%99[^,],%d,%d,%d,%f%%",
                   name,
                   &roll,
                   &presentCount,
                   &totalClasses,
                   &percentage) == 5)
        {
            /*
                Only display the attendance belonging
                to the currently logged-in student.
            */

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