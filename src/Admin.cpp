#include "../include/Admin.hpp"

Admin::Admin(const std::string& nome,
             const std::string& email,
             const std::string& senha)
    : Usuario(nome, email, senha) {
    _nivelAcesso = nivelAcesso::Admin;
}

bool Admin::podeCadastrarTemplate() const {
    return true;
}

nivelAcesso Admin::getAcesso() const {
    return nivelAcesso::Admin;
}
