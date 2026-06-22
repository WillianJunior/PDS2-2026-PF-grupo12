#include "doctest.h"

#include "../include/Avaliacao.hpp"
#include "../include/Usuario.hpp"
#include <stdexcept>
#include "../include/Cozinheiro.hpp"

TEST_CASE("Criacao de avaliacao") {

    Cozinheiro u("Pedro", "pedro@email.com", "123");

    Avaliacao a(&u, 5, "Muito boa");

    CHECK(a.getNota() == 5);
    CHECK(a.getComentario() == "Muito boa");
    CHECK(a.getAutor() == &u);
}

TEST_CASE("Outra avaliacao") {

    Cozinheiro u("kafka", "kafka@email.com", "123");

    
    Avaliacao a(&u, 2, "Ruim");

    CHECK(a.getNota() == 2);
    CHECK(a.getComentario() == "Ruim");
}

//testes de excecao ------------------

TEST_CASE("Avaliacao com autor nulo lanca excecao") {
    CHECK_THROWS_AS(
        Avaliacao(nullptr, 5, "Otima"),
        std::invalid_argument
    );
}
 
TEST_CASE("Avaliacao com nota fora do range eh ajustada, nao lanca") {
    Cozinheiro u("Carlos", "carlos@email.com", "123");
 
    // nota e ajustada via std::clamp, nao deve lancar excecao
    CHECK_NOTHROW(Avaliacao(&u, 10, "Excelente"));
    CHECK_NOTHROW(Avaliacao(&u, -5, "Pessimo"));
 
    Avaliacao alta(&u, 10, "Excelente");
    Avaliacao baixa(&u, -5, "Pessimo");
 
    CHECK(alta.getNota() == 5);
    CHECK(baixa.getNota() == 1);
}
 
TEST_CASE("Avaliacao valida nao lanca excecao") {
    Cozinheiro u("Maria", "maria@email.com", "123");
    CHECK_NOTHROW(Avaliacao(&u, 4, "Boa"));
}