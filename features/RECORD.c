#include <stdio.h>

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

    const float ATTENDANCE_THRESHOLD = 75.0;

    printf("\n========================================\n");
    printf("              RECORDS\n");
    printf("========================================\n");

    printf("\nEnter student roll: ");

    scanf("%d", &searchRoll);

    fp = fopen("../data/attendance.txt", "r");

    if (fp == NULL)
    {
        printf("\nError: Cannot open attendance.txt\n");

        return;
    }

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
                     totalClasses) *
                    100;
            }
            else
            {
                percentage = 0;
            }

            printf("\n========================================\n");
            printf("          STUDENT ATTENDANCE\n");
            printf("========================================\n");

            printf("\nName             : %s\n", name);
            printf("Roll Number      : %d\n", roll);
            printf("Days Present     : %d\n", present);
            printf("Total Classes    : %d\n", totalClasses);
            printf("Attendance       : %.2f%%\n", percentage);

            printf("========================================\n");

            if (percentage < 60.0)
            {
                printf("\nYour attendance is low. Contact your course teacher.\n");
            }
            else if (percentage < 70.0)
            {
                printf("\nBe attentive to your classes and attend your class regularly.\n");
            }
            else if (percentage < ATTENDANCE_THRESHOLD)
            {
                printf("\n\xE2\x9A\xA0 Warning: Attendance below %.0f%%!\n",
                       ATTENDANCE_THRESHOLD);
            }
            else
            {
                printf("\nAttendance status: Good standing.\n");
            }

            found = 1;

            break;
        }
    }

    fclose(fp);

    if (!found)
    {
        printf("\nStudent with roll %d not found.\n", searchRoll);
    }
}

int main(void)
{
    record();
    return 0;
}