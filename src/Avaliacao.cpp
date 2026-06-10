
#include "../include/Avaliacao.hpp"
#include <algorithm>

Avaliacao::Avaliacao(Usuario* autor, int nota, const std::string& comentario)
    : _comentario(comentario),
      _nota(std::clamp(nota, 1, 5)),   // garante 1..5
      _autor(autor) {}

int Avaliacao::getNota() const                       { return _nota; }
const std::string& Avaliacao::getComentario() const  { return _comentario; }
Usuario* Avaliacao::getAutor() const                 { return _autor; }