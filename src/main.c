
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../features/record.h"
#include "../features/present.h"
#include "../features/registration.h"
#include "../features/course.h"
#include "../features/managecourses.h"


#define ADMIN_ID "A001"
#define ADMIN_PASSWORD "admin123"

/* Function declarations */
void login(void);

void studentMenu(int studentRoll);
void adminMenu(void);

int studentLogin(int *loggedInRoll);
void teacherMenu(const char teacherId[]);




int main(void)
{
    int choice;

    while (1)
    {
        printf("\n\n");
        printf("\tSMART ATTENDANCE SYSTEM\n");
        printf("\n");
        printf("1. Login\n");
        printf("2. Exit\n");
        printf("\n");
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


// login menu

void login(void)
{
    int choice;
    int studentRoll;

    printf("\n\n");
    printf("\tLOGIN\n");
    printf("\n");
    printf("1. Admin Login\n");
    printf("2. Teacher Login\n");
    printf("3. Student Login\n");
    printf("4. Back\n");
    printf("\n");
    printf("Enter your choice: ");

    scanf("%d", &choice);
    getchar();

    switch (choice)
    {
        //teacher login

        case 2:
        {
            char teacherID[50];
            char password[50];

            printf("\n\n");
            printf("\tTEACHER LOGIN\n");
            printf("\n");

            printf("Teacher ID: ");
            fgets(teacherID, sizeof(teacherID), stdin);
            teacherID[strcspn(teacherID, "\n")] = '\0';

            printf("Password: ");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = '\0';

          if (authenticateTeacher(teacherID, password, sizeof(teacherID)))
         {
            printf("\nTeacher login successful!\n");
          teacherMenu(teacherID);
           }
         else
            {
            printf("\nInvalid Teacher ID or Password!\n");
            }

            break;
        }


       //student login

        case 3:

            if (studentLogin(&studentRoll))
            {
                studentMenu(studentRoll);
            }

            break;


       
            //admin login

        case 1:
        {
            char adminID[50];
            char password[50];

            printf("\n\n");
            printf("\tADMIN LOGIN\n");
            printf("\n");

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



        case 4:
            return;

        default:
            printf("\nInvalid choice!\n");
    }
}

// teacher menu

void teacherMenu(const char teacherId[])
{
    char courseCodes[100][50];
    char courseTitles[100][100];
    int courseCount;
    int courseChoice;
    int action;
    int i;

    while (1)
    {
        courseCount = getTeacherCourses( teacherId, courseCodes, courseTitles, 100);
           

        printf("\n \tYOUR ASSIGNED COURSES \n");

        if (courseCount == 0)
        {
            printf("No courses are assigned to you.\n");
            printf("1. Exit\n");
            printf("Choose: ");

            if (scanf("%d", &action) != 1 || action == 1)
            {
                while (getchar() != '\n') {}
                return;
            }

            while (getchar() != '\n') {}
            continue;
        }

        for (i = 0; i < courseCount; i++)
        {
            printf("%d. %s - %s\n",
                   i + 1, courseCodes[i], courseTitles[i]);
        }

        printf("%d. Exit\n", courseCount + 1);
        printf("Choose a course: ");

        if (scanf("%d", &courseChoice) != 1)
        {
            while (getchar() != '\n') {}
            return;
        }

        if (courseChoice == courseCount + 1)
        {
            while (getchar() != '\n') {}
            return;
        }

        if (courseChoice < 1 || courseChoice > courseCount)
        {
            printf("Invalid course choice.\n");
            while (getchar() != '\n') {}
            continue;
        }

        while (getchar() != '\n') {}

        while (1)
        {
            printf("\nCourse: %s - %s\n",
                   courseCodes[courseChoice - 1],
                   courseTitles[courseChoice - 1]);

            printf("1. Present Call\n");
            printf("2. Records\n");
            printf("3. Back to assigned courses\n");
            printf("Choose: ");

            if (scanf("%d", &action) != 1)
            {
                while (getchar() != '\n') {}
                break;
            }

            while (getchar() != '\n') {}

            if (action == 1)
            {
                present(courseCodes[courseChoice - 1],
                        courseTitles[courseChoice - 1]);
            }
            else if (action == 2)
            {
                recordForCourse(courseCodes[courseChoice - 1]);
            }
            else if (action == 3)
            {
                break;
            }
            else
            {
                printf("Invalid choice.\n");
            }
        }
    }
}

   




// manage students

void manageStudents(void)
{
    int choice;

    while (1)
    {
        printf("\n\n");
        printf("\tMANAGE STUDENTS\n");
        printf("\n");
        printf("1. New Registration\n");
        printf("2. Delete Registration\n");
        printf("3. Back\n");
        printf("\n");
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


// admin menu

void adminMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n\n");
        printf("\tADMIN MENU\n");
        printf("\n");
        printf("1. Manage Students\n");
       
        printf("2. Manage Courses\n");
        printf("3. Overall Attendance Record View\n");
        printf("4. Logout\n");
        printf("\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
                 manageStudents();
                 break;
            
            

            case 2:
                  manageCourses();
                
                break;

            case 3:
                overallAttendanceReport();
                break;

            case 4:
                printf("\nLogging out from admin account...\n");
                return;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
}


// student menu

void studentMenu(int studentRoll)
{
    int choice;

    while (1)
    {
        printf("\n\n");
        printf("\tSTUDENT MENU\n");
        printf("\n");
        printf("1. Records\n");
        printf("2. Exit\n");
        printf("\n");
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


// student login 

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

    printf("\n\n");
    printf("\tSTUDENT LOGIN\n");
    printf("\n");

    printf("Roll: ");
    scanf("%d", &inputRoll);
    getchar();

    printf("Password: ");
    fgets(inputPassword, sizeof(inputPassword), stdin);
    inputPassword[strcspn(inputPassword, "\n")] = '\0';

    while (fgets(line, sizeof(line), fp) != NULL)
    {
           // format-Name,Roll,Department,Session,Email,Password
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
