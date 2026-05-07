#ifndef RECEITA_HPP
#define RECEITA_HPP

#include <string>
#include <vector>
#include "Avaliacao.hpp"

/**
 * @class Receita
 * @brief Gerencia as informações da receita e suas avaliações.
 */
class Receita {
private:
    std::string _titulo;
    std::vector<std::string> _ingredientes;
    std::string _instrucoes;
    int _tempoPreparo;
    std::vector<Avaliacao> _avaliacoes;

public:
    Receita(std::string titulo, int tempo);
    void adicionarIngrediente(std::string ingrediente);
    void definirInstrucoes(std::string texto);
    void adicionarAvaliacao(Avaliacao post);
    float calcularMediaNotas();
};

#endif
