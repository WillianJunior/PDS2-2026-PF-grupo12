#include "../include/Sistema.hpp"
#include <algorithm>
#include <cctype>

namespace {
    // helper local: lowercase de uma string
    std::string toLower(const std::string& s) {
        std::string out(s.size(), '\0');
        std::transform(s.begin(), s.end(), out.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        return out;
    }
}

Sistema::Sistema() {}

bool Sistema::cadastrarUsuario(const std::string& nome,
                               const std::string& email,
                               const std::string& senha) {
    if (nome.empty() || email.empty() || senha.empty()) return false;
    for (const auto& u : _usuarios) {
        if (u.getEmail() == email) return false;     // não permite duplicado
    }
    _usuarios.emplace_back(nome, email, senha);
    return true;
}

bool Sistema::login(const std::string& email, const std::string& senha) {
    for (auto& u : _usuarios) {
        if (u.getEmail() == email && u.autenticar(senha)) {
            _usuarioAtivo = &u;                       // OK: list não invalida
            return true;
        }
    }
    return false;
}

void Sistema::logout() { _usuarioAtivo = nullptr; }

Usuario* Sistema::getUsuarioAtivo() const { return _usuarioAtivo; }

Receita* Sistema::cadastrarReceita(const std::string& titulo, int tempoPreparo,
                                   Dificuldade dificuldade, Categoria categoria) {
    _receitas.emplace_back(titulo, tempoPreparo, dificuldade, categoria);
    Receita* novo = &_receitas.back();
    if (_usuarioAtivo) _usuarioAtivo->adicionarReceitaPropria(novo);
    return novo;
}

bool Sistema::removerReceita(const std::string& titulo) {
    for (auto it = _receitas.begin(); it != _receitas.end(); ++it) {
        if (it->getTitulo() == titulo) {
            _receitas.erase(it);
            return true;
        }
    }
    return false;
}

std::vector<Receita*> Sistema::buscarPorTitulo(const std::string& titulo) {
    std::vector<Receita*> resultado;
    std::string alvo = toLower(titulo);
    for (auto& r : _receitas) {
        if (toLower(r.getTitulo()).find(alvo) != std::string::npos) {
            resultado.push_back(&r);
        }
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

bool Sistema::avaliar(const std::string& tituloReceita, int nota,
                      const std::string& comentario) {
    if (!_usuarioAtivo) return false;                 // exige login
    for (auto& r : _receitas) {
        if (r.getTitulo() == tituloReceita) {
            r.adicionarAvaliacao(Avaliacao(_usuarioAtivo, nota, comentario));
            return true;
        }
    }
    return false;
}

std::list<Receita>&       Sistema::getReceitas()       { return _receitas; }
const std::list<Receita>& Sistema::getReceitas() const { return _receitas; }