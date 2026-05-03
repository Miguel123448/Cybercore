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
    srand(time(NULL));  //Necessario para a funcao gerarNumeroAleatorio
    InitWindow(1280, 720, "Menu Raylib");
    SetTargetFPS(60);

    GameState estado = MENU;
    int menuOpcaoSelecionada = 0;
    int totalOpcoes = 3;
    const char *opcoes[3] = {"Iniciar Jogo","Estatísticas","Sair"};

    //variaveis referente a caixa de entrada do jogo
    char inputTexto[50] = "";
    int inputTamanho = 0;
    bool inputAtivo = false;
    Rectangle inputBox = {100, 200, 300, 40};
    
    int numeroAlvo; //variavel gerada que ser ao alvo
    char mensagem[50] = "Digite um número";
    Color corMsg;

    while (!WindowShouldClose()) {

        // ===== INPUT =====
        if (estado == MENU) {
            if (IsKeyPressed(KEY_DOWN)) menuOpcaoSelecionada++;
            if (IsKeyPressed(KEY_UP)) menuOpcaoSelecionada--;

            if (menuOpcaoSelecionada < 0) menuOpcaoSelecionada = totalOpcoes - 1;
            if (menuOpcaoSelecionada >= totalOpcoes) menuOpcaoSelecionada = 0;

            if (IsKeyPressed(KEY_ENTER)) {
                if (menuOpcaoSelecionada == 0){ 
                    estado = JOGO;
                    numeroAlvo = gerarNumeroAleatorio();
                }
                if (menuOpcaoSelecionada == 1) estado = ESTATISTICAS;
                if (menuOpcaoSelecionada == 2) estado = SAIR;
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
                Color cor = (i == menuOpcaoSelecionada) ? RED : DARKGRAY;
                DrawText(opcoes[i], 320, 250 + i * 40, 20, cor);
            }

        } else if (estado == JOGO) {
            Rectangle botao = {100, 100, 250, 40};
            bool hover = CheckCollisionPointRec(GetMousePosition(), botao);
            Color cor = hover ? RED : DARKGRAY;
            DrawText("Voltar ao Menu", botao.x, botao.y, 30, cor);
            if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                estado = MENU;  
        }
        // Clique ativa/desativa
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(GetMousePosition(), inputBox)) {
                    inputAtivo = true;
                } else {
                    inputAtivo = false;
                }
            }

            // Captura teclado
            if (inputAtivo) {
                int tecla = GetCharPressed();

                while (tecla > 0) {
                    if (tecla >= '0' && tecla <= '9'){      //so aceita numeros
                        if (inputTamanho < 2) {
                            inputTexto[inputTamanho] = (char)tecla;
                            inputTamanho++;
                            inputTexto[inputTamanho] = '\0';
                        }
                }
                    tecla = GetCharPressed();
                }

                // Backspace
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    if (inputTamanho > 0) {
                        inputTamanho--;
                        inputTexto[inputTamanho] = '\0';
                    }
                }
                    // Enter para confirmar
                if (IsKeyPressed(KEY_ENTER) && inputTamanho > 0) {
                    int valor = atoi(inputTexto);
                    if (valor > numeroAlvo) {
                        strcpy(mensagem, "Muito alto!");
                        corMsg = RED;
                    }
                    else if (valor < numeroAlvo) {
                        strcpy(mensagem, "Muito baixo!");
                        corMsg = BLUE;
                    }
                    else {
                        strcpy(mensagem, "Acertou!");
                        corMsg = GREEN;
                    }
                    // limpa input
                    inputTamanho = 0;
                    inputTexto[0] = '\0';
                }
            }
                DrawText(mensagem, inputBox.x, inputBox.y - 40, 20, corMsg);    //mensagem de "alto/baixo/acerto"
                // Caixa
                DrawRectangleRec(inputBox, LIGHTGRAY);
                DrawRectangleLinesEx(inputBox, 2, inputAtivo ? BLUE : DARKGRAY);

                // Texto digitado
                DrawText(inputTexto, inputBox.x + 10, inputBox.y + 10, 20, BLACK);

                // Placeholder
                if (inputTamanho == 0 && !inputAtivo) {
                    DrawText("Digite um número...", inputBox.x + 10, inputBox.y + 10, 20, GRAY);
                }

                // Cursor piscando
                if (inputAtivo && ((int)(GetTime() * 2) % 2 == 0)) {
                    int larguraTexto = MeasureText(inputTexto, 20);
                    DrawText("|",inputBox.x + 10 + larguraTexto, inputBox.y + 10,20,BLACK);
                }
    }

        EndDrawing();
    } 

    /*for (int i = 0; i < total; i++) {
        free(structHistorico[i].palpites);
    }
       LEMBRAR DE DAR FREE NOS PONTEIROS NO FINAL DO CODIGO 
    */
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
        if(sscanf(linha, "%19[^;];%d;%d;%d;%d;%[^\n]",
               structHistorico[i].timestamp,
               &structHistorico[i].alvo,
               &structHistorico[i].tentativas,
               &structHistorico[i].baixo,
               &structHistorico[i].altos,
               palpitesStr) != 6){
            continue; // pula linha inválida
        }

        // aloca memória para os palpites
        structHistorico[i].palpites = malloc(structHistorico[i].tentativas * sizeof(int));
        if (structHistorico[i].palpites == NULL) {
            printf("Erro de memória\n");
            break;
        }

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