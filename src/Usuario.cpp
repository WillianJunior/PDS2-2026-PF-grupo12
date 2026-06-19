#include "Usuario.hpp"
#include <algorithm>
#include <stdexcept>


Usuario::Usuario(const std::string& nome,
                 const std::string& email,
                 const std::string& senha)
    : _nome(nome), _email(email), _senha(senha) {
    if (nome.empty())
        throw std::invalid_argument("Usuario: nome nao pode ser vazio");
    if (email.find('@') == std::string::npos)
        throw std::invalid_argument("Usuario: email invalido");
}

const std::string& Usuario::getNome() const  { return _nome; }
const std::string& Usuario::getEmail() const { return _email; }
const std::string& Usuario::getSenha() const { return _senha; }


bool Usuario::autenticar(const std::string& senha) const {
    return _senha == senha;
}

bool Usuario::alterarSenha(const std::string& senhaAtual,
                           const std::string& novaSenha) {
    if (_senha != senhaAtual) return false;
    if (novaSenha.empty())
        throw std::invalid_argument("Usuario: nova senha nao pode ser vazia");
    _senha = novaSenha;
    return true;
}

void Usuario::adicionarReceitaPropria(Receita* r) {
    if (r) _receitasProprias.push_back(r);
}

void Usuario::removerReceitaPropria(Receita* r) {
    _receitasProprias.erase(
        std::remove(_receitasProprias.begin(), _receitasProprias.end(), r),
        _receitasProprias.end());
}

const std::vector<Receita*>& Usuario::getReceitasProprias() const {
    return _receitasProprias;
}

void Usuario::adicionarFavorita(Receita* r) {
    if (r && !ehFavorita(r)) _favoritas.push_back(r);
}

bool Usuario::removerFavorita(Receita* r) {
    auto it = std::find(_favoritas.begin(), _favoritas.end(), r);
    if (it == _favoritas.end()) return false;
    _favoritas.erase(it);
    return true;
}

bool Usuario::ehFavorita(Receita* r) const {
    return std::find(_favoritas.begin(), _favoritas.end(), r) != _favoritas.end();
}

const std::vector<Receita*>& Usuario::getFavoritas() const {
    return _favoritas;
}
