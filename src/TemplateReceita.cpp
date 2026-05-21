#include "../include/TemplateReceita.hpp"

TemplateReceita::TemplateReceita(std::string nome, std::string Descricao)
    : _nomeT(nome), _Desc(Descricao), _totalIngredientes(0) {
    for (int i = 0; i < 100; ++i) lista_ingredientes[i] = nullptr;
}

void TemplateReceita::adicionarIngrediente(Ingrediente ingrediente) {
    if (_totalIngredientes < 100) {
        lista_ingredientes[_totalIngredientes] = new Ingrediente(ingrediente);
        _totalIngredientes++;
    }
}

bool TemplateReceita::verificarReceita(Receita receita) {
    // verifica se todos os ingredientes do template estao presentes na receita
    std::vector<Ingrediente> ingredientesReceita = receita.getIngredientes();
    for (int i = 0; i < _totalIngredientes; ++i) {
        bool encontrado = false;
        for (auto& ing : ingredientesReceita) {
            if (ing._getnomeI() == lista_ingredientes[i]->_getnomeI()) {
                encontrado = true;
                break;
            }
        }
        if (!encontrado) return false;
    }
    return true;
}

void TemplateReceita::gerarReceita() {
    // Esqueleto: a geracao efetiva de uma Receita a partir do template
    // sera implementada em etapa futura (ver consideracoes sobre heranca).
}

std::vector<Ingrediente> TemplateReceita::getIngredientesT() {
    std::vector<Ingrediente> resultado;
    for (int i = 0; i < _totalIngredientes; ++i) {
        if (lista_ingredientes[i] != nullptr) {
            resultado.push_back(*lista_ingredientes[i]);
        }
    }
    return resultado;
}

TemplateReceita::~TemplateReceita() {
    for (int i = 0; i < _totalIngredientes; ++i) {
        delete lista_ingredientes[i];
        lista_ingredientes[i] = nullptr;
    }
}
