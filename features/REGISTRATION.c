#include <stdio.h>
#include <string.h>
#include "registration.h"

void regi(void)
{
    FILE *fp;
    FILE *checkFile;

    int roll;
    int existingRoll;

    char name[100];
    char dept[100];
    char session[100];
    char stu_email[100];
    char password[100];

    char line[500];

    printf("\n========================================\n");
    printf("           NEW REGISTRATION\n");
    printf("========================================\n");

    /* ================================
       STUDENT NAME
       ================================ */

    printf("Enter student name: ");

    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    /* ================================
       STUDENT ROLL
       ================================ */

    printf("Enter roll: ");

    if (scanf("%d", &roll) != 1)
    {
        printf("\nInvalid roll number.\n");

        while (getchar() != '\n');

        return;
    }

    while (getchar() != '\n');

    /* ================================
       DUPLICATE ROLL CHECK
       ================================ */

    checkFile = fopen("../data/student-new.txt", "r");

    if (checkFile != NULL)
    {
        while (fgets(line, sizeof(line), checkFile) != NULL)
        {
            /*
               File format:
               name,roll,department,session,email,password
            */

            if (sscanf(line, "%*99[^,],%d", &existingRoll) == 1)
            {
                if (existingRoll == roll)
                {
                    printf("\n========================================\n");
                    printf("ERROR: Roll number %d already exists!\n", roll);
                    printf("Registration rejected.\n");
                    printf("========================================\n");

                    fclose(checkFile);
                    return;
                }
            }
        }

        fclose(checkFile);
    }

    /* ================================
       DEPARTMENT
       ================================ */

    printf("Enter department: ");

    fgets(dept, sizeof(dept), stdin);
    dept[strcspn(dept, "\n")] = '\0';

    /* ================================
       SESSION
       ================================ */

    printf("Enter session: ");

    fgets(session, sizeof(session), stdin);
    session[strcspn(session, "\n")] = '\0';

    /* ================================
       EMAIL
       ================================ */

    printf("Enter student email: ");

    fgets(stu_email, sizeof(stu_email), stdin);
    stu_email[strcspn(stu_email, "\n")] = '\0';

    /* ================================
       PASSWORD
       ================================ */

    printf("Create student password: ");

    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    /* ================================
       SAVE STUDENT
       ================================ */

    fp = fopen("../data/student-new.txt", "a");

    if (fp == NULL)
    {
        printf("\nError: Could not open student file.\n");
        return;
    }

    fprintf(fp,
            "%s,%d,%s,%s,%s,%s\n",
            name,
            roll,
            dept,
            session,
            stu_email,
            password);

    fclose(fp);

    /* ================================
       SUCCESS MESSAGE
       ================================ */

    printf("\n========================================\n");
    printf("Student registered successfully!\n");
    printf("========================================\n");

    printf("Student Name  : %s\n", name);
    printf("Student Roll  : %d\n", roll);
    printf("Department    : %s\n", dept);
    printf("Session       : %s\n", session);
    printf("Email         : %s\n", stu_email);

    printf("\nStudent can now login using:\n");
    printf("Roll + Password\n");

    printf("========================================\n");
}