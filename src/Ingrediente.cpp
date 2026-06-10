#include "../include/Ingrediente.hpp"

Ingrediente::Ingrediente(const std::string& nome, double quant,
                         const std::string& unidade, const std::string& tipo)
    : _nome(nome), _quantidade(quant), _unidade(unidade), _tipo(tipo) {}

bool Ingrediente::verificarUnidades(double quant, const std::string& unidade) const {
    return (unidade == _unidade) && (quant <= _quantidade);
}

bool Ingrediente::validar() const {
    return !_nome.empty() && _quantidade > 0.0;
}

const std::string& Ingrediente::getNome() const     { return _nome; }

const std::string& Ingrediente::getUnidade() const  { return _unidade; }

const std::string& Ingrediente::getTipo() const     { return _tipo; }

double Ingrediente::getQuantidade() const { return _quantidade; }