#include <stdio.h>
#include <string.h>

#include "managecourses.h"

#define MAX_COURSES 100
#define MAX_TEACHERS 100
#define MAX_ASSIGNMENTS 200

typedef struct
{
    char code[50];
    char title[100];
    char password[100];
} Course;

typedef struct
{
    char id[50];
    char password[100];
} Teacher;

typedef struct
{
    char courseCode[50];
    char teacherId[50];
} Assignment;

static int loadCourses(Course courses[])
{
    FILE *file = fopen("data/courses.txt", "r");
    char line[300];
    int count = 0;

    if (file == NULL)
    {
        printf("Could not open data/courses.txt\n");
        return 0;
    }

    while (count < MAX_COURSES && fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\r\n")] = '\0';

        if (sscanf(line, "%49[^,],%99[^,],%99[^\r\n]",
                   courses[count].code,
                   courses[count].title,
                   courses[count].password) == 3)
        {
            count++;
        }
    }

    fclose(file);
    return count;
}

static int loadTeachers(Teacher teachers[])
{
    FILE *file = fopen("data/teacher.txt", "r");
    char line[200];
    int count = 0;

    if (file == NULL)
    {
        printf("Could not open data/teacher.txt\n");
        return 0;
    }

    while (count < MAX_TEACHERS && fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\r\n")] = '\0';

        if (sscanf(line, "%49[^,], %99[^\r\n]",
                   teachers[count].id,
                   teachers[count].password) == 2)
        {
            count++;
        }
    }

    fclose(file);
    return count;
}

static int loadAssignments(Assignment assignments[])
{
    FILE *file = fopen("data/course-assignments.txt", "r");
    char line[200];
    int count = 0;

    if (file == NULL)
    {
        return 0;
    }

    while (count < MAX_ASSIGNMENTS && fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\r\n")] = '\0';

        if (sscanf(line, "%49[^,],%49[^\r\n]",
                   assignments[count].courseCode,
                   assignments[count].teacherId) == 2)
        {
            count++;
        }
    }

    fclose(file);
    return count;
}

static int findAssignment(const Assignment assignments[],
                          int assignmentCount,
                          const char courseCode[])
{
    int i;

    for (i = 0; i < assignmentCount; i++)
    {
        if (strcmp(assignments[i].courseCode, courseCode) == 0)
        {
            return i;
        }
    }

    return -1;
}

static void assignTeacher(const Course courses[], int courseIndex)
{
    Teacher teachers[MAX_TEACHERS];
    Assignment assignments[MAX_ASSIGNMENTS];

    int teacherCount = loadTeachers(teachers);
    int assignmentCount = loadAssignments(assignments);
    int choice;
    int i;
    FILE *file;

    if (teacherCount == 0)
    {
        printf("No teachers are listed in data/teacher.txt\n");
        return;
    }

    if (assignmentCount >= MAX_ASSIGNMENTS)
    {
        printf("The assignment file is full.\n");
        return;
    }

    printf("\n========== TEACHERS ==========\n");
    for (i = 0; i < teacherCount; i++)
    {
        printf("%d. %s\n", i + 1, teachers[i].id);
    }

    printf("Choose a teacher number: ");
    if (scanf("%d", &choice) != 1 ||
        choice < 1 || choice > teacherCount)
    {
        printf("Invalid teacher choice.\n");
        return;
    }

    file = fopen("data/course-assignments.txt", "a");
    if (file == NULL)
    {
        printf("Could not save the assignment.\n");
        return;
    }

    fprintf(file, "%s,%s\n",
            courses[courseIndex].code,
            teachers[choice - 1].id);
    fclose(file);

    printf("%s assigned to %s.\n",
           teachers[choice - 1].id,
           courses[courseIndex].code);
}

void manageCourses(void)
{
    Course courses[MAX_COURSES];
    Assignment assignments[MAX_ASSIGNMENTS];

    int courseCount;
    int assignmentCount;
    int selectedCourse;
    int assignmentIndex;
    int choice;
    int i;
    char answer;

    while (1)
    {
        courseCount = loadCourses(courses);

        if (courseCount == 0)
        {
            printf("No courses found in data/courses.txt\n");
            return;
        }

        printf("\n========== MANAGE COURSES ==========\n");
        for (i = 0; i < courseCount; i++)
        {
            printf("%d. %s - %s\n",
                   i + 1, courses[i].code, courses[i].title);
        }
        printf("%d. Back\n", courseCount + 1);
        printf("Choose a course: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n') {}
            return;
        }

        if (choice == courseCount + 1)
        {
            return;
        }

        if (choice < 1 || choice > courseCount)
        {
            printf("Invalid course choice.\n");
            continue;
        }

        selectedCourse = choice - 1;
        assignmentCount = loadAssignments(assignments);
        assignmentIndex = findAssignment(
            assignments, assignmentCount, courses[selectedCourse].code);

        printf("\nCourse number : %s\n", courses[selectedCourse].code);
        printf("Course title  : %s\n", courses[selectedCourse].title);
        printf("Course password: %s\n", courses[selectedCourse].password);

        if (assignmentIndex >= 0)
        {
            printf("Status        : Assigned to %s\n",
                   assignments[assignmentIndex].teacherId);
            continue;
        }

        printf("Status        : Unassigned\n");
        printf("Assign a teacher? (Y/N): ");

        if (scanf(" %c", &answer) != 1)
        {
            return;
        }

        if (answer == 'Y' || answer == 'y')
        {
            assignTeacher(courses, selectedCourse);
        }
    }
}

void showTeacherCourses(const char teacherId[])
{
    Course courses[MAX_COURSES];
    Assignment assignments[MAX_ASSIGNMENTS];

    int courseCount = loadCourses(courses);
    int assignmentCount = loadAssignments(assignments);
    int i, j;
    int found = 0;

    printf("\n========== YOUR ASSIGNED COURSES ==========\n");

    for (i = 0; i < assignmentCount; i++)
    {
        if (strcmp(assignments[i].teacherId, teacherId) != 0)
        {
            continue;
        }

        for (j = 0; j < courseCount; j++)
        {
            if (strcmp(assignments[i].courseCode, courses[j].code) == 0)
            {
                printf("Course number  : %s\n", courses[j].code);
                printf("Course title   : %s\n", courses[j].title);
                printf("Course password: %s\n\n", courses[j].password);
                found = 1;
            }
        }
    }

    if (!found)
    {
        printf("No courses are assigned to you yet.\n");
    }
}

int authenticateTeacher(const char teacherId[],
                        const char password[],
                        size_t teacherIdSize)
{
    Teacher teachers[MAX_TEACHERS];
    int teacherCount = loadTeachers(teachers);
    int i;

    for (i = 0; i < teacherCount; i++)
    {
        if (strcmp(teachers[i].id, teacherId) == 0 &&
            strcmp(teachers[i].password, password) == 0)
        {
            (void)teacherIdSize;
            return 1;
        }
    }

    return 0;
}