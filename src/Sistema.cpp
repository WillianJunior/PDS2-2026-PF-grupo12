#include "../include/Sistema.hpp"

Sistema::Sistema() {}

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
    if (s == "Doce")       return Categoria::Doce;
    return Categoria::Outro;
}

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

void Sistema::salvar() {
    //usuarios
    std::ofstream fu("data/usuarios.csv");
    for(auto& u : _usuarios)
        fu << u.getNome() << "," << u.getEmail() << "\n";

    //receitas
    std::ofstream fr("data/receitas.csv");
    for(auto& r : _receitas) {
        std::string ings = "";
        for (auto& i : r.getIngredientes()) {
            if(!ings.empty()) ings += ";";
            ings += i._getnomeI() + "|"
                  + std::to_string(i._getquantI()) + "|"
                  + i._getunidadeI() + "|"
                  + i._getTipoI();
        }

        fr  << r.getTitulo()                          << ","
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
        std::string nome, email;
        std::getline(ss, nome,  ',');
        std::getline(ss, email, ',');
        if (!nome.empty())
            _usuarios.emplace_back(nome, email, "");
    }

    // --- receitas ---
    std::ifstream fr("data/receitas.csv");
    while (std::getline(fr, linha)) {
        std::stringstream ss(linha);
        std::string titulo, stempo, sdif, scat, instrucoes, ings;
        std::getline(ss, titulo,     ',');
        std::getline(ss, stempo,     ',');
        std::getline(ss, sdif,       ',');
        std::getline(ss, scat,       ',');
        std::getline(ss, instrucoes, ',');
        std::getline(ss, ings,       ',');

        if (titulo.empty()) continue;

       _receitas.emplace_back(titulo, std::stoi(stempo), strParaDificuldade(sdif), strParaCategoria(scat));
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
            if (!nome.empty())
                r.adicionarIngrediente(Ingrediente(nome, std::stoi(squant), unidade, tipo));
        }
    }
}

std::vector<Usuario>& Sistema::getUsuarios() { return _usuarios; }