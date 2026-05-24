# Dia Zero - Quebra de Frequencia

Versao em C com Raylib usando uma interface inspirada em sistemas antigos: janelas cinza estilo Windows 95/98, barras azuis, fundo de chuva digital verde, paineis de cronometro, status do sistema e pistas coletadas.

O jogo integra tres mecanicas em uma unica progressao:

1. **Sudoku 4x4**: estabiliza a matriz de sincronizacao e libera pistas basicas.
2. **Logica proposicional**: o jogador responde perguntas sobre `E`, `OU`, `NAO` e `->`; cada acerto libera pistas mais fortes.
3. **Chute da frequencia**: o jogador usa as pistas para descobrir o numero secreto entre 1 e 100.

## Design aplicado

A nova interface usa:

- fundo animado de caracteres verdes, estilo hacker/matrix;
- janelas flutuantes com barra azul e corpo cinza;
- cronometro em display preto com texto verde;
- janela de sistema com barra de progresso da frequencia;
- janela de pistas coletadas;
- botoes com relevo retro;
- entrada final de frequencia em uma janela propria.

## Historico e analise

O historico e salvo em `historico.txt` no formato:

```txt
timestamp;alvo;tentativas;baixos;altos;palpites_csv