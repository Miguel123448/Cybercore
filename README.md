# Atalhos
- [Requisitos PIF](https://github.com/renanalencar/pif-2026-1-desc?tab=readme-ov-file#vis%C3%A3o-geral)
- [Entregas FDS](https://drive.google.com/file/d/1QdrB_ex6qDF2VHWUT76D6Q5JakzSM-mX/view)
- [Relatório Evolutivo](https://docs.google.com/document/d/1gqUDhjhyERNt30xJk6r1T10XqWH3UZcp/edit?usp=sharing&ouid=111242244603130521782&rtpof=true&sd=true)

# 🕹️ Jogo de Adivinhação – Decodificação de Frequência: Contra-ataque Hacker

Projeto Integrador 2026.1 – Programação Imperativa e Funcional (PIF)  
**Tema:** Jogo de adivinhação com narrativa de segurança cibernética.

## 📖 Sinopse

Você é um analista de segurança da CyberCore. Um ataque hacker está injetando uma frequência desconhecida (entre 1 e 100 MHz) nos servidores. Para neutralizar a invasão, você deve **descobrir a frequência alvo** com o menor número de tentativas possível. A cada palpite, o sistema informa se a frequência é muito baixa, muito alta ou correta. Cada partida é registrada em um arquivo de histórico, permitindo estatísticas e sugestões de melhoria.

## 👥 Equipe

| Integrante | Papel | Responsabilidades |
|------------|-------|--------------------|
| Breno Luiz de Lima Cruz | Desenvolvedor / Organizador / Documentador | Gerência do time, documentação geral, auxílio no desenvolvimento |
| Miguel Pereira de Lemos | Desenvolvedor / Especialista em C | Implementação da lógica principal, recursão, otimização em C |
| Eloi de Lima Sousa | Desenvolvedor de Front-end | Criação das telas, interface com o usuário (modo texto ou gráfico) |
| Lucas Felipe Barreto Cavalcante | Desenvolvedor de Back-end | Lógica do jogo, validações, persistência em arquivo |
| Lucas Filipe de Lima Segundo | Desenvolvedor de Front-end | Telas, experiência do usuário, fluxo de navegação |
| Leticia Gomes da Silva | Desenvolvedora de Front-end | Telas, componentes visuais, integração com back-end |
| Pablo Arthur Eustáquio de Lima | Desenvolvedor de Back-end | Algoritmos de análise estatística, recursão, manipulação de arquivos |

> **Nota:** Os papéis podem ser rotativos conforme a necessidade das sprints.

## 📦 Entrega 01 – Histórias de usuário, backlog e organização

### Quadro Kanban
![Kanban completo](./assets/kanban/quadro-kanban.png)

### Backlog priorizado
![Backlog priorizado - parte 1](./assets/kanban/backlog-01.png)
![Backlog priorizado - parte 2](./assets/kanban/backlog-02.png)

### Histórias de usuário (detalhadas)
As 10 histórias completas estão disponíveis no [quadro do Trello](https://trello.com/b/MNMd59kf/kanban-projeto-interno) e também no arquivo [`historias.md`](docs/historias.md).

## 🎨 Entrega 02 – Prototipação, UX e Modelagem de Processos

### Protótipos de Baixa Fidelidade (Lo-Fi)
Os protótipos iniciais foram desenvolvidos no Figma para validar o fluxo de navegação e a hierarquia de informações.
- 🔗 [Acesse o protótipo no Figma aqui](https://www.figma.com/make/hvzhHgR8lGsDkOvLYZvtSs/CyberCore-Game-Prototype?t=PWjPK4Py01ldBkYl-20&fullscreen=1)

### Sketches e Storyboards
Abaixo estão os esboços manuais e a sequência narrativa das principais interações (mínimo de 10 unidades).

| História | Sketch / Storyboard |
|----------|---------------------|
| **UH1** | ![UH1 Sketch](./assets/sketches/uh1.png) |
| **UH2** | ![UH2 Sketch](./assets/sketches/uh2.png) |
| **UH3** | ![UH3 Sketch](./assets/sketches/uh3.png) |
| **UH4** | ![UH4 Sketch](./assets/sketches/uh4.png) |
| **UH5** | ![UH5 Sketch](./assets/sketches/uh5.png) |
| **UH6** | ![UH6 Sketch](./assets/sketches/uh6.png) |
| **UH7** | ![UH7 Sketch](./assets/sketches/uh7.png) |
| **UH8** | ![UH8 Sketch](./assets/sketches/uh8.png) |
| **UH9** | ![UH9 Sketch](./assets/sketches/uh9.png) |
| **UH10** | ![UH10 Sketch](./assets/sketches/uh10.png) |

- 🔗 [Acesse os sketches e storyboards no Figma aqui](https://www.figma.com/design/rhhIDpgt5qU3j4WoQvU7wj/Sem-t%C3%ADtulo?node-id=0-1&t=rytzOBqNChNGkl37-1)

> *Nota: Todos os arquivos também estão anexados aos seus respectivos cards no Trello.*

### Screencast do Protótipo
Vídeo demonstrativo da navegação e funcionalidades planejadas.
- 📺 [Assista ao Screencast](https://drive.google.com/file/d/1ptDQPoB4hk8FwIXI8O0mnft34nn2L9Tx/view?usp=sharing)

### Diagramas de Atividades do Sistema
Modelagem do fluxo lógico para cada uma das histórias de usuário.

- [X] **UH1:** [Visualizar Diagrama](./assets/diagramas/atividade-uh1.jpg)
- [X] **UH2:** [Visualizar Diagrama](./assets/diagramas/atividade-uh2.jpg)
- [X] **UH3:** [Visualizar Diagrama](./assets/diagramas/atividade-uh3.jpg)
- [X] **UH4:** [Visualizar Diagrama](./assets/diagramas/atividade-uh4.png)
- [X] **UH5:** [Visualizar Diagrama](./assets/diagramas/atividade-uh5.jpg)
- [X] **UH6:** [Visualizar Diagrama](./assets/diagramas/atividade-uh6.jpg)
- [ ] **UH7:** [Visualizar Diagrama](./assets/diagramas/atividade-uh7.png)
- [X] **UH8:** [Visualizar Diagrama](./assets/diagramas/atividade-uh8.jpg)
- [X] **UH9:** [Visualizar Diagrama](./assets/diagramas/atividade-uh9.jpg)
- [X] **UH10:** [Visualizar Diagrama](./assets/diagramas/atividade-uh10.jpg)

> *Nota: Todos os arquivos também estão anexados aos seus respectivos cards no Trello.*

---

## 🚀 Próximas etapas
- **Entrega 03:** Implementação do jogo, persistência e estatísticas.
- **Entrega 04:** Projeto final polido, testes e documentação.

## 📚 Tecnologias
- Linguagem C (padrão C11)
- Biblioteca padrão: `stdio.h`, `stdlib.h`, `time.h`, `math.h`, `raylib.h`
- Persistência em arquivo texto (`historico.txt`)
- Compilação: `gcc -std=c11 -Wall -lm -o jogo main.c`

## Programação em Par experimentada — persistência do histórico

Durante a implementação da persistência do jogo, usamos programação em par para organizar melhor a lógica de salvamento e leitura do histórico. A ideia foi trabalhar com uma pessoa escrevendo o código e outra acompanhando, conferindo se o formato do arquivo estava correto e se os dados seriam carregados depois sem erro. Em alguns momentos os papéis foram trocados, para que os dois pares entendessem como o historico.txt funcionava.

A persistência foi feita em arquivo texto simples, porque isso facilita os testes e permite abrir o arquivo manualmente para conferir se as partidas estão sendo salvas corretamente. Cada partida é armazenada em uma linha, usando o formato:

```txt
timestamp;alvo;tentativas;baixos;altos;palpites_csv
```

### Funcionalidades feitas por cada par

| Par | Funcionalidades executadas |
| --- | --- |
| **Par 1 — salvamento das partidas** | Implementou a gravação das partidas no `historico.txt`, usando abertura do arquivo em modo de acréscimo. Esse par ficou responsável por salvar o timestamp, o número alvo, a quantidade de tentativas, os palpites baixos, os palpites altos e a lista de palpites em formato CSV. Também verificou se o arquivo era criado corretamente quando ainda não existia. |
| **Par 2 — leitura e reconstrução do histórico** | Implementou a leitura do histórico com `fgets`, separando os campos por `;` e reconstruindo os dados das partidas antigas. Esse par também integrou o histórico com a tela de análise do jogo, permitindo calcular média de tentativas, melhor sessão, pior sessão, desvio padrão, viés baixo/alto e sugestões de estratégia. |

### O que aprendemos com essa parte

A programação em par ajudou bastante na parte de persistência, porque qualquer erro pequeno no formato do arquivo poderia atrapalhar a leitura depois. Com duas pessoas revisando o mesmo trecho, ficou mais fácil perceber problemas como separador errado, campo faltando, arquivo não encontrado ou diferença entre salvar uma partida nova e carregar partidas antigas.


Também foi importante manter o histórico em `.txt`, pois isso deixou os testes mais simples. Assim, conseguimos abrir o `historico.txt`, conferir as linhas salvas e confirmar se o jogo estava registrando corretamente as sessões.


## 📄 Licença
Projeto acadêmico – sem fins comerciais.

---
*Repositório criado para a disciplina de PIF – Prof. mr-costaalencar*
