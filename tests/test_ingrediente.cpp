#include "doctest.h"
#include "../include/Ingrediente.hpp"
#include <stdexcept>


TEST_CASE("Criacao de ingrediente") {

    Ingrediente i(
        "Acucar",
        500,
        "g",
        "Doce"
    );

    CHECK(i.getNome() == "Acucar");

    CHECK(i.getQuantidade() == 500);

    CHECK(i.getUnidade() == "g");

    CHECK(i.getTipo() == "Doce");
}

TEST_CASE("Validacao de quantidade") {

    Ingrediente i(
        "Farinha",
        1000,
        "g",
        "Massa"
    );

    CHECK(
        i.verificarUnidades(
            500,
            "g"
        )
    );
}

//testes de excecao ----------------

TEST_CASE("Ingrediente com nome vazio lanca excecao") {
    CHECK_THROWS_AS(
        Ingrediente("", 100, "g", "Doce"),
        std::invalid_argument
    );
}
 
TEST_CASE("Ingrediente com quantidade zero lanca excecao") {
    CHECK_THROWS_AS(
        Ingrediente("Sal", 0, "g", "Tempero"),
        std::invalid_argument
    );
}
 
TEST_CASE("Ingrediente com quantidade negativa lanca excecao") {
    CHECK_THROWS_AS(
        Ingrediente("Sal", -10, "g", "Tempero"),
        std::invalid_argument
    );
}
 
TEST_CASE("Ingrediente com unidade vazia lanca excecao") {
    CHECK_THROWS_AS(
        Ingrediente("Ovo", 2, "", "Proteina"),
        std::invalid_argument
    );
}
 
TEST_CASE("Ingrediente valido nao lanca excecao") {
    CHECK_NOTHROW(
        Ingrediente("Leite", 1, "l", "Laticinio")
    );
}
