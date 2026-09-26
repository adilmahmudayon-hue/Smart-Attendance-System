#include <stdio.h>

#define MAX_STUDENTS 100

struct Student {
    char name[100];
    int roll;
    int presentCount;
    int totalClasses;
};

int main(void) {
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

    /* Open student list */
    studentFile = fopen("student.txt", "r");

    if (studentFile == NULL) {
        printf("Could not open student.txt\n");
        return 1;
    }

    /* Read only name and roll from student.txt */
    while (fgets(line, sizeof(line), studentFile) != NULL &&
           count < MAX_STUDENTS) {

        if (sscanf(line, "%99[^,],%d",
                   students[count].name,
                   &students[count].roll) == 2) {

            students[count].presentCount = 0;
            students[count].totalClasses = 0;
            count++;
        }
    }

    fclose(studentFile);

    /* Read old attendance records */
    attendanceFile = fopen("attendance.txt", "r");

    if (attendanceFile != NULL) {
        while (fscanf(attendanceFile, "%99[^,],%d,%d,%d,%f%%",
                      oldName,
                      &oldRoll,
                      &oldPresentCount,
                      &oldTotalClasses,
                      &oldPercentage) == 5) {

            for (int i = 0; i < count; i++) {
                if (students[i].roll == oldRoll) {
                    students[i].presentCount = oldPresentCount;
                    students[i].totalClasses = oldTotalClasses;
                    break;
                }
            }
        }

        fclose(attendanceFile);
    }

    /* Each run means one class was held */
    for (int i = 0; i < count; i++) {
        students[i].totalClasses++;
    }

    /* Take attendance */
    for (int i = 0; i < count; i++) {
        printf("\nName: %s\n", students[i].name);
        printf("Roll: %d\n", students[i].roll);
        printf("Attendance (P = Present, A = Absent): ");

        scanf(" %c", &choice);

        if (choice == 'P' || choice == 'p') {
            students[i].presentCount++;
            printf("%s marked Present.\n", students[i].name);
        }
        else if (choice == 'A' || choice == 'a') {
            printf("%s marked Absent.\n", students[i].name);
        }
        else {
            printf("Invalid input. Marked Absent.\n");
        }
    }

    /* Save full records only in attendance.txt */
    attendanceFile = fopen("attendance.txt", "w");

    if (attendanceFile == NULL) {
        printf("Could not save attendance.txt\n");
        return 1;
    }

    for (int i = 0; i < count; i++) {
        float percentage;

        percentage = (float)students[i].presentCount /
                     students[i].totalClasses * 100;

        fprintf(attendanceFile, "%s,%d,%d,%d,%.2f%%\n",
                students[i].name,
                students[i].roll,
                students[i].presentCount,
                students[i].totalClasses,
                percentage);
    }

    fclose(attendanceFile);

    printf("\nAttendance saved successfully in attendance.txt\n");

    return 0;
}