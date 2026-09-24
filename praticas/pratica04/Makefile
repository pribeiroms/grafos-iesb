CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c11
TARGET = main
ifeq ($(OS),Windows_NT)
TARGET = main.exe
endif
SOURCES = main.c conectividade.c planaridade.c
HEADERS = conectividade.h planaridade.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
ifeq ($(OS),Windows_NT)
	cmd /C "if exist main.exe del /Q main.exe"
else
	$(RM) $(TARGET)
endif

.PHONY: all clean
