CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c11
TARGET = main
ifeq ($(OS),Windows_NT)
TARGET = main.exe
endif
SOURCES = main.c dag.c

all: $(TARGET)

$(TARGET): $(SOURCES) dag.h
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
ifeq ($(OS),Windows_NT)
	cmd /C "if exist main.exe del /Q main.exe"
else
	$(RM) $(TARGET)
endif

.PHONY: all clean
