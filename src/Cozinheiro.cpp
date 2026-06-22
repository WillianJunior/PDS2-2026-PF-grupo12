#include "Cozinheiro.hpp"

Cozinheiro::Cozinheiro(const std::string& nome,
                       const std::string& email,
                       const std::string& senha)
    : Usuario(nome, email, senha) {}


    bool Cozinheiro::podeCadastrarTemplate() const {
    return false;
}

nivelAcesso Cozinheiro::getAcesso() const {
    return nivelAcesso::Cozinheiro;
}

