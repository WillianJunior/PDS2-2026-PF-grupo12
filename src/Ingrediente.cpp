#include "../include/Ingrediente.hpp"
#include <stdexcept>


Ingrediente::Ingrediente(const std::string& nome, double quant,
                         const std::string& unidade, const std::string& tipo)
    : _nome(nome), _quantidade(quant), _unidade(unidade), _tipo(tipo) {
    if (nome.empty())
        throw std::invalid_argument("Ingrediente: nome nao pode ser vazio");
    if (quant <= 0.0)
        throw std::invalid_argument("Ingrediente: quantidade deve ser positiva");
    if (unidade.empty())
        throw std::invalid_argument("Ingrediente: unidade nao pode ser vazia");
}

bool Ingrediente::verificarUnidades(double quant, const std::string& unidade) const {
    return (unidade == _unidade) && (quant <= _quantidade);
}

bool Ingrediente::validar() const {
    return !_nome.empty() && _quantidade > 0.0;
}

void Ingrediente::alterarQuantidade(double razao){
    this->_quantidade = this->_quantidade*razao; 
}

const std::string& Ingrediente::getNome() const     { return _nome; }

const std::string& Ingrediente::getUnidade() const  { return _unidade; }

const std::string& Ingrediente::getTipo() const     { return _tipo; }

const double& Ingrediente::getQuantidade() const { return _quantidade; }