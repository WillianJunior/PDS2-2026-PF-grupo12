#include "doctest.h"

#include "../include/Receita.hpp"
#include "../include/Avaliacao.hpp"
#include "../include/Usuario.hpp"
#include <stdexcept>
#include "../include/Cozinheiro.hpp"

TEST_CASE("Criar receita") {

    Receita r(
        "Bolo de cenoura",
        60,
        Dificuldade::Facil,
        Categoria::Doce, 1
    );

    CHECK(
        r.getTitulo() == "Bolo de cenoura"
    );
}

TEST_CASE("Adicionar ingrediente") {

    Receita r(
        "Pizza",
        40,
        Dificuldade::Medio,
        Categoria::Salgado, 1
    );

    Ingrediente i(
        "Queijo",
        200,
        "g",
        "Laticinio"
    );

    r.adicionarIngrediente(i);

    CHECK(r.getIngredientes().size() == 1);
}

TEST_CASE("Definir instrucoes") {

    Receita r(
        "Pao",
        30,
        Dificuldade::Facil,
        Categoria::Outro, 1
    );

    r.definirInstrucoes(
        "Misture tudo"
    );

    CHECK(r.getInstrucoes() == "Misture tudo");
}   

TEST_CASE("Media de notas") {

    Receita r(
        "Torta",
        50,
        Dificuldade::Medio,
        Categoria::Doce, 1
    );

    Cozinheiro u("Joana", "joana@email.com", "123");

    Avaliacao a1(
        &u,
        4,
        "Boa"
    );

    Avaliacao a2(
        &u,
        2,
        "Regular"
    );

    r.adicionarAvaliacao(a1);

    r.adicionarAvaliacao(a2);

    CHECK(
        r.calcularMediaNotas()
        ==
        doctest::Approx(3.0)
    );
}

TEST_CASE("Apropriar rendimento"){

    std::vector<Ingrediente> ingrs;
    ingrs.push_back(Ingrediente("oregano", 50, "mg", "tempero"));
    Receita r(
        "Pizza",
        40,
        Dificuldade::Medio,
        Categoria::Salgado, 1
    );

    int rR = 1;
    int rT = 2;

    CHECK(r.apropriarRendimento(ingrs, rR, rT)[0].getQuantidade() == 25);
}

//tests de excecao --------------

TEST_CASE("Receita com titulo vazio lanca excecao") {
    CHECK_THROWS_AS(
        Receita("", 30, Dificuldade::Facil, Categoria::Doce, 1),
        std::invalid_argument
    );
}
 
TEST_CASE("Receita com tempo negativo lanca excecao") {
    CHECK_THROWS_AS(
        Receita("Bolo", -10, Dificuldade::Facil, Categoria::Doce, 1),
        std::invalid_argument
    );
}
 
TEST_CASE("Receita valida nao lanca excecao") {
    CHECK_NOTHROW(
        Receita("Bolo", 30, Dificuldade::Facil, Categoria::Doce, 1)
    );
}
 
TEST_CASE("Adicionar ingrediente invalido lanca excecao") {
    Receita r("Sopa", 20, Dificuldade::Facil, Categoria::Salgado, 1);
 
    // Ingrediente nao pode ser construido invalido (o proprio construtor
    // ja lanca), entao testamos que a tentativa de cria-lo falha
    CHECK_THROWS_AS(
        Ingrediente("", -5, "", ""),
        std::invalid_argument
    );
}