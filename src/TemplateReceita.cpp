#include "../include/TemplateReceita.hpp"

TemplateReceita::TemplateReceita(const std::string& nome, const std::string& descricao, const int rendimentoT)
    : _nome(nome), _descricao(descricao), _rendimentoT(rendimentoT) {}

void TemplateReceita::adicionarIngrediente(const Ingrediente& ingrediente) {
    _ingredientes.push_back(ingrediente);
}

bool TemplateReceita::verificarReceita(const Receita& receita) const {
    const auto& ingsReceita = receita.getIngredientes();
    for (const auto& alvo : _ingredientes) {
        bool encontrado = false;
        for (const auto& ing : ingsReceita) {
            if (ing.getNome() == alvo.getNome()) {
                encontrado = true;
                break;
            }
        }
        if (!encontrado) return false;
    }
    return true;
}

Receita TemplateReceita::gerarReceita(const std::string& titulo, int tempo,
                                      Dificuldade dificuldade, Categoria categoria, int rendimento) const {
    Receita r(titulo, tempo, dificuldade, categoria, rendimento);
    for (const auto& ing : _ingredientes) {
        r.adicionarIngrediente(ing);
    }
    return r;
}

const std::string& TemplateReceita::getNome() const      { return _nome; }

const std::string& TemplateReceita::getDescricao() const { return _descricao; }

const std::vector<Ingrediente>& TemplateReceita::getIngredientesT() const { return _ingredientes; }

const int& TemplateReceita::getRendimentoT() const { return _rendimentoT; }