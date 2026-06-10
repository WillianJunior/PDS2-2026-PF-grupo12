#ifndef RECEITA_HPP
#define RECEITA_HPP
#include <string>
#include <vector>
#include "Avaliacao.hpp"
#include "Ingrediente.hpp"

enum class Dificuldade { Facil, Medio, Dificil };
enum class Categoria   { Doce, Salgado, Vegano, Vegetariano, Outro };

/**
 * @class Receita
 * @brief Gerencia as informações da receita e suas avaliações.
 */
class Receita {
private:
    std::string _titulo;
    std::vector<Ingrediente> _ingredientes;
    std::string _instrucoes;
    int _tempoPreparo;
    Dificuldade _dificuldade;
    Categoria   _categoria;
    std::vector<Avaliacao> _avaliacoes;

public:
    Receita(const std::string& titulo, int tempo,
            Dificuldade dificuldade, Categoria categoria);

    void adicionarIngrediente(const Ingrediente& ingrediente);

    void definirInstrucoes(const std::string& texto);

    void adicionarAvaliacao(const Avaliacao& post);

    double calcularMediaNotas() const;

    const std::string& getTitulo() const;

    Dificuldade getDificuldade() const;

    Categoria   getCategoria() const;

    int getTempoPreparo() const;

    const std::string& getInstrucoes() const;

    const std::vector<Ingrediente>& getIngredientes() const;
    
    const std::vector<Avaliacao>& getAvaliacoes() const;
};

#endif