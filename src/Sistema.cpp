#include "Sistema.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <iostream>


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

// ───────────────────────────────────────────────────────────
// Conversores enum <-> string para persistência
// ───────────────────────────────────────────────────────────
std::string Sistema::dificuldadeParaStr(Dificuldade d) {
    switch (d) {
        case Dificuldade::Facil:   return "Facil";
        case Dificuldade::Medio:   return "Medio";
        case Dificuldade::Dificil: return "Dificil";
    }
    return "Facil";
}

std::string Sistema::categoriaParaStr(Categoria c) {
    switch (c) {
        case Categoria::Doce:        return "Doce";
        case Categoria::Salgado:     return "Salgado";
        case Categoria::Vegano:      return "Vegano";
        case Categoria::Vegetariano: return "Vegetariano";
        case Categoria::Outro:       return "Outro";
    }
    return "Outro";
}

Dificuldade Sistema::strParaDificuldade(const std::string& s) {
    if (s == "Medio")   return Dificuldade::Medio;
    if (s == "Dificil") return Dificuldade::Dificil;
    return Dificuldade::Facil;
}

Categoria Sistema::strParaCategoria(const std::string& s) {
    if (s == "Salgado")     return Categoria::Salgado;
    if (s == "Vegano")      return Categoria::Vegano;
    if (s == "Vegetariano") return Categoria::Vegetariano;
    if (s == "Doce")        return Categoria::Doce;
    return Categoria::Outro;
}

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
            Receita* alvo = &(*it);                  

            for (auto& u : _usuarios) {
                u.removerReceitaPropria(alvo);
                u.removerFavorita(alvo);
            }

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

std::list<Usuario>&       Sistema::getUsuarios()       { return _usuarios; }
const std::list<Usuario>& Sistema::getUsuarios() const { return _usuarios; }

// ───────────────────────────────────────────────────────────
// Persistência em CSV (data/usuarios.csv e data/receitas.csv)
// ───────────────────────────────────────────────────────────

// Helper para formatar double: se for inteiro, escreve sem casas decimais.
namespace {
    std::string formatarQuantidade(double q) {
        if (q == static_cast<int>(q)) {
            return std::to_string(static_cast<int>(q));
        }
        std::ostringstream oss;
        oss << q;
        return oss.str();
    }
}

void Sistema::salvar() {
    // --- usuarios ---
    std::ofstream fu("data/usuarios.csv");
    if (!fu) {
        std::cerr << "Erro: nao foi possivel abrir data/usuarios.csv para escrita\n";
        return;
    }
    for (const auto& u : _usuarios) {
        fu << u.getNome() << "," << u.getEmail() << "," << u.getSenha() << "\n";
        //atenção aqui! coloquei texto simples na senha, mas sugiro colocarmos  hash no futuro - Bernardo.
        //mas resolve problema de senha vazia passando no login que estava tendo. Olhar em carregar() tambem.
    }

    // --- receitas ---
    std::ofstream fr("data/receitas.csv");
    if (!fr) {                                          // <-- mesmo guard aqui
        std::cerr << "Erro: nao foi possivel abrir data/receitas.csv para escrita\n";
        return;
    }
    for (const auto& r : _receitas) {
        std::string ings;
        for (const auto& i : r.getIngredientes()) {
            if (!ings.empty()) ings += ";";
            ings += i.getNome() + "|"
                  + formatarQuantidade(i.getQuantidade()) + "|"
                  + i.getUnidade() + "|"
                  + i.getTipo();
        }

        fr << r.getTitulo()                          << ","
           << r.getTempoPreparo()                    << ","
           << dificuldadeParaStr(r.getDificuldade()) << ","
           << categoriaParaStr(r.getCategoria())     << ","
           << r.getInstrucoes()                      << ","
           << ings                                   << "\n";
    }
}

void Sistema::carregar() {
    _usuarios.clear();
    _receitas.clear();
    _usuarioAtivo = nullptr;

    // --- usuarios ---
    std::ifstream fu("data/usuarios.csv");
    std::string linha;
    while (std::getline(fu, linha)) {
        std::stringstream ss(linha);
        std::string nome, email, senha;
        std::getline(ss, nome,  ',');
        std::getline(ss, email, ',');
        std::getline(ss, senha, ',');
        //aqui coloquei pra carregar a senha, nao rolava no login pois nao salvava em nenhum lugar (precisa ser mais seguro na vida real)
        if (!nome.empty()) {
            _usuarios.emplace_back(nome, email, senha);
        }
    }

    // --- receitas ---
    std::ifstream fr("data/receitas.csv");
    while (std::getline(fr, linha)) {
        try {
            std::stringstream ss(linha);
            std::string titulo, stempo, sdif, scat, instrucoes, ings;
            std::getline(ss, titulo,     ',');
            std::getline(ss, stempo,     ',');
            std::getline(ss, sdif,       ',');
            std::getline(ss, scat,       ',');
            std::getline(ss, instrucoes, ',');
            std::getline(ss, ings,       ',');

            if (titulo.empty()) continue;

            _receitas.emplace_back(titulo, std::stoi(stempo),
                                   strParaDificuldade(sdif),
                                   strParaCategoria(scat));
            Receita& r = _receitas.back();
            r.definirInstrucoes(instrucoes);

            std::stringstream si(ings);
            std::string bloco;
            while (std::getline(si, bloco, ';')) {
                std::stringstream sb(bloco);
                std::string nome, squant, unidade, tipo;
                std::getline(sb, nome,    '|');
                std::getline(sb, squant,  '|');
                std::getline(sb, unidade, '|');
                std::getline(sb, tipo,    '|');
                if (!nome.empty()) {
                    r.adicionarIngrediente(Ingrediente(nome, std::stod(squant),
                                                       unidade, tipo));
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Aviso: linha de receita ignorada (" << e.what() << "): "
                      << linha << "\n";
        }
    }
}
