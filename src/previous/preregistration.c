#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function declarations */
void regi(void);
void present(void);
void record(void);

int main(void)
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
        scanf("%d", &choice);

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


/* ================= NEW REGISTRATION ================= */

void regi(void)
{
    typedef struct record
    {
        int roll;
        char name[100];
        char dept[100];
        char session[100];
        char stu_email[100];
    } record;

    FILE *fp = fopen("student-new.txt", "a");

    if (fp == NULL)
    {
        printf("Error: cannot open file\n");
        return;
    }

    record student;

    printf("\n========================================\n");
    printf("          NEW REGISTRATION\n");
    printf("========================================\n");

    printf("Enter student name: ");
    fgets(student.name, sizeof(student.name), stdin);
    student.name[strcspn(student.name, "\n")] = '\0';

    printf("Enter student's roll: ");
    scanf("%d", &student.roll);

    while (getchar() != '\n');

    printf("Enter student's department: ");
    fgets(student.dept, sizeof(student.dept), stdin);
    student.dept[strcspn(student.dept, "\n")] = '\0';

    printf("Enter session: ");
    fgets(student.session, sizeof(student.session), stdin);
    student.session[strcspn(student.session, "\n")] = '\0';

    printf("Enter student's e-mail: ");
    fgets(student.stu_email, sizeof(student.stu_email), stdin);
    student.stu_email[strcspn(student.stu_email, "\n")] = '\0';

    fprintf(fp, "%s,%d,%s,%s,%s\n",
            student.name,
            student.roll,
            student.dept,
            student.session,
            student.stu_email);

    fclose(fp);

    printf("\nStudent information saved successfully!\n");
}


/* ================= PRESENT CALL ================= */

void present(void)
{
    printf("\nPresent Call selected.\n");

    /* We will add your attendance code here later */
}


/* ================= RECORDS ================= */

void record(void)
{
    printf("\nRecords selected.\n");

    /* We will add your records code here later */
}