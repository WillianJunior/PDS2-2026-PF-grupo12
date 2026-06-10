#include "doctest.h"
#include "../include/Ingrediente.hpp"

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