CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c11
TARGET = main
SOURCES = main.c busca_largura.c busca_profundidade.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).exe

.PHONY: all clean
