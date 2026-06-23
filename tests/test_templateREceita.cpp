#include "doctest.h"
#include "../include/TemplateReceita.hpp"

TEST_CASE("Criar template") {

    TemplateReceita t(
        "Bolo Base",
        "Template para bolos", 1
    );

    CHECK(t.getIngredientesT().size() == 0);
    CHECK(t.getNome() == "Bolo Base");
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

TEST_CASE("Verificar Receita") {
    TemplateReceita t("Pizza", "Template Pizza", 1);
    Ingrediente i("Queijo", 200, "g", "Laticinio");
    t.adicionarIngrediente(i);

    Receita r("Calzone", 10, Dificuldade::Medio, Categoria::Salgado, 1);
    r.adicionarIngrediente(i);

    CHECK(t.verificarReceita(r));
}

TEST_CASE("Gerar Receita") {
    TemplateReceita t("Pizza", "Template Pizza", 1);
    Ingrediente i("Queijo", 200, "g", "Laticinio");
    t.adicionarIngrediente(i);

    Receita r = t.gerarReceita("Calzone", 10, Dificuldade::Medio, Categoria::Salgado, 1);

    CHECK(t.verificarReceita(r));
}