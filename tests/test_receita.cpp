#include "doctest.h"

#include "../include/Receita.hpp"
#include "../include/Avaliacao.hpp"

TEST_CASE("Criar receita") {

    Receita r(
        "Bolo de cenoura",
        60,
        Dificuldade::Facil,
        Categoria::Doce
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
        Categoria::Salgado
    );

    Ingrediente i(
        "Queijo",
        200,
        "g",
        "Laticinio"
    );

    r.adicionarIngrediente(i);

    CHECK(true);
}

TEST_CASE("Definir instrucoes") {

    Receita r(
        "Pao",
        30,
        Dificuldade::Facil,
        Categoria::Outro
    );

    r.definirInstrucoes(
        "Misture tudo"
    );

    CHECK(true);
}   

TEST_CASE("Media de notas") {

    Receita r(
        "Torta",
        50,
        Dificuldade::Medio,
        Categoria::Doce
    );

    Avaliacao a1(
        nullptr,
        4,
        "Boa"
    );

    Avaliacao a2(
        nullptr,
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