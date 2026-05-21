#include "../include/Ingrediente.hpp"

Ingrediente::Ingrediente(std::string nome, int quant, std::string unidade, std::string tipo)
    : _nomeI(nome), _quantidadeI(quant), _unidadeI(unidade), _tipoI(tipo) {}

bool Ingrediente::verificar_unidades(int quant, std::string unidade) {
    // verdadeiro se a unidade bate e ha quantidade local suficiente
    return (unidade == _unidadeI) && (quant <= _quantidadeI);
}

bool Ingrediente::validar_ingrediente() {
    // um ingrediente valido tem nome nao vazio e quantidade positiva
    return !_nomeI.empty() && _quantidadeI > 0;
}

std::string Ingrediente::_getnomeI() { return _nomeI; }

std::string Ingrediente::_getunidadeI() { return _unidadeI; }

std::string Ingrediente::_getTipoI() { return _tipoI; }

int Ingrediente::_getquantI() { return _quantidadeI; }

Ingrediente::~Ingrediente() {}
