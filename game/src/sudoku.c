/*
    sudoku.c

    Aqui fica a fase do Sudoku.
    O tabuleiro é 4x4 para a partida não ficar muito demorada.
*/

#include "sudoku.h"
#include "raylib.h"


// Bibliotecas que uso para sortear, limpar memória e montar textos.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// Posição do Sudoku dentro da tela.
#define GRID_X 492
#define GRID_Y 212
#define CELL 84
#define WIN_X 430
#define WIN_Y 100
#define WIN_W 470
#define WIN_H 535


// Cores usadas só nessa tela do Sudoku.
static const Color CYBER_GREEN = {0, 255, 55, 255};
static const Color CYBER_DARK_GREEN = {0, 95, 28, 255};
static const Color WIN_BLUE = {0, 35, 165, 255};
static const Color WIN_BODY = {205, 205, 205, 255};
static const Color WIN_SHADOW = {50, 50, 50, 255};
static const Color TEXT_DARK = {18, 18, 18, 255};

// Desenha a janela do Sudoku no estilo retrô.
static void DrawClassicWindowLocal(Rectangle r, const char *title) {
    DrawRectangle((int)r.x + 7, (int)r.y + 7, (int)r.width, (int)r.height, (Color){0, 0, 0, 95});
    DrawRectangleRec(r, WIN_BODY);
    DrawRectangleLinesEx(r, 2, WHITE);
    DrawRectangleLinesEx((Rectangle){r.x + 2, r.y + 2, r.width - 4, r.height - 4}, 1, WIN_SHADOW);

    Rectangle titleBar = {r.x + 4, r.y + 4, r.width - 8, 25};
    DrawRectangleRec(titleBar, WIN_BLUE);
    DrawText(title, (int)r.x + 34, (int)r.y + 8, 17, WHITE);
    DrawRectangle((int)r.x + 12, (int)r.y + 9, 15, 15, WHITE);

    int bx = (int)(r.x + r.width - 64);
    for (int i = 0; i < 3; i++) {
        DrawRectangle(bx + i * 20, (int)r.y + 8, 16, 16, WIN_BODY);
        DrawRectangleLines(bx + i * 20, (int)r.y + 8, 16, 16, BLACK);
    }
    DrawText("_", bx + 4, (int)r.y + 5, 15, BLACK);
    DrawText("o", bx + 23, (int)r.y + 8, 12, BLACK);
    DrawText("x", bx + 44, (int)r.y + 8, 12, BLACK);
}

// Desenha uma área interna com efeito de relevo.
static void DrawClassicInsetLocal(Rectangle r, Color fill) {
    DrawRectangleRec(r, fill);
    DrawLine((int)r.x, (int)r.y, (int)(r.x + r.width), (int)r.y, WIN_SHADOW);
    DrawLine((int)r.x, (int)r.y, (int)r.x, (int)(r.y + r.height), WIN_SHADOW);
    DrawLine((int)r.x, (int)(r.y + r.height), (int)(r.x + r.width), (int)(r.y + r.height), WHITE);
    DrawLine((int)(r.x + r.width), (int)r.y, (int)(r.x + r.width), (int)(r.y + r.height), WHITE);
}

// Verifica se o número pode entrar naquela posição do Sudoku.
static bool CanPlace(int board[SUDOKU_SIZE][SUDOKU_SIZE], int row, int col, int value) {
    // Primeiro vejo se já tem o número na linha ou na coluna.
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        if (board[row][i] == value || board[i][col] == value) {
            return false;
        }
    }

    // Depois vejo o bloco 2x2 da célula.
    int startRow = (row / 2) * 2;
    int startCol = (col / 2) * 2;

    for (int r = startRow; r < startRow + 2; r++) {
        for (int c = startCol; c < startCol + 2; c++) {
            if (board[r][c] == value) {
                return false;
            }
        }
    }

    return true;
}

// Conta quantas soluções o Sudoku tem. Uso isso para garantir que só exista uma.
static void CountSolutions(int board[SUDOKU_SIZE][SUDOKU_SIZE], int *counter) {
    if (*counter > 1) {
        return;
    }

    // Procuro uma casa vazia para tentar preencher.
    for (int r = 0; r < SUDOKU_SIZE; r++) {
        for (int c = 0; c < SUDOKU_SIZE; c++) {
            if (board[r][c] == 0) {
                for (int value = 1; value <= 4; value++) {
                    if (CanPlace(board, r, c, value)) {
                        board[r][c] = value;
                        CountSolutions(board, counter);
                        board[r][c] = 0;
                    }
                }
                return;
            }
        }
    }

    (*counter)++;
}

// Gera o gabarito do Sudoku usando tentativa e volta.
static bool FillSolution(int board[SUDOKU_SIZE][SUDOKU_SIZE]) {
    for (int r = 0; r < SUDOKU_SIZE; r++) {
        for (int c = 0; c < SUDOKU_SIZE; c++) {
            if (board[r][c] == 0) {
                // Embaralho os números para o Sudoku mudar a cada partida.
                int values[4] = {1, 2, 3, 4};

                for (int i = 0; i < 4; i++) {
                    int j = rand() % 4;
                    int temp = values[i];
                    values[i] = values[j];
                    values[j] = temp;
                }

                for (int i = 0; i < 4; i++) {
                    if (CanPlace(board, r, c, values[i])) {
                        board[r][c] = values[i];
                        if (FillSolution(board)) {
                            return true;
                        }
                        board[r][c] = 0;
                    }
                }

                return false;
            }
        }
    }

    return true;
}

// Copia um tabuleiro para outro.
static void CopyBoard(int dst[SUDOKU_SIZE][SUDOKU_SIZE], int src[SUDOKU_SIZE][SUDOKU_SIZE]) {
    for (int r = 0; r < SUDOKU_SIZE; r++) {
        for (int c = 0; c < SUDOKU_SIZE; c++) {
            dst[r][c] = src[r][c];
        }
    }
}

// Transforma as teclas 1, 2, 3 e 4 em número do Sudoku.
static int KeyToNumber(void) {
    if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) return 1;
    if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) return 2;
    if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) return 3;
    if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) return 4;
    return 0;
}

// Cria uma nova partida de Sudoku.
void Sudoku_Init(SudokuGame *game) {
    memset(game, 0, sizeof(*game));
    game->selectedRow = 0;
    game->selectedCol = 0;
    snprintf(game->message, sizeof(game->message), "Preencha as celulas vazias com 1 a 4.");

    // Primeiro gero o gabarito completo.
    int solution[SUDOKU_SIZE][SUDOKU_SIZE] = {0};
    FillSolution(solution);

    // Depois copio o gabarito para começar a esconder casas.
    int puzzle[SUDOKU_SIZE][SUDOKU_SIZE];
    CopyBoard(puzzle, solution);

    int positions[16];
    // Embaralho as posições para remover casas diferentes.
    for (int i = 0; i < 16; i++) {
        positions[i] = i;
    }

    for (int i = 0; i < 16; i++) {
        int j = rand() % 16;
        int temp = positions[i];
        positions[i] = positions[j];
        positions[j] = temp;
    }

    const int targetEmptyCells = 8;
    int removed = 0;

    // Removo uma casa, mas só mantenho se o Sudoku continuar com uma única resposta.
    for (int i = 0; i < 16 && removed < targetEmptyCells; i++) {
        int row = positions[i] / 4;
        int col = positions[i] % 4;
        int backup = puzzle[row][col];
        puzzle[row][col] = 0;

        int copy[SUDOKU_SIZE][SUDOKU_SIZE];
        CopyBoard(copy, puzzle);

        int solutions = 0;
        CountSolutions(copy, &solutions);

        if (solutions == 1) {
            removed++;
        } else {
            puzzle[row][col] = backup;
        }
    }

    // Salvo o tabuleiro inicial, o atual e o gabarito.
    CopyBoard(game->solution, solution);
    CopyBoard(game->board, puzzle);
    CopyBoard(game->original, puzzle);
    game->emptyCells = removed;
}

// Confere se o tabuleiro atual está igual ao gabarito.
bool Sudoku_IsCompleted(const SudokuGame *game) {
    for (int r = 0; r < SUDOKU_SIZE; r++) {
        for (int c = 0; c < SUDOKU_SIZE; c++) {
            if (game->board[r][c] != game->solution[r][c]) {
                return false;
            }
        }
    }
    return true;
}

// Atualiza o Sudoku: clique, teclado e validação.
void Sudoku_Update(SudokuGame *game) {
    if (game->completed) {
        return;
    }

    // Permite mudar a célula selecionada pelo teclado.
    if (IsKeyPressed(KEY_RIGHT)) game->selectedCol++;
    if (IsKeyPressed(KEY_LEFT)) game->selectedCol--;
    if (IsKeyPressed(KEY_DOWN)) game->selectedRow++;
    if (IsKeyPressed(KEY_UP)) game->selectedRow--;

    if (game->selectedRow < 0) game->selectedRow = 0;
    if (game->selectedRow > 3) game->selectedRow = 3;
    if (game->selectedCol < 0) game->selectedCol = 0;
    if (game->selectedCol > 3) game->selectedCol = 3;

    // Permite selecionar uma célula com o mouse.
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        for (int r = 0; r < SUDOKU_SIZE; r++) {
            for (int c = 0; c < SUDOKU_SIZE; c++) {
                Rectangle rect = {(float)(GRID_X + c * CELL), (float)(GRID_Y + r * CELL), (float)CELL, (float)CELL};
                if (CheckCollisionPointRec(mouse, rect)) {
                    game->selectedRow = r;
                    game->selectedCol = c;
                }
            }
        }
    }

    int row = game->selectedRow;
    int col = game->selectedCol;

    // Casas que vieram prontas não podem ser alteradas.
    if (game->original[row][col] == 0) {
        int value = KeyToNumber();

        // Quando digita, eu comparo com a resposta certa.
        if (value != 0) {
            if (value == game->solution[row][col]) {
                game->board[row][col] = value;
                snprintf(game->message, sizeof(game->message), "Celula correta. A matriz esta estabilizando.");
            } else {
                game->errors++;
                snprintf(game->message, sizeof(game->message), "Valor incorreto para esta celula. Erros: %d", game->errors);
            }
        }

        if (IsKeyPressed(KEY_ZERO) || IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_DELETE)) {
            game->board[row][col] = 0;
            snprintf(game->message, sizeof(game->message), "Celula apagada.");
        }
    } else {
        int ignored = KeyToNumber();
        if (ignored != 0) {
            snprintf(game->message, sizeof(game->message), "Esta celula faz parte da matriz original.");
        }
    }

    game->completed = Sudoku_IsCompleted(game);
    if (game->completed) {
        snprintf(game->message, sizeof(game->message), "Matriz sincronizada. Pistas liberadas.");
    }
}

// Desenha o Sudoku na tela.
void Sudoku_Draw(const SudokuGame *game) {
    Rectangle win = {(float)WIN_X, (float)WIN_Y, (float)WIN_W, (float)WIN_H};
    DrawClassicWindowLocal(win, "Fase 1 - Sudoku 4x4");

    DrawText("Preencha o grid. Numeros de 1 a 4 em cada linha,", WIN_X + 30, WIN_Y + 48, 17, TEXT_DARK);
    DrawText("coluna e bloco 2x2.", WIN_X + 30, WIN_Y + 70, 17, TEXT_DARK);

    DrawClassicInsetLocal((Rectangle){GRID_X - 8, GRID_Y - 8, CELL * 4 + 16, CELL * 4 + 16}, (Color){240, 240, 240, 255});

    // Desenho cada casa com uma cor diferente dependendo do tipo dela.
    for (int r = 0; r < SUDOKU_SIZE; r++) {
        for (int c = 0; c < SUDOKU_SIZE; c++) {
            Rectangle rect = {(float)(GRID_X + c * CELL), (float)(GRID_Y + r * CELL), (float)CELL, (float)CELL};
            Color fill = (game->original[r][c] != 0) ? WIN_BODY : (Color){245, 245, 245, 255};

            DrawRectangleRec(rect, fill);
            DrawRectangleLinesEx(rect, 1, GRAY);

            if (r == game->selectedRow && c == game->selectedCol) {
                DrawRectangleLinesEx((Rectangle){rect.x + 2, rect.y + 2, rect.width - 4, rect.height - 4}, 3, CYBER_GREEN);
            }

            if (game->board[r][c] != 0) {
                Color textColor = (game->original[r][c] != 0) ? BLACK : CYBER_DARK_GREEN;
                DrawText(TextFormat("%d", game->board[r][c]), (int)rect.x + 32, (int)rect.y + 21, 42, textColor);
            }
        }
    }

    DrawLine(GRID_X + CELL * 2, GRID_Y, GRID_X + CELL * 2, GRID_Y + CELL * 4, BLACK);
    DrawLine(GRID_X, GRID_Y + CELL * 2, GRID_X + CELL * 4, GRID_Y + CELL * 2, BLACK);
    DrawLineEx((Vector2){GRID_X + CELL * 2, GRID_Y}, (Vector2){GRID_X + CELL * 2, GRID_Y + CELL * 4}, 3, BLACK);
    DrawLineEx((Vector2){GRID_X, GRID_Y + CELL * 2}, (Vector2){GRID_X + CELL * 4, GRID_Y + CELL * 2}, 3, BLACK);

    DrawClassicInsetLocal((Rectangle){WIN_X + 22, WIN_Y + WIN_H - 80, WIN_W - 44, 46}, (Color){225, 225, 225, 255});
    DrawRectangle(WIN_X + 44, WIN_Y + WIN_H - 64, 14, 14, CYBER_GREEN);
    DrawText("Status:", WIN_X + 72, WIN_Y + WIN_H - 68, 18, TEXT_DARK);
    DrawText(game->completed ? "Sincronizado" : "Em andamento", WIN_X + 140, WIN_Y + WIN_H - 68, 18, game->completed ? CYBER_DARK_GREEN : CYBER_DARK_GREEN);
    DrawText(TextFormat("Erros: %d", game->errors), WIN_X + 305, WIN_Y + WIN_H - 68, 18, TEXT_DARK);
    DrawText(game->message, WIN_X + 26, WIN_Y + WIN_H - 27, 15, DARKGRAY);
}
