#include "Receita.hpp"

Receita::Receita(const std::string& titulo, int tempo,
                 Dificuldade dificuldade, Categoria categoria)
    : _titulo(titulo), _instrucoes(""), _tempoPreparo(tempo),
      _dificuldade(dificuldade), _categoria(categoria) {}

void Receita::adicionarIngrediente(const Ingrediente& ingrediente) {
    _ingredientes.push_back(ingrediente);
}

void Receita::definirInstrucoes(const std::string& texto) {
    _instrucoes = texto;
}

void Receita::adicionarAvaliacao(const Avaliacao& post) {
    _avaliacoes.push_back(post);
}

double Receita::calcularMediaNotas() const {
    if (_avaliacoes.empty()) return 0.0;
    double soma = 0.0;
    for (const auto& a : _avaliacoes) soma += a.getNota();
    return soma / static_cast<double>(_avaliacoes.size());
}

const std::string& Receita::getTitulo() const       { return _titulo; }
Dificuldade        Receita::getDificuldade() const  { return _dificuldade; }
Categoria          Receita::getCategoria() const    { return _categoria; }
int                Receita::getTempoPreparo() const { return _tempoPreparo; }
const std::string& Receita::getInstrucoes() const   { return _instrucoes; }

const std::vector<Ingrediente>& Receita::getIngredientes() const {
    return _ingredientes;
}
const std::vector<Avaliacao>& Receita::getAvaliacoes() const {
    return _avaliacoes;
}
