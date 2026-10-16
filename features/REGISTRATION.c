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

    struct StudentLine
    {
    int roll;
    char data[500];
    };

    struct StudentLine students[1000];
    struct StudentLine temp;

    int count = 0;
    int i, j;
    int savedRoll;

    printf("\n\n");
    printf("\tNEW REGISTRATION\n");
    printf("\n");

  
    printf("Enter student name: ");

    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

   
    printf("Enter roll: ");

    if (scanf("%d", &roll) != 1)
    {
        printf("\nInvalid roll number.\n");

        while (getchar() != '\n');

        return;
    }

    while (getchar() != '\n');

    
      //duplicate roll check
       

    checkFile = fopen("data/student-new.txt", "r");

    if (checkFile != NULL)
    {
        while (fgets(line, sizeof(line), checkFile) != NULL)
        {
           
              // File format:name,roll,department,session,email,password
           

            if (sscanf(line, "%*99[^,],%d", &existingRoll) == 1)
            {
                if (existingRoll == roll)
                {
                    printf("\n\n");
                    printf("ERROR: Roll number %d already exists!\n", roll);
                    printf("Registration rejected.\n");
                    printf("\n");

                    fclose(checkFile);
                    return;
                }
            }
        }

        fclose(checkFile);
    }


    printf("Enter department: ");

    fgets(dept, sizeof(dept), stdin);
    dept[strcspn(dept, "\n")] = '\0';

    printf("Enter session: ");

    fgets(session, sizeof(session), stdin);
    session[strcspn(session, "\n")] = '\0';

    printf("Enter student email: ");

    fgets(stu_email, sizeof(stu_email), stdin);
    stu_email[strcspn(stu_email, "\n")] = '\0';

    printf("Create student password: ");

    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    /* Read existing student records */
fp = fopen("data/student-new.txt", "r");

if (fp != NULL)
{
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (count >= 999)
        {
            printf("\nError: Student storage limit reached.\n");
            fclose(fp);
            return;
        }

        if (sscanf(line, "%*99[^,],%d", &savedRoll) == 1)
        {
            students[count].roll = savedRoll;

            strcpy(students[count].data, line);

            if (strchr(students[count].data, '\n') == NULL)
            {
                strcat(students[count].data, "\n");
            }

            count++;
        }
    }

    fclose(fp);
}

// Add the new student 
students[count].roll = roll;

snprintf(
    students[count].data,
    sizeof(students[count].data),
    "%s,%d,%s,%s,%s,%s\n",
    name, roll, dept, session, stu_email, password
);

count++;

//Sort by roll number, smallest to largest 
for (i = 0; i < count - 1; i++)
{
    for (j = 0; j < count - 1 - i; j++)
    {
        if (students[j].roll > students[j + 1].roll)
        {
            temp = students[j];
            students[j] = students[j + 1];
            students[j + 1] = temp;
        }
    }
}

// Rewrite the student file in sorted order
fp = fopen("data/student-new.txt", "w");

if (fp == NULL)
{
    printf("\nError: Could not update student file.\n");
    return;
}

for (i = 0; i < count; i++)
{
    fputs(students[i].data, fp);
}

fclose(fp);

    printf("\n\n");
    printf("Student registered successfully!\n");
    printf("\n");

    printf("Student Name  : %s\n", name);
    printf("Student Roll  : %d\n", roll);
    printf("Department    : %s\n", dept);
    printf("Session       : %s\n", session);
    printf("Email         : %s\n", stu_email);

    printf("\nStudent can now login using:\n");
    printf("Roll + Password\n");

    printf("\n\n");
}

//delete registration function


void deleteRegistration(void)
{
    FILE *fp;
    FILE *tempFile;

    char line[500];
    int rollToDelete;
    int roll;
    int found = 0;

    fp = fopen("data/student-new.txt", "r");

    if (fp == NULL)
    {
        printf("\nNo student registration file found.\n");
        return;
    }

    tempFile = fopen("data/student-temp.txt", "w");

    if (tempFile == NULL)
    {
        printf("\nError: Could not create temporary file.\n");
        fclose(fp);
        return;
    }

    printf("\n\n");
    printf("\tDELETE REGISTRATION\n");
    printf("\n");

    printf("Enter student roll to delete: ");

    if (scanf("%d", &rollToDelete) != 1)
    {
        printf("\nInvalid roll number.\n");

        while (getchar() != '\n');

        fclose(fp);
        fclose(tempFile);
        remove("data/student-temp.txt");

        return;
    }

    while (getchar() != '\n');

    /*
        If the roll matches, skip that student.
        Otherwise, copy the student to the temporary file.
    */

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line,"%*99[^,],%d", &roll) == 1)
                  
        {
            if (roll == rollToDelete)
            {
                found = 1;
                continue;
            }
        }

        fputs(line, tempFile);
    }

    fclose(fp);
    fclose(tempFile);

    if (found==0)
    {
        remove("data/student-temp.txt");

        printf("\n\n");
        printf("No student found with roll %d.\n", rollToDelete);
        printf("\n");

        return;
    }

    /*
        Replace the original student file
        with the updated temporary file.
    */

    remove("data/student-new.txt");

    if (rename("data/student-temp.txt","data/student-new.txt") != 0)
               
    {
        printf("\nError: Could not update student file.\n");
        return;
    }

    printf("\n\n");
    printf("Registration deleted successfully!\n");
    printf("Deleted student roll: %d\n", rollToDelete);
    printf("\n");
}