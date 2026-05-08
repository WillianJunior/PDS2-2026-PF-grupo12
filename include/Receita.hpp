#ifndef RECEITA_HPP
#define RECEITA_HPP

#include <string>
#include <vector>
#include "Avaliacao.hpp"
#include "Ingrediente.hpp"

enum class Dificuldade { Facil, Medio, Dificil };
enum class Categoria  { Doce, Salgado, Vegano, Vegetariano, Outro };


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
    Categoria _categoria;
    std::vector<Avaliacao> _avaliacoes;

public:
    Receita(std::string titulo, int tempo, Dificuldade dificuldade, Categoria categoria);

    void adicionarIngrediente(const Ingrediente& ingrediente);

    void definirInstrucoes(std::string texto);
    void adicionarAvaliacao(Avaliacao post);
    double calcularMediaNotas();

    std::string& getTitulo();
    Dificuldade getDificuldade();
    double calcularMediaNotas();

};

#endif
