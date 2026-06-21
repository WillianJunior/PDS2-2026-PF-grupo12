
#include "../include/Avaliacao.hpp"
#include <algorithm>
#include <stdexcept>

Avaliacao::Avaliacao(Usuario* autor, int nota, const std::string& comentario)
    : _comentario(comentario),
      _nota(std::clamp(nota, 1, 5)),   // garante 1..5
      _autor(autor) {
    if (!autor)
      throw std::invalid_argument("Avaliacao: autor nao pode ser nulo");
}

int Avaliacao::getNota() const                       { return _nota; }
const std::string& Avaliacao::getComentario() const  { return _comentario; }
Usuario* Avaliacao::getAutor() const                 { return _autor; }