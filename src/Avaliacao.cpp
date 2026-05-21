#include "../include/Avaliacao.hpp"

Avaliacao::Avaliacao(Usuario* autor, int nota, std::string comentario)
    : _comentario(comentario), _nota(nota), _autor(autor) {}

int Avaliacao::getNota() { return _nota; }

std::string Avaliacao::getComentario() { return _comentario; }

Usuario* Avaliacao::getAutor() { return _autor; }
