#include <stdio.h>
#include <string.h>
#include "course.h"

int selectCourse(char courseCode[], char courseName[])
{
    FILE *fp;

    char line[200];
    char codes[50][50];
    char names[50][100];
    char passwords[50][100];

    int count = 0;
    int choice;

    fp = fopen("data/courses.txt", "r");

    if (fp == NULL)
    {
        printf("\nError: Could not open courses.txt\n");
        return 0;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';

        if (sscanf(line, "%49[^,],%99[^,],%99[^\r\n]",
           codes[count],
           names[count],
           passwords[count]) == 3)
{
    count++;
}
        
        if (count >= 50)
        {
            break;
        }
    }

    fclose(fp);

    if (count == 0)
    {
        printf("\nNo courses available.\n");
        return 0;
    }

    printf("\n========================================\n");
    printf("              SELECT COURSE\n");
    printf("========================================\n");

    for (int i = 0; i < count; i++)
    {
        printf("%d. %s - %s\n",
               i + 1,
               codes[i],
               names[i]);
    }

    printf("========================================\n");

    printf("Enter course number: ");

    if (scanf("%d", &choice) != 1)
    {
        printf("\nInvalid choice.\n");

        while (getchar() != '\n');

        return 0;
    }

    while (getchar() != '\n');

    if (choice < 1 || choice > count)
    {
        printf("\nInvalid course selection.\n");
        return 0;
    }

    strcpy(courseCode, codes[choice - 1]);
    strcpy(courseName, names[choice - 1]);

    printf("\nSelected Course:\n");
    printf("%s - %s\n",
           courseCode,
           courseName);

    return 1;
}