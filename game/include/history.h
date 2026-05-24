#ifndef CYBERCORE_HISTORY_H
#define CYBERCORE_HISTORY_H

#include <stdbool.h>

#define HISTORY_FILE "historico.txt"
#define MAX_GUESSES 100
#define MAX_SESSIONS 200

typedef struct {
    char timestamp[32];
    int target;
    int attempts;
    int lows;
    int highs;
    int guesses[MAX_GUESSES];
    int guessCount;
} SessionRecord;

typedef struct {
    int totalSessions;
    double averageAttempts;
    int bestAttempts;
    int worstAttempts;
    double standardDeviation;
    double averageLowBias;
    double averageHighBias;
    int monotonicSteps;
    char suggestion[256];
} AnalysisReport;

bool History_SaveSession(const SessionRecord *session);
int History_LoadSessions(SessionRecord sessions[], int maxSessions);
AnalysisReport History_BuildReport(const SessionRecord sessions[], int count);

#endif
