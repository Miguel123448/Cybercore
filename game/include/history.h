#ifndef CYBERCORE_HISTORY_H
#define CYBERCORE_HISTORY_H

/*
    history.h

    Aqui ficam as structs e funções usadas para salvar e analisar o histórico.
*/

#include <stdbool.h>

// Nome do arquivo onde as partidas ficam salvas.
#define HISTORY_FILE "historico.txt"

// Limites simples para os vetores.
#define MAX_GUESSES 100
#define MAX_SESSIONS 200

// Representa uma partida salva no historico.txt.
typedef struct {
    char timestamp[32];          // Data e hora da partida.
    int target;                  // Número secreto.
    int attempts;                // Total de tentativas.
    int lows;                    // Chutes abaixo do número.
    int highs;                   // Chutes acima do número.
    int guesses[MAX_GUESSES];    // Chutes feitos.
    int guessCount;              // Quantidade válida de palpites no vetor.
} SessionRecord;

// Resultado final das contas feitas em cima do histórico.
typedef struct {
    int totalSessions;           // Quantidade de partidas salvas.
    double averageAttempts;      // Média de tentativas.
    int bestAttempts;            // Melhor resultado.
    int worstAttempts;           // Pior resultado.
    double standardDeviation;    // Desvio padrão das tentativas.
    double averageLowBias;       // Média de chutes baixos.
    double averageHighBias;      // Média de chutes altos.
    int monotonicSteps;          // Ajuda a ver se os chutes ficaram muito em sequência.
    char suggestion[256];        // Sugestão textual baseada no comportamento do jogador.
} AnalysisReport;

// Cria o historico.txt se ele não existir.
bool History_EnsureFile(void);

// Salva uma partida no arquivo.
bool History_SaveSession(const SessionRecord *session);

// Lê o arquivo e carrega as partidas.
int History_LoadSessions(SessionRecord sessions[], int maxSessions);

// Calcula o relatório do histórico.
AnalysisReport History_BuildReport(const SessionRecord sessions[], int count);

#endif
