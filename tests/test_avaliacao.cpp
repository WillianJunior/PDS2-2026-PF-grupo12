#include "doctest.h"

#include "../include/Avaliacao.hpp"
#include "../include/Usuario.hpp"

TEST_CASE("Criacao de avaliacao") {

    Usuario* u = nullptr;

    Avaliacao a(u, 5, "Muito boa");

    CHECK(a.getNota() == 5);
    CHECK(a.getComentario() == "Muito boa");
    CHECK(a.getAutor() == u);
}

TEST_CASE("Outra avaliacao") {

    Usuario* u = nullptr;

    
    Avaliacao a(u, 2, "Ruim");

    CHECK(a.getNota() == 2);
    CHECK(a.getComentario() == "Ruim");
}