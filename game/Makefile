CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Iinclude
SRC = src/main.c src/sudoku.c src/history.c
OUT = Jogo.exe

# Windows / MinGW usando a raylib incluida em game/lib
all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) -Llib -lraylib -lopengl32 -lgdi32 -lwinmm -lm

# Linux/macOS: requer raylib instalada no sistema
linux:
	$(CC) $(CFLAGS) $(SRC) -o cybercore -lraylib -lm

clean:
	rm -f $(OUT) cybercore *.o src/*.o
