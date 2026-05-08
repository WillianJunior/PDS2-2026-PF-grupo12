#ifndef TEMPLATERECEITA_H
#define TEMPLATERECEITA_H
#include <string>
#include "Ingrediente.hpp"
#include "Receita.hpp"

/**
 * @class Ingrediente
 * @brief Representa um ingrediente com nome, quantidade, unidade e tipo.
 */

class TemplateReceita //essa classe, seguindo o cartão, pede por uma maneira de fazer listas de ingredientes (ja tem em receita)
                      //e pede por um método que inicializa uma nova receita a partir de template. Creio que fazer herança com receita ia facilitar tudo isso.
{
private:

    std::string _nomeT; //nome do template
    std::string _Desc; //descrição do template
    int _totalIngredientes; // variável para contar quantos ingredientes existem na lista de ingredientes.
    Ingrediente lista_ingredientes [100];

public:

    TemplateReceita(std::string nome, std::string Descricao); //construtor, insere apenas nome e descrição.
    void adicionarIngrediente(Ingrediente ingrediente); //adiciona ingrediente à lista do template.
    bool verificarReceita(Receita receita); //verifica se receita segue os mesmos parâmetros de templateReceita.
    void gerarReceita(); //gera uma nova receita usando parametros do template.

    ~TemplateReceita();
};


#endif