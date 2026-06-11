# 📄 Guia de Contribuição e Configuração de Ambiente – Dia Zero

Este documento orienta desenvolvedores e avaliadores sobre como configurar o ambiente local, instalar as dependências necessárias, compilar e executar o jogo **Dia Zero**.

---

## 🛠️ Pré-requisitos do Sistema

O projeto foi construído utilizando a **Linguagem C (Padrão C11)** e depende da biblioteca multimídia **Raylib** para a execução do sistema de áudio e interface.

### 1. Compilador GCC

Certifique-se de ter o GCC instalado no seu sistema operacional:

* **Windows:** Recomendado utilizar o w64devkit ou MSYS2 (com o pacote `mingw-w64-x86_64-gcc`).
* **Linux (Ubuntu/Debian):**

```bash
sudo apt install build-essential
```

* **macOS:**

```bash
xcode-select --install
```

### 2. Instalação da Biblioteca Raylib

A aplicação necessita da Raylib (versão 4.5 ou superior) configurada no sistema.

#### Windows (via MSYS2)

```bash
pacman -S mingw-w64-x86_64-raylib
```

#### Linux (Ubuntu/Debian)

```bash
sudo apt install libraylib-dev
```

#### macOS (via Homebrew)

```bash
brew install raylib
```

---

## 🚀 Como Executar o Projeto Localmente

Siga o passo a passo abaixo para clonar, compilar e rodar a aplicação na sua máquina.

### 1. Clonar o Repositório

```bash
git clone https://github.com/SEU_USUARIO/NOME_DO_REPOSITORIO.git
cd NOME_DO_REPOSITORIO
```

### 2. Compilação do Código

Execute o comando de compilação adequado para o seu sistema operacional a partir do terminal na raiz do projeto.

#### Linux / macOS

```bash
gcc -std=c11 -Wall main.c -o diazero -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

> **Observação:** Em algumas distribuições Linux ou versões do macOS, apenas `-lraylib -lm` pode ser suficiente, dependendo da forma como a biblioteca foi instalada.

#### Windows (MinGW / MSYS2)

```bash
gcc -std=c11 -Wall main.c -o diazero.exe -lraylib -lopengl32 -lgdi32 -lwinmm
```

### 3. Executar o Jogo

Após a compilação bem-sucedida, execute o binário gerado.

#### Linux / macOS

```bash
./diazero
```

#### Windows

```bash
.\diazero.exe
```

> ⚠️ **Importante:** O executável deve ser executado a partir da raiz do projeto para que o sistema consiga localizar corretamente os arquivos de áudio e demais assets armazenados em diretórios como `./assets/sons/`.

---

## 🌿 Padrão de Commits e Fluxo de Trabalho

Caso deseje contribuir com melhorias para o projeto, siga as diretrizes abaixo.

### 1. Criação de Branches

Sempre crie uma branch a partir da `main` antes de iniciar uma nova funcionalidade ou correção.

Exemplos:

```text
feature/nome-da-historia
bugfix/nome-do-bug
```

### 2. Boas Práticas de Commit

Mantenha mensagens de commit claras e objetivas, preferencialmente no gerúndio ou no imperativo.

Exemplos:

```text
Implementando sistema de combate
Corrigindo erro de carregamento de fases
Refatorando lógica de progressão do jogador
```

### 3. Pull Requests

Ao finalizar suas alterações:

1. Faça o push da sua branch para o repositório remoto;
2. Abra um Pull Request descrevendo as mudanças realizadas;
3. Aguarde a revisão de pelo menos um integrante da equipe;
4. Após aprovação, realize o merge para a branch principal.

---

## 🤝 Código de Conduta

* Mantenha uma comunicação respeitosa e colaborativa.
* Documente alterações relevantes sempre que possível.
* Evite commits muito grandes ou com múltiplas responsabilidades.
* Priorize a legibilidade e manutenção do código.

A colaboração de todos é fundamental para a evolução do projeto **Dia Zero**.
