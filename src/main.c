#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "raylib.h"

typedef enum {
    MENU,
    JOGO,
    ESTATISTICAS,
    SAIR
} GameState;

typedef struct formatoHistorico{
    char timestamp[20]; //dependendo de como vamos fazer o tempo isso aqui pode mudar
    int alvo;
    int tentativas;
    int baixo;
    int altos;
    int *palpites;
} FormatoHistorico;

int gerarNumeroAleatorio();
int lerArquivo(FormatoHistorico *structHistorico);
void salvarPartida(FormatoHistorico partida);

int main() {
    InitWindow(800, 600, "Menu Raylib");
    SetTargetFPS(60);

    GameState estado = MENU;
    int opcaoSelecionada = 0;
    int totalOpcoes = 3;

    const char *opcoes[3] = {"Iniciar Jogo","Estatísticas","Sair"};

    while (!WindowShouldClose()) {

        // ===== INPUT =====
        if (estado == MENU) {
            if (IsKeyPressed(KEY_DOWN)) opcaoSelecionada++;
            if (IsKeyPressed(KEY_UP)) opcaoSelecionada--;

            if (opcaoSelecionada < 0) opcaoSelecionada = totalOpcoes - 1;
            if (opcaoSelecionada >= totalOpcoes) opcaoSelecionada = 0;

            if (IsKeyPressed(KEY_ENTER)) {
                if (opcaoSelecionada == 0) estado = JOGO;
                if (opcaoSelecionada == 1) estado = ESTATISTICAS;
                if (opcaoSelecionada == 2) estado = SAIR;
            }
        }

        // ===== UPDATE =====
        if (estado == SAIR) break;

        // ===== DRAW =====
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (estado == MENU) {

            DrawText("Cybercore", 300, 100, 40, GREEN);

            for (int i = 0; i < 3; i++) {
                Color cor = (i == opcaoSelecionada) ? RED : DARKGRAY;
                DrawText(opcoes[i], 320, 250 + i * 40, 20, cor);
            }

        } else if (estado == JOGO) {
            DrawText("JOGO RODANDO!", 280, 280, 30, GREEN);

            if (IsKeyPressed(KEY_ESCAPE)) {
                estado = MENU;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

int gerarNumeroAleatorio() {
    return (rand() % 100) + 1;
}

int lerArquivo(FormatoHistorico *structHistorico){

    FILE *arquivo = fopen("historico.txt","r");

    if(arquivo == NULL){
        return -1;
    }

    char linha[256];
    int i = 0;

    while(fgets(linha, sizeof(linha), arquivo) != NULL){

        char palpitesStr[200];

        // faz o parse da linha
        if(sscanf(linha, "%[^;];%d;%d;%d;%d;%[^\n]",
               structHistorico[i].timestamp,
               &structHistorico[i].alvo,
               &structHistorico[i].tentativas,
               &structHistorico[i].baixo,
               &structHistorico[i].altos,
               palpitesStr) != 6){
            continue; // pula linha inválida
        }

        // aloca memória para os palpites
        structHistorico[i].palpites =
            malloc(structHistorico[i].tentativas * sizeof(int));

        // separa CSV dos palpites
        char *token = strtok(palpitesStr, ",");
        int j = 0;

        while(token != NULL && j < structHistorico[i].tentativas){
            structHistorico[i].palpites[j]= atoi(token);
            token = strtok(NULL, ","); 
            j++;
        }

        i++;
    }

    fclose(arquivo);
    return i; // quantidade de linhas lidas
}

void salvarPartida(FormatoHistorico partida){

    FILE *arquivo = fopen("historico.txt", "a");

    if(arquivo == NULL){
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    fprintf(arquivo, "%s;%d;%d;%d;%d;",
            partida.timestamp,
            partida.alvo,
            partida.tentativas,
            partida.baixo,
            partida.altos);

    for(int i = 0; i < partida.tentativas; i++){
        fprintf(arquivo, "%d", partida.palpites[i]);
        if(i < partida.tentativas - 1){
            fprintf(arquivo, ",");
        }
    }

    fprintf(arquivo, "\n");

    fclose(arquivo);
}