# PDS2-2026-PF-grupo12 — Ricettario

Livro de receitas digital desenvolvido como Projeto Final da disciplina de Programação e Desenvolvimento de Software II.

## Integrantes do Grupo

- Bernardo Amaral Lima
- Caetano Salomon Nunes
- Murilo Marciel
- Rafael Estevão Silva Cunha

## Descrição do Problema

O intuito dessa aplicação é criar um livro de receitas digital que pode ser usado pelo usuário para adicionar, pesquisar e organizar receitas próprias ou de outros usuários, além de avaliá-las.

## Motivação da Escolha

Ao analisar os possíveis temas, a ideia de um Ricettario parecia conter a oportunidade de aplicar de maneira simples e direta muitos dos conceitos explorados nas aulas de PDS2. A principal motivação para a escolha foi a facilidade de visualizar a programação orientada a objetos na aplicação.

## Funcionalidades

A aplicação é operada por um menu de terminal interativo. As funcionalidades disponíveis são:

### Conta e níveis de acesso

- **Cadastro de usuário** com nome, e-mail, senha e escolha do perfil de acesso.
- **Login e logout**, com sessão de usuário ativo mantida pelo sistema.
- **Três níveis de acesso**, implementados como subclasses de `Usuario`:
  - **Cozinheiro** — acesso básico às receitas.
  - **Chef** — pode cadastrar templates de receita.
  - **Administrador** — pode cadastrar templates e remover usuários do sistema.

  As permissões são resolvidas por polimorfismo (cada subclasse sobrescreve `podeCadastrarTemplate()` e `getAcesso()`), e não por verificações espalhadas pelo código.

### Receitas

- **Cadastro de receita** com título, tempo de preparo, dificuldade (Fácil, Médio, Difícil), categoria (Doce, Salgado, Vegano, Vegetariano, Outro), rendimento, instruções e lista de ingredientes.
- **Cadastro de receita a partir de um template**, ajustando automaticamente as quantidades dos ingredientes para o rendimento desejado.
- **Listagem de todas as receitas**, exibindo tempo de preparo e nota média.
- **Busca de receita por título.**
- **Filtragem de receitas por dificuldade.**
- **Visualização completa de uma receita**, com categoria, dificuldade, tempo, rendimento, nota média, ingredientes, instruções e avaliações.

### Avaliações e favoritas

- **Avaliação de receita** com nota de 1 a 5 e comentário, registrando o autor da avaliação.
- **Cálculo automático da nota média** a partir das avaliações.
- **Marcar receitas como favoritas** e **listar as favoritas** do usuário ativo.

### Templates

- **Cadastro de template de receita** (exclusivo de Chef e Administrador), com título, descrição, rendimento e ingredientes.
- **Listagem de templates** disponíveis, com descrição e rendimento.

### Ingredientes do usuário (despensa)

- **Cadastro dos ingredientes disponíveis** do usuário, com validação de quantidade positiva e bloqueio de duplicatas.
- **Sugestão de receitas** que podem ser preparadas com os ingredientes que o usuário tem na despensa.

### Administração

- **Remoção de usuário por e-mail**, exclusiva de administradores. A regra é validada em duas camadas: na interface (mensagem amigável) e no domínio (`Sistema::removerUsuario`), que impede um admin de remover a si mesmo.

### Persistência

- **Salvamento e carregamento dos dados** em arquivos CSV (`data/usuarios.csv`, `data/receitas.csv`, `data/templates.csv`), de forma que usuários, receitas e templates permanecem entre execuções.

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

- **Usuario** — classe base de um usuário do sistema, com autenticação, receitas próprias, favoritas e ingredientes disponíveis. Define a interface polimórfica de permissões.
- **Cozinheiro / Chef / Admin** — subclasses de `Usuario` que definem os níveis de acesso e suas permissões.
- **Receita** — uma receita com título, instruções, ingredientes, dificuldade, categoria, rendimento e avaliações.
- **Ingrediente** — um ingrediente com nome, quantidade, unidade de medida e tipo.
- **Avaliacao** — uma avaliação de receita, com nota, comentário e autor.
- **TemplateReceita** — modelo de receita usado para gerar novas receitas com rendimento ajustável.
- **Menu** — camada de apresentação (interface de terminal), responsável por toda a entrada e saída interativa.
- **Sistema** — fachada que coordena usuários, receitas, templates, login, busca, filtros, avaliações e persistência.

## Compilação e Execução

### Pré-requisitos

- `g++` com suporte a C++17
- `make`
- `gcovr` para relatório de cobertura. Instale com `pip install gcovr` ou `sudo apt install gcovr`.

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

O `make test` compila os testes com instrumentação de cobertura. O relatório aparece em `build/coverage/coverage.html`.

Limpar os artefatos de compilação:

```bash
make clean
```

A documentação do código é gerada com Doxygen a partir da configuração em `Doxyfile`:

```bash
doxygen Doxyfile
```

## Exemplos de Uso

Os exemplos abaixo usam os dados que já vêm cadastrados em `data/`. As saídas foram capturadas da execução real do programa (`./build/app`); o cabeçalho ASCII e a repetição do menu foram omitidos para encurtar.

### Exemplo 1 — Login e acesso de administrador

Ao fazer login com uma conta de administrador, a seção **ADMIN** (opção 18) passa a aparecer no menu — ela fica oculta para os demais perfis.

```
Opcao: 2
Email: murilo@admin.com
Senha: admin456

Logado!

(LOGADO: Murilo Costa)

  CONTA              RECEITAS                TEMPLATES
  1. Cadastrar       4. Cadastrar receita    6. Cadastrar template
  2. Login           5. Receita via template 8. Listar templates
  3. Logout          7. Listar receitas
                     9. Buscar receita
                     10. Filtrar dificuldade
                     11. Avaliar receita
                     12. Favoritar
                     13. Ver favoritas
                     15. Ver receita completa

  INGREDIENTES
  16. Adicionar meus ingredientes   17. Sugerir receitas

  ADMIN
  18. Remover usuario

  14. Salvar dados     0. Sair
```

### Exemplo 2 — Buscar receita por título

```
Opcao: 9
Titulo a buscar: Bolo
1 receita(s) encontrada(s):
- Bolo de Cenoura
```

### Exemplo 3 — Ver uma receita completa

```
Opcao: 15
Titulo da receita: Brigadeiro Gourmet

============================================
  Brigadeiro Gourmet
============================================
Categoria   : Doce
Dificuldade : Facil
Tempo       : 30 min
Rendimento  : 20 pessoa(s)
Nota        : Sem avaliacoes

--- Ingredientes ---
  - Leite Condensado: 395 g [Liquido]
  - Chocolate em Po: 4 tbsp [Seco]
  - Manteiga: 1 tbsp [Fresco]
  - Granulado: 100 g [Seco]

--- Instrucoes ---
  Misture o leite condensado com chocolate em po e manteiga. Cozinhe mexendo ate desgrudar do fundo. Enrole e passe no granulado.

--- Avaliacoes ---
  (nenhuma avaliacao)
============================================
```

### Exemplo 4 — Listar templates disponíveis

```
Opcao: 8

=== Templates ===
1. Bolo Basico, serve (8) pessoas.
Descricao: Template para qualquer bolo simples de liquidificador
2. Marinada de Carne, serve (6) pessoas.
Descricao: Template para temperar carnes antes do preparo
3. Risoto Simples, serve (2) pessoas.
Descricao: Risoto modelo
4. Sopa Padrao, serve (4) pessoas.
Descricao: Base para sopas e caldos variados
```

### Exemplo 5 — Sugerir receitas a partir da despensa

Depois de logar, a opção 17 cruza os ingredientes que o usuário tem cadastrados com as receitas existentes e sugere o que dá para preparar:

```
Opcao: 17

=== Receitas Disponiveis ===
1. Risotto meu (30 min, nota 0)
```