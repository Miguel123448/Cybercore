#ifndef DIA_ZERO_SUDOKU_H
#define DIA_ZERO_SUDOKU_H

#include <stdbool.h>

#define SUDOKU_SIZE 4

typedef struct {
    int board[SUDOKU_SIZE][SUDOKU_SIZE];
    int original[SUDOKU_SIZE][SUDOKU_SIZE];
    int solution[SUDOKU_SIZE][SUDOKU_SIZE];
    int selectedRow;
    int selectedCol;
    int errors;
    int emptyCells;
    bool completed;
    char message[128];
} SudokuGame;

void Sudoku_Init(SudokuGame *game);
void Sudoku_Update(SudokuGame *game);
void Sudoku_Draw(const SudokuGame *game);
bool Sudoku_IsCompleted(const SudokuGame *game);

#endif