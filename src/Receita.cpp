#include "../include/Receita.hpp"

Receita::Receita(std::string titulo, int tempo, Dificuldade dificuldade, Categoria categoria)
    : _titulo(titulo), _instrucoes(""), _tempoPreparo(tempo),
      _dificuldade(dificuldade), _categoria(categoria) {}

void Receita::adicionarIngrediente(const Ingrediente& ingrediente) {
    _ingredientes.push_back(ingrediente);
}

void Receita::definirInstrucoes(std::string texto) {
    _instrucoes = texto;
}

void Receita::adicionarAvaliacao(Avaliacao post) {
    _avaliacoes.push_back(post);
}

double Receita::calcularMediaNotas() {
    if (_avaliacoes.empty()) return 0.0;
    double soma = 0.0;
    for (auto& a : _avaliacoes) {
        soma += a.getNota();
    }
    return soma / _avaliacoes.size();
}

std::string& Receita::getTitulo() { return _titulo; }

Dificuldade Receita::getDificuldade() { return _dificuldade; }

Categoria Receita::getCategoria() { return _categoria; }

std::string Receita::getInstrucoes() { return _instrucoes; }

std::vector<Ingrediente> Receita::getIngredientes() { return _ingredientes; }
