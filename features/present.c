#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "present.h"

#define MAX_STUDENTS 100
#define NAME_LENGTH 100
#define LINE_LENGTH 500

#define ATTENDANCE_FILE "../data/attendance.txt"
#define SESSION_FILE "../data/attendance-session.txt"

static int save_attendance(
    const char name[][NAME_LENGTH],
    const int roll[],
    const int presentCount[],
    const int totalClasses[],
    int studentCount)
{
    FILE *file = fopen(ATTENDANCE_FILE, "w");
    int i;

    if (file == NULL)
    {
        printf("\nError: Could not save attendance.\n");
        return 0;
    }

    for (i = 0; i < studentCount; i++)
    {
        float percentage = 0.0f;

        if (totalClasses[i] > 0)
        {
            percentage =
                ((float)presentCount[i] / totalClasses[i]) * 100.0f;
        }

        fprintf(file, "%s,%d,%d,%d,%.2f%%\n",
                name[i], roll[i], presentCount[i],
                totalClasses[i], percentage);
    }

    fclose(file);
    return 1;
}

/*
 * Session file format:
 * roll,status
 *
 * status is:
 * P = present
 * A = absent
 * U = not yet called
 */
static int save_session(const int roll[], const char status[],
                        int studentCount)
{
    FILE *file = fopen(SESSION_FILE, "w");
    int i;

    if (file == NULL)
    {
        printf("\nError: Could not save the attendance session.\n");
        return 0;
    }

    for (i = 0; i < studentCount; i++)
    {
        fprintf(file, "%d,%c\n", roll[i], status[i]);
    }

    fclose(file);
    return 1;
}

void present(void)
{
    FILE *studentFile;
    FILE *attendanceFile;
    FILE *sessionFile;
    char line[LINE_LENGTH];

    char name[MAX_STUDENTS][NAME_LENGTH];
    int roll[MAX_STUDENTS];
    int presentCount[MAX_STUDENTS] = {0};
    int totalClasses[MAX_STUDENTS] = {0};
    char status[MAX_STUDENTS];

    int studentCount = 0;
    int i;
    int hasUnfinishedSession = 0;
    int choice;
    char input;

    studentFile = fopen("../data/student-new.txt", "r");

    if (studentFile == NULL)
    {
        printf("\nNo student registration file found.\n");
        return;
    }

    /* Registration format: Name,Roll,Department,Session,Email,Password */
    while (fgets(line, sizeof(line), studentFile) != NULL &&
           studentCount < MAX_STUDENTS)
    {
        if (sscanf(line, "%99[^,],%d",
                   name[studentCount], &roll[studentCount]) == 2)
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

    for (i = 0; i < studentCount; i++)
    {
        status[i] = 'U';
    }

    /* Load cumulative attendance. */
    attendanceFile = fopen(ATTENDANCE_FILE, "r");

    if (attendanceFile != NULL)
    {
        while (fgets(line, sizeof(line), attendanceFile) != NULL)
        {
            char oldName[NAME_LENGTH];
            int oldRoll;
            int oldPresent;
            int oldTotal;

            if (sscanf(line, "%99[^,],%d,%d,%d",
                       oldName, &oldRoll, &oldPresent, &oldTotal) == 4)
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

    /* Detect and load an unfinished session by matching student roll. */
    sessionFile = fopen(SESSION_FILE, "r");

    if (sessionFile != NULL)
    {
        int oldRoll;
        char oldStatus;

        while (fgets(line, sizeof(line), sessionFile) != NULL)
        {
            if (sscanf(line, "%d,%c", &oldRoll, &oldStatus) == 2)
            {
                for (i = 0; i < studentCount; i++)
                {
                    if (roll[i] == oldRoll &&
                        (oldStatus == 'P' || oldStatus == 'A' ||
                         oldStatus == 'U'))
                    {
                        status[i] = oldStatus;
                        break;
                    }
                }
            }
        }

        fclose(sessionFile);

        for (i = 0; i < studentCount; i++)
        {
            if (status[i] == 'U')
            {
                hasUnfinishedSession = 1;
                break;
            }
        }
    }

    if (hasUnfinishedSession)
    {
        printf("\nAn unfinished attendance record was found.\n");
        printf("1. Continue the previous record\n");
        printf("2. Start a new record\n");
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid choice.\n");
            return;
        }

        if (choice == 1)
        {
            printf("\nContinuing the previous attendance record.\n");
        }
        else if (choice == 2)
        {
            for (i = 0; i < studentCount; i++)
            {
                status[i] = 'U';
            }
        }
        else
        {
            printf("\nInvalid choice. Returning to the main menu.\n");
            return;
        }
    }
    else
    {
        for (i = 0; i < studentCount; i++)
        {
            status[i] = 'U';
        }
    }

    printf("\n========================================\n");
    printf("              PRESENT CALL\n");
    printf("Enter S at a prompt to stop and save.\n");
    printf("========================================\n");

    for (i = 0; i < studentCount; i++)
    {
        if (status[i] != 'U')
        {
            continue;
        }

        printf("\n%d. %s (Roll: %d)\n", i + 1, name[i], roll[i]);
        printf("Enter P for Present, A for Absent, or S to stop: ");

        if (scanf(" %c", &input) != 1)
        {
            printf("\nCould not read input. Saving and returning.\n");
            save_session(roll, status, studentCount);
            return;
        }

        input = (char)toupper((unsigned char)input);

        if (input == 'S')
        {
            save_session(roll, status, studentCount);
            printf("\nAttendance session saved. Returning to the main menu.\n");
            return;
        }

        if (input != 'P' && input != 'A')
        {
            printf("Invalid input. Please enter P, A, or S.\n");
            i--;
            continue;
        }

        status[i] = input;
        totalClasses[i]++;

        if (input == 'P')
        {
            presentCount[i]++;
        }

        /*
         * Save after each answer so a partial record is retained even if
         * the program exits unexpectedly.
         */
        if (!save_attendance(name, roll, presentCount, totalClasses,
                             studentCount) ||
            !save_session(roll, status, studentCount))
        {
            printf("\nCould not save. Returning to the main menu.\n");
            return;
        }

        printf("%s marked %s.\n", name[i],
               input == 'P' ? "Present" : "Absent");
    }

    /* All students have been called; remove the unfinished-session file. */
    if (remove(SESSION_FILE) != 0)
    {
        printf("\nAttendance was saved, but the session file could not be "
               "removed.\n");
    }

    printf("\n========================================\n");
    printf("Attendance saved successfully!\n");
    printf("========================================\n");
}