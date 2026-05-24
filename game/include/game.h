#ifndef CYBERCORE_GAME_H
#define CYBERCORE_GAME_H

#include <stdbool.h>
#include "history.h"
#include "sudoku.h"

#define MAX_HINTS 8
#define MAX_LOGIC_QUESTIONS 3

typedef enum {
    SCREEN_MENU,
    SCREEN_INTRO,
    SCREEN_SUDOKU,
    SCREEN_LOGIC,
    SCREEN_GUESS,
    SCREEN_RESULT,
    SCREEN_HISTORY,
    SCREEN_HELP
} Screen;

typedef struct {
    char prompt[512];
    char options[4][160];
    int correctOption;
    int hintToUnlock;
    bool answered;
} LogicQuestion;

typedef struct {
    int secretNumber;
    int basicMin;
    int basicMax;
    int decadeMin;
    int decadeMax;
    int digitSum;
    char hints[MAX_HINTS][128];
    bool hintUnlocked[MAX_HINTS];
    int totalHints;
    LogicQuestion questions[MAX_LOGIC_QUESTIONS];
    int currentQuestion;
    int logicErrors;
    int guessInput;
    int attempts;
    int lows;
    int highs;
    int guesses[MAX_GUESSES];
    char feedback[128];
    bool completed;
    bool saved;
    int score;
    char rating[64];
} CyberGame;

void Game_Init(CyberGame *game);
void Game_UnlockHint(CyberGame *game, int index);
void Game_HandleGuess(CyberGame *game);
void Game_CalculateScore(CyberGame *game, int sudokuErrors);
SessionRecord Game_ToSession(const CyberGame *game);

#endif
