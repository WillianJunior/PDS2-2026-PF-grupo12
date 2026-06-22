#include "Chef.hpp"

Chef::Chef(const std::string& nome,
           const std::string& email,
           const std::string& senha)
    : Usuario(nome, email, senha) {}

bool Chef::podeCadastrarTemplate() const {
    return true;
}

nivelAcesso Chef::getAcesso() const {
    return nivelAcesso::Chef;
}

