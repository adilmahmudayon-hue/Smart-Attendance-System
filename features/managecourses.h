#ifndef MANAGECOURSES_H
#define MANAGECOURSES_H

#include <stddef.h>

void manageCourses(void);
void showTeacherCourses(const char teacherId[]);

int authenticateTeacher(const char teacherId[],
                       const char password[],
                       size_t teacherIdSize);

#endif