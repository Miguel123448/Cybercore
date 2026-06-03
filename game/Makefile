CC = gcc
WINDRES = windres
CFLAGS = -std=c11 -Wall -Wextra -Iinclude
SRC = src/main.c src/sudoku.c src/history.c
OUT = DiaZero.exe
RES = icon.res

# Windows / MinGW usando a raylib incluida em game/lib
all: $(OUT)

$(RES): icon.rc assets/icons/diazero.ico
	$(WINDRES) icon.rc -O coff -o $(RES)

$(OUT): $(SRC) $(RES)
	$(CC) $(CFLAGS) $(SRC) $(RES) -o $(OUT) -Llib -lraylib -lopengl32 -lgdi32 -lwinmm -lm

# Linux/macOS: requer raylib instalada no sistema
linux:
	$(CC) $(CFLAGS) $(SRC) -o diazero -lraylib -lm

clean:
	rm -f $(OUT) diazero $(RES) *.o src/*.o
