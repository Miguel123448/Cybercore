# Documentação das Principais Funções do Jogo

## Projeto: Dia Zero — Quebra de Frequência

Este documento complementa a documentação do Capstone 3, explicando as principais funções usadas no código do jogo **Dia Zero**.  
A ideia é deixar mais claro o papel de cada arquivo e como as funções se conectam durante a execução da partida.

---

## 1. Visão geral dos arquivos principais

O código do jogo está dividido principalmente nestes arquivos:

| Arquivo | Função no projeto |
| --- | --- |
| `main.c` | Controla a janela, as telas, a interface, o fluxo geral da partida e a interação do jogador. |
| `sudoku.c` | Controla a criação, validação, atualização e desenho da fase do Sudoku. |
| `history.c` | Controla a persistência em `historico.txt` e a análise estatística das partidas. |
| `game.h` | Define as principais estruturas do jogo, como `CyberGame`, `LogicQuestion` e as telas. |
| `sudoku.h` | Define a estrutura `SudokuGame` e as funções públicas da fase do Sudoku. |
| `history.h` | Define `SessionRecord`, `AnalysisReport` e as funções públicas do histórico. |

---

## 2. Funções principais do `main.c`

O arquivo `main.c` é o centro do jogo.  
Ele inicializa o Raylib, controla a tela atual e chama as funções específicas de cada fase.

---

### `main`

```c
int main(void)
```

É a função principal do programa.

Ela faz:

- configura a janela do Raylib;
- carrega o fundo e os assets;
- inicializa as estruturas do jogo;
- controla o loop principal;
- atualiza a tela atual;
- desenha os elementos visuais;
- fecha o programa corretamente no final.

O jogo fica rodando dentro do loop principal até o jogador confirmar a saída.

---

### `UpdateFullscreenTransform`

```c
static void UpdateFullscreenTransform(void)
```

Essa função calcula a escala da interface quando o jogo está em fullscreen.

Como o layout foi pensado para uma resolução base, essa função ajuda a adaptar os elementos para outras resoluções de tela sem quebrar tudo.

Ela é importante para:

- ajustar o tamanho da interface;
- manter a proporção dos elementos;
- evitar que botões e janelas fiquem fora da tela.

---

### `GetVirtualMousePosition`

```c
static Vector2 GetVirtualMousePosition(void)
```

Converte a posição real do mouse para a posição usada no layout virtual do jogo.

Isso é necessário porque o jogo pode estar redimensionado em fullscreen.  
Sem essa conversão, o clique poderia não bater exatamente com os botões desenhados na tela.

---

### `Clicked`

```c
static bool Clicked(Rectangle rect)
```

Verifica se o jogador clicou dentro de um retângulo da interface.

É usada em botões do menu, alternativas da lógica, botão de confirmar chute e botões de confirmação de saída.

Exemplo de uso:

```c
if (Clicked(botaoIniciar)) {
    // começa uma nova partida
}
```

---

### `Hovered`

```c
static bool Hovered(Rectangle rect)
```

Verifica se o mouse está em cima de algum elemento da interface.

Ela é usada para mudar a aparência dos botões quando o jogador passa o mouse por cima.

---

### `DrawWrappedText`

```c
static void DrawWrappedText(const char *text, int x, int y, int fontSize, int maxWidth, Color color)
```

Desenha textos quebrando linhas automaticamente quando eles passam do tamanho máximo.

Foi usada principalmente nas perguntas de lógica, porque alguns enunciados são maiores e precisam caber dentro da janela.

---

### `LoadBackgroundFile`

```c
static void LoadBackgroundFile(void)
```

Carrega o fundo do jogo.

A função tenta carregar primeiro o arquivo:

```txt
assets/background/36030.gif
```

Se o fundo animado não funcionar, o jogo pode usar uma imagem estática ou o fundo Matrix feito pelo código como alternativa.

---

### `DrawMatrixBackground`

```c
static void DrawMatrixBackground(void)
```

Desenha o fundo estilo Matrix.

Quando o GIF não está disponível ou não é carregado, essa função garante que o jogo ainda tenha um fundo visual funcionando.

---

### `DrawClassicWindow`

```c
static void DrawClassicWindow(Rectangle r, const char *title)
```

Desenha uma janela no estilo antigo, com barra azul e corpo cinza.

Essa função é usada para manter o padrão visual retrô do jogo.

Exemplos de janelas desenhadas com esse estilo:

- menu principal;
- cronômetro;
- painel do sistema;
- fase de lógica;
- fase de chute;
- tela de resultado;
- tela de histórico.

---

### `DrawClassicButton`

```c
static void DrawClassicButton(Rectangle r, const char *label, bool active, bool disabled)
```

Desenha um botão no estilo retrô.

Parâmetros principais:

| Parâmetro | Uso |
| --- | --- |
| `r` | Área do botão na tela |
| `label` | Texto do botão |
| `active` | Indica destaque ou seleção |
| `disabled` | Indica se o botão está desativado |

É usada no menu, nas alternativas da lógica, na confirmação do chute e na confirmação de saída.

---

### `DrawHeaderPanel`

```c
static void DrawHeaderPanel(void)
```

Desenha o painel superior esquerdo com o nome do jogo **Dia Zero** e a frase de introdução.

Esse painel ajuda a manter a identidade visual do jogo durante as telas.

---

### `DrawTimerWindow`

```c
static void DrawTimerWindow(double startTime)
```

Desenha a janela do cronômetro.

O cronômetro mostra o tempo da partida.  
Ele para quando o jogador chega à tela de resultado e reinicia quando uma nova partida começa.

---

### `DrawSystemWindow`

```c
static void DrawSystemWindow(const CyberGame *game, Screen screen)
```

Desenha o painel de status do sistema.

Esse painel mostra informações como:

- status do sistema;
- integridade/frequência;
- barra visual de progresso.

Ele reforça a temática de invasão/defesa digital.

---

### `DrawGlobalDesktop`

```c
static void DrawGlobalDesktop(const CyberGame *game, Screen screen)
```

Desenha os elementos que aparecem em várias telas, como:

- fundo;
- painel superior;
- cronômetro;
- painel do sistema.

Assim, o código evita repetir esses desenhos em todas as telas.

---

## 3. Funções principais de controle da partida

Essas funções trabalham diretamente com a estrutura `CyberGame`.

---

### `Game_Init`

```c
void Game_Init(CyberGame *game)
```

Inicializa uma nova partida.

Ela faz:

- sorteia o número secreto;
- define as pistas possíveis;
- zera tentativas;
- zera erros de lógica;
- limpa mensagens anteriores;
- escolhe 3 perguntas aleatórias do banco de perguntas;
- prepara a fase de chute.

Essa função é chamada sempre que o jogador escolhe **Iniciar defesa**.

---

### `Game_UnlockHint`

```c
void Game_UnlockHint(CyberGame *game, int index)
```

Libera uma pista específica da partida.

Cada pista ajuda o jogador a descobrir o número secreto na fase final.

Exemplo de pistas:

- maior que um valor;
- menor que um valor;
- número par ou ímpar;
- soma dos dígitos.

---

### `Game_HandleGuess`

```c
void Game_HandleGuess(CyberGame *game)
```

Processa o chute feito pelo jogador na fase final.

Ela verifica se o número digitado é:

- menor que o número secreto;
- maior que o número secreto;
- igual ao número secreto.

Também atualiza:

- quantidade de tentativas;
- chutes baixos;
- chutes altos;
- lista de palpites;
- mensagem de feedback.

Quando o jogador acerta, a partida é marcada como concluída.

---

### `Game_CalculateScore`

```c
void Game_CalculateScore(CyberGame *game, int sudokuErrors)
```

Calcula a pontuação final da partida.

A pontuação considera:

- quantidade de tentativas no chute;
- erros no Sudoku;
- erros na lógica;
- desempenho geral do jogador.

Depois do cálculo, também define a classificação textual do jogador.

Exemplos de classificação:

- excelente;
- bom;
- regular;
- acesso comprometido.

---

### `Game_ToSession`

```c
SessionRecord Game_ToSession(const CyberGame *game)
```

Converte os dados da partida atual para o formato usado pelo histórico.

Ela monta uma estrutura `SessionRecord`, que depois é salva no arquivo `historico.txt`.

Essa função é importante porque separa os dados internos do jogo do formato usado na persistência.

---

## 4. Funções principais da lógica proposicional

A lógica proposicional fica controlada principalmente no `main.c`.

---

### `DrawLogicScreen`

```c
static void DrawLogicScreen(CyberGame *game)
```

Desenha a tela das perguntas de lógica.

Ela mostra:

- título da fase;
- número da pergunta atual;
- enunciado;
- alternativas;
- pistas obtidas;
- pop-up de acerto ou erro, quando necessário.

---

### `UpdateLogicScreen`

```c
static void UpdateLogicScreen(CyberGame *game, Screen *screen)
```

Atualiza a fase de lógica.

Ela verifica:

- se o jogador clicou em alguma alternativa;
- se a resposta está correta;
- se deve mostrar pop-up;
- se deve liberar nova pista;
- se deve passar para a próxima pergunta;
- se deve avançar para a fase de chute.

O jogo mantém apenas 3 perguntas por partida, mas elas são sorteadas a partir de um banco maior.

---

### `DrawLogicFeedbackPopup`

```c
static void DrawLogicFeedbackPopup(const CyberGame *game)
```

Desenha o pop-up que aparece depois de responder uma pergunta de lógica.

O pop-up informa se o jogador acertou ou errou.

Isso melhora o feedback do jogo e evita que a resposta passe despercebida.

---

## 5. Funções principais da fase de chute

---

### `DrawGuessScreen`

```c
static void DrawGuessScreen(const CyberGame *game)
```

Desenha a tela da fase final.

Essa tela mostra:

- pistas coletadas;
- campo de entrada do chute;
- botão de confirmar;
- feedback dizendo se o chute foi alto, baixo ou correto;
- número de tentativas.

---

### `UpdateGuessScreen`

```c
static void UpdateGuessScreen(CyberGame *game, Screen *screen, int sudokuErrors)
```

Atualiza a fase do chute.

Ela lê os números digitados pelo jogador, processa o botão de confirmação e chama `Game_HandleGuess`.

Quando o jogador acerta, ela chama `Game_CalculateScore`, salva a sessão no histórico e muda para a tela de resultado.

---

## 6. Funções principais das telas auxiliares

---

### `DrawMenu`

```c
static void DrawMenu(void)
```

Desenha o menu principal do jogo.

Opções principais:

- Iniciar defesa;
- Histórico e análise;
- Como jogar;
- Sair.

---

### `DrawIntro`

```c
static void DrawIntro(void)
```

Desenha a tela de introdução da missão.

Essa tela explica rapidamente a proposta do jogo antes de começar o Sudoku.

---

### `DrawResultScreen`

```c
static void DrawResultScreen(const CyberGame *game, const SudokuGame *sudoku)
```

Desenha a tela de resultado final.

Mostra informações como:

- número secreto;
- pontuação;
- classificação;
- erros no Sudoku;
- erros na lógica;
- tentativas no chute.

---

### `DrawHistoryScreen`

```c
static void DrawHistoryScreen(void)
```

Desenha a tela de histórico e análise.

Ela carrega os dados do `historico.txt`, chama a análise estatística e mostra o relatório para o jogador.

---

### `DrawHelpScreen`

```c
static void DrawHelpScreen(void)
```

Desenha a tela de ajuda.

Explica o fluxo básico do jogo:

```txt
Sudoku -> Lógica -> Chute
```

---

### `StartNewGame`

```c
static void StartNewGame(CyberGame *game, SudokuGame *sudoku, Screen *screen)
```

Começa uma nova partida.

Ela chama:

- `Game_Init`;
- `Sudoku_Init`;

Depois altera a tela para o início da partida.

---

## 7. Funções de confirmação de saída

---

### `DrawExitConfirmModal`

```c
static void DrawExitConfirmModal(void)
```

Desenha a janela de confirmação de saída.

Ela aparece quando o jogador aperta `ESC`.

---

### `UpdateExitConfirmModal`

```c
static void UpdateExitConfirmModal(bool *showExitConfirm, bool *exitRequested)
```

Atualiza os botões da confirmação de saída.

O jogador pode:

- confirmar a saída;
- cancelar e voltar para o jogo.

Isso evita que o jogo feche acidentalmente.

---

## 8. Funções principais do `sudoku.c`

O arquivo `sudoku.c` cuida de toda a lógica da fase do Sudoku.

---

### `Sudoku_Init`

```c
void Sudoku_Init(SudokuGame *game)
```

Cria um novo Sudoku.

Ela faz:

- limpa o tabuleiro;
- gera uma solução válida;
- remove algumas células para criar o desafio;
- guarda o tabuleiro inicial;
- guarda o gabarito;
- zera erros;
- limpa a seleção do jogador.

---

### `Sudoku_Update`

```c
void Sudoku_Update(SudokuGame *game)
```

Atualiza a fase do Sudoku.

Ela verifica:

- clique do mouse nas células;
- número digitado pelo jogador;
- se a célula pode ser alterada;
- se o número digitado está correto;
- se o Sudoku foi concluído.

Quando o jogador erra, a quantidade de erros aumenta.

---

### `Sudoku_Draw`

```c
void Sudoku_Draw(const SudokuGame *game)
```

Desenha o Sudoku na tela.

Ela mostra:

- janela da fase;
- instruções;
- grid 4x4;
- números fixos;
- números digitados;
- célula selecionada;
- mensagens da fase;
- quantidade de erros.

---

### `Sudoku_IsCompleted`

```c
bool Sudoku_IsCompleted(const SudokuGame *game)
```

Verifica se o Sudoku foi resolvido.

Ela confere se todas as células estão preenchidas corretamente de acordo com o gabarito.

---

### `CanPlace`

```c
static bool CanPlace(int board[SUDOKU_SIZE][SUDOKU_SIZE], int row, int col, int value)
```

Verifica se um número pode ser colocado em uma posição do Sudoku.

A função confere:

- linha;
- coluna;
- bloco 2x2.

Ela é usada durante a geração da solução.

---

### `FillSolution`

```c
static bool FillSolution(int board[SUDOKU_SIZE][SUDOKU_SIZE])
```

Preenche o tabuleiro com uma solução válida.

Essa função usa uma estratégia de tentativa e erro com retorno, parecida com backtracking.

---

### `CountSolutions`

```c
static void CountSolutions(int board[SUDOKU_SIZE][SUDOKU_SIZE], int *counter)
```

Conta quantas soluções possíveis existem para o Sudoku.

Ela ajuda a evitar gerar um tabuleiro ambíguo demais.

---

### `CopyBoard`

```c
static void CopyBoard(int dst[SUDOKU_SIZE][SUDOKU_SIZE], int src[SUDOKU_SIZE][SUDOKU_SIZE])
```

Copia um tabuleiro para outro.

É usada para salvar o tabuleiro inicial, o tabuleiro atual e a solução.

---

### `KeyToNumber`

```c
static int KeyToNumber(void)
```

Converte a tecla pressionada pelo jogador em um número de 1 a 4.

Ela aceita tanto o teclado principal quanto o teclado numérico.

---

## 9. Funções principais do `history.c`

O arquivo `history.c` cuida do salvamento e da leitura do histórico.

---

### `History_EnsureFile`

```c
bool History_EnsureFile(void)
```

Verifica se o arquivo `historico.txt` existe.

Se o arquivo não existir, a função cria um novo arquivo vazio.

---

### `History_SaveSession`

```c
bool History_SaveSession(const SessionRecord *session)
```

Salva uma partida no `historico.txt`.

O arquivo é aberto em modo de acréscimo, ou seja, uma nova partida é adicionada no final sem apagar as anteriores.

O formato usado é:

```txt
timestamp;alvo;tentativas;baixos;altos;palpites_csv
```

---

### `History_LoadSessions`

```c
int History_LoadSessions(SessionRecord sessions[], int maxSessions)
```

Lê o `historico.txt` e carrega as partidas salvas.

Ela usa `fgets` para ler linha por linha.

Depois, separa os campos usando `;` e separa os palpites usando `,`.

Retorna a quantidade de sessões carregadas.

---

### `History_BuildReport`

```c
AnalysisReport History_BuildReport(const SessionRecord sessions[], int count)
```

Gera o relatório estatístico com base nas sessões carregadas.

Calcula:

- total de sessões;
- média de tentativas;
- melhor sessão;
- pior sessão;
- desvio padrão;
- média de viés baixo;
- média de viés alto;
- sugestão textual de estratégia.

---

## 10. Funções recursivas em `history.c`

A recursão foi usada na parte de análise do histórico.

---

### `RecursiveSumAttempts`

```c
static int RecursiveSumAttempts(const SessionRecord sessions[], int count)
```

Soma recursivamente o total de tentativas das sessões.

---

### `RecursiveMinAttempts`

```c
static int RecursiveMinAttempts(const SessionRecord sessions[], int count)
```

Procura recursivamente a menor quantidade de tentativas.

Essa função ajuda a encontrar a melhor sessão.

---

### `RecursiveMaxAttempts`

```c
static int RecursiveMaxAttempts(const SessionRecord sessions[], int count)
```

Procura recursivamente a maior quantidade de tentativas.

Essa função ajuda a encontrar a pior sessão.

---

### `RecursiveSquareDiff`

```c
static double RecursiveSquareDiff(const SessionRecord sessions[], int count, double mean)
```

Calcula recursivamente a soma dos quadrados das diferenças em relação à média.

Ela é usada para calcular o desvio padrão.

---

### `RecursiveLowBias`

```c
static double RecursiveLowBias(const SessionRecord sessions[], int count)
```

Calcula recursivamente a média de chutes baixos.

---

### `RecursiveHighBias`

```c
static double RecursiveHighBias(const SessionRecord sessions[], int count)
```

Calcula recursivamente a média de chutes altos.

---

### `RecursiveMonotonicSteps`

```c
static int RecursiveMonotonicSteps(const int guesses[], int count, int index)
```

Conta passos em sequência crescente ou decrescente dentro dos palpites.

Essa informação ajuda a perceber se o jogador está usando uma estratégia muito linear.

---

### `RecursiveTotalMonotonic`

```c
static int RecursiveTotalMonotonic(const SessionRecord sessions[], int count)
```

Soma os passos monotônicos de todas as sessões do histórico.

---

## 11. Principais estruturas usadas

---

### `CyberGame`

```c
typedef struct {
    int secretNumber;
    char hints[MAX_HINTS][128];
    bool hintUnlocked[MAX_HINTS];
    LogicQuestion questions[MAX_LOGIC_QUESTIONS];
    int attempts;
    int lows;
    int highs;
    int guesses[MAX_GUESSES];
    int score;
    char rating[64];
} CyberGame;
```

Guarda os dados principais da partida atual.

Ela concentra informações como:

- número secreto;
- pistas;
- perguntas de lógica;
- tentativas;
- palpites;
- pontuação;
- classificação.

---

### `LogicQuestion`

```c
typedef struct {
    char prompt[512];
    char options[4][160];
    int correctOption;
    int hintToUnlock;
    bool answered;
} LogicQuestion;
```

Representa uma pergunta de lógica.

Cada pergunta tem:

- enunciado;
- quatro alternativas;
- resposta correta;
- pista liberada ao acertar;
- indicação de resposta.

---

### `SudokuGame`

```c
typedef struct {
    int board[4][4];
    int original[4][4];
    int solution[4][4];
    int selectedRow;
    int selectedCol;
    int errors;
    bool completed;
    char message[128];
} SudokuGame;
```

Guarda os dados da fase do Sudoku.

---

### `SessionRecord`

```c
typedef struct {
    char timestamp[32];
    int target;
    int attempts;
    int lows;
    int highs;
    int guesses[MAX_GUESSES];
    int guessCount;
} SessionRecord;
```

Representa uma partida salva no histórico.

---

### `AnalysisReport`

```c
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
```

Guarda o resultado da análise estatística do histórico.

---

## 12. Funções da Raylib mais usadas

O projeto usa várias funções da Raylib para janela, desenho e entrada.

| Função | Uso no jogo |
| --- | --- |
| `InitWindow` | Cria a janela do jogo. |
| `CloseWindow` | Fecha a janela e libera recursos. |
| `WindowShouldClose` | Verifica se a janela recebeu pedido de fechamento. |
| `BeginDrawing` | Inicia o desenho de um frame. |
| `EndDrawing` | Finaliza o desenho de um frame. |
| `ClearBackground` | Limpa a tela antes de desenhar. |
| `DrawText` | Desenha textos. |
| `DrawRectangle` | Desenha retângulos da interface. |
| `DrawRectangleLines` | Desenha bordas dos retângulos. |
| `DrawTexturePro` | Desenha imagem ou frame de fundo redimensionado. |
| `LoadImage` | Carrega imagens, como ícone e fundo. |
| `LoadTextureFromImage` | Transforma imagem em textura para desenhar na tela. |
| `UnloadTexture` | Libera uma textura carregada. |
| `GetMousePosition` | Pega a posição do mouse. |
| `IsMouseButtonPressed` | Detecta clique do mouse. |
| `IsKeyPressed` | Detecta tecla pressionada. |
| `GetCharPressed` | Captura caracteres digitados. |
| `GetTime` | Pega o tempo atual do jogo. |
| `SetTargetFPS` | Define o limite de FPS. |

---

## 13. Como as funções se conectam

O fluxo principal pode ser entendido assim:

```txt
main
 ├── Game_Init
 ├── Sudoku_Init
 │
 ├── SCREEN_MENU
 │    └── DrawMenu
 │
 ├── SCREEN_SUDOKU
 │    ├── Sudoku_Update
 │    └── Sudoku_Draw
 │
 ├── SCREEN_LOGIC
 │    ├── UpdateLogicScreen
 │    └── DrawLogicScreen
 │
 ├── SCREEN_GUESS
 │    ├── UpdateGuessScreen
 │    ├── DrawGuessScreen
 │    └── Game_HandleGuess
 │
 ├── SCREEN_RESULT
 │    ├── Game_CalculateScore
 │    ├── Game_ToSession
 │    ├── History_SaveSession
 │    └── DrawResultScreen
 │
 └── SCREEN_HISTORY
      ├── History_LoadSessions
      ├── History_BuildReport
      └── DrawHistoryScreen
```

Esse fluxo mostra que o `main.c` controla a partida, mas as partes específicas ficam separadas em funções próprias.

---

## 14. Resumo final

As principais funções do jogo foram separadas por responsabilidade:

- funções de interface desenham janelas, botões, fundo e telas;
- funções de partida controlam número secreto, pistas, perguntas, chutes e pontuação;
- funções do Sudoku controlam a fase numérica;
- funções do histórico cuidam da persistência e da análise;
- funções recursivas atendem à parte analítica exigida pelo projeto.

Essa organização deixa o código mais fácil de entender, testar e modificar.
