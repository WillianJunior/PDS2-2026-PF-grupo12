#include "doctest.h"
#include "Sistema.hpp"
#include <fstream>
#include <cstdio>

TEST_CASE("Sistema: cadastrar usuario") {
    Sistema s;
    CHECK(s.cadastrarUsuario("david", "davidbowie@gmail.com", "123"));
}

TEST_CASE("Sistema: cadastrar usuario recusa email duplicado") {
    Sistema s;
    CHECK(s.cadastrarUsuario("a", "x@x.com", "1"));
    CHECK_FALSE(s.cadastrarUsuario("b", "x@x.com", "2"));
}

TEST_CASE("Sistema: cadastrar usuario recusa campos vazios") {
    Sistema s;
    CHECK_FALSE(s.cadastrarUsuario("", "x@x.com", "1"));
    CHECK_FALSE(s.cadastrarUsuario("a", "",       "1"));
    CHECK_FALSE(s.cadastrarUsuario("a", "x@x.com", ""));
}

TEST_CASE("Sistema: login bem sucedido") {
    Sistema s;
    s.cadastrarUsuario("ziggy", "ziggystardust@email.com", "abc");
    CHECK(s.login("ziggystardust@email.com", "abc"));
    CHECK(s.getUsuarioAtivo() != nullptr);
    CHECK(s.getUsuarioAtivo()->getNome() == "ziggy");
}

TEST_CASE("Sistema: login com senha errada falha") {
    Sistema s;
    s.cadastrarUsuario("ziggy", "z@z.com", "abc");
    CHECK_FALSE(s.login("z@z.com", "errada"));
    CHECK(s.getUsuarioAtivo() == nullptr);
}

TEST_CASE("Sistema: login com email inexistente falha") {
    Sistema s;
    CHECK_FALSE(s.login("naoexiste@x.com", "qualquer"));
}

TEST_CASE("Sistema: logout") {
    Sistema s;
    s.cadastrarUsuario("Joao", "joaoguimaraesrosa@email.com", "123");
    s.login("joaoguimaraesrosa@email.com", "123");
    s.logout();
    CHECK(s.getUsuarioAtivo() == nullptr);
}

TEST_CASE("Sistema: cadastrar receita") {
    Sistema s;
    Receita* r = s.cadastrarReceita("Bolo de ninho", 60,
                                    Dificuldade::Facil, Categoria::Doce);
    CHECK(r != nullptr);
    CHECK(s.getReceitas().size() == 1);
    CHECK(&s.getReceitas().front() == r);
}

TEST_CASE("Sistema: cadastrar receita vincula ao usuario logado") {
    Sistema s;
    s.cadastrarUsuario("a", "a@a", "1");
    s.login("a@a", "1");
    Receita* r = s.cadastrarReceita("Pizza", 30,
                                    Dificuldade::Medio, Categoria::Salgado);
    CHECK(s.getUsuarioAtivo()->getReceitasProprias().size() == 1);
    CHECK(s.getUsuarioAtivo()->getReceitasProprias()[0] == r);
}

TEST_CASE("Sistema: buscar receita por substring case-insensitive") {
    Sistema s;
    s.cadastrarReceita("Pizza Margherita", 30,
                       Dificuldade::Medio, Categoria::Salgado);
    s.cadastrarReceita("Bolo de cenoura", 60,
                       Dificuldade::Facil, Categoria::Doce);

    auto r1 = s.buscarPorTitulo("pizza");
    CHECK(r1.size() == 1);

    auto r2 = s.buscarPorTitulo("BOLO");
    CHECK(r2.size() == 1);

    auto r3 = s.buscarPorTitulo("xyz");
    CHECK(r3.empty());
}

TEST_CASE("Sistema: filtrar por dificuldade") {
    Sistema s;
    s.cadastrarReceita("Bolo",    20, Dificuldade::Facil,   Categoria::Doce);
    s.cadastrarReceita("Lasanha", 90, Dificuldade::Dificil, Categoria::Salgado);
    auto faceis = s.filtrarPorDificuldade(Dificuldade::Facil);
    CHECK(faceis.size() == 1);
    CHECK(faceis[0]->getTitulo() == "Bolo");
}

TEST_CASE("Sistema: remover receita existente") {
    Sistema s;
    s.cadastrarReceita("Bolo", 20, Dificuldade::Facil, Categoria::Doce);
    CHECK(s.removerReceita("Bolo"));
    CHECK(s.getReceitas().empty());
}

TEST_CASE("Sistema: remover receita inexistente retorna false") {
    Sistema s;
    CHECK_FALSE(s.removerReceita("Nao Existe"));
}

TEST_CASE("Sistema: avaliar exige usuario logado") {
    Sistema s;
    s.cadastrarReceita("Bolo", 20, Dificuldade::Facil, Categoria::Doce);
    CHECK_FALSE(s.avaliar("Bolo", 5, "otimo"));   // sem login
}

TEST_CASE("Sistema: avaliar com login e filtrar por nota minima") {
    Sistema s;
    s.cadastrarUsuario("a", "a@a", "1");
    s.login("a@a", "1");
    s.cadastrarReceita("Bolo",    20, Dificuldade::Facil,   Categoria::Doce);
    s.cadastrarReceita("Lasanha", 90, Dificuldade::Dificil, Categoria::Salgado);

    CHECK(s.avaliar("Bolo",    5, "perfeito"));
    CHECK(s.avaliar("Lasanha", 2, "ruim"));

    auto bons = s.filtrarPorNotaMinima(4.0);
    CHECK(bons.size() == 1);
    CHECK(bons[0]->getTitulo() == "Bolo");
}

TEST_CASE("Sistema: avaliar receita inexistente retorna false") {
    Sistema s;
    s.cadastrarUsuario("a", "a@a", "1");
    s.login("a@a", "1");
    CHECK_FALSE(s.avaliar("nao existe", 5, "x"));
}

TEST_CASE("Sistema: ponteiros sobrevivem a multiplas insercoes (regressao)") {
    // Regressao: garante que list nao invalida ponteiros.
    Sistema s;
    Receita* primeira = s.cadastrarReceita("A", 10, Dificuldade::Facil, Categoria::Doce);
    for (int i = 0; i < 50; ++i) {
        std::string t = "R" + std::to_string(i);
        s.cadastrarReceita(t, 10, Dificuldade::Facil, Categoria::Doce);
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
    s.cadastrarUsuario("Ana", "ana@email.com", "123");
    s.salvar();

    std::string conteudo = lerArquivo("data/usuarios.csv");
    CHECK(conteudo.find("Ana")           != std::string::npos);
    CHECK(conteudo.find("ana@email.com") != std::string::npos);
}

TEST_CASE("Persistencia: salvar gera arquivo de receitas com titulo, dificuldade, categoria") {
    Sistema s;
    s.cadastrarReceita("Bolo de cenoura", 40, Dificuldade::Facil, Categoria::Doce);
    s.salvar();

    std::string conteudo = lerArquivo("data/receitas.csv");
    CHECK(conteudo.find("Bolo de cenoura") != std::string::npos);
    CHECK(conteudo.find("Facil")           != std::string::npos);
    CHECK(conteudo.find("Doce")            != std::string::npos);
}

TEST_CASE("Persistencia: salvar persiste ingredientes") {
    Sistema s;
    Receita* r = s.cadastrarReceita("Omelete", 10,
                                    Dificuldade::Facil, Categoria::Salgado);
    r->adicionarIngrediente(Ingrediente("Ovo", 2, "un", "Proteina"));
    s.salvar();

    std::string conteudo = lerArquivo("data/receitas.csv");
    CHECK(conteudo.find("Ovo") != std::string::npos);
}

TEST_CASE("Persistencia: salvar persiste multiplas receitas") {
    Sistema s;
    s.cadastrarReceita("Pizza",   30, Dificuldade::Medio,   Categoria::Salgado);
    s.cadastrarReceita("Lasanha", 90, Dificuldade::Dificil, Categoria::Salgado);
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
    CHECK(s.getUsuarios().front().getNome()  == "Carlos");
    CHECK(s.getUsuarios().front().getEmail() == "carlos@email.com");
}

TEST_CASE("Persistencia: carregar recupera receita") {
    escreverCSV("data/usuarios.csv", "",
                "data/receitas.csv",
                "Frango grelhado,30,Medio,Salgado,Grelhe o frango,\n");

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
                "Omelete,10,Facil,Salgado,Bata os ovos,Ovo|2|un|Proteina\n");

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
                "data/receitas.csv", "Sopa,20,Facil,Salgado,Ferva tudo,\n");

    Sistema s;
    s.carregar();
    s.carregar();   // segunda chamada nao deve duplicar

    CHECK(s.getUsuarios().size() == 1);
    CHECK(s.getReceitas().size() == 1);
}

TEST_CASE("Persistencia: salvar + carregar preserva dados (round-trip)") {
    Sistema original;
    original.cadastrarUsuario("Lucas", "lucas@email.com", "abc");
    Receita* r = original.cadastrarReceita("Bolo de fuba", 50,
                                           Dificuldade::Facil, Categoria::Doce);
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
