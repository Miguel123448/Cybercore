#ifndef CYBERCORE_GAME_H
#define CYBERCORE_GAME_H

/*
    game.h

    Aqui ficam as structs e enums principais do jogo.
    Separei assim para o main.c não ficar com tudo misturado.
*/

#include <stdbool.h>
#include "history.h"
#include "sudoku.h"

// Máximo de pistas que a partida pode ter.
#define MAX_HINTS 8

// Quantidade de perguntas antes do chute final.
#define MAX_LOGIC_QUESTIONS 3

// Telas possíveis do jogo.
typedef enum {
    SCREEN_MENU,     // Menu inicial.
    SCREEN_INTRO,    // Introdução da missão.
    SCREEN_SUDOKU,   // Fase do Sudoku.
    SCREEN_LOGIC,    // Fase da lógica.
    SCREEN_GUESS,    // Fase do chute.
    SCREEN_RESULT,   // Resultado final.
    SCREEN_HISTORY,  // Histórico e análise.
    SCREEN_HELP      // Como jogar.
} Screen;

// Dados de uma pergunta do quiz de lógica.
typedef struct {
    char prompt[512];         // Enunciado da pergunta.
    char options[4][160];     // Alternativas.
    int correctOption;        // Resposta certa.
    int hintToUnlock;         // Pista que libera ao acertar.
    bool answered;            // Marca se já acertou.
} LogicQuestion;

// Dados principais de uma partida inteira.
typedef struct {
    int secretNumber;                  // Número secreto.

    int basicMin;                      // Menor valor da pista inicial.
    int basicMax;                      // Maior valor da pista inicial.
    int decadeMin;                     // Menor valor da pista mais específica.
    int decadeMax;                     // Maior valor da pista mais específica.
    int digitSum;                      // Soma dos dígitos.

    char hints[MAX_HINTS][128];        // Textos das pistas.
    bool hintUnlocked[MAX_HINTS];      // Diz quais pistas já apareceram.
    int totalHints;                    // Total de pistas.

    LogicQuestion questions[MAX_LOGIC_QUESTIONS]; // Perguntas do quiz.
    int currentQuestion;               // Pergunta atual.
    int logicErrors;                   // Erros na lógica.

    int guessInput;                    // Valor digitado no chute.
    int attempts;                      // Tentativas no chute.
    int lows;                          // Chutes abaixo do número.
    int highs;                         // Chutes acima do número.
    int guesses[MAX_GUESSES];          // Lista dos chutes feitos.

    char feedback[128];                // Mensagem para o jogador.
    bool completed;                    // Fica true quando acerta.
    bool saved;                        // Evita salvar duas vezes.

    int score;                         // Pontuação.
    char rating[64];                   // Classificação.
} CyberGame;

// Começa uma partida nova.
void Game_Init(CyberGame *game);

// Libera uma pista.
void Game_UnlockHint(CyberGame *game, int index);

// Processa o chute do jogador.
void Game_HandleGuess(CyberGame *game);

// Calcula a pontuação final.
void Game_CalculateScore(CyberGame *game, int sudokuErrors);

// Prepara os dados para salvar no histórico.
SessionRecord Game_ToSession(const CyberGame *game);

#endif
