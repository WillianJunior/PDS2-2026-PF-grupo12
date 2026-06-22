#include "doctest.h"

#include "../include/Usuario.hpp"
#include "../include/Receita.hpp"
#include <stdexcept>
#include "../include/Cozinheiro.hpp"

TEST_CASE("Criar usuario") {

    std::string nome = "djavan";
    std::string email = "djavan@gmail.com";
    std::string senha = "123";

    Cozinheiro u(
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

    Cozinheiro u(
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

    Cozinheiro u(
        nome,
        email,
        senha
    );

    Receita r(
        "Bolo",
        20,
        Dificuldade::Facil,
        Categoria::Doce, 1
    );

    u.adicionarReceitaPropria(&r);

    CHECK(u.getReceitasProprias().size() == 1);
}

TEST_CASE("Favoritas") {

    std::string nome = "machado";
    std::string email = "machadodeassis@gmail.com";
    std::string senha = "321";

    Cozinheiro u(
        nome,
        email,
        senha
    );

    Receita r(
        "Pizza",
        40,
        Dificuldade::Medio,
        Categoria::Salgado, 1
    );

    u.adicionarFavorita(&r);

    CHECK(u.ehFavorita(&r));
}

//testes de excecao ------------------

TEST_CASE("Usuario com nome vazio lanca excecao") {
    std::string nome = "";
    std::string email = "valido@email.com";
    std::string senha = "123";
 
    CHECK_THROWS_AS(
        Cozinheiro(nome, email, senha),
        std::invalid_argument
    );
}
 
TEST_CASE("Usuario com email invalido (sem @) lanca excecao") {
    std::string nome = "Joao";
    std::string email = "emailinvalido.com";
    std::string senha = "123";
 
    CHECK_THROWS_AS(
        Cozinheiro(nome, email, senha),
        std::invalid_argument
    );
}
 
TEST_CASE("Usuario valido nao lanca excecao") {
    std::string nome = "Joao";
    std::string email = "joao@email.com";
    std::string senha = "123";
 
    CHECK_NOTHROW(Cozinheiro(nome, email, senha));
}
 
TEST_CASE("AlterarSenha com nova senha vazia lanca excecao") {
    Cozinheiro u("Felipe", "felipe@email.com", "senha123");
 
    CHECK_THROWS_AS(
        u.alterarSenha("senha123", ""),
        std::invalid_argument
    );
}
 
TEST_CASE("AlterarSenha com senha atual incorreta retorna false, nao lanca") {
    Cozinheiro u("Felipe", "felipe@email.com", "senha123");
 
    bool resultado = true;
    CHECK_NOTHROW(resultado = u.alterarSenha("senhaErrada", "novaSenha"));
    CHECK_FALSE(resultado);
}
 
TEST_CASE("AlterarSenha valida funciona e nao lanca") {
    Cozinheiro u("Felipe", "felipe@email.com", "senha123");
 
    bool resultado = false;
    CHECK_NOTHROW(resultado = u.alterarSenha("senha123", "novaSenha"));
    CHECK(resultado);
    CHECK(u.autenticar("novaSenha"));
}