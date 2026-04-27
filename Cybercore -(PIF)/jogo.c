#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int opcao;
    int senha;
    int palpite;
    int tentativas;
    int vetorTentativas[100];
    int vetorSenhas[100];
    int totalSessoes;
    srand(time(NULL));

    do {
        printf("\n=== MEU JOGO ===\n");
        printf("1. Jogar\n");
        printf("2. Analisar Historico\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d%*c", &opcao);

        switch(opcao) {
            case 1:
                printf("\nIniciando o jogo...\n");
                senha = rand() % 101; 
                tentativas = 0;       
                do {
                    do {
                        printf("\nInsira o seu palpite (0 a 100): ");
                        scanf("%d%*c", &palpite);
                        
                        if (palpite < 0 || palpite > 100) {
                            printf("Erro! O palpite deve ser entre 0 e 100.\n");
                        }
                    } while (palpite < 0 || palpite > 100); 
                    tentativas++; 
                    
                    if (palpite < senha) {
                        printf("Muito baixo!\n");
                    }
                    else if (palpite > senha) {
                        printf("Muito alto!\n");
                    }
                    else {
                        printf("Parabens! Voce acertou em %d tentativas!\n", tentativas);
                    }

                } while (palpite != senha);
                FILE *arquivo = fopen("historico.txt", "a");
                
                if (arquivo == NULL) {
                    printf("Erro: Nao foi possivel salvar o historico da partida.\n");
                } else {
                    fprintf(arquivo, "%d %d\n", tentativas, senha);
                    fclose(arquivo);
                    printf("Resultado salvo no historico com sucesso!\n");
                }
                break;
            case 2:
                printf("\n--- HISTORICO DE PARTIDAS ---\n");
                FILE *arquivoLeitura = fopen("historico.txt", "r");

                if (arquivoLeitura == NULL) {
                    printf("Nenhum historico encontrado. Jogue uma partida primeiro!\n");
                } else {
                    int tentativasLidas, senhaLida;
                    totalSessoes = 0; 

                    printf("Sessao | Tentativas | Frequencia Alvo\n");
                    printf("---------------------------------------\n");
                    
                    while (fscanf(arquivoLeitura, "%d %d", &tentativasLidas, &senhaLida) != EOF) {
                        vetorTentativas[totalSessoes] = tentativasLidas;
                        vetorSenhas[totalSessoes] = senhaLida;
                        totalSessoes++; 
                        printf("  %02d   |     %02d     |      %02d\n", totalSessoes, tentativasLidas, senhaLida);
                    }
                    fclose(arquivoLeitura);
                    printf("---------------------------------------\n");
                    printf("Total de partidas carregadas na memoria: %d\n", totalSessoes);
                }
                break;
            case 3:
                printf("\nSaindo do programa. Ate logo!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }

    } while(opcao != 3);

    return 0;
}