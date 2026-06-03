@echo off
cd /d "%~dp0"
echo Compilando Dia Zero com Raylib...

where windres >nul 2>nul
if %errorlevel% equ 0 (
    echo Gerando icone do executavel...
    windres icon.rc -O coff -o icon.res
    gcc -std=c11 -Wall -Wextra -Iinclude src/main.c src/sudoku.c src/history.c icon.res -o DiaZero.exe -Llib -lraylib -lopengl32 -lgdi32 -lwinmm -lm
) else (
    echo windres nao encontrado. Compilando sem icone embutido no executavel...
    gcc -std=c11 -Wall -Wextra -Iinclude src/main.c src/sudoku.c src/history.c -o DiaZero.exe -Llib -lraylib -lopengl32 -lgdi32 -lwinmm -lm
)

if %errorlevel% neq 0 (
    echo.
    echo Erro na compilacao. Verifique se o MinGW/GCC esta instalado e no PATH.
    pause
    exit /b %errorlevel%
)

echo.
echo Compilado com sucesso: DiaZero.exe
pause
