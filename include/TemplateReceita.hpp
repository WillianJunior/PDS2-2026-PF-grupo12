
#ifndef TEMPLATERECEITA_HPP
#define TEMPLATERECEITA_HPP
#include <string>
#include <vector>
#include "Ingrediente.hpp"
#include "Receita.hpp"

/**
 * @class TemplateReceita
 * @brief Template para validar e gerar receitas com ingredientes obrigatórios.
 */
class TemplateReceita {
private:
    std::string _nome;
    std::string _descricao;
    std::vector<Ingrediente> _ingredientes;

public:
    TemplateReceita(const std::string& nome, const std::string& descricao);

    void adicionarIngrediente(const Ingrediente& ingrediente);

    /** Retorna true se a receita contém TODOS os ingredientes do template (por nome). */
    bool verificarReceita(const Receita& receita) const;

    /** Gera uma nova Receita pré-preenchida com os ingredientes deste template. */
    Receita gerarReceita(const std::string& titulo, int tempo,
                         Dificuldade dificuldade, Categoria categoria) const;

    const std::string& getNome() const;
    const std::string& getDescricao() const;
    const std::vector<Ingrediente>& getIngredientesT() const;
};

#endif