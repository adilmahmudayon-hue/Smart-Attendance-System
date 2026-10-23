CC     = gcc
CFLAGS = -Wall -Wextra -Ifeatures
SRC    = src/main.c $(wildcard features/*.c)
TARGET = attendance.exe

all: $(TARGET)

$(TARGET): $(SRC) $(wildcard features/*.h)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	$(TARGET)

clean:
	del /Q $(TARGET)