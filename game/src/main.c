/*
    main.c

    Esse é o arquivo principal do jogo.
    Aqui eu abro a janela, controlo as telas e junto as três fases:
    Sudoku, lógica e chute da frequência.
*/

// Imports do Raylib e dos arquivos que eu criei para separar o jogo.
#include "raylib.h"
#include "game.h"
#include "history.h"
#include "sudoku.h"


// Bibliotecas padrão que uso para texto, tempo, memória e contas simples.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>


// Base do layout. Mesmo em tela cheia, eu desenho pensando nesse tamanho.
#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768


// Cores principais do visual retrô/hacker.
static const Color CYBER_GREEN = {0, 255, 55, 255};
static const Color CYBER_DARK_GREEN = {0, 95, 28, 255};
static const Color CYBER_DIM_GREEN = {0, 150, 35, 145};
static const Color WIN_BLUE = {0, 35, 165, 255};
static const Color WIN_BODY = {205, 205, 205, 255};
static const Color WIN_BODY_DARK = {185, 185, 185, 255};
static const Color WIN_SHADOW = {50, 50, 50, 255};
static const Color TEXT_DARK = {18, 18, 18, 255};


// Algumas variáveis globais simples usadas pela interface.
static double gMatchStartTime = 0.0;
static char gGuessText[8] = "";


// Dados do GIF de fundo.
static Image gBackgroundImage = {0};
static Texture2D gBackgroundTexture = {0};
static int gBackgroundFrames = 0;
static int gBackgroundCurrentFrame = 0;
static int gBackgroundFrameCounter = 0;
static bool gBackgroundLoaded = false;


// Escala usada para o jogo ficar proporcional em outras resoluções.
static float gScreenScale = 1.0f;
static Vector2 gScreenOffset = {0.0f, 0.0f};

// Aqui ajusto a escala da tela para o layout não quebrar no fullscreen.
static void UpdateFullscreenTransform(void) {
    float scaleX = (float)GetScreenWidth() / (float)SCREEN_WIDTH;
    float scaleY = (float)GetScreenHeight() / (float)SCREEN_HEIGHT;
    gScreenScale = fminf(scaleX, scaleY);
    if (gScreenScale <= 0.0f) gScreenScale = 1.0f;

    gScreenOffset.x = ((float)GetScreenWidth() - (float)SCREEN_WIDTH * gScreenScale) * 0.5f;
    gScreenOffset.y = ((float)GetScreenHeight() - (float)SCREEN_HEIGHT * gScreenScale) * 0.5f;
}

// Como a tela pode estar escalada, eu converto a posição real do mouse.
static Vector2 GetVirtualMousePosition(void) {
    Vector2 mouse = GetMousePosition();
    return (Vector2){
        (mouse.x - gScreenOffset.x) / gScreenScale,
        (mouse.y - gScreenOffset.y) / gScreenScale
    };
}

// Limita um número para ele não passar do mínimo nem do máximo.
static int ClampInt(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Soma os dígitos do número secreto para montar uma pista.
static int DigitSum(int value) {
    int sum = 0;
    if (value == 0) return 0;
    while (value > 0) {
        sum += value % 10;
        value /= 10;
    }
    return sum;
}

// Verifica clique em um botão ou área da interface.
static bool Clicked(Rectangle rect) {
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetVirtualMousePosition(), rect);
}

// Verifica se o mouse está em cima de uma área.
static bool Hovered(Rectangle rect) {
    return CheckCollisionPointRec(GetVirtualMousePosition(), rect);
}

// Desenha textos maiores quebrando linha automaticamente.
static void DrawWrappedText(const char *text, int x, int y, int fontSize, int maxWidth, Color color) {
    char buffer[1400];
    snprintf(buffer, sizeof(buffer), "%s", text);

    char line[1400] = "";
    char *word = strtok(buffer, " ");
    int currentY = y;

    while (word != NULL) {
        char candidate[1400];
        candidate[0] = '\0';
        strncat(candidate, line, sizeof(candidate) - strlen(candidate) - 1);
        if (line[0] != '\0') {
            strncat(candidate, " ", sizeof(candidate) - strlen(candidate) - 1);
        }
        strncat(candidate, word, sizeof(candidate) - strlen(candidate) - 1);

        if (MeasureText(candidate, fontSize) > maxWidth && line[0] != '\0') {
            DrawText(line, x, currentY, fontSize, color);
            currentY += fontSize + 8;
            line[0] = '\0';
            strncat(line, word, sizeof(line) - 1);
        } else {
            line[0] = '\0';
            strncat(line, candidate, sizeof(line) - 1);
        }

        word = strtok(NULL, " ");
    }

    if (line[0] != '\0') {
        DrawText(line, x, currentY, fontSize, color);
    }
}

// Desenha o fundo Matrix. Primeiro tenta usar o GIF, senão usa o fundo feito no código.
static void DrawMatrixBackground(void) {
    ClearBackground(BLACK);

    if (gBackgroundLoaded && gBackgroundTexture.id != 0 && gBackgroundImage.data != NULL && gBackgroundFrames > 0) {
        gBackgroundFrameCounter++;
        if (gBackgroundFrameCounter >= 4) {
            gBackgroundFrameCounter = 0;
            gBackgroundCurrentFrame = (gBackgroundCurrentFrame + 1) % gBackgroundFrames;

            size_t frameSize = (size_t)gBackgroundImage.width * (size_t)gBackgroundImage.height * 4u;
            const unsigned char *frameData = ((const unsigned char *)gBackgroundImage.data) + frameSize * (size_t)gBackgroundCurrentFrame;
            UpdateTexture(gBackgroundTexture, frameData);
        }

        Rectangle source = {0.0f, 0.0f, (float)gBackgroundTexture.width, (float)gBackgroundTexture.height};
        Rectangle dest = {0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
        DrawTexturePro(gBackgroundTexture, source, dest, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 28});
        return;
    }

    const char glyphs[] = "01010123456789#$%&+-<>/\\[]{}=?*!";
    const int glyphCount = (int)strlen(glyphs);
    const int fontSize = 24;
    int t = (int)(GetTime() * 22.0);

    for (int x = -16; x < SCREEN_WIDTH + 16; x += 28) {
        int seed = (x + 233) * 37;
        int fall = (t + seed) % 56;
        for (int y = -56; y < SCREEN_HEIGHT + 56; y += 28) {
            int yy = y + fall;
            int idx = abs(seed + y * 11 + t / 2) % glyphCount;
            unsigned char alpha = (unsigned char)(45 + abs((seed + y + t) % 95));
            Color c = CYBER_DIM_GREEN;
            c.a = alpha;
            if (((seed + y + t) % 47) == 0) {
                c = (Color){245, 255, 245, 210};
            }
            DrawText(TextFormat("%c", glyphs[idx]), x, yy, fontSize, c);
        }
    }

    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 70});
}

// Desenha uma janela no estilo antigo do Windows.
static void DrawClassicWindow(Rectangle r, const char *title) {
    DrawRectangle((int)r.x + 7, (int)r.y + 7, (int)r.width, (int)r.height, (Color){0, 0, 0, 95});
    DrawRectangleRec(r, WIN_BODY);
    DrawRectangleLinesEx(r, 2, WHITE);
    DrawRectangleLinesEx((Rectangle){r.x + 2, r.y + 2, r.width - 4, r.height - 4}, 1, WIN_SHADOW);

    Rectangle titleBar = {r.x + 4, r.y + 4, r.width - 8, 25};
    DrawRectangleRec(titleBar, WIN_BLUE);
    DrawText(title, (int)r.x + 34, (int)r.y + 8, 17, WHITE);
    DrawRectangle((int)r.x + 12, (int)r.y + 9, 15, 15, WHITE);

    int bx = (int)(r.x + r.width - 64);
    for (int i = 0; i < 3; i++) {
        DrawRectangle(bx + i * 20, (int)r.y + 8, 16, 16, WIN_BODY);
        DrawRectangleLines(bx + i * 20, (int)r.y + 8, 16, 16, BLACK);
    }
    DrawText("_", bx + 4, (int)r.y + 5, 15, BLACK);
    DrawText("o", bx + 23, (int)r.y + 8, 12, BLACK);
    DrawText("x", bx + 44, (int)r.y + 8, 12, BLACK);
}

// Desenha uma caixinha afundada, tipo display antigo.
static void DrawClassicInset(Rectangle r, Color fill) {
    DrawRectangleRec(r, fill);
    DrawLine((int)r.x, (int)r.y, (int)(r.x + r.width), (int)r.y, WIN_SHADOW);
    DrawLine((int)r.x, (int)r.y, (int)r.x, (int)(r.y + r.height), WIN_SHADOW);
    DrawLine((int)r.x, (int)(r.y + r.height), (int)(r.x + r.width), (int)(r.y + r.height), WHITE);
    DrawLine((int)(r.x + r.width), (int)r.y, (int)(r.x + r.width), (int)(r.y + r.height), WHITE);
}

// Desenha os botões do jogo no estilo retrô.
static void DrawClassicButton(Rectangle r, const char *label, bool active, bool disabled) {
    Color fill = disabled ? (Color){170, 170, 170, 255} : (Hovered(r) ? (Color){222, 222, 222, 255} : WIN_BODY_DARK);
    DrawRectangleRec(r, fill);
    DrawLine((int)r.x, (int)r.y, (int)(r.x + r.width), (int)r.y, WHITE);
    DrawLine((int)r.x, (int)r.y, (int)r.x, (int)(r.y + r.height), WHITE);
    DrawLine((int)r.x, (int)(r.y + r.height), (int)(r.x + r.width), (int)(r.y + r.height), BLACK);
    DrawLine((int)(r.x + r.width), (int)r.y, (int)(r.x + r.width), (int)(r.y + r.height), BLACK);
    if (active) DrawRectangleLinesEx(r, 2, CYBER_GREEN);
    int fs = 20;
    int tw = MeasureText(label, fs);
    DrawText(label, (int)(r.x + r.width / 2 - tw / 2), (int)(r.y + r.height / 2 - fs / 2), fs, disabled ? GRAY : TEXT_DARK);
}

// Desenha o painel do título no canto da tela.
static void DrawHeaderPanel(void) {
    Rectangle panel = {16, 18, 382, 166};
    DrawRectangleRec(panel, (Color){0, 0, 0, 205});
    DrawRectangleLinesEx(panel, 1, CYBER_DARK_GREEN);

    DrawCircleLines(58, 62, 32, CYBER_GREEN);
    DrawRectangleLines(38, 42, 40, 40, CYBER_GREEN);
    DrawText("<>_", 40, 54, 20, CYBER_GREEN);

    DrawText("CyberCore", 108, 34, 42, CYBER_GREEN);
    DrawText("QUEBRA DE FREQUENCIA", 111, 88, 19, CYBER_GREEN);
    DrawRectangleLines(36, 111, 342, 48, CYBER_DARK_GREEN);
    DrawText("Resolva desafios. Colete pistas.", 48, 121, 15, CYBER_GREEN);
    DrawText("Restaure o sistema.", 48, 142, 15, CYBER_GREEN);
}

// Desenha o cronômetro da partida.
static void DrawTimerWindow(double startTime) {
    Rectangle r = {44, 206, 238, 130};
    DrawClassicWindow(r, "Cronometro");
    Rectangle display = {72, 254, 182, 58};
    DrawClassicInset(display, BLACK);

    int elapsed = 0;
    if (startTime > 0.01) elapsed = (int)(GetTime() - startTime);
    int minutes = elapsed / 60;
    int seconds = elapsed % 60;
    DrawText(TextFormat("%02d:%02d", minutes, seconds), 102, 267, 34, CYBER_GREEN);
    DrawText("Tempo de invasao", 88, 318, 15, CYBER_GREEN);
}

// Desenha o painel de status do sistema.
static void DrawSystemWindow(const CyberGame *game, Screen screen) {
    Rectangle r = {18, 602, 318, 136};
    DrawClassicWindow(r, "Sistema");
    DrawRectangleLines(44, 651, 52, 38, CYBER_GREEN);
    DrawRectangle(59, 690, 22, 8, CYBER_GREEN);
    DrawLine(50, 701, 92, 701, CYBER_GREEN);

    int freq = 11;
    if (screen == SCREEN_SUDOKU) freq = 28;
    if (screen == SCREEN_LOGIC) freq = 51 + game->currentQuestion * 12;
    if (screen == SCREEN_GUESS) freq = 76;
    if (screen == SCREEN_RESULT) freq = 100;
    if (screen == SCREEN_HISTORY) freq = 63;
    if (screen == SCREEN_HELP) freq = 47;

    DrawText(TextFormat("Status: %s", screen == SCREEN_RESULT ? "ESTAVEL" : "INSTAVEL"), 115, 650, 16, CYBER_GREEN);
    DrawText(TextFormat("Frequencia: %d%%", freq), 115, 677, 16, CYBER_GREEN);
    DrawRectangleLines(115, 710, 178, 15, CYBER_GREEN);
    DrawRectangle(116, 711, (int)(176 * (freq / 100.0f)), 13, CYBER_GREEN);
}

// Coisas que aparecem em praticamente todas as telas.
static void DrawGlobalDesktop(const CyberGame *game, Screen screen) {
    DrawMatrixBackground();
    DrawHeaderPanel();
    DrawTimerWindow(gMatchStartTime);
    DrawSystemWindow(game, screen);
    DrawRectangleLines(1235, 713, 112, 32, CYBER_DARK_GREEN);
    DrawText("v2.0 - CyberCore OS", 1243, 724, 12, CYBER_GREEN);
}

// Libera uma pista quando o jogador avança no jogo.
void Game_UnlockHint(CyberGame *game, int index) {
    if (index >= 0 && index < game->totalHints) {
        game->hintUnlocked[index] = true;
    }
}

// Começa uma nova partida, sorteando o número e preparando pistas e perguntas.
void Game_Init(CyberGame *game) {
    memset(game, 0, sizeof(*game));

    // Sorteio o número secreto e calculo dados para criar pistas verdadeiras.
    game->secretNumber = (rand() % 100) + 1;
    game->basicMin = ((game->secretNumber - 1) / 25) * 25 + 1;
    game->basicMax = ClampInt(game->basicMin + 24, 1, 100);
    game->decadeMin = ((game->secretNumber - 1) / 10) * 10 + 1;
    game->decadeMax = ClampInt(game->decadeMin + 9, 1, 100);
    game->digitSum = DigitSum(game->secretNumber);
    game->totalHints = 5;

    // As primeiras pistas são mais abertas e depois ficam mais específicas.
    snprintf(game->hints[0], sizeof(game->hints[0]), "Frequencia entre %d e %d", game->basicMin, game->basicMax);
    snprintf(game->hints[1], sizeof(game->hints[1]), "Paridade: %s", (game->secretNumber % 2 == 0) ? "PAR" : "IMPAR");
    snprintf(game->hints[2], sizeof(game->hints[2]), "Intervalo refinado: %d a %d", game->decadeMin, game->decadeMax);
    snprintf(game->hints[3], sizeof(game->hints[3]), "%s multipla de 5", (game->secretNumber % 5 == 0) ? "E" : "NAO E");
    snprintf(game->hints[4], sizeof(game->hints[4]), "Soma dos digitos: %d", game->digitSum);

    // Primeira pergunta: usa E para testar se o jogador entendeu as pistas.
    snprintf(
        game->questions[0].prompt,
        sizeof(game->questions[0].prompt),
        "P: a frequencia esta no intervalo do Sudoku. Q: a paridade liberada esta correta. Se P E Q sao verdadeiros, qual conclusao e valida?"
    );
    snprintf(game->questions[0].options[0], sizeof(game->questions[0].options[0]), "P e falso, mas Q e verdadeiro.");
    snprintf(game->questions[0].options[1], sizeof(game->questions[0].options[1]), "P e Q sao verdadeiros ao mesmo tempo.");
    snprintf(game->questions[0].options[2], sizeof(game->questions[0].options[2]), "Q obrigatoriamente e falso.");
    snprintf(game->questions[0].options[3], sizeof(game->questions[0].options[3]), "Nenhuma pista pode ser usada.");
    game->questions[0].correctOption = 1;
    game->questions[0].hintToUnlock = 2;

    // Segunda pergunta: usa implicação.
    snprintf(
        game->questions[1].prompt,
        sizeof(game->questions[1].prompt),
        "P: um numero e multiplo de 5. Q: o ultimo digito dele e 0 ou 5. Sabendo que P -> Q e que NAO Q e verdadeiro, qual regra foi aplicada?"
    );
    snprintf(game->questions[1].options[0], sizeof(game->questions[1].options[0]), "Afirmacao do consequente: logo P.");
    snprintf(game->questions[1].options[1], sizeof(game->questions[1].options[1]), "Modus tollens: logo NAO P.");
    snprintf(game->questions[1].options[2], sizeof(game->questions[1].options[2]), "Contradicao: P e Q sao verdadeiros.");
    snprintf(game->questions[1].options[3], sizeof(game->questions[1].options[3]), "Disjuncao: P OU Q.");
    game->questions[1].correctOption = 1;
    game->questions[1].hintToUnlock = 3;

    // Terceira pergunta: junta condições para liberar a pista final.
    snprintf(
        game->questions[2].prompt,
        sizeof(game->questions[2].prompt),
        "P: intervalo refinado confere. Q: soma dos digitos confere. R: nucleo aceita acesso. Se (P E Q) -> R e P E Q sao verdadeiros, o que segue?"
    );
    snprintf(game->questions[2].options[0], sizeof(game->questions[2].options[0]), "R: o nucleo aceita o acesso.");
    snprintf(game->questions[2].options[1], sizeof(game->questions[2].options[1]), "NAO R: o nucleo bloqueia o acesso.");
    snprintf(game->questions[2].options[2], sizeof(game->questions[2].options[2]), "P e falso.");
    snprintf(game->questions[2].options[3], sizeof(game->questions[2].options[3]), "Q e falso.");
    game->questions[2].correctOption = 0;
    game->questions[2].hintToUnlock = 4;

    snprintf(game->feedback, sizeof(game->feedback), "Use as pistas para reduzir o intervalo de busca.");
}

// Aqui trato o chute do jogador e dou o feedback de alto, baixo ou certo.
void Game_HandleGuess(CyberGame *game) {
    int guess = game->guessInput;

    if (guess < 1 || guess > 100) {
        snprintf(game->feedback, sizeof(game->feedback), "Digite uma frequencia entre 1 e 100.");
        return;
    }

    if (game->attempts < MAX_GUESSES) {
        game->guesses[game->attempts] = guess;
    }
    game->attempts++;

    if (guess == game->secretNumber) {
        game->completed = true;
        snprintf(game->feedback, sizeof(game->feedback), "ACESSO CONCEDIDO. Frequencia descoberta.");
    } else if (guess < game->secretNumber) {
        game->lows++;
        snprintf(game->feedback, sizeof(game->feedback), "Muito baixo. A frequencia real e maior.");
    } else {
        game->highs++;
        snprintf(game->feedback, sizeof(game->feedback), "Muito alto. A frequencia real e menor.");
    }
}

// Calcula a pontuação final da partida.
void Game_CalculateScore(CyberGame *game, int sudokuErrors) {
    int score = 1000;
    score -= sudokuErrors * 40;
    score -= game->logicErrors * 70;
    score -= game->attempts * 25;

    if (game->attempts <= 3) score += 100;
    if (game->logicErrors == 0) score += 50;

    game->score = ClampInt(score, 0, 1000);

    if (game->score >= 900) {
        snprintf(game->rating, sizeof(game->rating), "Hacker Lendario");
    } else if (game->score >= 750) {
        snprintf(game->rating, sizeof(game->rating), "Invasor Avancado");
    } else if (game->score >= 500) {
        snprintf(game->rating, sizeof(game->rating), "Operador de Rede");
    } else if (game->score >= 300) {
        snprintf(game->rating, sizeof(game->rating), "Aprendiz Digital");
    } else {
        snprintf(game->rating, sizeof(game->rating), "Acesso Quase Negado");
    }
}

// Passa os dados da partida para o formato salvo no histórico.
SessionRecord Game_ToSession(const CyberGame *game) {
    SessionRecord session;
    memset(&session, 0, sizeof(session));

    time_t now = time(NULL);
    struct tm timeInfo;
    struct tm *local = localtime(&now);
    if (local != NULL) {
        timeInfo = *local;
    } else {
        memset(&timeInfo, 0, sizeof(timeInfo));
    }

    strftime(session.timestamp, sizeof(session.timestamp), "%Y-%m-%d %H:%M:%S", &timeInfo);
    session.target = game->secretNumber;
    session.attempts = game->attempts;
    session.lows = game->lows;
    session.highs = game->highs;
    session.guessCount = game->attempts < MAX_GUESSES ? game->attempts : MAX_GUESSES;

    for (int i = 0; i < session.guessCount; i++) {
        session.guesses[i] = game->guesses[i];
    }

    return session;
}

// Mostra as pistas que o jogador já conseguiu.
static void DrawHintsWindow(const CyberGame *game, Rectangle r, const char *title) {
    DrawClassicWindow(r, title);
    DrawText("Pistas coletadas", (int)r.x + 22, (int)r.y + 48, 20, TEXT_DARK);
    int rowY = (int)r.y + 82;
    for (int i = 0; i < game->totalHints; i++) {
        Rectangle item = {r.x + 22, (float)rowY, r.width - 44, 42};
        DrawClassicInset(item, BLACK);
        if (game->hintUnlocked[i]) {
            DrawText(TextFormat("%02d", i + 1), (int)item.x + 12, rowY + 10, 17, CYBER_GREEN);
            DrawWrappedText(game->hints[i], (int)item.x + 54, rowY + 11, 16, (int)item.width - 65, CYBER_GREEN);
        } else {
            DrawText(TextFormat("%02d", i + 1), (int)item.x + 12, rowY + 10, 17, DARKGRAY);
            DrawText("[bloqueada]", (int)item.x + 54, rowY + 11, 16, GRAY);
        }
        rowY += 50;
    }
}

// Define a posição dos botões do menu.
static Rectangle MenuButtonRect(int index) {
    return (Rectangle){520, 342 + index * 62, 340, 46};
}

// Desenha o menu inicial.
static void DrawMenu(void) {
    Rectangle win = {470, 178, 440, 455};
    DrawClassicWindow(win, "Controle de Acesso");
    DrawText("CyberCore", 548, 238, 44, TEXT_DARK);
    DrawText("Quebra de Frequencia", 555, 302, 20, CYBER_DARK_GREEN);

    DrawClassicButton(MenuButtonRect(0), "Iniciar ataque", false, false);
    DrawClassicButton(MenuButtonRect(1), "Historico e analise", false, false);
    DrawClassicButton(MenuButtonRect(2), "Como jogar", false, false);
    DrawClassicButton(MenuButtonRect(3), "Sair", false, false);

    DrawText("Fluxo: Sudoku -> Logica -> Chute", 515, 595, 17, DARKGRAY);
}

// Tela que explica a missão antes do jogo começar.
static void DrawIntro(void) {
    Rectangle win = {430, 185, 760, 390};
    DrawClassicWindow(win, "Briefing da Missao");
    DrawText("MISSAO: INVADIR O CYBERCORE", 485, 242, 30, TEXT_DARK);
    DrawClassicInset((Rectangle){478, 295, 660, 168}, BLACK);
    DrawWrappedText(
        "O CyberCore protege sua frequencia central com tres camadas. Primeiro, estabilize uma matriz Sudoku 4x4. Depois, interprete o firewall proposicional. Por fim, use as pistas para descobrir a frequencia secreta entre 1 e 100.",
        505, 322, 21, 605, CYBER_GREEN
    );
    DrawClassicButton((Rectangle){572, 492, 470, 52}, "Pressione ENTER para iniciar", false, false);
}

// Define onde ficam os botões das alternativas.
static Rectangle LogicOptionRect(int i) {
    return (Rectangle){565, 340 + i * 58, 690, 42};
}

// Desenha a tela do quiz de lógica.
static void DrawLogicScreen(CyberGame *game) {
    Rectangle win = {515, 108, 800, 520};
    DrawClassicWindow(win, "Fase 2 - Raciocinio Logico");

    LogicQuestion *question = &game->questions[game->currentQuestion];
    DrawText(TextFormat("Pergunta %d de %d", game->currentQuestion + 1, MAX_LOGIC_QUESTIONS), 552, 163, 22, WIN_BLUE);
    DrawWrappedText(question->prompt, 552, 206, 20, 715, TEXT_DARK);

    for (int i = 0; i < 4; i++) {
        Rectangle opt = LogicOptionRect(i);
        bool active = question->answered && i == question->correctOption;
        DrawClassicButton(opt, TextFormat("%c   %s", 'A' + i, question->options[i]), active, false);
        if (active) {
            DrawRectangle((int)opt.x + 12, (int)opt.y + 10, 22, 22, CYBER_GREEN);
            DrawText(TextFormat("%c", 'A' + i), (int)opt.x + 17, (int)opt.y + 12, 17, BLACK);
        }
    }

    DrawClassicInset((Rectangle){546, 580, 735, 30}, (Color){230, 230, 230, 255});
    if (question->answered) {
        DrawText("Correto. ENTER para continuar.", 566, 586, 17, CYBER_DARK_GREEN);
    } else {
        DrawText(TextFormat("Erros de logica: %d | Clique em uma alternativa ou use 1-4", game->logicErrors), 566, 586, 17, DARKGRAY);
    }

    DrawHintsWindow(game, (Rectangle){42, 365, 395, 265}, "Pistas do Firewall");
}

// Atualiza a fase de lógica, vendo se a resposta está certa ou errada.
static void UpdateLogicScreen(CyberGame *game, Screen *screen) {
    LogicQuestion *question = &game->questions[game->currentQuestion];

    if (!question->answered) {
        int selected = -1;
        if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) selected = 0;
        if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) selected = 1;
        if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) selected = 2;
        if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) selected = 3;
        for (int i = 0; i < 4; i++) {
            if (Clicked(LogicOptionRect(i))) selected = i;
        }

        if (selected >= 0) {
            if (selected == question->correctOption) {
                question->answered = true;
                Game_UnlockHint(game, question->hintToUnlock);
            } else {
                game->logicErrors++;
            }
        }
    } else if (IsKeyPressed(KEY_ENTER) || Clicked((Rectangle){565, 580, 690, 30})) {
        game->currentQuestion++;
        if (game->currentQuestion >= MAX_LOGIC_QUESTIONS) {
            *screen = SCREEN_GUESS;
            gGuessText[0] = '\0';
            game->guessInput = 0;
        }
    }
}

// Desenha a fase final de chute da frequência.
static void DrawGuessScreen(const CyberGame *game) {
    Rectangle win = {390, 245, 880, 415};
    DrawClassicWindow(win, "Fase 3 - Quebra de Frequencia");

    DrawText("Pistas coletadas", 420, 304, 20, TEXT_DARK);
    int clueY = 338;
    for (int i = 0; i < game->totalHints; i++) {
        if (!game->hintUnlocked[i]) continue;
        Rectangle item = {420, (float)clueY, 340, 42};
        DrawClassicInset(item, BLACK);
        DrawText("=>", 438, clueY + 11, 17, CYBER_GREEN);
        DrawText(game->hints[i], 483, clueY + 11, 16, CYBER_GREEN);
        clueY += 50;
        if (clueY > 540) break;
    }

    DrawText("Com base nas pistas, descubra a frequencia correta.", 795, 310, 19, TEXT_DARK);
    DrawText("Digite sua resposta:", 795, 367, 18, TEXT_DARK);
    Rectangle input = {795, 397, 330, 48};
    DrawClassicInset(input, WHITE);
    DrawText(gGuessText[0] ? gGuessText : "Digite um numero...", 812, 411, 20, gGuessText[0] ? BLACK : GRAY);
    DrawClassicInset((Rectangle){1145, 397, 55, 48}, BLACK);
    DrawText("/\\/", 1159, 410, 25, CYBER_GREEN);
    DrawClassicButton((Rectangle){795, 468, 410, 54}, "Confirmar", false, false);

    DrawClassicInset((Rectangle){420, 585, 780, 42}, (Color){225, 225, 225, 255});
    DrawText(game->feedback, 440, 597, 18, CYBER_DARK_GREEN);
    DrawText(TextFormat("Tentativas: %d | Baixos: %d | Altos: %d", game->attempts, game->lows, game->highs), 897, 597, 17, CYBER_DARK_GREEN);
}

// Lida com o texto digitado no chute e salva a partida quando acerta.
static void UpdateGuessScreen(CyberGame *game, Screen *screen, int sudokuErrors) {
    int key = GetCharPressed();
    while (key > 0) {
        int len = (int)strlen(gGuessText);
        if (key >= '0' && key <= '9' && len < 3) {
            gGuessText[len] = (char)key;
            gGuessText[len + 1] = '\0';
        }
        key = GetCharPressed();
    }

    if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_DELETE)) && strlen(gGuessText) > 0) {
        gGuessText[strlen(gGuessText) - 1] = '\0';
    }

    bool confirm = IsKeyPressed(KEY_ENTER) || Clicked((Rectangle){795, 468, 410, 54});
    if (confirm) {
        game->guessInput = atoi(gGuessText);
        Game_HandleGuess(game);
        if (game->completed) {
            Game_CalculateScore(game, sudokuErrors);
            if (!game->saved) {
                SessionRecord session = Game_ToSession(game);
                History_SaveSession(&session);
                game->saved = true;
            }
            *screen = SCREEN_RESULT;
        } else {
            gGuessText[0] = '\0';
            game->guessInput = 0;
        }
    }
}

// Mostra o resultado da partida.
static void DrawResultScreen(const CyberGame *game, const SudokuGame *sudoku) {
    Rectangle win = {420, 150, 760, 455};
    DrawClassicWindow(win, "Resultado da Invasao");
    DrawText("ACESSO CONCEDIDO", 520, 210, 44, CYBER_DARK_GREEN);
    DrawClassicInset((Rectangle){490, 278, 620, 220}, BLACK);
    DrawText(TextFormat("Frequencia descoberta: %d", game->secretNumber), 525, 305, 24, CYBER_GREEN);
    DrawText(TextFormat("Tentativas no chute: %d", game->attempts), 525, 345, 22, CYBER_GREEN);
    DrawText(TextFormat("Erros no Sudoku: %d", sudoku->errors), 525, 383, 22, CYBER_GREEN);
    DrawText(TextFormat("Erros na logica: %d", game->logicErrors), 525, 421, 22, CYBER_GREEN);
    DrawText(TextFormat("Pontuacao final: %d", game->score), 815, 345, 25, GOLD);
    DrawText(TextFormat("Classe: %s", game->rating), 815, 388, 21, CYBER_GREEN);
    DrawClassicButton((Rectangle){535, 525, 260, 46}, "ENTER - Menu", false, false);
    DrawClassicButton((Rectangle){825, 525, 260, 46}, "H - Historico", false, false);
}

// Mostra o histórico salvo no arquivo txt e a análise.
static void DrawHistoryScreen(void) {
    SessionRecord sessions[MAX_SESSIONS];
    int count = History_LoadSessions(sessions, MAX_SESSIONS);
    AnalysisReport report = History_BuildReport(sessions, count);

    Rectangle win = {400, 110, 885, 535};
    DrawClassicWindow(win, "Historico e Analise");
    DrawText("RELATORIO DO CYBERCORE", 440, 168, 30, TEXT_DARK);

    if (count <= 0) {
        DrawClassicInset((Rectangle){440, 230, 785, 200}, BLACK);
        DrawWrappedText(report.suggestion, 465, 260, 24, 735, CYBER_GREEN);
    } else {
        DrawClassicInset((Rectangle){440, 220, 365, 270}, BLACK);
        DrawText(TextFormat("Total de sessoes: %d", report.totalSessions), 465, 245, 20, CYBER_GREEN);
        DrawText(TextFormat("Media de tentativas: %.2f", report.averageAttempts), 465, 285, 20, CYBER_GREEN);
        DrawText(TextFormat("Melhor sessao: %d tentativas", report.bestAttempts), 465, 325, 20, CYBER_GREEN);
        DrawText(TextFormat("Pior sessao: %d tentativas", report.worstAttempts), 465, 365, 20, CYBER_GREEN);
        DrawText(TextFormat("Desvio padrao: %.2f", report.standardDeviation), 465, 405, 20, CYBER_GREEN);

        DrawClassicInset((Rectangle){835, 220, 390, 270}, BLACK);
        DrawText(TextFormat("Vies medio baixo: %.1f%%", report.averageLowBias * 100.0), 860, 245, 20, CYBER_GREEN);
        DrawText(TextFormat("Vies medio alto: %.1f%%", report.averageHighBias * 100.0), 860, 285, 20, CYBER_GREEN);
        DrawWrappedText(TextFormat("Passos monotonicos: %d", report.monotonicSteps), 860, 325, 20, 340, CYBER_GREEN);

        DrawText("Sugestao do CyberCore:", 440, 520, 22, TEXT_DARK);
        DrawWrappedText(report.suggestion, 440, 555, 19, 770, CYBER_DARK_GREEN);
    }
    DrawClassicButton((Rectangle){694, 598, 300, 36}, "ENTER/H - voltar", false, false);
}

// Tela com as instruções de como jogar.
static void DrawHelpScreen(void) {
    Rectangle win = {390, 105, 890, 555};
    DrawClassicWindow(win, "Como Jogar");
    DrawText("COMO JOGAR", 430, 165, 34, TEXT_DARK);
    DrawClassicInset((Rectangle){430, 220, 785, 318}, BLACK);
    DrawWrappedText("1) Sudoku: selecione as casas vazias e preencha com 1 a 4. A matriz libera as primeiras pistas.", 462, 250, 21, 720, CYBER_GREEN);
    DrawWrappedText("2) Logica: responda perguntas sobre E, OU, NAO e implicacao. Cada acerto desbloqueia mais informacao.", 462, 335, 21, 720, CYBER_GREEN);
    DrawWrappedText("3) Chute: use as pistas para descobrir a frequencia secreta entre 1 e 100. O jogo salva o historico e gera analise recursiva.", 462, 420, 21, 720, CYBER_GREEN);
    DrawClassicButton((Rectangle){690, 585, 300, 42}, "ENTER - voltar", false, false);
}

// Reinicia tudo para começar uma partida nova.
static void StartNewGame(CyberGame *game, SudokuGame *sudoku, Screen *screen) {
    Game_Init(game);
    Sudoku_Init(sudoku);
    gMatchStartTime = GetTime();
    gGuessText[0] = '\0';
    *screen = SCREEN_INTRO;
}

// Função principal: inicia o Raylib, carrega os arquivos e roda o loop do jogo.
int main(void) {
    // Inicio o rand para o número e o Sudoku mudarem a cada jogo.
    srand((unsigned int)time(NULL));

    // Crio a janela do Raylib.
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CyberCore - Quebra de Frequencia");

    // Coloco a janela em fullscreen usando o tamanho do monitor.
    int monitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);
    if (monitorWidth > 0 && monitorHeight > 0) {
        SetWindowSize(monitorWidth, monitorHeight);
        ToggleFullscreen();
    }

    SetTargetFPS(60);

    // Deixo o jogo procurando os arquivos a partir da pasta do executável.
    // Assim o historico.txt fica junto do jogo.
    const char *appDir = GetApplicationDirectory();
    if (appDir != NULL && appDir[0] != '\0') {
        ChangeDirectory(appDir);
    }
    History_EnsureFile();

    // Carrego o GIF do fundo. Se não achar de primeira, tento outro caminho.
    const char *backgroundPath = "assets/background/36030.gif";
    if (!FileExists(backgroundPath)) {
        backgroundPath = "game/assets/background/36030.gif";
    }
    if (FileExists(backgroundPath)) {
        gBackgroundImage = LoadImageAnim(backgroundPath, &gBackgroundFrames);
        if (gBackgroundImage.data != NULL && gBackgroundFrames > 0) {
            Image firstFrame = {
                .data = gBackgroundImage.data,
                .width = gBackgroundImage.width,
                .height = gBackgroundImage.height,
                .mipmaps = 1,
                .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
            };
            gBackgroundTexture = LoadTextureFromImage(firstFrame);
            SetTextureFilter(gBackgroundTexture, TEXTURE_FILTER_POINT);
            gBackgroundLoaded = (gBackgroundTexture.id != 0);
        }
    }

    // Estado inicial: começa no menu.
    Screen screen = SCREEN_MENU;
    bool exitRequested = false;
    CyberGame game;
    SudokuGame sudoku;
    Game_Init(&game);
    Sudoku_Init(&sudoku);

    // Loop principal do jogo.
    while (!WindowShouldClose() && !exitRequested) {
        UpdateFullscreenTransform();

        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

        // Primeiro atualizo os comandos e mudanças de tela.
        switch (screen) {
            case SCREEN_MENU:
                if (IsKeyPressed(KEY_ENTER) || Clicked(MenuButtonRect(0))) StartNewGame(&game, &sudoku, &screen);
                if (IsKeyPressed(KEY_H) || Clicked(MenuButtonRect(1))) screen = SCREEN_HISTORY;
                if (IsKeyPressed(KEY_C) || Clicked(MenuButtonRect(2))) screen = SCREEN_HELP;
                if (Clicked(MenuButtonRect(3)) || IsKeyPressed(KEY_ESCAPE)) exitRequested = true;
                break;

            case SCREEN_INTRO:
                if (IsKeyPressed(KEY_ENTER) || Clicked((Rectangle){572, 492, 470, 52})) screen = SCREEN_SUDOKU;
                break;

            case SCREEN_SUDOKU:
                Sudoku_Update(&sudoku);
                if (sudoku.completed) {
                    Game_UnlockHint(&game, 0);
                    Game_UnlockHint(&game, 1);
                    if (IsKeyPressed(KEY_ENTER) || Clicked((Rectangle){965, 600, 325, 42})) {
                        screen = SCREEN_LOGIC;
                    }
                }
                break;

            case SCREEN_LOGIC:
                UpdateLogicScreen(&game, &screen);
                break;

            case SCREEN_GUESS:
                UpdateGuessScreen(&game, &screen, sudoku.errors);
                break;

            case SCREEN_RESULT:
                if (IsKeyPressed(KEY_ENTER) || Clicked((Rectangle){535, 525, 260, 46})) screen = SCREEN_MENU;
                if (IsKeyPressed(KEY_H) || Clicked((Rectangle){825, 525, 260, 46})) screen = SCREEN_HISTORY;
                break;

            case SCREEN_HISTORY:
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_H) || Clicked((Rectangle){694, 598, 300, 36})) screen = SCREEN_MENU;
                break;

            case SCREEN_HELP:
                if (IsKeyPressed(KEY_ENTER) || Clicked((Rectangle){690, 585, 300, 42})) screen = SCREEN_MENU;
                break;
        }

        // Depois desenho o frame na tela.
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D((Camera2D){
            .offset = gScreenOffset,
            .target = (Vector2){0.0f, 0.0f},
            .rotation = 0.0f,
            .zoom = gScreenScale
        });

        DrawGlobalDesktop(&game, screen);

        // Desenho apenas a tela que está aberta agora.
        switch (screen) {
            case SCREEN_MENU:
                DrawMenu();
                break;

            case SCREEN_INTRO:
                DrawIntro();
                break;

            case SCREEN_SUDOKU:
                Sudoku_Draw(&sudoku);
                DrawHintsWindow(&game, (Rectangle){945, 125, 395, 420}, "Pistas Obtidas");
                if (sudoku.completed) {
                    DrawClassicWindow((Rectangle){945, 540, 395, 120}, "Firewall Desbloqueado");
                    DrawText("Matriz sincronizada.", 974, 582, 18, TEXT_DARK);
                    DrawClassicButton((Rectangle){965, 600, 325, 42}, "Abrir firewall", false, false);
                }
                break;

            case SCREEN_LOGIC:
                DrawLogicScreen(&game);
                break;

            case SCREEN_GUESS:
                DrawGuessScreen(&game);
                break;

            case SCREEN_RESULT:
                DrawResultScreen(&game, &sudoku);
                break;

            case SCREEN_HISTORY:
                DrawHistoryScreen();
                break;

            case SCREEN_HELP:
                DrawHelpScreen();
                break;
        }

        EndMode2D();
        EndDrawing();
    }

    // Libero o GIF e a textura antes de fechar.
    if (gBackgroundTexture.id != 0) {
        UnloadTexture(gBackgroundTexture);
    }
    if (gBackgroundImage.data != NULL) {
        UnloadImage(gBackgroundImage);
    }

    CloseWindow();
    return 0;
}
