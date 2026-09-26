#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define MAX_STUDENTS 100


/* =========================================================
                    FUNCTION DECLARATIONS
   ========================================================= */


// Function Declears: ()

void regi(void);
void present(void);
void record(void);


/* =========================================================
                         MAIN FUNCTION
   ========================================================= */

int main()
{
    int choice;

    while (1)
    {
        printf("\n\n");
        printf("========================================\n");
        printf("       SMART ATTENDANCE SYSTEM\n");
        printf("========================================\n");

        printf("\n1. New Registration\n");
        printf("2. Present Call\n");
        printf("3. Records\n");
        printf("4. Exit\n");

        printf("\nEnter Your Choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input! Please enter a number from 1 to 4.\n");

            while (getchar() != '\n');

            continue;
        }

        /* Clear input buffer */
        while (getchar() != '\n');


        switch (choice)
        {
            case 1:
                regi();
                break;

            case 2:
                present();
                break;

            case 3:
                record();
                break;

            case 4:
                printf("\nThank you for using Smart Attendance System!\n");
                return 0;

            default:
                printf("\nInvalid Choice! Please enter 1-4.\n");
        }
    }

    return 0;
}


/* =========================================================
                    NEW REGISTRATION FUNCTION
   ========================================================= */

void regi(void)
{
    typedef struct
    {
        int roll;
        char name[100];
        char dept[100];
        char session[100];
        char stu_email[100];

    } record;


    FILE *fp;

    record student;


    fp = fopen("student-new.txt", "a");


    if (fp == NULL)
    {
        printf("\nError: Cannot open student-new.txt\n");
        return;
    }


    printf("\n========================================\n");
    printf("          NEW REGISTRATION\n");
    printf("========================================\n");


    /* Student Name */

    printf("Enter student name: ");

    fgets(student.name,
          sizeof(student.name),
          stdin);

    student.name[strcspn(student.name, "\n")] = '\0';


    /* Student Roll */

    printf("Enter student's roll: ");

    if (scanf("%d", &student.roll) != 1)
    {
        printf("\nInvalid roll number!\n");

        while (getchar() != '\n');

        fclose(fp);

        return;
    }

    while (getchar() != '\n');


    /* Department */

    printf("Enter student's department: ");

    fgets(student.dept,
          sizeof(student.dept),
          stdin);

    student.dept[strcspn(student.dept, "\n")] = '\0';


    /* Session */

    printf("Enter session: ");

    fgets(student.session,
          sizeof(student.session),
          stdin);

    student.session[strcspn(student.session, "\n")] = '\0';


    /* Email */

    printf("Enter student's e-mail: ");

    fgets(student.stu_email,
          sizeof(student.stu_email),
          stdin);

    student.stu_email[strcspn(student.stu_email, "\n")] = '\0';


    /* Save student information */

    fprintf(fp,
            "%s,%d,%s,%s,%s\n",
            student.name,
            student.roll,
            student.dept,
            student.session,
            student.stu_email);


    fclose(fp);


    printf("\nStudent information saved successfully!\n");
}


/* =========================================================
                       PRESENT CALL FUNCTION
   ========================================================= */

void present(void)
{
    struct Student
    {
        char name[100];

        int roll;

        int presentCount;

        int totalClasses;
    };


    FILE *studentFile;

    FILE *attendanceFile;


    struct Student students[MAX_STUDENTS];


    char line[300];

    char oldName[100];

    char choice;


    int count = 0;

    int oldRoll;

    int oldPresentCount;

    int oldTotalClasses;

    float oldPercentage;


    /* -----------------------------------------
                OPEN STUDENT FILE
       ----------------------------------------- */

    studentFile = fopen("student-new.txt", "r");


    if (studentFile == NULL)
    {
        printf("\nCould not open student-new.txt\n");

        printf("Please register at least one student first.\n");

        return;
    }


    /* -----------------------------------------
             READ REGISTERED STUDENTS
       ----------------------------------------- */

    while (fgets(line,
                 sizeof(line),
                 studentFile) != NULL &&
           count < MAX_STUDENTS)
    {
        /*
           student-new.txt format:

           Name,Roll,Department,Session,Email
        */

        if (sscanf(line,
                   "%99[^,],%d",
                   students[count].name,
                   &students[count].roll) == 2)
        {
            students[count].presentCount = 0;

            students[count].totalClasses = 0;

            count++;
        }
    }


    fclose(studentFile);


    /* -----------------------------------------
             CHECK IF STUDENTS EXIST
       ----------------------------------------- */

    if (count == 0)
    {
        printf("\nNo registered students found.\n");

        return;
    }


    /* -----------------------------------------
          READ PREVIOUS ATTENDANCE RECORDS
       ----------------------------------------- */

    attendanceFile = fopen("attendance.txt", "r");


    if (attendanceFile != NULL)
    {
        while (fscanf(attendanceFile,
                      "%99[^,],%d,%d,%d,%f%%",
                      oldName,
                      &oldRoll,
                      &oldPresentCount,
                      &oldTotalClasses,
                      &oldPercentage) == 5)
        {
            for (int i = 0; i < count; i++)
            {
                if (students[i].roll == oldRoll)
                {
                    students[i].presentCount =
                        oldPresentCount;

                    students[i].totalClasses =
                        oldTotalClasses;

                    break;
                }
            }
        }


        fclose(attendanceFile);
    }


    /* -----------------------------------------
             ONE NEW CLASS IS HELD
       ----------------------------------------- */

    for (int i = 0; i < count; i++)
    {
        students[i].totalClasses++;
    }


    /* -----------------------------------------
                 TAKE ATTENDANCE
       ----------------------------------------- */

    printf("\n========================================\n");
    printf("             PRESENT CALL\n");
    printf("========================================\n");


    for (int i = 0; i < count; i++)
    {
        printf("\nName : %s\n",
               students[i].name);

        printf("Roll : %d\n",
               students[i].roll);


        while (1)
        {
            printf("Attendance (P = Present, A = Absent): ");

            scanf(" %c", &choice);


            if (choice == 'P' ||
                choice == 'p')
            {
                students[i].presentCount++;

                printf("%s marked Present.\n",
                       students[i].name);

                break;
            }


            else if (choice == 'A' ||
                     choice == 'a')
            {
                printf("%s marked Absent.\n",
                       students[i].name);

                break;
            }


            else
            {
                printf("Invalid input! Enter P or A.\n");
            }
        }
    }


    /* Clear input buffer */

    while (getchar() != '\n');


    /* -----------------------------------------
             SAVE ATTENDANCE RECORDS
       ----------------------------------------- */

    attendanceFile = fopen("attendance.txt", "w");


    if (attendanceFile == NULL)
    {
        printf("\nCould not save attendance.txt\n");

        return;
    }


    for (int i = 0; i < count; i++)
    {
        float percentage;


        if (students[i].totalClasses > 0)
        {
            percentage =
                ((float)students[i].presentCount /
                 students[i].totalClasses) * 100;
        }
        else
        {
            percentage = 0;
        }


        fprintf(attendanceFile,
                "%s,%d,%d,%d,%.2f%%\n",
                students[i].name,
                students[i].roll,
                students[i].presentCount,
                students[i].totalClasses,
                percentage);
    }


    fclose(attendanceFile);


    printf("\n========================================\n");
    printf(" Attendance saved successfully!\n");
    printf("========================================\n");
}


/* =========================================================
                         RECORDS FUNCTION
   ========================================================= */

void record(void)
{
    FILE *fp;


    char name[100];


    int roll;

    int present;

    int totalClasses;


    int searchRoll;

    int found = 0;


    float percentage;


    /* -----------------------------------------
                    RECORD MENU
       ----------------------------------------- */

    printf("\n========================================\n");
    printf("              RECORDS\n");
    printf("========================================\n");


    printf("\nEnter student roll: ");


    if (scanf("%d", &searchRoll) != 1)
    {
        printf("\nInvalid roll number!\n");

        while (getchar() != '\n');

        return;
    }


    while (getchar() != '\n');


    /* -----------------------------------------
             OPEN ATTENDANCE FILE
       ----------------------------------------- */

    fp = fopen("attendance.txt", "r");


    if (fp == NULL)
    {
        printf("\nError: Cannot open attendance.txt\n");

        printf("Please take attendance first.\n");

        return;
    }


    /* -----------------------------------------
                SEARCH FOR STUDENT
       ----------------------------------------- */

    while (fscanf(fp,
                  "%99[^,],%d,%d,%d,%*f%%",
                  name,
                  &roll,
                  &present,
                  &totalClasses) == 4)
    {
        if (roll == searchRoll)
        {
            if (totalClasses > 0)
            {
                percentage =
                    ((float)present /
                     totalClasses) * 100;
            }
            else
            {
                percentage = 0;
            }


            printf("\n========================================\n");
            printf("          STUDENT ATTENDANCE\n");
            printf("========================================\n");


            printf("\nName             : %s\n",
                   name);


            printf("Roll Number      : %d\n",
                   roll);


            printf("Days Present     : %d\n",
                   present);


            printf("Total Classes    : %d\n",
                   totalClasses);


            printf("Attendance       : %.2f%%\n",
                   percentage);


            printf("========================================\n");


            found = 1;


            break;
        }
    }


    fclose(fp);


    /* -----------------------------------------
                STUDENT NOT FOUND
       ----------------------------------------- */

    if (!found)
    {
        printf("\nStudent with roll %d not found.\n",
               searchRoll);
    }
}