#ifndef AVALIACAO_HPP
#define AVALIACAO_HPP

#include <string>


class Usuario;
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
    Avaliacao(Usuario* autor, int nota, std::string comentario);
    int getNota();
    std::string getComentario();
    Usuario* getAutor();

};

#endif
