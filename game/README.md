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

O historico e salvo obrigatoriamente em arquivo texto simples, no caminho `game/historico.txt` quando o executavel esta na pasta `game`. O formato e:

```txt
timestamp;alvo;tentativas;baixos;altos;palpites_csv
```

A tela de historico calcula:

- total de sessoes;
- media de tentativas;
- melhor e pior sessao;
- desvio padrao;
- vies medio para baixo e para cima;
- sugestoes de estrategia.

As funcoes analiticas usam recursao para soma, minimo, maximo, soma dos quadrados das diferencas, vies e passos monotonicamente ordenados.

## Como compilar no Windows

Entre na pasta `game` e rode:

```bat
compilar.bat
```

Ou compile manualmente:

```bat
gcc -std=c11 -Wall -Wextra -Iinclude src/main.c src/sudoku.c src/history.c -o DiaZero.exe -Llib -lraylib -lopengl32 -lgdi32 -lwinmm -lm
```

Se aparecer que `gcc` nao e reconhecido, instale o MinGW/MSYS2 e adicione o caminho do GCC ao `PATH` do Windows.

## Como jogar

- **Menu**: `ENTER` inicia, `H` abre historico, `C` abre instrucoes, `ESC` sai.
- **Sudoku**: setas ou mouse selecionam celulas; `1-4` preenche; `Backspace` ou `0` apaga.
- **Logica**: clique em uma alternativa ou use `1-4`.
- **Chute**: digite a frequencia, use `Backspace` para apagar e `ENTER` ou o botao `Confirmar` para testar.

## Estrutura

```txt
game/
  include/
    game.h
    history.h
    sudoku.h
    raylib.h
  src/
    main.c
    sudoku.c
    history.c
  lib/
    libraylib.a
  Makefile
  compilar.bat
```

## Atualização visual

Esta versão usa o arquivo `assets/background/36030.gif` como fundo animado do Dia Zero. Caso o GIF não seja encontrado, o jogo volta automaticamente para o fundo procedural antigo em estilo Matrix.

A tela inicial também foi ajustada para melhorar o espaçamento entre `Dia Zero` e `Quebra de Frequência`, e o texto do painel superior esquerdo foi reposicionado para permanecer dentro do quadro.


## Persistencia do historico

A persistencia das partidas e feita somente em arquivo texto simples: `historico.txt`. O jogo cria o arquivo automaticamente caso ele nao exista e grava uma sessao por linha.


## Programação em Par

O relato da programação em par usada na persistência do histórico está documentado no README principal do repositório. A parte principal foi a criação e leitura do `historico.txt`, mantendo uma partida por linha e permitindo que a tela de análise reconstrua as sessões salvas.
