#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "present.h"
#include "course.h"

#define MAX_STUDENTS 100
#define NAME_LENGTH 100
#define LINE_LENGTH 500

static int save_attendance(
    const char name[][NAME_LENGTH],
    const int roll[],
    const int presentCount[],
    const int totalClasses[],
    int studentCount,
    const char attendanceFileName[])
{
    FILE *file = fopen(attendanceFileName, "w");

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

        fprintf(file,
                "%s,%d,%d,%d,%.2f%%\n",
                name[i],
                roll[i],
                presentCount[i],
                totalClasses[i],
                percentage);
    }

    fclose(file);

    return 1;
}


/*
 * Session file format:
 *
 * roll,status
 *
 * status:
 * P = present
 * A = absent
 * U = not yet called
 */
static int save_session(
    const int roll[],
    const char status[],
    int studentCount,
    const char sessionFileName[])
{
    FILE *file = fopen(sessionFileName, "w");

    int i;

    if (file == NULL)
    {
        printf("\nError: Could not save the attendance session.\n");
        return 0;
    }

    for (i = 0; i < studentCount; i++)
    {
        fprintf(file,
                "%d,%c\n",
                roll[i],
                status[i]);
    }

    fclose(file);

    return 1;
}


void present(const char selectedCourseCode[],
             const char selectedCourseName[])
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

    /*
     * Course information
     */
    char courseCode[50];
    char courseName[100];

    char attendanceFileName[150];
    char sessionFileName[150];


    /*
     * STEP 1:
     * Select course before doing anything with attendance.
     */

    printf("\n========================================\n");
    printf("          COURSE-WISE ATTENDANCE\n");
    printf("========================================\n");

    
        snprintf(courseCode, sizeof(courseCode), "%s", selectedCourseCode);
        snprintf(courseName, sizeof(courseName), "%s", selectedCourseName);
        
    


    /*
     * Build the attendance file name.
     *
     * Example:
     * CSE1101 -> ../data/CSE1101.txt
     */

   snprintf(attendanceFileName,
         sizeof(attendanceFileName),
         "data/%s.txt",
         courseCode);

    /*
     * Build the session file name.
     *
     * Example:
     * CSE1101 -> ../data/CSE1101-session.txt
     */

     snprintf(sessionFileName,
         sizeof(sessionFileName),
         "data/%s-session.txt",
         courseCode);


    printf("\n========================================\n");
    printf("Course : %s\n", courseCode);
    printf("Name   : %s\n", courseName);
    printf("========================================\n");


    /*
     * STEP 2:
     * Load registered students.
     */

    studentFile = fopen("data/student-new.txt", "r");

    if (studentFile == NULL)
    {
        printf("\nNo student registration file found.\n");
        return;
    }


    /*
     * Registration format:
     *
     * Name,Roll,Department,Session,Email,Password
     */

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


    /*
     * Initially nobody has been called.
     */

    for (i = 0; i < studentCount; i++)
    {
        status[i] = 'U';
    }


    /*
     * STEP 3:
     * Load cumulative attendance for THIS COURSE ONLY.
     */

    attendanceFile = fopen(attendanceFileName, "r");

    if (attendanceFile != NULL)
    {
        while (fgets(line, sizeof(line), attendanceFile) != NULL)
        {
            char oldName[NAME_LENGTH];

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


    /*
     * STEP 4:
     * Check whether THIS COURSE has an unfinished session.
     */

    sessionFile = fopen(sessionFileName, "r");

    if (sessionFile != NULL)
    {
        int oldRoll;
        char oldStatus;

        while (fgets(line, sizeof(line), sessionFile) != NULL)
        {
            if (sscanf(line,
                       "%d,%c",
                       &oldRoll,
                       &oldStatus) == 2)
            {
                for (i = 0; i < studentCount; i++)
                {
                    if (roll[i] == oldRoll &&
                        (oldStatus == 'P' ||
                         oldStatus == 'A' ||
                         oldStatus == 'U'))
                    {
                        status[i] = oldStatus;

                        break;
                    }
                }
            }
        }

        fclose(sessionFile);


        /*
         * Check if anyone is still unmarked.
         */

        for (i = 0; i < studentCount; i++)
        {
            if (status[i] == 'U')
            {
                hasUnfinishedSession = 1;

                break;
            }
        }
    }


    /*
     * STEP 5:
     * Ask whether to continue unfinished attendance.
     */

    if (hasUnfinishedSession)
    {
        printf("\nAn unfinished attendance record was found for ");
        printf("%s.\n", courseCode);

        printf("1. Continue the previous record\n");
        printf("2. Start a new record\n");

        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid choice.\n");

            while (getchar() != '\n');

            return;
        }

        while (getchar() != '\n');

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

            /*
             * Starting a new session.
             * Previous partial session is replaced as students are marked.
             */

        }
        else
        {
            printf("\nInvalid choice. Returning to the main menu.\n");

            return;
        }
    }


    /*
     * STEP 6:
     * Start attendance.
     */

    printf("\n========================================\n");
    printf("           PRESENT CALL\n");
    printf("Course: %s\n", courseCode);
    printf("Enter S at a prompt to stop and save.\n");
    printf("========================================\n");


    for (i = 0; i < studentCount; i++)
    {
        if (status[i] != 'U')
        {
            continue;
        }


        printf("\n%d. %s (Roll: %d)\n",
               i + 1,
               name[i],
               roll[i]);

        printf("Enter P for Present, A for Absent, or S to stop: ");


        if (scanf(" %c", &input) != 1)
        {
            printf("\nCould not read input. Saving and returning.\n");

            save_session(
                roll,
                status,
                studentCount,
                sessionFileName);

            return;
        }


        input = (char)toupper((unsigned char)input);


        /*
         * Stop attendance.
         */

        if (input == 'S')
        {
            save_session(
                roll,
                status,
                studentCount,
                sessionFileName);

            printf("\nAttendance session saved.\n");
            printf("Course: %s\n", courseCode);
            printf("Returning to the main menu.\n");

            return;
        }


        /*
         * Invalid input.
         */

        if (input != 'P' && input != 'A')
        {
            printf("Invalid input. Please enter P, A, or S.\n");

            i--;

            continue;
        }


        /*
         * Store the attendance status.
         */

        status[i] = input;

        totalClasses[i]++;


        if (input == 'P')
        {
            presentCount[i]++;
        }


        /*
         * Save after every answer.
         *
         * This protects the attendance data if the
         * program closes unexpectedly.
         */

        if (!save_attendance(
                name,
                roll,
                presentCount,
                totalClasses,
                studentCount,
                attendanceFileName) ||

            !save_session(
                roll,
                status,
                studentCount,
                sessionFileName))
        {
            printf("\nCould not save attendance.\n");
            printf("Returning to the main menu.\n");

            return;
        }


        printf("%s marked %s.\n",
               name[i],
               input == 'P' ? "Present" : "Absent");
    }


    /*
     * STEP 7:
     * All students have been called.
     *
     * Remove the course-specific unfinished session file.
     */

    if (remove(sessionFileName) != 0)
    {
        printf("\nAttendance was saved, but the session file could not be removed.\n");
    }


    printf("\n========================================\n");
    printf("Attendance saved successfully!\n");
    printf("Course: %s\n", courseCode);
    printf("========================================\n");
}