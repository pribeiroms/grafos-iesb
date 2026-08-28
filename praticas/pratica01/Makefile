CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
TARGET = main
SOURCES = main.c grafo_matriz.c grafo_lista.c

# compila todos os arquivos do projeto
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

# remove os arquivos gerados pela compilacao
clean:
	$(RM) $(TARGET) $(TARGET).exe

.PHONY: all clean
