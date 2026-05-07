#ifndef AVALIACAO_HPP
#define AVALIACAO_HPP

#include <string>

/**
 * @class Avaliacao
 * @brief Representa o feedback do usuário sobre uma receita.
 */
class Avaliacao{
private:
    std::string _comentario;
    int _nota;
    std::string _autor;

public:
    Avaliacao(std::string autor, int nota, std::string comentario);
    int getNota();
    std::string getComentario();
    std::string getAutor();
};

#endif
