#include <stdio.h>
#include "present.h"

#define MAX_STUDENTS 100

void present(void)
{
    FILE *studentFile;
    FILE *attendanceFile;

    char line[500];

    char name[MAX_STUDENTS][100];
    int roll[MAX_STUDENTS];
    int presentCount[MAX_STUDENTS];
    int totalClasses[MAX_STUDENTS];

    int studentCount = 0;
    int i;
    char status;

    /* =========================================
       OPEN STUDENT REGISTRATION FILE
       ========================================= */

    studentFile = fopen("../data/student-new.txt", "r");

    if (studentFile == NULL)
    {
        printf("\nNo student registration file found.\n");
        return;
    }

    /* =========================================
       READ STUDENTS
       Format:
       Name,Roll,Department,Session,Email,Password
       ========================================= */

    while (fgets(line, sizeof(line), studentFile) != NULL &&
           studentCount < MAX_STUDENTS)
    {
        if (sscanf(line,
                   "%99[^,],%d",
                   name[studentCount],
                   &roll[studentCount]) == 2)
        {
            studentCount++;
        }
    }

    fclose(studentFile);

    if (studentCount == 0)
    {
        printf("\nNo students are registered.\n");
        return;
    }

    /* =========================================
       INITIALIZE ATTENDANCE ARRAYS
       ========================================= */

    for (i = 0; i < studentCount; i++)
    {
        presentCount[i] = 0;
        totalClasses[i] = 0;
    }

    /* =========================================
       READ PREVIOUS ATTENDANCE
       ========================================= */

    attendanceFile = fopen("../data/attendance.txt", "r");

    if (attendanceFile != NULL)
    {
        while (fgets(line, sizeof(line), attendanceFile) != NULL)
        {
            char oldName[100];
            int oldRoll;
            int oldPresent;
            int oldTotal;

            if (sscanf(line,
                       "%99[^,],%d,%d,%d",
                       oldName,
                       &oldRoll,
                       &oldPresent,
                       &oldTotal) == 4)
            {
                for (i = 0; i < studentCount; i++)
                {
                    if (roll[i] == oldRoll)
                    {
                        presentCount[i] = oldPresent;
                        totalClasses[i] = oldTotal;
                        break;
                    }
                }
            }
        }

        fclose(attendanceFile);
    }

    /* =========================================
       THIS RUN = ONE NEW CLASS
       ========================================= */

    for (i = 0; i < studentCount; i++)
    {
        totalClasses[i]++;
    }

    /* =========================================
       PRESENT CALL
       ========================================= */

    printf("\n========================================\n");
    printf("              PRESENT CALL\n");
    printf("========================================\n");

    for (i = 0; i < studentCount; i++)
    {
        printf("\n%d. %s", i + 1, name[i]);
        printf(" (Roll: %d)\n", roll[i]);

        printf("Enter P for Present / A for Absent: ");
        scanf(" %c", &status);

        if (status == 'P' || status == 'p')
        {
            presentCount[i]++;

            printf("%s marked Present.\n", name[i]);
        }
        else if (status == 'A' || status == 'a')
        {
            printf("%s marked Absent.\n", name[i]);
        }
        else
        {
            printf("Invalid input. Student marked Absent.\n");
        }
    }

    /* =========================================
       SAVE UPDATED ATTENDANCE
       ========================================= */

    attendanceFile = fopen("../data/attendance.txt", "w");

    if (attendanceFile == NULL)
    {
        printf("\nError: Could not open attendance file.\n");
        return;
    }

    for (i = 0; i < studentCount; i++)
    {
        float percentage = 0.0;

        if (totalClasses[i] > 0)
        {
            percentage =
                ((float)presentCount[i] /
                 totalClasses[i]) *
                100.0;
        }

        fprintf(attendanceFile,
                "%s,%d,%d,%d,%.2f%%\n",
                name[i],
                roll[i],
                presentCount[i],
                totalClasses[i],
                percentage);
    }

    fclose(attendanceFile);

    printf("\n========================================\n");
    printf("Attendance saved successfully!\n");
    printf("========================================\n");
}