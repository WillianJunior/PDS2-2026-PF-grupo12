#ifndef AVALIACAO_HPP
#define AVALIACAO_HPP
#include "Usuario.hpp"
#include <string>

/**
 * @class Avaliacao
 * @brief Representa o feedback do usuário sobre uma receita.
 */
class Avaliacao{
private:
    std::string _comentario;
    int _nota;
    Usuario* _autor;

public:
    Avaliacao(Usuario* autor, int nota, const std::string& comentario);
    int getNota() const;
    const std::string& getComentario() const;
    Usuario* getAutor() const;

};

#endif
