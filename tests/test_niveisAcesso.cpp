#include "doctest.h"
#include "../include/Chef.hpp"
#include "../include/Cozinheiro.hpp"
#include "../include/Admin.hpp"

TEST_CASE("Chef pode cadastrar template") {
    Chef c("Fulano", "fulano@email.com", "senha123");
    CHECK(c.podeCadastrarTemplate() == true);
}

TEST_CASE("Cozinheiro nao pode cadastrar template") {
    Cozinheiro c("BobEsponja", "bob@email.com", "senha123");
    CHECK(c.podeCadastrarTemplate() == false);
}

TEST_CASE("Admin pode cadastrar template") {
    Admin a("Ciclano", "ciclano@email.com", "senha123");
    CHECK(a.podeCadastrarTemplate() == true);
}

TEST_CASE("Chef se identifica como Chef") {
    Chef c("Fulano", "fulano@email.com", "senha123");
    CHECK(c.getAcesso() == nivelAcesso::Chef);
}

TEST_CASE("Cozinheiro se identifica como Cozinheiro") {
    Cozinheiro c("BobEsponja", "bob@email.com", "senha123");
    CHECK(c.getAcesso() == nivelAcesso::Cozinheiro);
}

TEST_CASE("Admin se identifica como Admin") {
    Admin a("Ciclano", "ciclano@email.com", "senha123");
    CHECK(a.getAcesso() == nivelAcesso::Admin);
}

TEST_CASE("Check de polimorfismo via ponteiro de Usuario") {
    Chef chef("Fulano", "fulano@email.com", "senha123");
    Cozinheiro coz("BobEsponja", "bob@email.com", "senha123");
    Admin a("Ciclano", "ciclano@email.com", "senha123");

    Usuario* u1 = &chef;
    Usuario* u2 = &coz;
    Usuario* u3 = &a;

    
    CHECK(u1->podeCadastrarTemplate() == true);
    CHECK(u2->podeCadastrarTemplate() == false);
    CHECK(u3->podeCadastrarTemplate() == true);
    //tem que dar diferente aqui.
}

TEST_CASE("Subclasse herda metodos comuns de Usuario") {
    Chef c("Fulano", "fulano@email.com", "senha123");
    // getNome, autenticar etc vem da base Usuario, nao sendo reescritos no Chef
    CHECK(c.getNome() == "Fulano");
    CHECK(c.autenticar("senha123") == true);
    CHECK(c.autenticar("errada") == false);
}