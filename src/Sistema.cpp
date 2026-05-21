#include "../include/Sistema.hpp"

Sistema::Sistema() {}

bool Sistema::cadastrarUsuario(std::string& nome, std::string& email, std::string& senha) {
    // nao permite emails duplicados
    for (auto& u : _usuarios) {
        if (u.getEmail() == email) return false;
    }
    _usuarios.emplace_back(nome, email, senha);
    return true;
}

bool Sistema::login(std::string& email, std::string& senha) {
    for (auto& u : _usuarios) {
        if (u.getEmail() == email && u.autenticar(senha)) {
            _usuarioAtivo = &u;
            return true;
        }
    }
    return false;
}

void Sistema::logout() { _usuarioAtivo = nullptr; }

Usuario* Sistema::getUsuarioAtivo() { return _usuarioAtivo; }

Receita* Sistema::cadastrarReceita(std::string& titulo, int tempoPreparo,
                                   Dificuldade dificuldade, Categoria categoria) {
    _receitas.emplace_back(titulo, tempoPreparo, dificuldade, categoria);
    return &_receitas.back();
}

void Sistema::removerReceita(std::string& titulo) {
    for (auto it = _receitas.begin(); it != _receitas.end(); ++it) {
        if (it->getTitulo() == titulo) {
            _receitas.erase(it);
            return;
        }
    }
}

std::vector<Receita*> Sistema::buscarPorTitulo(std::string& titulo) {
    std::vector<Receita*> resultado;
    for (auto& r : _receitas) {
        if (r.getTitulo() == titulo) resultado.push_back(&r);
    }
    return resultado;
}

std::vector<Receita*> Sistema::filtrarPorDificuldade(Dificuldade d) {
    std::vector<Receita*> resultado;
    for (auto& r : _receitas) {
        if (r.getDificuldade() == d) resultado.push_back(&r);
    }
    return resultado;
}

std::vector<Receita*> Sistema::filtrarPorNotaMinima(double notaMinima) {
    std::vector<Receita*> resultado;
    for (auto& r : _receitas) {
        if (r.calcularMediaNotas() >= notaMinima) resultado.push_back(&r);
    }
    return resultado;
}

void Sistema::avaliar(std::string& tituloReceita, int nota, std::string& comentario) {
    for (auto& r : _receitas) {
        if (r.getTitulo() == tituloReceita) {
            r.adicionarAvaliacao(Avaliacao(_usuarioAtivo, nota, comentario));
            return;
        }
    }
}

std::vector<Receita>& Sistema::getReceitas() { return _receitas; }
