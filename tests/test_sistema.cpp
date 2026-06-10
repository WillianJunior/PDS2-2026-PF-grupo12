#include "doctest.h"
#include "../include/Sistema.hpp"
#include <fstream>
#include <cstdio>  // remove()
//faltava os headers para fazer esse codigo funcionar. Inclui eles.

TEST_CASE("Cadastrar usuario") {

    Sistema s;

    std::string nome = "david";
    std::string email = "davidbowie@gmail.com";
    std::string senha = "123";

    CHECK(
        s.cadastrarUsuario(
            nome,
            email,
            senha
        )
    );
}

TEST_CASE("Login usuario") {

    Sistema s;

    std::string nome = "ziggy";
    std::string email = "ziggystardust@email.com";
    std::string senha = "abc";

    s.cadastrarUsuario(
        nome,
        email,
        senha
    );

    CHECK(
        s.login(
            email,
            senha
        )
    );

    CHECK(
        s.getUsuarioAtivo()
        !=
        nullptr
    );
}

TEST_CASE("Logout") {

    Sistema s;

    std::string nome = "Joao";
    std::string email = "joaoguimaraesrosa@email.com";
    std::string senha = "123";

    s.cadastrarUsuario(
        nome,
        email,
        senha
    );

    s.login(
        email,
        senha
    );

    s.logout();

    CHECK(s.getUsuarioAtivo() == nullptr);
}

TEST_CASE("Cadastrar receita") {

    Sistema s;

    std::string titulo = "Bolo de ninho com nutella";

    Receita* r = s.cadastrarReceita(titulo, 60, Dificuldade::Facil, Categoria::Doce);

    CHECK(r != nullptr);

    CHECK(s.getReceitas().size() == 1);

     CHECK(&s.getReceitas()[0] == r);
}

TEST_CASE("Buscar receita") {

    Sistema s;

    std::string titulo = "Pizza";

    s.cadastrarReceita(titulo, 30, Dificuldade::Medio, Categoria::Salgado);

    auto resultado = s.buscarPorTitulo(titulo);

    CHECK(resultado.size() == 1); 
}

TEST_CASE("Filtrar dificuldade") {

    Sistema s;

    std::string r1 = "Bolo";
    std::string r2 = "Lasanha";

    s.cadastrarReceita(r1, 20, Dificuldade::Facil, Categoria::Doce);

    s.cadastrarReceita(r2, 90, Dificuldade::Dificil, Categoria::Salgado);

    auto lista = s.filtrarPorDificuldade(Dificuldade::Facil);

    CHECK(lista.size() == 1);
}

// Sobrescreve os arquivos CSV com conteudo customizado para simular um estado salvo
static void escreverCSV(const std::string& pathUsuarios,
                        const std::string& conteudoUsuarios,
                        const std::string& pathReceitas,
                        const std::string& conteudoReceitas) {
    std::ofstream fu(pathUsuarios);
    fu << conteudoUsuarios;
 
    std::ofstream fr(pathReceitas);
    fr << conteudoReceitas;
}
 
// Le o conteudo de um arquivo e retorna como string
static std::string lerArquivo(const std::string& path) {
    std::ifstream f(path);
    return std::string(
        std::istreambuf_iterator<char>(f),
        std::istreambuf_iterator<char>()
    );
}
 
// ─────────────────────────────────────────────
// Testes de salvar()
// ─────────────────────────────────────────────
 
TEST_CASE("salvar() gera arquivo de usuarios com nome e email") {
    Sistema s;
    std::string nome  = "Ana";
    std::string email = "ana@email.com";
    std::string senha = "123";
    s.cadastrarUsuario(nome, email, senha);
 
    s.salvar();
 
    std::string conteudo = lerArquivo("data/usuarios.csv");
    CHECK(conteudo.find("Ana")           != std::string::npos);
    CHECK(conteudo.find("ana@email.com") != std::string::npos);
}
 
TEST_CASE("salvar() gera arquivo de receitas com titulo, dificuldade e categoria") {
    Sistema s;
    std::string titulo = "Bolo de cenoura";
    s.cadastrarReceita(titulo, 40, Dificuldade::Facil, Categoria::Doce);
 
    s.salvar();
 
    std::string conteudo = lerArquivo("data/receitas.csv");
    CHECK(conteudo.find("Bolo de cenoura") != std::string::npos);
    CHECK(conteudo.find("Facil")           != std::string::npos);
    CHECK(conteudo.find("Doce")            != std::string::npos);
}
 
TEST_CASE("salvar() persiste ingredientes na receita") {
    Sistema s;
    std::string titulo = "Omelete";
    Receita* r = s.cadastrarReceita(titulo, 10, Dificuldade::Facil, Categoria::Salgado);
    r->adicionarIngrediente(Ingrediente("Ovo", 2, "un", "Proteina"));
 
    s.salvar();
 
    std::string conteudo = lerArquivo("data/receitas.csv");
    CHECK(conteudo.find("Ovo") != std::string::npos);
}
 
TEST_CASE("salvar() persiste multiplas receitas, uma por linha") {
    Sistema s;
    std::string t1 = "Pizza";
    std::string t2 = "Lasanha";
    s.cadastrarReceita(t1, 30, Dificuldade::Medio,   Categoria::Salgado);
    s.cadastrarReceita(t2, 90, Dificuldade::Dificil, Categoria::Salgado);
 
    s.salvar();
 
    std::string conteudo = lerArquivo("data/receitas.csv");
    CHECK(conteudo.find("Pizza")   != std::string::npos);
    CHECK(conteudo.find("Lasanha") != std::string::npos);
}
 
// ─────────────────────────────────────────────
// Testes de carregar()
// ─────────────────────────────────────────────
 
TEST_CASE("carregar() recupera usuarios do CSV") {
    // prepara arquivo manualmente
    escreverCSV(
        "data/usuarios.csv", "Carlos,carlos@email.com\n",
        "data/receitas.csv", ""
    );
 
    Sistema s;
    s.carregar();
 
    CHECK(s.getUsuarios().size() == 1);
    CHECK(s.getUsuarios()[0].getNome()  == "Carlos");
    CHECK(s.getUsuarios()[0].getEmail() == "carlos@email.com");
}
 
TEST_CASE("carregar() recupera receita com titulo, dificuldade e categoria") {
    escreverCSV(
        "data/usuarios.csv", "",
        "data/receitas.csv", "Frango grelhado,30,Medio,Salgado,Grelhe o frango,\n"
    );
 
    Sistema s;
    s.carregar();
 
    REQUIRE(s.getReceitas().size() == 1);
    CHECK(s.getReceitas()[0].getTitulo()      == "Frango grelhado");
    CHECK(s.getReceitas()[0].getDificuldade() == Dificuldade::Medio);
    CHECK(s.getReceitas()[0].getCategoria()   == Categoria::Salgado);
}
 
TEST_CASE("carregar() recupera ingredientes da receita") {
    escreverCSV(
        "data/usuarios.csv", "",
        "data/receitas.csv", "Omelete,10,Facil,Salgado,Bata os ovos,Ovo|2|un|Proteina\n"
    );
 
    Sistema s;
    s.carregar();
 
    REQUIRE(s.getReceitas().size() == 1);
    auto ings = s.getReceitas()[0].getIngredientes();
    REQUIRE(ings.size() == 1);
    CHECK(ings[0]._getnomeI()    == "Ovo");
    CHECK(ings[0]._getquantI()   == 2);
    CHECK(ings[0]._getunidadeI() == "un");
}
 
TEST_CASE("carregar() nao duplica dados em chamadas consecutivas") {
    escreverCSV(
        "data/usuarios.csv", "Maria,maria@email.com\n",
        "data/receitas.csv", "Sopa,20,Facil,Salgado,Ferva tudo,\n"
    );
 
    Sistema s;
    s.carregar();
    s.carregar();  // segunda chamada nao deve duplicar
 
    CHECK(s.getUsuarios().size() == 1);
    CHECK(s.getReceitas().size() == 1);
}
 
// ─────────────────────────────────────────────
// Teste de ida e volta (salvar + carregar)
// ─────────────────────────────────────────────
 
TEST_CASE("salvar() seguido de carregar() preserva os dados") {
    // monta sistema com dados
    Sistema original;
    std::string nome  = "Lucas";
    std::string email = "lucas@email.com";
    std::string senha = "abc";
    original.cadastrarUsuario(nome, email, senha);
 
    std::string titulo = "Bolo de fuba";
    Receita* r = original.cadastrarReceita(titulo, 50, Dificuldade::Facil, Categoria::Doce);
    r->adicionarIngrediente(Ingrediente("Fuba", 300, "g", "Grao"));
    r->definirInstrucoes("Misture tudo e asse.");
    original.salvar();
 
    // carrega em novo sistema e verifica
    Sistema carregado;
    carregado.carregar();
 
    REQUIRE(carregado.getReceitas().size() >= 1);
 
    // busca a receita pelo titulo
    std::string busca = "Bolo de fuba";
    auto res = carregado.buscarPorTitulo(busca);
    REQUIRE(res.size() == 1);
    CHECK(res[0]->getTitulo()      == "Bolo de fuba");
    CHECK(res[0]->getDificuldade() == Dificuldade::Facil);
    CHECK(res[0]->getCategoria()   == Categoria::Doce);
    CHECK(res[0]->getTempoPreparo() == 50);
 
    auto ings = res[0]->getIngredientes();
    REQUIRE(ings.size() == 1);
    CHECK(ings[0]._getnomeI() == "Fuba");
}