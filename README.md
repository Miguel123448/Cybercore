# Atalhos
- [Requesitos PIF](https://github.com/renanalencar/pif-2026-1-desc?tab=readme-ov-file#vis%C3%A3o-geral)
- [Entregas FDS](https://drive.google.com/file/d/1BAMUIo1b_WoI1TR8rMUF-cdp-Rh9lb6P/view)

# 🕹️ Jogo de Adivinhação – Decodificação de Frequência: Contra-ataque Hacker

Projeto Integrador 2026.1 – Programação Imperativa e Funcional (PIF)  
**Tema:** Jogo de adivinhação com narrativa de segurança cibernética.

## 📖 Sinopse

Você é um analista de segurança da CyberCore. Um ataque hacker está injetando uma frequência desconhecida (entre 1 e 100 MHz) nos servidores. Para neutralizar a invasão, você deve **descobrir a frequência alvo** com o menor número de tentativas possível. A cada palpite, o sistema informa se a frequência é muito baixa, muito alta ou correta. Cada partida é registrada em um arquivo de histórico, permitindo estatísticas e sugestões de melhoria.

## 👥 Equipe

| Integrante | Papel | Responsabilidades |
|------------|-------|--------------------|
| Breno Luiz de Lima Cruz | Desenvolvedor / Organizador / Documentador | Gerência do time, documentação geral, auxílio no desenvolvimento 
| Miguel Pereira de Lemos | Desenvolvedor / Especialista em C | Implementação da lógica principal, recursão, otimização em C |
| Eloi de Lima Sousa | Desenvolvedor de Front-end | Criação das telas, interface com o usuário (modo texto ou gráfico) |
| Lucas Felipe Barreto Cavalcante | Desenvolvedor de Back-end | Lógica do jogo, validações, persistência em arquivo |
| Lucas Filipe de Lima Segundo | Desenvolvedor de Front-end | Telas, experiência do usuário, fluxo de navegação |
| Leticia Gomes da Silva | Desenvolvedora de Front-end | Telas, componentes visuais, integração com back-end |
| Pablo Arthur Eustáquio de Lima | Desenvolvedor de Back-end | Algoritmos de análise estatística, recursão, manipulação de arquivos |

> **Nota:** Os papéis podem ser rotativos conforme a necessidade das sprints.

## 📦 Entrega 01 – Histórias de usuário, backlog e organização

### Quadro Kanban

![Kanban completo](./assets/kanban.png)

A coluna **Backlog** contém as 11 histórias priorizadas (da mais prioritária no topo para a menos prioritária na base). As demais colunas (A Fazer, Em andamento, Revisão, Concluído) serão preenchidas durante o desenvolvimento.

### Backlog priorizado

![Backlog priorizado - parte 1](./assets/backlog-1.png)
![Backlog priorizado - parte 2](./assets/backlog-2.png)

A ordem atual é:

1. **UH1** – Iniciar nova partida (Prioridade Alta)
2. **UH2** – Receber dicas “muito alto / muito baixo” (Prioridade Alta)
3. **UH3** – Registrar sessão no histórico (Prioridade Alta)
4. **UH4** – Tratar entradas inválidas sem travar (Prioridade Alta)
5. **UH5** – Visualizar estatísticas agregadas (Prioridade Média)
6. **UH6** – Recuperar histórico ao iniciar (Prioridade Média)
7. **UH7** – Adicionar música de fundo (Prioridade Média)
8. **UH8** – Sugestões estratégicas personalizadas (Prioridade Média)
9. **UH9** – Calcular desvio padrão usando recursão (Prioridade Baixa)
10. **UH10** – Identificar monotonicidade dos palpites (recursivo) (Prioridade Baixa)
11. **UH11** – Comparar desempenho com busca binária (Prioridade Baixa)

Cada história segue o **padrão 3Cs** (Card, Conversation, Confirmation) e possui checklist de tarefas.

### Histórias de usuário (detalhadas)

As 11 histórias completas estão disponíveis no [quadro do Trello](https://trello.com/b/MNMd59kf/kanban-projeto-interno) e também no arquivo [`historias.md`](docs/historias.md).

## 🚀 Próximas etapas

- **Entrega 03:** Implementação do jogo, persistência e estatísticas.
- **Entrega 04:** Projeto final polido, testes e documentação.

## 📚 Tecnologias

- Linguagem C (padrão C11)
- Biblioteca padrão: `stdio.h`, `stdlib.h`, `time.h`, `math.h` `raylib.h`
- Persistência em arquivo texto (`historico.txt`)
- Compilação: `gcc -std=c11 -Wall -lm -o jogo main.c`

## 📄 Licença

Projeto acadêmico – sem fins comerciais.

---
*Repositório criado para a disciplina de PIF – Prof. mr-costaalencar*
