#ifndef CYBERCORE_SUDOKU_H
#define CYBERCORE_SUDOKU_H

/*
    sudoku.h

    Aqui ficam os dados e funções da fase do Sudoku.
*/

#include <stdbool.h>

// Sudoku 4x4.
#define SUDOKU_SIZE 4

// Guarda o tabuleiro atual, o inicial e o gabarito.
typedef struct {
    int board[SUDOKU_SIZE][SUDOKU_SIZE];     // Tabuleiro atual.
    int original[SUDOKU_SIZE][SUDOKU_SIZE];  // Tabuleiro inicial, para saber o que não pode mudar.
    int solution[SUDOKU_SIZE][SUDOKU_SIZE];  // Gabarito.
    int selectedRow;                         // Linha selecionada pelo jogador.
    int selectedCol;                         // Coluna selecionada pelo jogador.
    int errors;                              // Total de erros nessa fase.
    int emptyCells;                          // Quantidade de células removidas do tabuleiro.
    bool completed;                          // Indica se o Sudoku foi resolvido.
    char message[128];                       // Mensagem da fase.
} SudokuGame;

// Cria um Sudoku novo.
void Sudoku_Init(SudokuGame *game);

// Atualiza os comandos do Sudoku.
void Sudoku_Update(SudokuGame *game);

// Desenha o Sudoku na tela.
void Sudoku_Draw(const SudokuGame *game);

// Confere se o Sudoku foi resolvido.
bool Sudoku_IsCompleted(const SudokuGame *game);

#endif
