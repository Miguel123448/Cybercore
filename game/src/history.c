#include "history.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int RecursiveSumAttempts(const SessionRecord sessions[], int count) {
    if (count <= 0) return 0;
    return sessions[count - 1].attempts + RecursiveSumAttempts(sessions, count - 1);
}

static int RecursiveMinAttempts(const SessionRecord sessions[], int count) {
    if (count <= 1) return sessions[0].attempts;
    int minRest = RecursiveMinAttempts(sessions, count - 1);
    return sessions[count - 1].attempts < minRest ? sessions[count - 1].attempts : minRest;
}

static int RecursiveMaxAttempts(const SessionRecord sessions[], int count) {
    if (count <= 1) return sessions[0].attempts;
    int maxRest = RecursiveMaxAttempts(sessions, count - 1);
    return sessions[count - 1].attempts > maxRest ? sessions[count - 1].attempts : maxRest;
}

static double RecursiveSquareDiff(const SessionRecord sessions[], int count, double mean) {
    if (count <= 0) return 0.0;
    double diff = (double)sessions[count - 1].attempts - mean;
    return diff * diff + RecursiveSquareDiff(sessions, count - 1, mean);
}

static double RecursiveLowBias(const SessionRecord sessions[], int count) {
    if (count <= 0) return 0.0;
    double current = sessions[count - 1].attempts > 0 ?
        (double)sessions[count - 1].lows / (double)sessions[count - 1].attempts : 0.0;
    return current + RecursiveLowBias(sessions, count - 1);
}

static double RecursiveHighBias(const SessionRecord sessions[], int count) {
    if (count <= 0) return 0.0;
    double current = sessions[count - 1].attempts > 0 ?
        (double)sessions[count - 1].highs / (double)sessions[count - 1].attempts : 0.0;
    return current + RecursiveHighBias(sessions, count - 1);
}

static int RecursiveMonotonicSteps(const int guesses[], int count, int index) {
    if (index >= count - 1) return 0;

    int step = 0;
    if (guesses[index] < guesses[index + 1] || guesses[index] > guesses[index + 1]) {
        step = 1;
    }

    return step + RecursiveMonotonicSteps(guesses, count, index + 1);
}

static int RecursiveTotalMonotonic(const SessionRecord sessions[], int count) {
    if (count <= 0) return 0;
    int current = RecursiveMonotonicSteps(
        sessions[count - 1].guesses,
        sessions[count - 1].guessCount,
        0
    );
    return current + RecursiveTotalMonotonic(sessions, count - 1);
}

bool History_SaveSession(const SessionRecord *session) {
    FILE *file = fopen(HISTORY_FILE, "a");
    if (file == NULL) {
        return false;
    }

    fprintf(
        file,
        "%s;%d;%d;%d;%d;",
        session->timestamp,
        session->target,
        session->attempts,
        session->lows,
        session->highs
    );

    for (int i = 0; i < session->guessCount; i++) {
        fprintf(file, "%d", session->guesses[i]);
        if (i < session->guessCount - 1) {
            fprintf(file, ",");
        }
    }

    fprintf(file, "\n");
    fclose(file);
    return true;
}

int History_LoadSessions(SessionRecord sessions[], int maxSessions) {
    FILE *file = fopen(HISTORY_FILE, "r");
    if (file == NULL) {
        return 0;
    }

    char line[1024];
    int count = 0;

    while (fgets(line, sizeof(line), file) != NULL && count < maxSessions) {
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0') {
            continue;
        }

        SessionRecord session = {0};
        char *field = strtok(line, ";");
        if (field == NULL) continue;
        snprintf(session.timestamp, sizeof(session.timestamp), "%s", field);

        field = strtok(NULL, ";");
        if (field == NULL) continue;
        session.target = atoi(field);

        field = strtok(NULL, ";");
        if (field == NULL) continue;
        session.attempts = atoi(field);

        field = strtok(NULL, ";");
        if (field == NULL) continue;
        session.lows = atoi(field);

        field = strtok(NULL, ";");
        if (field == NULL) continue;
        session.highs = atoi(field);

        field = strtok(NULL, ";");
        if (field != NULL) {
            char *guessToken = strtok(field, ",");
            while (guessToken != NULL && session.guessCount < MAX_GUESSES) {
                session.guesses[session.guessCount++] = atoi(guessToken);
                guessToken = strtok(NULL, ",");
            }
        }

        sessions[count++] = session;
    }

    fclose(file);
    return count;
}

AnalysisReport History_BuildReport(const SessionRecord sessions[], int count) {
    AnalysisReport report;
    memset(&report, 0, sizeof(report));
    report.totalSessions = count;

    if (count <= 0) {
        snprintf(report.suggestion, sizeof(report.suggestion), "Nenhum historico encontrado. Jogue uma partida para gerar dados.");
        return report;
    }

    int totalAttempts = RecursiveSumAttempts(sessions, count);
    report.averageAttempts = (double)totalAttempts / (double)count;
    report.bestAttempts = RecursiveMinAttempts(sessions, count);
    report.worstAttempts = RecursiveMaxAttempts(sessions, count);

    double variance = RecursiveSquareDiff(sessions, count, report.averageAttempts) / (double)count;
    if (variance < 0.0) variance = 0.0;
    report.standardDeviation = sqrt(variance);

    report.averageLowBias = RecursiveLowBias(sessions, count) / (double)count;
    report.averageHighBias = RecursiveHighBias(sessions, count) / (double)count;
    report.monotonicSteps = RecursiveTotalMonotonic(sessions, count);

    if (report.averageLowBias > 0.60) {
        snprintf(report.suggestion, sizeof(report.suggestion), "Tendencia: muitos palpites abaixo do alvo. Use as pistas para iniciar mais perto do intervalo correto.");
    } else if (report.averageHighBias > 0.60) {
        snprintf(report.suggestion, sizeof(report.suggestion), "Tendencia: muitos palpites acima do alvo. Reduza o primeiro chute usando a faixa liberada pelo Sudoku.");
    } else if (report.averageAttempts > 7.0) {
        snprintf(report.suggestion, sizeof(report.suggestion), "Sugestao: media alta de tentativas. Aplique busca binaria dentro do intervalo revelado.");
    } else if (report.standardDeviation < 1.0 && report.averageAttempts > 5.0) {
        snprintf(report.suggestion, sizeof(report.suggestion), "Padrao repetitivo detectado. Varie a estrategia e combine as pistas antes de chutar.");
    } else {
        snprintf(report.suggestion, sizeof(report.suggestion), "Boa estrategia: suas tentativas indicam uso consistente das pistas e aproximacao logica.");
    }

    return report;
}
