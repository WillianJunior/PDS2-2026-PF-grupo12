#include "../include/Usuario.hpp"
#include <algorithm>

Usuario::Usuario(const std::string& _nome, const std::string& _email, const std::string& _senha)
    : nome(_nome), email(_email), senha(_senha) {}

std::string Usuario::getNome() { return nome; }

std::string Usuario::getEmail() { return email; }

bool Usuario::autenticar(const std::string& senha) {
    return this->senha == senha;
}

void Usuario::alterarSenha(const std::string& senhaAtual) {
    // Mantida a assinatura original do header.
    // (Idealmente receberia tambem a nova senha; ver consideracoes.)
    this->senha = senhaAtual;
}

void Usuario::adicionarReceitaPropria(Receita* r) {
    receitasProprias.push_back(r);
}

void Usuario::removerReceitaPropria(Receita* r) {
    receitasProprias.erase(
        std::remove(receitasProprias.begin(), receitasProprias.end(), r),
        receitasProprias.end());
}

std::vector<Receita*>& Usuario::getReceitasProprias() { return receitasProprias; }

void Usuario::adicionarFavorita(Receita* r) {
    if (!ehFavorita(r)) favoritas.push_back(r);
}

bool Usuario::removerFavorita(Receita* r) {
    auto it = std::find(favoritas.begin(), favoritas.end(), r);
    if (it == favoritas.end()) return false;
    favoritas.erase(it);
    return true;
}

bool Usuario::ehFavorita(Receita* r) {
    return std::find(favoritas.begin(), favoritas.end(), r) != favoritas.end();
}

std::vector<Receita*>& Usuario::getFavoritas() { return favoritas; }
