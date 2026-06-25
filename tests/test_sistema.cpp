#include "doctest.h"
#include "Sistema.hpp"
#include <fstream>
#include <cstdio>
#include "../include/Cozinheiro.hpp"

TEST_CASE("Sistema: cadastrar usuario") {
    Sistema s;
    CHECK(s.cadastrarUsuario("david", "davidbowie@gmail.com", "123", nivelAcesso::Cozinheiro));
}

TEST_CASE("Sistema: cadastrar usuario recusa email duplicado") {
    Sistema s;
    CHECK(s.cadastrarUsuario("a", "x@x.com", "1", nivelAcesso::Cozinheiro));
    CHECK_FALSE(s.cadastrarUsuario("b", "x@x.com", "2", nivelAcesso::Cozinheiro));
}

TEST_CASE("Sistema: cadastrar usuario recusa campos vazios") {
    Sistema s;
    CHECK_FALSE(s.cadastrarUsuario("", "x@x.com", "1", nivelAcesso::Cozinheiro));
    CHECK_FALSE(s.cadastrarUsuario("a", "",       "1", nivelAcesso::Cozinheiro));
    CHECK_FALSE(s.cadastrarUsuario("a", "x@x.com", "", nivelAcesso::Cozinheiro));
}

TEST_CASE("Sistema: login bem sucedido") {
    Sistema s;
    s.cadastrarUsuario("ziggy", "ziggystardust@email.com", "abc", nivelAcesso::Cozinheiro);
    CHECK(s.login("ziggystardust@email.com", "abc"));
    CHECK(s.getUsuarioAtivo() != nullptr);
    CHECK(s.getUsuarioAtivo()->getNome() == "ziggy");
}

TEST_CASE("Sistema: login com senha errada falha") {
    Sistema s;
    s.cadastrarUsuario("ziggy", "z@z.com", "abc", nivelAcesso::Cozinheiro);
    CHECK_FALSE(s.login("z@z.com", "errada"));
    CHECK(s.getUsuarioAtivo() == nullptr);
}

TEST_CASE("Sistema: login com email inexistente falha") {
    Sistema s;
    CHECK_FALSE(s.login("naoexiste@x.com", "qualquer"));
}

TEST_CASE("Sistema: logout") {
    Sistema s;
    s.cadastrarUsuario("Joao", "joaoguimaraesrosa@email.com", "123", nivelAcesso::Cozinheiro);
    s.login("joaoguimaraesrosa@email.com", "123");
    s.logout();
    CHECK(s.getUsuarioAtivo() == nullptr);
}

TEST_CASE("Sistema: cadastrar receita") {
    Sistema s;
    Receita* r = s.cadastrarReceita("Bolo de ninho", 60,
                                    Dificuldade::Facil, Categoria::Doce, 1);
    CHECK(r != nullptr);
    CHECK(s.getReceitas().size() == 1);
    CHECK(&s.getReceitas().front() == r);
}

TEST_CASE("Sistema: cadastrar receita vincula ao usuario logado") {
    Sistema s;
    s.cadastrarUsuario("a", "a@a", "1", nivelAcesso::Cozinheiro);
    s.login("a@a", "1");
    Receita* r = s.cadastrarReceita("Pizza", 30,
                                    Dificuldade::Medio, Categoria::Salgado, 1);
    CHECK(s.getUsuarioAtivo()->getReceitasProprias().size() == 1);
    CHECK(s.getUsuarioAtivo()->getReceitasProprias()[0] == r);
}

TEST_CASE("Sistema: buscar receita por substring case-insensitive") {
    Sistema s;
    s.cadastrarReceita("Pizza Margherita", 30,
                       Dificuldade::Medio, Categoria::Salgado, 1);
    s.cadastrarReceita("Bolo de cenoura", 60,
                       Dificuldade::Facil, Categoria::Doce, 1);

    auto r1 = s.buscarPorTitulo("pizza");
    CHECK(r1.size() == 1);

    auto r2 = s.buscarPorTitulo("BOLO");
    CHECK(r2.size() == 1);

    auto r3 = s.buscarPorTitulo("xyz");
    CHECK(r3.empty());
}

TEST_CASE("Sistema: filtrar por dificuldade") {
    Sistema s;
    s.cadastrarReceita("Bolo",    20, Dificuldade::Facil,   Categoria::Doce, 1);
    s.cadastrarReceita("Lasanha", 90, Dificuldade::Dificil, Categoria::Salgado, 1);
    auto faceis = s.filtrarPorDificuldade(Dificuldade::Facil);
    CHECK(faceis.size() == 1);
    CHECK(faceis[0]->getTitulo() == "Bolo");
}

TEST_CASE("Sistema: remover receita existente") {
    Sistema s;
    s.cadastrarReceita("Bolo", 20, Dificuldade::Facil, Categoria::Doce, 1);
    CHECK(s.removerReceita("Bolo"));
    CHECK(s.getReceitas().empty());
}

TEST_CASE("Sistema: remover receita inexistente retorna false") {
    Sistema s;
    CHECK_FALSE(s.removerReceita("Nao Existe"));
}

TEST_CASE("Sistema: avaliar exige usuario logado") {
    Sistema s;
    s.cadastrarReceita("Bolo", 20, Dificuldade::Facil, Categoria::Doce, 1);
    CHECK_FALSE(s.avaliar("Bolo", 5, "otimo"));   // sem login
}

TEST_CASE("Sistema: avaliar com login e filtrar por nota minima") {
    Sistema s;
    s.cadastrarUsuario("a", "a@a", "1", nivelAcesso::Cozinheiro);
    s.login("a@a", "1");
    s.cadastrarReceita("Bolo",    20, Dificuldade::Facil,   Categoria::Doce, 1);
    s.cadastrarReceita("Lasanha", 90, Dificuldade::Dificil, Categoria::Salgado, 1);

    CHECK(s.avaliar("Bolo",    5, "perfeito"));
    CHECK(s.avaliar("Lasanha", 2, "ruim"));

    auto bons = s.filtrarPorNotaMinima(4.0);
    CHECK(bons.size() == 1);
    CHECK(bons[0]->getTitulo() == "Bolo");
}

TEST_CASE("Sistema: avaliar receita inexistente retorna false") {
    Sistema s;
    s.cadastrarUsuario("a", "a@a", "1", nivelAcesso::Cozinheiro);
    s.login("a@a", "1");
    CHECK_FALSE(s.avaliar("nao existe", 5, "x"));
}

TEST_CASE("Sistema: ponteiros sobrevivem a multiplas insercoes (regressao)") {
    // Regressao: garante que list nao invalida ponteiros.
    Sistema s;
    Receita* primeira = s.cadastrarReceita("A", 10, Dificuldade::Facil, Categoria::Doce, 1);
    for (int i = 0; i < 50; ++i) {
        std::string t = "R" + std::to_string(i);
        s.cadastrarReceita(t, 10, Dificuldade::Facil, Categoria::Doce, 1);
    }
    // Se fosse vector com realocacao, isto seria UB. Com list, e seguro.
    CHECK(primeira->getTitulo() == "A");
}

// ─────────────────────────────────────────────────────────────
// Helpers para os testes de persistência (do colega, adaptados)
// ─────────────────────────────────────────────────────────────
static void escreverCSV(const std::string& pathUsuarios,
                        const std::string& conteudoUsuarios,
                        const std::string& pathReceitas,
                        const std::string& conteudoReceitas) {
    std::ofstream fu(pathUsuarios);
    fu << conteudoUsuarios;

    std::ofstream fr(pathReceitas);
    fr << conteudoReceitas;
}

static std::string lerArquivo(const std::string& path) {
    std::ifstream f(path);
    return std::string(std::istreambuf_iterator<char>(f),
                       std::istreambuf_iterator<char>());
}

// ─────────────────────────────────────────────────────────────
// Persistencia: salvar()
// ─────────────────────────────────────────────────────────────

TEST_CASE("Persistencia: salvar gera arquivo de usuarios") {
    Sistema s;
    s.cadastrarUsuario("Ana", "ana@email.com", "123", nivelAcesso::Cozinheiro);
    s.salvar();

    std::string conteudo = lerArquivo("data/usuarios.csv");
    CHECK(conteudo.find("Ana")           != std::string::npos);
    CHECK(conteudo.find("ana@email.com") != std::string::npos);
}

TEST_CASE("Persistencia: salvar gera arquivo de receitas com titulo, dificuldade, categoria") {
    Sistema s;
    s.cadastrarReceita("Bolo de cenoura", 40, Dificuldade::Facil, Categoria::Doce, 1);
    s.salvar();

    std::string conteudo = lerArquivo("data/receitas.csv");
    CHECK(conteudo.find("Bolo de cenoura") != std::string::npos);
    CHECK(conteudo.find("Facil")           != std::string::npos);
    CHECK(conteudo.find("Doce")            != std::string::npos);
}

TEST_CASE("Persistencia: salvar persiste ingredientes") {
    Sistema s;
    Receita* r = s.cadastrarReceita("Omelete", 10,
                                    Dificuldade::Facil, Categoria::Salgado, 1);
    r->adicionarIngrediente(Ingrediente("Ovo", 2, "un", "Proteina"));
    s.salvar();

    std::string conteudo = lerArquivo("data/receitas.csv");
    CHECK(conteudo.find("Ovo") != std::string::npos);
}

TEST_CASE("Persistencia: salvar persiste multiplas receitas") {
    Sistema s;
    s.cadastrarReceita("Pizza",   30, Dificuldade::Medio,   Categoria::Salgado, 1);
    s.cadastrarReceita("Lasanha", 90, Dificuldade::Dificil, Categoria::Salgado, 1);
    s.salvar();

    std::string conteudo = lerArquivo("data/receitas.csv");
    CHECK(conteudo.find("Pizza")   != std::string::npos);
    CHECK(conteudo.find("Lasanha") != std::string::npos);
}

// ─────────────────────────────────────────────────────────────
// Persistencia: carregar()
// ─────────────────────────────────────────────────────────────

TEST_CASE("Persistencia: carregar recupera usuarios") {
    escreverCSV("data/usuarios.csv", "Carlos,carlos@email.com\n",
                "data/receitas.csv", "");

    Sistema s;
    s.carregar();

    REQUIRE(s.getUsuarios().size() == 1);
    CHECK(s.getUsuarios().front()->getNome()  == "Carlos");
    CHECK(s.getUsuarios().front()->getEmail() == "carlos@email.com");
}

TEST_CASE("Persistencia: carregar recupera receita") {
    escreverCSV("data/usuarios.csv", "",
                "data/receitas.csv",
                "Frango grelhado,30,1,Medio,Salgado,Grelhe o frango,\n");

    Sistema s;
    s.carregar();

    REQUIRE(s.getReceitas().size() == 1);
    const Receita& r = s.getReceitas().front();
    CHECK(r.getTitulo()      == "Frango grelhado");
    CHECK(r.getDificuldade() == Dificuldade::Medio);
    CHECK(r.getCategoria()   == Categoria::Salgado);
}

TEST_CASE("Persistencia: carregar recupera ingredientes") {
    escreverCSV("data/usuarios.csv", "",
                "data/receitas.csv",
                "Omelete,10,1,Facil,Salgado,Bata os ovos,Ovo|2|un|Proteina\n");

    Sistema s;
    s.carregar();

    REQUIRE(s.getReceitas().size() == 1);
    const auto& ings = s.getReceitas().front().getIngredientes();
    REQUIRE(ings.size() == 1);
    CHECK(ings[0].getNome()       == "Ovo");
    CHECK(ings[0].getQuantidade() == doctest::Approx(2.0));
    CHECK(ings[0].getUnidade()    == "un");
}

TEST_CASE("Persistencia: carregar nao duplica em chamadas consecutivas") {
    escreverCSV("data/usuarios.csv", "Maria,maria@email.com\n",
                "data/receitas.csv", "Sopa,20,1,Facil,Salgado,Ferva tudo,\n");

    Sistema s;
    s.carregar();
    s.carregar();   // segunda chamada nao deve duplicar

    CHECK(s.getUsuarios().size() == 1);
    CHECK(s.getReceitas().size() == 1);
}

TEST_CASE("Persistencia: salvar + carregar preserva dados (round-trip)") {
    Sistema original;
    original.cadastrarUsuario("Lucas", "lucas@email.com", "abc", nivelAcesso::Cozinheiro);
    Receita* r = original.cadastrarReceita("Bolo de fuba", 50,
                                           Dificuldade::Facil, Categoria::Doce, 1);
    r->adicionarIngrediente(Ingrediente("Fuba", 300.0, "g", "Grao"));
    r->definirInstrucoes("Misture tudo e asse.");
    original.salvar();

    Sistema carregado;
    carregado.carregar();

    REQUIRE(carregado.getReceitas().size() >= 1);
    auto res = carregado.buscarPorTitulo("Bolo de fuba");
    REQUIRE(res.size() == 1);
    CHECK(res[0]->getTitulo()        == "Bolo de fuba");
    CHECK(res[0]->getDificuldade()   == Dificuldade::Facil);
    CHECK(res[0]->getCategoria()     == Categoria::Doce);
    CHECK(res[0]->getTempoPreparo()  == 50);

    const auto& ings = res[0]->getIngredientes();
    REQUIRE(ings.size() == 1);
    CHECK(ings[0].getNome() == "Fuba");
}


TEST_CASE("Persistencia: salvar serializa a despensa do usuario") {
    Sistema s;
    s.cadastrarUsuario("Caetano", "cae@email.com", "123", nivelAcesso::Chef);
    s.login("cae@email.com", "123");
    s.getUsuarioAtivo()->adicionarIngredienteDisponivel(
        Ingrediente("Farinha", 500, "g", "Seco"));
    s.salvar();

    std::string conteudo = lerArquivo("data/usuarios.csv");
    // a despensa vira o 5o campo, no sub-formato nome|qtd|unidade|tipo
    CHECK(conteudo.find("Farinha|500|g|Seco") != std::string::npos);
}

TEST_CASE("Persistencia: carregar reconstroi a despensa do usuario") {
    escreverCSV("data/usuarios.csv",
                "Caetano,cae@email.com,123,Chef,Ovo|3|un|Fresco;Leite|1.5|L|Liquido\n",
                "data/receitas.csv", "");

    Sistema s;
    s.carregar();

    REQUIRE(s.getUsuarios().size() == 1);
    const auto& disp = s.getUsuarios().front()->getIngredientesDisp();
    REQUIRE(disp.size() == 2);
    CHECK(disp[0].getNome()       == "Ovo");
    CHECK(disp[0].getQuantidade() == doctest::Approx(3.0));
    CHECK(disp[1].getNome()       == "Leite");
    CHECK(disp[1].getQuantidade() == doctest::Approx(1.5));
    CHECK(disp[1].getUnidade()    == "L");
    CHECK(disp[1].getTipo()       == "Liquido");
}

TEST_CASE("Persistencia: despensa sobrevive a um round-trip salvar/carregar") {
    Sistema original;
    original.cadastrarUsuario("Murilo", "mur@email.com", "abc", nivelAcesso::Cozinheiro);
    original.login("mur@email.com", "abc");
    original.getUsuarioAtivo()->adicionarIngredienteDisponivel(
        Ingrediente("Acucar", 200, "g", "Seco"));
    original.salvar();

    Sistema carregado;
    carregado.carregar();

    REQUIRE(carregado.getUsuarios().size() == 1);
    const auto& disp = carregado.getUsuarios().front()->getIngredientesDisp();
    REQUIRE(disp.size() == 1);
    CHECK(disp[0].getNome()       == "Acucar");
    CHECK(disp[0].getQuantidade() == doctest::Approx(200.0));
}

TEST_CASE("Persistencia: CSV antigo (4 campos, sem despensa) carrega sem quebrar") {
    escreverCSV("data/usuarios.csv",
                "Bernardo,ber@email.com,senha,Cozinheiro\n",
                "data/receitas.csv", "");

    Sistema s;
    s.carregar();

    REQUIRE(s.getUsuarios().size() == 1);
    CHECK(s.getUsuarios().front()->getIngredientesDisp().empty());
}

TEST_CASE("Despensa: adicionarIngredienteDisponivel recusa nome duplicado") {
    Sistema s;
    s.cadastrarUsuario("Caetano", "cae@email.com", "123", nivelAcesso::Chef);
    s.login("cae@email.com", "123");
    Usuario* u = s.getUsuarioAtivo();

    CHECK(u->adicionarIngredienteDisponivel(Ingrediente("Ovo", 3, "un", "Fresco")));
    // mesmo nome -> recusado, retorna false e nao incha a despensa
    CHECK_FALSE(u->adicionarIngredienteDisponivel(Ingrediente("Ovo", 6, "un", "Fresco")));
    CHECK(u->getIngredientesDisp().size() == 1);
}

TEST_CASE("Despensa: deduplicacao e case-insensitive") {
    Sistema s;
    s.cadastrarUsuario("Murilo", "mur@email.com", "123", nivelAcesso::Cozinheiro);
    s.login("mur@email.com", "123");
    Usuario* u = s.getUsuarioAtivo();

    CHECK(u->adicionarIngredienteDisponivel(Ingrediente("Leite", 1, "L", "Liquido")));
    // "leite" deve colidir com "Leite" (mesma nocao usada por sugerirReceitas)
    CHECK_FALSE(u->adicionarIngredienteDisponivel(Ingrediente("leite", 2, "L", "Liquido")));
    CHECK_FALSE(u->adicionarIngredienteDisponivel(Ingrediente("LEITE", 3, "L", "Liquido")));
    CHECK(u->getIngredientesDisp().size() == 1);
}

TEST_CASE("Despensa: ingredientes de nomes distintos sao todos aceitos") {
    Sistema s;
    s.cadastrarUsuario("Bernardo", "ber@email.com", "123", nivelAcesso::Chef);
    s.login("ber@email.com", "123");
    Usuario* u = s.getUsuarioAtivo();

    CHECK(u->adicionarIngredienteDisponivel(Ingrediente("Farinha", 500, "g", "Seco")));
    CHECK(u->adicionarIngredienteDisponivel(Ingrediente("Ovo", 3, "un", "Fresco")));
    CHECK(u->adicionarIngredienteDisponivel(Ingrediente("Acucar", 200, "g", "Seco")));
    CHECK(u->getIngredientesDisp().size() == 3);
}

// ────────────────────────────────────────────────────────────────────────────
// Persistencia: favoritas e receitas proprias (vinculos Receita* por titulo)
// Opcao 12 (favoritar) e cadastro de receita logado: sem persistir, o usuario
// perde favoritas e a autoria das proprias receitas ao reabrir o programa.
// ────────────────────────────────────────────────────────────────────────────

TEST_CASE("Persistencia: salvar serializa favoritas e proprias por titulo") {
    Sistema s;
    s.cadastrarUsuario("Caetano", "cae@email.com", "123", nivelAcesso::Chef);
    s.login("cae@email.com", "123");
    s.cadastrarReceita("Bolo", 40, Dificuldade::Facil, Categoria::Doce, 1); // vira propria
    auto bolo = s.buscarPorTitulo("Bolo");
    s.getUsuarioAtivo()->adicionarFavorita(bolo[0]);
    s.salvar();

    std::string conteudo = lerArquivo("data/usuarios.csv");
    // o titulo "Bolo" deve aparecer no campo de favoritas e no de proprias
    CHECK(conteudo.find("Bolo") != std::string::npos);
}

TEST_CASE("Persistencia: round-trip preserva favoritas apontando pro objeto real") {
    {
        Sistema s;
        s.cadastrarUsuario("Murilo", "mur@email.com", "abc", nivelAcesso::Cozinheiro);
        s.login("mur@email.com", "abc");
        s.cadastrarReceita("Pizza", 30, Dificuldade::Medio, Categoria::Salgado, 1);
        auto piz = s.buscarPorTitulo("Pizza");
        s.getUsuarioAtivo()->adicionarFavorita(piz[0]);
        s.salvar();
    }

    Sistema carregado;
    carregado.carregar();

    Usuario* u = nullptr;
    for (auto& up : carregado.getUsuarios())
        if (up->getEmail() == "mur@email.com") u = up.get();
    REQUIRE(u != nullptr);

    const auto& fav = u->getFavoritas();
    REQUIRE(fav.size() == 1);
    CHECK(fav[0]->getTitulo() == "Pizza");

    // o ponteiro religado tem que ser O MESMO objeto vivo na lista _receitas,
    // nao uma copia: ehFavorita() depende de igualdade de ponteiro.
    Receita* pizzaNaLista = nullptr;
    for (auto& r : carregado.getReceitas())
        if (r.getTitulo() == "Pizza") pizzaNaLista = &r;
    REQUIRE(pizzaNaLista != nullptr);
    CHECK(fav[0] == pizzaNaLista);
    CHECK(u->ehFavorita(pizzaNaLista));
}

TEST_CASE("Persistencia: religar usa titulo EXATO, nao substring") {
    // "Bolo" nao pode religar para "Bolo de Cenoura"
    escreverCSV("data/usuarios.csv",
                "Ana,ana@email.com,1,Chef,,Bolo,\n",
                "data/receitas.csv",
                "Bolo de Cenoura,50,1,Medio,Doce,Misture,\n");

    Sistema s;
    s.carregar();

    Usuario* u = s.getUsuarios().front().get();
    // a favorita era "Bolo" exato; so existe "Bolo de Cenoura" -> nada religa
    CHECK(u->getFavoritas().empty());
}

TEST_CASE("Persistencia: titulo de favorita inexistente e ignorado sem quebrar") {
    escreverCSV("data/usuarios.csv",
                "Bob,bob@email.com,1,Chef,,Receita Fantasma,\n",
                "data/receitas.csv", "");

    Sistema s;
    s.carregar();   // nao deve lancar

    REQUIRE(s.getUsuarios().size() == 1);
    CHECK(s.getUsuarios().front()->getFavoritas().empty());
}

TEST_CASE("Persistencia: round-trip preserva receitas proprias") {
    {
        Sistema s;
        s.cadastrarUsuario("Bernardo", "ber@email.com", "x", nivelAcesso::Chef);
        s.login("ber@email.com", "x");
        s.cadastrarReceita("Risoto",   45, Dificuldade::Dificil, Categoria::Salgado, 1);
        s.cadastrarReceita("Brigadeiro", 20, Dificuldade::Facil, Categoria::Doce, 1);
        s.salvar();
    }

    Sistema carregado;
    carregado.carregar();

    Usuario* u = nullptr;
    for (auto& up : carregado.getUsuarios())
        if (up->getEmail() == "ber@email.com") u = up.get();
    REQUIRE(u != nullptr);
    CHECK(u->getReceitasProprias().size() == 2);
}

// ---- Testes adicionais (PF): busca exata, delecao de usuario (Admin),
//      filtros, avaliacao, sugestao e persistencia ----

TEST_CASE("Busca: titulo exato tem prioridade sobre substring (bug Risoto)") {
    Sistema s;
    s.cadastrarReceita("Risoto de Cogumelos", 45, Dificuldade::Medio, Categoria::Vegetariano, 4);
    s.cadastrarReceita("Risoto",               30, Dificuldade::Facil, Categoria::Salgado, 4);

    // "Risoto" exato deve retornar SO a receita "Risoto"
    auto exato = s.buscarPorTitulo("Risoto");
    REQUIRE(exato.size() == 1);
    CHECK(exato[0]->getTitulo() == "Risoto");

    // o titulo completo da outra tambem resolve para ela so
    auto outro = s.buscarPorTitulo("Risoto de Cogumelos");
    REQUIRE(outro.size() == 1);
    CHECK(outro[0]->getTitulo() == "Risoto de Cogumelos");
}

TEST_CASE("Busca: sem match exato, cai no fallback de substring") {
    Sistema s;
    s.cadastrarReceita("Risoto de Cogumelos", 45, Dificuldade::Medio, Categoria::Vegetariano, 4);
    s.cadastrarReceita("Risoto",               30, Dificuldade::Facil, Categoria::Salgado, 4);

    // "Riso" nao casa exato com nenhuma -> substring acha as duas
    auto parcial = s.buscarPorTitulo("Riso");
    CHECK(parcial.size() == 2);
}

TEST_CASE("Busca: exato e case-insensitive") {
    Sistema s;
    s.cadastrarReceita("Risoto", 30, Dificuldade::Facil, Categoria::Salgado, 4);
    auto r = s.buscarPorTitulo("RISOTO");
    REQUIRE(r.size() == 1);
    CHECK(r[0]->getTitulo() == "Risoto");
}

// ─────────────────────────────────────────────────────────────────────────────
// Item 3: remocao de usuario por Admin
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Admin: remove outro usuario com sucesso") {
    Sistema s;
    s.cadastrarUsuario("Chefe", "admin@ex.com", "1", nivelAcesso::Admin);
    s.cadastrarUsuario("Alvo",  "alvo@ex.com",  "2", nivelAcesso::Cozinheiro);
    s.login("admin@ex.com", "1");

    CHECK(s.getUsuarios().size() == 2);
    CHECK(s.removerUsuario("alvo@ex.com") == true);
    CHECK(s.getUsuarios().size() == 1);
}

TEST_CASE("Admin: nao pode remover a si mesmo") {
    Sistema s;
    s.cadastrarUsuario("Chefe", "admin@ex.com", "1", nivelAcesso::Admin);
    s.login("admin@ex.com", "1");

    CHECK(s.removerUsuario("admin@ex.com") == false);
    CHECK(s.getUsuarios().size() == 1);   // continua la
}

TEST_CASE("Admin: remover email inexistente retorna false") {
    Sistema s;
    s.cadastrarUsuario("Chefe", "admin@ex.com", "1", nivelAcesso::Admin);
    s.login("admin@ex.com", "1");

    CHECK(s.removerUsuario("naoexiste@ex.com") == false);
}

TEST_CASE("Permissao: nao-Admin nao consegue remover usuario") {
    Sistema s;
    s.cadastrarUsuario("Comum", "comum@ex.com", "1", nivelAcesso::Cozinheiro);
    s.cadastrarUsuario("Outro", "outro@ex.com", "2", nivelAcesso::Cozinheiro);
    s.login("comum@ex.com", "1");

    // logado como Cozinheiro -> sem privilegio
    CHECK(s.removerUsuario("outro@ex.com") == false);
    CHECK(s.getUsuarios().size() == 2);
}

TEST_CASE("Permissao: sem ninguem logado, remover usuario falha") {
    Sistema s;
    s.cadastrarUsuario("Alguem", "a@ex.com", "1", nivelAcesso::Cozinheiro);
    // sem login
    CHECK(s.removerUsuario("a@ex.com") == false);
}

TEST_CASE("Chef logado nao tem privilegio de Admin para remover") {
    Sistema s;
    s.cadastrarUsuario("Cozinheiro Chefe", "chef@ex.com", "1", nivelAcesso::Chef);
    s.cadastrarUsuario("Vitima",           "v@ex.com",    "2", nivelAcesso::Cozinheiro);
    s.login("chef@ex.com", "1");
    CHECK(s.removerUsuario("v@ex.com") == false);
}

// ─────────────────────────────────────────────────────────────────────────────
// Cobertura: filtros, avaliacao, remocao de receita
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Filtro: por dificuldade retorna so as que batem") {
    Sistema s;
    s.cadastrarReceita("A", 10, Dificuldade::Facil,   Categoria::Doce, 1);
    s.cadastrarReceita("B", 20, Dificuldade::Dificil, Categoria::Salgado, 1);
    s.cadastrarReceita("C", 30, Dificuldade::Facil,   Categoria::Vegano, 1);

    CHECK(s.filtrarPorDificuldade(Dificuldade::Facil).size()   == 2);
    CHECK(s.filtrarPorDificuldade(Dificuldade::Dificil).size() == 1);
    CHECK(s.filtrarPorDificuldade(Dificuldade::Medio).size()   == 0);
}

TEST_CASE("Avaliacao: avaliar receita existente e refletir na nota") {
    Sistema s;
    s.cadastrarUsuario("U", "u@ex.com", "1", nivelAcesso::Cozinheiro);
    s.login("u@ex.com", "1");
    s.cadastrarReceita("Torta", 40, Dificuldade::Medio, Categoria::Doce, 4);

    CHECK(s.avaliar("Torta", 5, "Otima!") == true);
    auto r = s.buscarPorTitulo("Torta");
    REQUIRE(r.size() == 1);
    CHECK(r[0]->calcularMediaNotas() == doctest::Approx(5.0));
}

TEST_CASE("Avaliacao: avaliar receita inexistente retorna false") {
    Sistema s;
    s.cadastrarUsuario("U", "u@ex.com", "1", nivelAcesso::Cozinheiro);
    s.login("u@ex.com", "1");
    CHECK(s.avaliar("Fantasma", 5, "x") == false);
}

TEST_CASE("Filtro: por nota minima") {
    Sistema s;
    s.cadastrarUsuario("U", "u@ex.com", "1", nivelAcesso::Cozinheiro);
    s.login("u@ex.com", "1");
    s.cadastrarReceita("Boa",  10, Dificuldade::Facil, Categoria::Doce, 1);
    s.cadastrarReceita("Ruim", 10, Dificuldade::Facil, Categoria::Doce, 1);
    s.avaliar("Boa", 5, "");
    s.avaliar("Ruim", 2, "");

    auto acima3 = s.filtrarPorNotaMinima(3.0);
    REQUIRE(acima3.size() == 1);
    CHECK(acima3[0]->getTitulo() == "Boa");
}

TEST_CASE("Remocao: remover receita existente e inexistente") {
    Sistema s;
    s.cadastrarReceita("Removivel", 10, Dificuldade::Facil, Categoria::Doce, 1);
    CHECK(s.removerReceita("Removivel") == true);
    CHECK(s.removerReceita("Removivel") == false); // ja foi
}

// ─────────────────────────────────────────────────────────────────────────────
// Cobertura: construcao de Receita (validacoes que lancam)
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Template: cadastrar e recuperar rendimento e ingredientes") {
    Sistema s;
    s.cadastrarUsuario("Chef", "chef@ex.com", "1", nivelAcesso::Chef);
    s.login("chef@ex.com", "1");
    TemplateReceita* t = s.cadastrarTemplate("Base Bolo", "desc", 8);
    REQUIRE(t != nullptr);
    t->adicionarIngrediente(Ingrediente("Farinha", 500, "g", "Seco"));

    CHECK(s.getRendimentoTemplate("Base Bolo") == 8);
    CHECK(s.getIngredientesTemplate("Base Bolo").size() == 1);
}

// ─────────────────────────────────────────────────────────────────────────────
// Cobertura: sugerirReceitas (opcao 17) — caminho feliz e excecao
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Sugerir: retorna receita cujos ingredientes o usuario tem") {
    Sistema s;
    s.cadastrarUsuario("U", "u@ex.com", "1", nivelAcesso::Cozinheiro);
    s.login("u@ex.com", "1");

    s.cadastrarReceita("Omelete", 10, Dificuldade::Facil, Categoria::Salgado, 1);
    auto r = s.buscarPorTitulo("Omelete");
    r[0]->adicionarIngrediente(Ingrediente("Ovo", 2, "un", "Fresco"));
    r[0]->adicionarIngrediente(Ingrediente("Sal", 1, "tsp", "Seco"));

    // usuario tem os dois ingredientes -> Omelete e sugerida
    s.getUsuarioAtivo()->adicionarIngredienteDisponivel(Ingrediente("Ovo", 6, "un", "Fresco"));
    s.getUsuarioAtivo()->adicionarIngredienteDisponivel(Ingrediente("Sal", 100, "g", "Seco"));

    auto sugestoes = s.sugerirReceitas();
    REQUIRE(sugestoes.size() == 1);
    CHECK(sugestoes[0]->getTitulo() == "Omelete");
}

TEST_CASE("Sugerir: nao sugere receita faltando um ingrediente") {
    Sistema s;
    s.cadastrarUsuario("U", "u@ex.com", "1", nivelAcesso::Cozinheiro);
    s.login("u@ex.com", "1");

    s.cadastrarReceita("Bolo", 40, Dificuldade::Facil, Categoria::Doce, 1);
    auto r = s.buscarPorTitulo("Bolo");
    r[0]->adicionarIngrediente(Ingrediente("Farinha", 2, "cup", "Seco"));
    r[0]->adicionarIngrediente(Ingrediente("Ovo", 3, "un", "Fresco"));

    // usuario so tem farinha -> Bolo NAO entra
    s.getUsuarioAtivo()->adicionarIngredienteDisponivel(Ingrediente("Farinha", 1, "kg", "Seco"));

    auto sugestoes = s.sugerirReceitas();
    CHECK(sugestoes.empty());
}

TEST_CASE("Sugerir: despensa vazia lanca excecao") {
    Sistema s;
    s.cadastrarUsuario("U", "u@ex.com", "1", nivelAcesso::Cozinheiro);
    s.login("u@ex.com", "1");
    s.cadastrarReceita("Qualquer", 10, Dificuldade::Facil, Categoria::Doce, 1);

    CHECK_THROWS_AS(s.sugerirReceitas(), std::invalid_argument);
}

// ─────────────────────────────────────────────────────────────────────────────
// Cobertura: persistencia da remocao de usuario (some do CSV ao salvar)
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Persistencia: usuario removido pelo Admin nao volta apos salvar/carregar") {
    {
        Sistema s;
        s.cadastrarUsuario("Chefe", "admin@ex.com", "1", nivelAcesso::Admin);
        s.cadastrarUsuario("Alvo",  "alvo@ex.com",  "2", nivelAcesso::Cozinheiro);
        s.login("admin@ex.com", "1");
        REQUIRE(s.removerUsuario("alvo@ex.com") == true);
        s.salvar();
    }

    Sistema carregado;
    carregado.carregar();

    // o Alvo nao deve reaparecer
    bool achouAlvo = false;
    for (auto& up : carregado.getUsuarios())
        if (up->getEmail() == "alvo@ex.com") achouAlvo = true;
    CHECK_FALSE(achouAlvo);
    CHECK(carregado.getUsuarios().size() == 1);
}
