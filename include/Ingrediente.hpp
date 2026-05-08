#ifndef INGREDIENTES_H
#define INGREDIENTES_H
#include <string>

class Ingrediente
{
private:

    std::string _nomeI; //nome do ingrediente
    int _quantidadeI; //quantidade (disponivel ou necessária, a depender de qual lista esta sendo chamada)
    std::string _unidadeI; //unidade de medida da receita (talvez precisemos de uma função de conversão...)
    std::string _tipoI; //esse eu não estou certo se vai aqui ou não. Imagino que sua utilidade esteja relacionada com a ferramenta de pesquisa.

public:
    Ingrediente(std::string nome, int quant, std::string unidade, std::string tipo); //construtor para ser usado em funções do tipo "adicionar ingrediente".

    bool verificar_unidades(int quant, std::string unidade){}; //verifica se o item está disponivel na quantidade/unidade informada (compara com os valores locais).
    bool validar_ingrediente(){}; //verifica se o ingrediente 

    //getters
    std::string _getnomeI(){};
    std::string _getunidadeI(){};
    std::string _getTipoI(){};
    int _getquantI(){};

    ~Ingrediente();
};

#endif