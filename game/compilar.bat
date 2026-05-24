@echo off
cd /d "%~dp0"
echo Compilando CyberCore com Raylib...
gcc -std=c11 -Wall -Wextra -Iinclude src/main.c src/sudoku.c src/history.c -o Jogo.exe -Llib -lraylib -lopengl32 -lgdi32 -lwinmm -lm
if %errorlevel% neq 0 (
    echo.
    echo Erro na compilacao. Verifique se o MinGW/GCC esta instalado e no PATH.
    pause
    exit /b %errorlevel%
)
echo.
echo Compilado com sucesso: Jogo.exe
pause
