#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100

void regi(void);
void present(void);
void record(void);

int main()
{
    int choice;

    while (1)
    {
        printf("\n========================================\n");
        printf("       SMART ATTENDANCE SYSTEM\n");
        printf("========================================\n");
        printf("1. New Registration\n");
        printf("2. Present Call\n");
        printf("3. Records\n");
        printf("4. Exit\n");
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
            present();
            break;

        case 3:
            record();
            break;

        case 4:
            printf("\nThank you for using Smart Attendance System.\n");
            return 0;

        default:
            printf("\nInvalid choice! Please try again.\n");
        }
    }

    return 0;
}


/* =========================================
   NEW REGISTRATION
   ========================================= */
void regi(void)
{
    FILE *fp;

    int roll;
    char name[100];
    char dept[100];
    char session[100];
    char stu_email[100];

    fp = fopen("data/student-new.txt", "a");

    if (fp == NULL)
    {
        printf("\nError: Could not open student file.\n");
        return;
    }

    printf("\n========================================\n");
    printf("          NEW REGISTRATION\n");
    printf("========================================\n");

    printf("Enter student name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Enter roll: ");
    scanf("%d", &roll);
    getchar();

    printf("Enter department: ");
    fgets(dept, sizeof(dept), stdin);
    dept[strcspn(dept, "\n")] = '\0';

    printf("Enter session: ");
    fgets(session, sizeof(session), stdin);
    session[strcspn(session, "\n")] = '\0';

    printf("Enter student email: ");
    fgets(stu_email, sizeof(stu_email), stdin);
    stu_email[strcspn(stu_email, "\n")] = '\0';

    fprintf(fp, "%s,%d,%s,%s,%s\n",
            name,
            roll,
            dept,
            session,
            stu_email);

    fclose(fp);

    printf("\nStudent registered successfully!\n");
}


/* =========================================
   PRESENT CALL
   ========================================= */
void present(void)
{
    FILE *studentFile;
    FILE *attendanceFile;

    char line[500];

    char name[MAX_STUDENTS][100];
    int roll[MAX_STUDENTS];
    char dept[MAX_STUDENTS][100];
    char session[MAX_STUDENTS][100];
    char email[MAX_STUDENTS][100];

    int presentCount[MAX_STUDENTS];
    int totalClasses[MAX_STUDENTS];

    int studentCount = 0;
    int i;
    char status;

    /*
       Read registered students
    */
    studentFile = fopen("data/student-new.txt", "r");

    if (studentFile == NULL)
    {
        printf("\nNo student registration file found.\n");
        return;
    }

    while (fgets(line, sizeof(line), studentFile) != NULL &&
           studentCount < MAX_STUDENTS)
    {
        sscanf(line, "%99[^,],%d,%99[^,],%99[^,],%99[^\n]",
               name[studentCount],
               &roll[studentCount],
               dept[studentCount],
               session[studentCount],
               email[studentCount]);

        studentCount++;
    }

    fclose(studentFile);

    if (studentCount == 0)
    {
        printf("\nNo students are registered.\n");
        return;
    }

    /*
       Initialize attendance data
    */
    for (i = 0; i < studentCount; i++)
    {
        presentCount[i] = 0;
        totalClasses[i] = 0;
    }

    /*
       Read previous attendance data
    */
    attendanceFile = fopen("data/attendance.txt", "r");

    if (attendanceFile != NULL)
    {
        while (fgets(line, sizeof(line), attendanceFile) != NULL)
        {
            char oldName[100];
            int oldRoll;
            int oldPresent;
            int oldTotal;

            if (sscanf(line, "%99[^,],%d,%d,%d",
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
       Increase total class count
    */
    for (i = 0; i < studentCount; i++)
    {
        totalClasses[i]++;
    }

    /*
       Take attendance
    */
    printf("\n========================================\n");
    printf("             PRESENT CALL\n");
    printf("========================================\n");

    for (i = 0; i < studentCount; i++)
    {
        printf("\n%d. %s (Roll: %d)",
               i + 1,
               name[i],
               roll[i]);

        printf("\nEnter P for Present / A for Absent: ");

        scanf(" %c", &status);

        if (status == 'P' || status == 'p')
        {
            presentCount[i]++;
        }
        else if (status == 'A' || status == 'a')
        {
            /* Student is absent */
        }
        else
        {
            printf("Invalid input. Student marked absent.\n");
        }
    }

    /*
       Rewrite attendance file
    */
    attendanceFile = fopen("data/attendance.txt", "w");

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
                ((float)presentCount[i] / totalClasses[i]) * 100.0;
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


/* =========================================
   RECORDS
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
    char percentage[50];

    fp = fopen("data/attendance.txt", "r");

    if (fp == NULL)
    {
        printf("\nNo attendance records found.\n");
        return;
    }

    printf("\n========================================\n");
    printf("              RECORDS\n");
    printf("========================================\n");

    printf("Enter student roll: ");
    scanf("%d", &searchRoll);

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line,
                   "%99[^,],%d,%d,%d,%49[^\n]",
                   name,
                   &roll,
                   &presentCount,
                   &totalClasses,
                   percentage) == 5)
        {
            if (roll == searchRoll)
            {
                printf("\n----------------------------------------\n");
                printf("Student Name   : %s\n", name);
                printf("Roll           : %d\n", roll);
                printf("Present        : %d\n", presentCount);
                printf("Total Classes  : %d\n", totalClasses);
                printf("Attendance     : %s\n", percentage);
                printf("----------------------------------------\n");

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