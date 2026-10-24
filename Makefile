CC = gcc
CFLAGS = -Ifeatures -Wall

all: attendance.exe

attendance.exe: src/main.o features/REGISTRATION.o features/present.o features/RECORD.o features/course.o features/managecourses.o
	$(CC) src/main.o features/REGISTRATION.o features/present.o features/RECORD.o features/course.o features/managecourses.o -o attendance.exe

src/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

features/REGISTRATION.o: features/REGISTRATION.c
	$(CC) $(CFLAGS) -c features/REGISTRATION.c -o features/REGISTRATION.o

features/present.o: features/present.c
	$(CC) $(CFLAGS) -c features/present.c -o features/present.o

features/RECORD.o: features/RECORD.c
	$(CC) $(CFLAGS) -c features/RECORD.c -o features/RECORD.o

features/course.o: features/course.c
	$(CC) $(CFLAGS) -c features/course.c -o features/course.o

features/managecourses.o: features/managecourses.c
	$(CC) $(CFLAGS) -c features/managecourses.c -o features/managecourses.o

clean:
	del src\*.o features\*.o attendance.exe