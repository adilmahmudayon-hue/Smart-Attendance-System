
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../features/record.h"
#include "../features/present.h"
#include "../features/registration.h"
#include "../features/course.h"

#define TEACHER_ID "T001"
#define TEACHER_PASSWORD "teacher123"

#define ADMIN_ID "A001"
#define ADMIN_PASSWORD "admin123"

/* Function declarations */
void login(void);
void teacherMenu(void);
void studentMenu(int studentRoll);
void adminMenu(void);

int studentLogin(int *loggedInRoll);


/* =========================================
   MAIN
   ========================================= */

int main(void)
{
    int choice;

    while (1)
    {
        printf("\n========================================\n");
        printf("       SMART ATTENDANCE SYSTEM\n");
        printf("========================================\n");
        printf("1. Login\n");
        printf("2. Exit\n");
        printf("========================================\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
                login();
                break;

            case 2:
                printf("\nThank you for using Smart Attendance System.\n");
                return 0;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }

    return 0;
}


/* =========================================
   LOGIN TYPE SELECTION
   ========================================= */

void login(void)
{
    int choice;
    int studentRoll;

    printf("\n========================================\n");
    printf("                 LOGIN\n");
    printf("========================================\n");
    printf("1. Teacher Login\n");
    printf("2. Student Login\n");
    printf("3. Admin Login\n");
    printf("4. Back\n");
    printf("========================================\n");
    printf("Enter your choice: ");

    scanf("%d", &choice);
    getchar();

    switch (choice)
    {
        /* ---------------------------------
           TEACHER LOGIN
           --------------------------------- */

        case 1:
        {
            char teacherID[50];
            char password[50];

            printf("\n========================================\n");
            printf("            TEACHER LOGIN\n");
            printf("========================================\n");

            printf("Teacher ID: ");
            fgets(teacherID, sizeof(teacherID), stdin);
            teacherID[strcspn(teacherID, "\n")] = '\0';

            printf("Password: ");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = '\0';

            if (strcmp(teacherID, TEACHER_ID) == 0 &&
                strcmp(password, TEACHER_PASSWORD) == 0)
            {
                printf("\nTeacher login successful!\n");
                teacherMenu();
            }
            else
            {
                printf("\nInvalid Teacher ID or Password!\n");
            }

            break;
        }


        /* ---------------------------------
           STUDENT LOGIN
           --------------------------------- */

        case 2:

            if (studentLogin(&studentRoll))
            {
                studentMenu(studentRoll);
            }

            break;


        /* ---------------------------------
           ADMIN LOGIN
           --------------------------------- */

        case 3:
        {
            char adminID[50];
            char password[50];

            printf("\n========================================\n");
            printf("             ADMIN LOGIN\n");
            printf("========================================\n");

            printf("Admin ID: ");
            fgets(adminID, sizeof(adminID), stdin);
            adminID[strcspn(adminID, "\n")] = '\0';

            printf("Password: ");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = '\0';

            if (strcmp(adminID, ADMIN_ID) == 0 &&
                strcmp(password, ADMIN_PASSWORD) == 0)
            {
                printf("\nAdmin login successful!\n");
                adminMenu();
            }
            else
            {
                printf("\nInvalid Admin ID or Password!\n");
            }

            break;
        }


        /* ---------------------------------
           BACK
           --------------------------------- */

        case 4:
            return;

        default:
            printf("\nInvalid choice!\n");
    }
}


/* =========================================
   TEACHER MENU
   ========================================= */

void teacherMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n========================================\n");
        printf("             TEACHER MENU\n");
        printf("========================================\n");
        
        printf("1. Present Call\n");
        printf("2. Records\n");
        
        printf("3. Exit\n");
        printf("========================================\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            
            case 1:
                present();
                break;

            case 2:
                record();
                break;

            case 3:
                printf("\nLogging out from teacher account...\n");
                return;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
}



   




/* =========================================
   MANAGE STUDENTS
   ========================================= */

void manageStudents(void)
{
    int choice;

    while (1)
    {
        printf("\n========================================\n");
        printf("            MANAGE STUDENTS\n");
        printf("========================================\n");
        printf("1. New Registration\n");
        printf("2. Delete Registration\n");
        printf("3. Back\n");
        printf("========================================\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
                regi();
                break;

            case 2:
                deleteRegistration();
                break;

            case 3:
                return;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
}


/* =========================================
   ADMIN MENU
   ========================================= */

void adminMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n========================================\n");
        printf("               ADMIN MENU\n");
        printf("========================================\n");
        printf("1. Manage Students\n");
        printf("2. Manage Teachers\n");
        printf("3. Manage Courses\n");
        printf("4. Overall Attendance Record View\n");
        printf("5. Logout\n");
        printf("========================================\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
                 manageStudents();
                 break;
            
            case 2:
                printf("\nTeacher Management feature is under development.\n");
                break;

            case 3:
                printf("\nCourse Management feature is under development.\n");
                break;

            case 4:
                overallAttendanceReport();
                break;

            case 5:
                printf("\nLogging out from admin account...\n");
                return;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
}


/* =========================================
   STUDENT MENU
   ========================================= */

void studentMenu(int studentRoll)
{
    int choice;

    while (1)
    {
        printf("\n========================================\n");
        printf("             STUDENT MENU\n");
        printf("========================================\n");
        printf("1. Records\n");
        printf("2. Exit\n");
        printf("========================================\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
                studentRecord(studentRoll);
                break;

            case 2:
                printf("\nLogging out from student account...\n");
                return;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
}


/* =========================================
   STUDENT LOGIN
   ========================================= */

int studentLogin(int *loggedInRoll)
{
    FILE *fp;

    char line[500];

    int inputRoll;

    char inputPassword[100];

    int roll;

    char name[100];
    char dept[100];
    char session[100];
    char email[100];
    char password[100];

    fp = fopen("data/student-new.txt", "r");

    if (fp == NULL)
    {
        printf("\nNo student registration file found.\n");
        return 0;
    }

    printf("\n========================================\n");
    printf("             STUDENT LOGIN\n");
    printf("========================================\n");

    printf("Roll: ");
    scanf("%d", &inputRoll);
    getchar();

    printf("Password: ");
    fgets(inputPassword, sizeof(inputPassword), stdin);
    inputPassword[strcspn(inputPassword, "\n")] = '\0';

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        /*
            Format:

            Name,Roll,Department,Session,Email,Password
        */

        if (sscanf(line,
                   "%99[^,],%d,%99[^,],%99[^,],%99[^,],%99[^\n]",
                   name,
                   &roll,
                   dept,
                   session,
                   email,
                   password) == 6)
        {
            if (roll == inputRoll &&
                strcmp(password, inputPassword) == 0)
            {
                *loggedInRoll = roll;

                fclose(fp);

                printf("\nStudent login successful!\n");
                printf("Welcome, %s!\n", name);

                return 1;
            }
        }
    }

    fclose(fp);

    printf("\nInvalid roll or password!\n");

    return 0;
}
