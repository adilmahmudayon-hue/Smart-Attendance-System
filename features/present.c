#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "present.h"
#include "course.h"

#define MAX_STUDENTS 100
#define NAME_LENGTH 100
#define LINE_LENGTH 500
#define COMMENT_LENGTH 300


static void extract_comment(const char line[],
                           char comment[],
                           int size)
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



static int save_attendance(
    const char name[][NAME_LENGTH],
    const int roll[],
    const int presentCount[],
    const int totalClasses[],
    const char comments[][COMMENT_LENGTH],
    int studentCount,
    const char attendanceFileName[])
{
    FILE *file = fopen(attendanceFileName, "w");
    int i;

    if (file == NULL)
    {
        printf("\nCould not save attendance.\n");
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

        if (comments[i][0] != '\0')
        {
            fprintf(file, "%s,%d,%d,%d,%.2f%%,%s\n",
                    name[i], roll[i],
                    presentCount[i], totalClasses[i],
                    percentage, comments[i]);
        }
        else
        {
            fprintf(file, "%s,%d,%d,%d,%.2f%%\n",
                    name[i], roll[i],
                    presentCount[i], totalClasses[i],
                    percentage);
        }
    }

    fclose(file);
    return 1;
}


//   Session file format: roll, status
//   status:
//   P = present
//   A = absent
//   U = not yet called
 
static int save_session(
    const int roll[],
    const char status[],
    int studentCount,
    const char sessionFileName[])
{
    FILE *file = fopen(sessionFileName, "w");

    if (file == NULL)
    {
        printf("\nError: Could not save the attendance session.\n");
        return 0;
    }

    for (int i = 0; i < studentCount; i++)
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
    FILE *sessionFile; //for temporary session data

    char line[LINE_LENGTH];

    char name[MAX_STUDENTS][NAME_LENGTH];

    int roll[MAX_STUDENTS];

    int presentCount[MAX_STUDENTS] = {0};
    int totalClasses[MAX_STUDENTS] = {0};
    char comments[MAX_STUDENTS][COMMENT_LENGTH] = {{0}};

    char status[MAX_STUDENTS];

    int studentCount = 0,i;

    int hasUnfinishedSession = 0;
    int choice;

    char input;

    
    char courseCode[50];
    char courseName[100];

    char attendanceFileName[150];
    char sessionFileName[150];

// Select course 
     
    printf("\n\n");
    printf(" \tCOURSE-WISE ATTENDANCE\n");
    printf("\n");

    
        snprintf(courseCode, sizeof(courseCode), "%s", selectedCourseCode);
        snprintf(courseName, sizeof(courseName), "%s", selectedCourseName);
        
       // Build the attendance file name.
     

   snprintf(attendanceFileName,
         sizeof(attendanceFileName),
         "data/%s.txt",
         courseCode);

    //Build the session file name.
     
         snprintf(sessionFileName,
         sizeof(sessionFileName),
         "data/%s-session.txt",
         courseCode);


    printf("\n\n");
    printf("Course : %s\n", courseCode);
    printf("Name   : %s\n", courseName);
    printf("\n");

     //Load registered students.
     

    studentFile = fopen("data/student-new.txt", "r");

    if (studentFile == NULL)
    {
        printf("\nNo student registration file found.\n");
        return;
    }


    // Registration format:Name,Roll,Department,Session,Email,Password
    
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


    // initially everyone is uncalled

    for (i = 0; i < studentCount; i++)
    {
        status[i] = 'U';
    }


     //Load cumulative attendance for this course only
     

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
                    if (roll[i] == oldRoll) //matches with file's roll
                    {
                        presentCount[i] = oldPresent;
                        totalClasses[i] = oldTotal;
                         extract_comment(line, comments[i], COMMENT_LENGTH);

                        break;
                    }
                }
            }
        }

        fclose(attendanceFile);
    }


     // Check whether this course has an unfinished session.
    

    sessionFile = fopen(sessionFileName, "r");

    if (sessionFile != NULL)
    {
        int oldRoll;
        char oldStatus;

        while (fgets(line, sizeof(line), sessionFile) != NULL)
        {
            if (sscanf(line,"%d,%c",&oldRoll,&oldStatus) == 2)
                                 
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


        
         // Check if anyone is still unmarked.
        
        for (i = 0; i < studentCount; i++)
        {
            if (status[i] == 'U')
            {
                hasUnfinishedSession = 1;
                break;
            }
        }
    }



     // Ask whether to continue unfinished attendance.
     

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

           
             // Starting a new session.
             // Previous partial session is replaced as students are marked.
             

        }
        else
        {
            printf("\nInvalid choice. Returning to the main menu.\n");

            return;
        }
    }


     // Start taking attendance.
 

    printf("\n\n");
    printf("\tPRESENT CALL\n");
    printf("Course: %s\n", courseCode);
    printf("Enter S at a prompt to stop and save.\n");
    printf("\n");


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


         // Stop attendance.
        

        if (input == 'S')
        {
            save_session(roll,status,studentCount,sessionFileName);    

            printf("\nAttendance session saved.\n");
            printf("Course: %s\n", courseCode);
            printf("Returning to the main menu.\n");

            return;
        }


        if (input != 'P' && input != 'A')
        {
            printf("Invalid input. Please enter P, A, or S.\n");

            i--;

            continue;
        }


        
         // Storing the attendance status.
        

        status[i] = input;

        totalClasses[i]++;


        if (input == 'P')
        {
            presentCount[i]++;
        }

//saves after every call which protects the data if suddenly program is off
        
        if (!save_attendance(
                name,
                roll,
                presentCount,
                totalClasses,
                comments,
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

    
     // All students have been called.
     
     // Remove the course-specific unfinished session file.

    if (remove(sessionFileName) != 0)
    {
        printf("\nAttendance was saved, but the session file could not be removed.\n");
    }


    printf("\n\n");
    printf("Attendance saved successfully!\n");
    printf("Course: %s\n", courseCode);
    printf("\n");
}