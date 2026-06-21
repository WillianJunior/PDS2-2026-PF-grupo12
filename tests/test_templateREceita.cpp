#include "doctest.h"
#include "../include/TemplateReceita.hpp"

TEST_CASE("Criar template") {

    TemplateReceita t(
        "Bolo Base",
        "Template para bolos", 1
    );

    CHECK(t.getIngredientesT().size() == 0);
}

TEST_CASE("Adicionar ingrediente") {

    TemplateReceita t(
        "Pizza",
        "Template pizza", 1
    );

    Ingrediente i(
        "Queijo",
        200,
        "g",
        "Laticinio"
    );

    t.adicionarIngrediente(i);

    CHECK(t.getIngredientesT().size() == 1);
}