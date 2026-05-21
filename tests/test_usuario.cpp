#include "doctest.h"

#include "../include/Usuario.hpp"
#include "../include/Receita.hpp"

TEST_CASE("Criar usuario") {

    std::string nome = "djavan";
    std::string email = "djavan@gmail.com";
    std::string senha = "123";

    Usuario u(
        nome,
        email,
        senha
    );

    CHECK(u.getNome() == "djavan");

    CHECK(u.getEmail() == "djavan@gmail.com");
}

TEST_CASE("Autenticacao") {

    std::string nome = "hulk";
    std::string email = "hulkparaiba@gmail.com";
    std::string senha = "abc";

    Usuario u(
        nome,
        email,
        senha
    );

    CHECK(u.autenticar("abc"));

    CHECK_FALSE(u.autenticar("errada"));
}

TEST_CASE("Receitas proprias") {

    std::string nome = "Carlos";
    std::string email = "c@hotmail.com";
    std::string senha = "123";

    Usuario u(
        nome,
        email,
        senha
    );

    Receita r(
        "Bolo",
        20,
        Dificuldade::Facil,
        Categoria::Doce
    );

    u.adicionarReceitaPropria(&r);

    CHECK(u.getReceitasProprias().size() == 1);
}

TEST_CASE("Favoritas") {

    std::string nome = "machado";
    std::string email = "machadodeassis@gmail.com";
    std::string senha = "321";

    Usuario u(
        nome,
        email,
        senha
    );

    Receita r(
        "Pizza",
        40,
        Dificuldade::Medio,
        Categoria::Salgado
    );

    u.adicionarFavorita(&r);

    CHECK(u.ehFavorita(&r));
}