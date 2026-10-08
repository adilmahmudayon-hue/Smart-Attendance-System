#ifndef MANAGECOURSES_H
#define MANAGECOURSES_H

#include <stddef.h>

void manageCourses(void);
void showTeacherCourses(const char teacherId[]);

int authenticateTeacher(const char teacherId[],
                       const char password[],
                       size_t teacherIdSize);
int getTeacherCourses(const char teacherId[],
                     char courseCodes[][50],
                     char courseTitles[][100],
                     int maxCourses);

#endif