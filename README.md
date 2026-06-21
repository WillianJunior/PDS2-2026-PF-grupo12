# PDS2-2026-PF-grupo12 — Ricettario

Livro de receitas digital desenvolvido como Projeto Final da disciplina de Programação e Desenvolvimento de Software II

## Integrantes do Grupo

- Bernardo Amaral Lima 
- Caetano Salomon Nunes
- Murilo Marciel
- Rafael Estevão Silva Cunha

## Descrição do Problema

O intuito dessa aplicação é criar um livro de receitas digital que pode ser usado pelo usuário para adicionar, pesquisar e organizar receitas próprias ou de outros usuários, alem de avaliá-las.

## Motivação da Escolha

Ao analisar os possíveis temas, a ideia de um Ricettario parecia conter a oportunidade de aplicar de maneira simples e direta muitos dos conceitos explorados nas aulas de PDS2. A principal motivação para a escolha foi a facilidade de visualizar a programação orientada a objetos na aplicação. Acreditamos que será uma ótima opportunidade de colocar em prática os nossos conhecimentos.

## Objetivos

- Permitir o cadastro e a autenticação de usuários.
- Permitir que usuários cadastrem, editem e removam suas próprias receitas.
- Oferecer busca e filtragem de receitas por título, dificuldade e nota mínima.
- Permitir que usuários avaliem e comentem receitas.
- Permitir marcar receitas como favoritas.
- Disponibilizar templates de receitas para verificação e geração de novas receitas.
- Mais funcionalidades COZINHANDO !

## Estrutura de Diretórios

```
.
├── src/         # Implementacoes .cpp das classes e o arquivo main
├── include/     # Cabecalhos .hpp com as assinaturas das classes
├── tests/       # Testes de unidade via doctest
├── build/       # Artefatos de compilacao e relatorios de cobertura gerados 
├── design/      # User Stories e Cartoes CRC 
├── data/        # Dados persistidos (ingredientes, receitas, templates...)
├── Makefile     # Automacao de build, testes e coverage
├── Doxyfile     # Configuracao da documentacao Doxygen
└── README.md    # Este arquivo :)
```

## Classes

- **Usuario** — representa um usuário do sistema, com autenticação, receitas próprias e favoritas.
- **Receita** — uma receita com título, instruções, ingredientes, dificuldade, categoria e avaliações.
- **Ingrediente** — um ingrediente com nome, quantidade, unidade de medida e tipo.
- **Avaliacao** — uma avaliação de receita, com nota, comentário e autor.
- **TemplateReceita** — modelo de receita usado para verificação e geração de novas receitas.
- **Sistema** — fachada que coordena usuários, receitas, login, busca, filtros e avaliações.

## Compilação e Execução

### Pré-requisitos

- `g++` com suporte a C++17
- `make`
- `gcovr` para relatorio de cobrertura. Instale com os seguintes comandos na propria maquina: `pip install gcovr` ou `sudo apt install gcovr`

### Comandos

Compilar o executável principal:

```bash
make
```

Executar o programa:

```bash
./build/app
```

Rodar os testes de unidade e gerar o relatório de cobertura:

```bash
make test
```

O `make test` compila os testes com instrumentação de cobertura. O relatorio aparece em `build/coverage/coverage.html`.

Limpar os artefatos de compilação:

```bash
make clean
```



A documentação do código é gerada com Doxygen a partir da configuração em `Doxyfile`:

```bash
doxygen Doxyfile
```