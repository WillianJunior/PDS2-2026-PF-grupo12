#include "Sistema.hpp"
#include "Chef.hpp"
#include "Cozinheiro.hpp"
#include "Admin.hpp"

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

std::string Sistema::nAcessoParaStr(nivelAcesso n) {
    switch (n) {
        case nivelAcesso::Chef:   return "Chef";
        case nivelAcesso::Cozinheiro:   return "Cozinheiro";
        case nivelAcesso::Admin:   return "Admin";
    }
    return "Cozinheiro";
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

nivelAcesso Sistema::strParaNAcesso(const std::string& s) {
    if (s == "Chef") return nivelAcesso::Chef;
    if (s == "Admin") return nivelAcesso::Admin;
    return nivelAcesso::Cozinheiro;
}

bool Sistema::cadastrarUsuario(const std::string& nome,
                               const std::string& email,
                               const std::string& senha,
                               const nivelAcesso& nAcesso) {
    if (nome.empty() || email.empty() || senha.empty()) return false;
    for (const auto& u : _usuarios) {
        if (u->getEmail() == email) return false;     // não permite duplicado 
        //edit 2: agora como sendo ponteiro, usuario precisa de -> e isso vale pra todos os atributos
    }
    if (nAcesso == nivelAcesso::Chef) {
        _usuarios.push_back(std::make_unique<Chef>(nome, email, senha));
    } else if (nAcesso == nivelAcesso::Admin) {
        _usuarios.push_back(std::make_unique<Admin>(nome, email, senha));
    } else {
        _usuarios.push_back(std::make_unique<Cozinheiro>(nome, email, senha));
    }
    return true;
}

bool Sistema::login(const std::string& email, const std::string& senha) {
    for (auto& u : _usuarios) {
        if (u->getEmail() == email && u->autenticar(senha)) {
            _usuarioAtivo = u.get();                       // OK: list não invalida
            //essa edit nova u.get é pra pegar o Usuario* 
            return true;
        }
    }
    return false;
}

void Sistema::logout() { _usuarioAtivo = nullptr; }

Usuario* Sistema::getUsuarioAtivo() const { return _usuarioAtivo; }

nivelAcesso Sistema::getAcessoUsuarioAtivo() const { return _usuarioAtivo->getAcesso(); }

Receita* Sistema::cadastrarReceita(const std::string& titulo, int tempoPreparo,
                                   Dificuldade dificuldade, Categoria categoria, int rendimento) {
    _receitas.emplace_back(titulo, tempoPreparo, dificuldade, categoria, rendimento);
    Receita* novo = &_receitas.back();
    if (_usuarioAtivo) _usuarioAtivo->adicionarReceitaPropria(novo);
    return novo;
}

TemplateReceita* Sistema::cadastrarTemplate(const std::string& titulo, std::string descricao, int rendimentoT){
     _templates.insert_or_assign(titulo, TemplateReceita(titulo, descricao, rendimentoT));
     TemplateReceita* novo = &_templates.at(titulo);
     return novo;
}
std::vector<Ingrediente> Sistema::getIngredientesTemplate(std::string chave) {
    return _templates.at(chave).getIngredientesT();
}

int Sistema::getRendimentoTemplate(std::string chave) {
    return _templates.at(chave).getRendimentoT();
}

bool Sistema::removerReceita(const std::string& titulo) {
    for (auto it = _receitas.begin(); it != _receitas.end(); ++it) {
        if (it->getTitulo() == titulo) {
            Receita* alvo = &(*it);                  

            for (auto& u : _usuarios) {
                u->removerReceitaPropria(alvo);
                u->removerFavorita(alvo);
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

// Sugere receitas que o usuario ativo consegue fazer com os ingredientes que tem disponiveis.
// Um ingrediente da receita e considerado "disponivel" se o usuario possui um ingrediente
// de mesmo nome (comparacao por nome, ignorando quantidade/unidade) -- ver toLower para
// tornar a comparacao insensivel a maiusculas/minusculas.
std::vector<Receita*> Sistema::sugerirReceitas(){

    std::vector<Receita*> SujestReceita;
    const auto& disponiveis = getUsuarioAtivo()->getIngredientesDisp();
    if(disponiveis.empty()){
        throw std::invalid_argument("Nenhum ingrediente disponivel foi adicionado pelo usuário!");
    }

    for (auto& receita : this->_receitas) {

        bool podeFazer = true;

        for (const auto& ingrediente : receita.getIngredientes())
        {
            // o usuario tem um ingrediente com este nome?
            bool tem = std::any_of(
                disponiveis.begin(), disponiveis.end(),
                [&](const Ingrediente& disp){
                    return toLower(disp.getNome()) == toLower(ingrediente.getNome());
                });

            if (!tem)
            {
                podeFazer = false;
                break;
            }
        }

        if (podeFazer)
        {
            SujestReceita.push_back(&receita);
        }
    }

    return SujestReceita;
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
    std::string alvo = toLower(tituloReceita);
    for (auto& r : _receitas) {
        if (toLower(r.getTitulo()) == alvo) {
            r.adicionarAvaliacao(Avaliacao(_usuarioAtivo, nota, comentario));
            return true;
        }
    }
    return false;
}

std::list<Receita>& Sistema::getReceitas()       { return _receitas; }
const std::list<Receita>& Sistema::getReceitas() const { return _receitas; }

std::list<std::unique_ptr<Usuario>>&  Sistema::getUsuarios()       { return _usuarios; }
const std::list<std::unique_ptr<Usuario>>& Sistema::getUsuarios() const { return _usuarios; }

std::map<std::string,TemplateReceita>& Sistema::getTemplates()       { return _templates; }
const std::map<std::string,TemplateReceita>& Sistema::getTemplates() const { return _templates; }

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
        // serializa a despensa (ingredientes disponiveis) do usuario no MESMO
        // sub-formato usado por receitas/templates: nome|qtd|unidade|tipo, com
        // blocos separados por ';'. Vira o 5o campo do registro do usuario.
        std::string despensa;
        for (const auto& i : u->getIngredientesDisp()) {
            if (!despensa.empty()) despensa += ";";
            despensa += i.getNome() + "|"
                      + formatarQuantidade(i.getQuantidade()) + "|"
                      + i.getUnidade() + "|"
                      + i.getTipo();
        }

        // favoritas e receitas proprias sao vetores de Receita* apontando para
        // a lista _receitas. Nao da pra salvar ponteiros (endereco muda a cada
        // execucao), entao salvamos o TITULO de cada receita como chave estavel.
        // Blocos separados por ';'. No carregar() os ponteiros sao religados por
        // titulo, DEPOIS que receitas e usuarios ja existem.
        std::string favoritas;
        for (const auto* r : u->getFavoritas()) {
            if (!r) continue;
            if (!favoritas.empty()) favoritas += ";";
            favoritas += r->getTitulo();
        }

        std::string proprias;
        for (const auto* r : u->getReceitasProprias()) {
            if (!r) continue;
            if (!proprias.empty()) proprias += ";";
            proprias += r->getTitulo();
        }

        fu << u->getNome() << "," << u->getEmail() << "," << u->getSenha() << ","
           << nAcessoParaStr(u->getAcesso()) << "," << despensa << ","
           << favoritas << "," << proprias << "\n"; //como temos override, getAcesso vem das filhas.
        //atenção aqui! coloquei texto simples na senha, mas sugiro colocarmos  hash no futuro - Bernardo.
        //mas resolve problema de senha vazia passando no login que estava tendo. Olhar em carregar() tambem.
        //coloquei nivel de acesso pra persistir. Despensa=5o campo, favoritas=6o, proprias=7o.
    }

    // --- receitas ---
    std::ofstream fr("data/receitas.csv");
    if (!fr) {                                          
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
           << r.getRendimento()                      << ","
           << dificuldadeParaStr(r.getDificuldade()) << ","
           << categoriaParaStr(r.getCategoria())     << ","
           << r.getInstrucoes()                      << ","
           << ings                                   << "\n";
    }

    // --- templates ---
    std::ofstream ft("data/templates.csv");
    if (!ft) {
        std::cerr << "Erro: nao foi possivel abrir data/templates.csv para escrita\n";
        return;
    }
    for (const auto& [chave, t] : _templates) {
        std::string ings;
        for (const auto& i : t.getIngredientesT()) {
            if (!ings.empty()) ings += ";";
            ings += i.getNome() + "|"
                  + formatarQuantidade(i.getQuantidade()) + "|"
                  + i.getUnidade() + "|"
                  + i.getTipo();
        }

        ft << t.getNome()        << ","
           << t.getDescricao()   << ","
           << t.getRendimentoT() << ","
           << ings               << "\n";
    }
}

void Sistema::carregar() {
    _usuarios.clear();
    _receitas.clear();
    _templates.clear();
    _usuarioAtivo = nullptr;

    // Guarda temporariamente os titulos de favoritas/proprias lidos de cada
    // usuario. Os ponteiros Receita* so podem ser religados DEPOIS que a lista
    // _receitas estiver carregada (mais abaixo). Cada item: (usuario, titulos).
    struct VinculoPendente {
        Usuario* usuario;
        std::vector<std::string> favoritas;
        std::vector<std::string> proprias;
    };
    std::vector<VinculoPendente> pendentes;

    // helper local: divide "a;b;c" no vetor {a,b,c}, ignorando blocos vazios.
    auto split = [](const std::string& s){
        std::vector<std::string> out;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, ';')) {
            if (!item.empty()) out.push_back(item);
        }
        return out;
    };

    // --- usuarios ---
    std::ifstream fu("data/usuarios.csv");
    std::string linha;
    while (std::getline(fu, linha)) {
        std::stringstream ss(linha);
        std::string nome, email, senha, sacesso, despensa, sfav, sprop;
        std::getline(ss, nome,  ',');
        std::getline(ss, email, ',');
        std::getline(ss, senha, ',');
        //aqui coloquei pra carregar a senha, nao rolava no login pois nao salvava em nenhum lugar (precisa ser mais seguro na vida real)
        std::getline(ss, sacesso, ',');
        std::getline(ss, despensa, ',');  // 5o campo: despensa serializada (pode faltar em CSVs antigos)
        std::getline(ss, sfav,  ',');     // 6o campo: titulos das favoritas
        std::getline(ss, sprop, ',');     // 7o campo: titulos das receitas proprias
        
        if (!nome.empty()) {
            nivelAcesso nAcesso = strParaNAcesso(sacesso);
            if (nAcesso == nivelAcesso::Chef) {
                _usuarios.push_back(std::make_unique<Chef>(nome, email, senha));
            } else if (nAcesso == nivelAcesso::Admin) {
                _usuarios.push_back(std::make_unique<Admin>(nome, email, senha));
            } else {
                _usuarios.push_back(std::make_unique<Cozinheiro>(nome, email, senha));
            }

            // reconstroi a despensa no usuario recem-criado, espelhando o salvar().
            // Mesmo sub-formato dos ingredientes de receita: nome|qtd|unidade|tipo,
            // blocos separados por ';'. CSV antigo (4 campos) deixa 'despensa' vazia,
            // entao o laco simplesmente nao executa -> retrocompativel.
            Usuario* u = _usuarios.back().get();
            std::stringstream sd(despensa);
            std::string bloco;
            while (std::getline(sd, bloco, ';')) {
                std::stringstream sb(bloco);
                std::string inome, squant, unidade, tipo;
                std::getline(sb, inome,   '|');
                std::getline(sb, squant,  '|');
                std::getline(sb, unidade, '|');
                std::getline(sb, tipo,    '|');
                if (!inome.empty()) {
                    try {
                        u->adicionarIngredienteDisponivel(
                            Ingrediente(inome, std::stod(squant), unidade, tipo));
                    } catch (const std::exception& e) {
                        std::cerr << "Aviso: ingrediente de despensa ignorado ("
                                  << e.what() << "): " << bloco << "\n";
                    }
                }
            }

            // guarda os titulos para religar os ponteiros depois das receitas.
            // CSV antigo (sem esses campos) gera vetores vazios -> retrocompativel.
            pendentes.push_back({u, split(sfav), split(sprop)});
        }
    }

    // --- receitas ---
    std::ifstream fr("data/receitas.csv");
    while (std::getline(fr, linha)) {
        try {
            std::stringstream ss(linha);
            std::string titulo, stempo, srend, sdif, scat, instrucoes, ings;
            std::getline(ss, titulo,     ',');
            std::getline(ss, stempo,     ',');
            std::getline(ss, srend,      ',');
            std::getline(ss, sdif,       ',');
            std::getline(ss, scat,       ',');
            std::getline(ss, instrucoes, ',');
            std::getline(ss, ings,       ',');

            if (titulo.empty()) continue;

            _receitas.emplace_back(titulo, std::stoi(stempo),
                                   strParaDificuldade(sdif),
                                   strParaCategoria(scat), std::stoi(srend));
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

    // --- templates ---
    std::ifstream ft("data/templates.csv");
    while (std::getline(ft, linha)) {
        try {
            std::stringstream ss(linha);
            std::string nome, descricao, srend, ings;
            std::getline(ss, nome,      ',');
            std::getline(ss, descricao, ',');
            std::getline(ss, srend,     ',');
            std::getline(ss, ings,      ',');

            if (nome.empty()) continue;

            // cadastrarTemplate cria o template no map e retorna ponteiro para ele
            TemplateReceita* t = cadastrarTemplate(nome, descricao, std::stoi(srend));

            std::stringstream si(ings);
            std::string bloco;
            while (std::getline(si, bloco, ';')) {
                std::stringstream sb(bloco);
                std::string inome, squant, unidade, tipo;
                std::getline(sb, inome,   '|');
                std::getline(sb, squant,  '|');
                std::getline(sb, unidade, '|');
                std::getline(sb, tipo,    '|');
                if (!inome.empty()) {
                    t->adicionarIngrediente(Ingrediente(inome, std::stod(squant),
                                                        unidade, tipo));
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Aviso: linha de template ignorada (" << e.what() << "): "
                      << linha << "\n";
        }
    }

    
    // Agora que _receitas esta completa, traduzimos cada titulo guardado de
    // Busca por titulo EXATO  
    auto acharReceitaPorTitulo = [this](const std::string& titulo) -> Receita* {
        for (auto& r : _receitas) {
            if (r.getTitulo() == titulo) return &r;
        }
        return nullptr;
    };

    for (auto& p : pendentes) {
        for (const auto& titulo : p.proprias) {
            if (Receita* r = acharReceitaPorTitulo(titulo)) {
                p.usuario->adicionarReceitaPropria(r);
            } else {
                std::cerr << "Aviso: receita propria nao encontrada ao religar: "
                          << titulo << "\n";
            }
        }
        for (const auto& titulo : p.favoritas) {
            if (Receita* r = acharReceitaPorTitulo(titulo)) {
                p.usuario->adicionarFavorita(r);  // ja evita duplicata internamente
            } else {
                std::cerr << "Aviso: receita favorita nao encontrada ao religar: "
                          << titulo << "\n";
            }
        }
    }
}