#include "../include/Menu.hpp"
#include "../include/Sistema.hpp"
#include "../include/Ingrediente.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <string>
#include <cctype>

void Menu::exibirCabecalho() const {
    std::cout << R"(
      _.--._  _.--._
,-=.-":;:;:;\':;:;:;"-._
\\\:;:;:;:;:;\:;:;:;:;:;\
 \\\:;:;:;:;:;\:;:;:;:;:;\
  \\\:;:;:;:;:;\:;:;:;:;:;\
   \\\:;:;:;:;:;\:;::;:;:;:\
    \\\;:;::;:;:;\:;:;:;::;:\
     \\\;;:;:_:--:\:_:--:_;:;\
      \\\_.-"      :      "-._\
       \`_..--""--.;.--""--.._=>

,------. ,--. ,-----.,------.,--------.,--------. ,---.  ,------. ,--. ,-----.  
|  .--. '|  |'  .--./|  .---''--.  .--''--.  .--'/  O  \ |  .--. '|  |'  .-.  ' 
|  '--'.'|  ||  |    |  `--,    |  |      |  |  |  .-.  ||  '--'.'|  ||  | |  | 
|  |\  \ |  |'  '--'\|  `---.   |  |      |  |  |  | |  ||  |\  \ |  |'  '-'  ' 
`--' '--'`--' `-----'`------'   `--'      `--'  `--' `--'`--' '--'`--' `-----'  
-Grupo 12
)" << "\n";
}

void Menu::telaCarregamento() const {
    std::cout << "\nCarregando";
    for (int i = 0; i < 3; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        std::cout << "." << std::flush;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    std::cout << "\n\n";
}

void Menu::limparTela() const {
    system("clear");
}

void Menu::pausar() const {
    std::cout << "\nPressione Enter para continuar...";
    std::string descarta;
    std::getline(std::cin, descarta);  // espera o usuario apertar Enter
}

void Menu::exibirOpcoes(bool logado, const std::string& nome, bool admin) const {
    
    std::cout << "██▄  ▄██ ██████ ███  ██ ██  ██\n";
    std::cout << "██ ▀▀ ██ ██▄▄   ██ ▀▄██ ██  ██\n";
    std::cout << "██    ██ ██▄▄▄▄ ██   ██ ▀████▀\n";

    if (logado)
        std::cout << "\n(LOGADO: " << nome << ")\n\n";
    else
        std::cout << "\n(NAO LOGADO)\n\n";

    // opcoes agora estao agrupadas por tema em colunas
    std::cout << "  CONTA              RECEITAS                TEMPLATES\n";
    std::cout << "  1. Cadastrar       4. Cadastrar receita    6. Cadastrar template\n";
    std::cout << "  2. Login           5. Receita via template 8. Listar templates\n";
    std::cout << "  3. Logout          7. Listar receitas\n";
    std::cout << "                     9. Buscar receita\n";
    std::cout << "                     10. Filtrar dificuldade\n";
    std::cout << "                     11. Avaliar receita\n";
    std::cout << "                     12. Favoritar\n";
    std::cout << "                     13. Ver favoritas\n";
    std::cout << "                     15. Ver receita completa\n";
    std::cout << "\n  INGREDIENTES\n";
    std::cout << "  16. Adicionar meus ingredientes   17. Sugerir receitas\n";
    if (admin) {
        std::cout << "\n  ADMIN\n";
        std::cout << "  18. Remover usuario\n";
    }
    std::cout << "\n  14. Salvar dados     0. Sair\n";
    std::cout << "\nOpcao: ";
}

int Menu::lerInteiro(const std::string& prompt) const {
    std::string linha;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, linha);

        // tenta converter a linha INTEIRA em inteiro.
        // stoi le o prefixo numerico e informa em 'pos' quantos caracteres usou.
        try {
            size_t pos;
            int valor = std::stoi(linha, &pos);
            // pula espacos em branco que venham depois do numero
            while (pos < linha.size() &&
                   std::isspace(static_cast<unsigned char>(linha[pos]))) {
                pos++;
            }
            // so aceita se TODA a linha era o numero 
            if (pos == linha.size() && !linha.empty()) {
                return valor;
            }
        } catch (const std::exception&) {
            // stoi lanca se a linha nao comeca com numero (ex: "arroz")
        }
        std::cout << "Entrada invalida. Digite um numero inteiro.\n";
        std::cout << "\a" << std::flush;
    }
}

int Menu::lerOpcao() const {
    // usa o leitor robusto: entrada invalida no menu nao trava mais o programa
    return lerInteiro("");
}

double Menu::lerDouble(const std::string& prompt) const {
    std::string linha;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, linha);

        // aceita virgula como separador decimal e troca por ponto
        for (char& c : linha) {
            if (c == ',') c = '.';
        }

        try {
            size_t pos;
            double valor = std::stod(linha, &pos);
            // pula espacos depois do numero
            while (pos < linha.size() &&
                   std::isspace(static_cast<unsigned char>(linha[pos]))) {
                pos++;
            }
            // so aceita se TODA a linha era o numero 
            if (pos == linha.size() && !linha.empty()) {
                return valor;
            }
        } catch (const std::exception&) {
            // stod lanca se a linha nao comeca com numero
        }
        std::cout << "Entrada invalida. Digite um numero.\n";
        std::cout << "\a" << std::flush;
    }
}

bool Menu::exigirLogin(Sistema& s) const {
    if (!s.getUsuarioAtivo()) {
        std::cout << "\nFaca login primeiro.\n";
        std::cout << "\a" << std::flush;
        return false;     
    }
    return true;         
}






void Menu::loginUI(Sistema& s) const {
    std::string email, senha;
    std::cout << "Email: "; std::getline(std::cin, email);
    std::cout << "Senha: "; std::getline(std::cin, senha);
    if (s.login(email, senha)) {
        std::cout << "\nLogado!\n";
    } else {
        std::cout << "\nEmail ou senha invalidos.\n";
        std::cout << "\a" << std::flush;
    }
}

void Menu::cadastrarUsuarioUI(Sistema& s) const {
    try {
        std::string nome, email, senha;
        int Acesso;

        std::cout << "Nome: ";
        std::getline(std::cin, nome);
        std::cout << "Email: ";
        std::getline(std::cin, email);
        std::cout << "Senha: ";
        std::getline(std::cin, senha);

        Acesso = lerInteiro("Escolha seu o tipo do seu perfil \n1 - Cozinheiro \n2 - Chef de cozinha \n3 - Administrador: ");

        if (Acesso < 1 || Acesso > 3) {
            throw std::invalid_argument("Tipo de acesso invalido");
        }

        nivelAcesso nAcesso = (Acesso == 1) ? nivelAcesso::Cozinheiro
                            : (Acesso == 2) ? nivelAcesso::Chef
                            : nivelAcesso::Admin;

        if (s.cadastrarUsuario(nome, email, senha, nAcesso))
            std::cout << "Usuario cadastrado!\n";
        else {
            std::cout << "Falha no cadastro.\n";
            std::cout << "\a" << std::flush;
        }
    } catch (const std::invalid_argument& e) {
        std::cout << "Falha no cadastro: " << e.what() << std::endl;
        std::cout << "\a" << std::flush;
    }
    
}

void Menu::logoutUI(Sistema& s) const {
    s.logout();
    std::cout << "\nLogout feito.\n";
}






void Menu::cadastrarReceitaUI(Sistema& s) const {
    if (!exigirLogin(s)) return; 

    std::string titulo, instrucoes;
    int tempo, dif, cat, n, rend;

    std::cout << "Titulo: ";
    std::getline(std::cin, titulo);

    tempo = lerInteiro("Tempo (min): ");
    dif = lerInteiro("Dificuldade (1=Facil, 2=Medio, 3=Dificil): ");
    cat = lerInteiro("Categoria (1=Doce, 2=Salgado, 3=Vegano, 4=Vegetariano, 5=Outro): ");
    rend = lerInteiro("Rendimento (Numero de pessoas que a receita serve.): ");

    std::cout << "Instrucoes: ";
    std::getline(std::cin, instrucoes);

    Dificuldade d = (dif == 2) ? Dificuldade::Medio : (dif == 3) ? Dificuldade::Dificil : Dificuldade::Facil;
    Categoria c = (cat == 2) ? Categoria::Salgado : (cat == 3) ? Categoria::Vegano :
                  (cat == 4) ? Categoria::Vegetariano : (cat == 5) ? Categoria::Outro : Categoria::Doce;

    try{
        Receita* r = s.cadastrarReceita(titulo, tempo, d, c, rend);
        r->definirInstrucoes(instrucoes);

        n = lerInteiro("Quantos ingredientes? ");
        for (int i = 0; i < n; ++i) {
            std::string nome, unidade, tipo;
            double quant;
            std::cout << "-- Ingrediente " << (i+1) << " --\n";
            std::cout << "Nome: ";
            std::getline(std::cin, nome);

            quant = lerDouble("Quantidade: ");

            std::cout << "Unidade: ";
            std::getline(std::cin, unidade);

            std::cout << "Tipo: ";
            std::getline(std::cin, tipo);

            r->adicionarIngrediente(Ingrediente(nome, quant, unidade, tipo));
        }
        std::cout << "Receita cadastrada!\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "Erro ao cadastrar receita: " << e.what() << "\n";
        std::cout << "\a" << std::flush;
    }
}

void Menu::cadastrarReceitaTemplateUI(Sistema& s) const {
    if (!exigirLogin(s)) return;
    std::string titulo, instrucoes, chave_template;
    int tempo, dif, cat, rend;

    std::cout << "Titulo: ";
    std::getline(std::cin, titulo);

    tempo = lerInteiro("Tempo (min): ");
    dif = lerInteiro("Dificuldade (1=Facil, 2=Medio, 3=Dificil): ");
    cat = lerInteiro("Categoria (1=Doce, 2=Salgado, 3=Vegano, 4=Vegetariano, 5=Outro): ");
    rend = lerInteiro("Rendimento (Numero de pessoas que a receita serve.): ");

    std::cout << "Instrucoes: ";
    std::getline(std::cin, instrucoes);

    Dificuldade d = (dif == 2) ? Dificuldade::Medio : (dif == 3) ? Dificuldade::Dificil : Dificuldade::Facil;
    Categoria c = (cat == 2) ? Categoria::Salgado : (cat == 3) ? Categoria::Vegano :
                  (cat == 4) ? Categoria::Vegetariano : (cat == 5) ? Categoria::Outro : Categoria::Doce;

    try{
        Receita* r = s.cadastrarReceita(titulo, tempo, d, c, rend);
        r->definirInstrucoes(instrucoes);

        std::cout << "Insira o nome do template a ser usado: ";
        std::getline(std::cin, chave_template);

        // Pega os ingredientes do template (lanca out_of_range se o template nao existir)
        std::vector<Ingrediente> ingsTemplate = s.getIngredientesTemplate(chave_template);
        int rendTemplate = s.getRendimentoTemplate(chave_template);

        // Ajusta as quantidades do template para o rendimento desejado da receita.
        // apropriarRendimento RETORNA o vetor ajustado (nao altera no lugar), por isso
        // usamos o retorno em atribuirIngredientes.
        r->atribuirIngredientes(
            r->apropriarRendimento(ingsTemplate, r->getRendimento(), rendTemplate)
        );

        std::cout << "Receita cadastrada via template!\n";

    } catch (const std::invalid_argument& e) {
        std::cout << "Erro ao cadastrar receita: " << e.what() << "\n";
        std::cout << "\a" << std::flush;
    } catch (const std::out_of_range&) {
        std::cout << "Template nao encontrado.\n";
        std::cout << "\a" << std::flush;
    }
}

void Menu::cadastrarTemplateUI(Sistema& s) const {
    if (!exigirLogin(s)) return;
    if (!s.getUsuarioAtivo()->podeCadastrarTemplate()){
        std::cout << "Você não tem o nivel de acesso para cadastrar templates!\n";
        std::cout << "\a" << std::flush;
        return;
        //como passamos a usar subclasses, podemos pegar o bool do metodo especifico de cada uma
        //ou seja, Chef::podeCadastrarTemplate é true e Cozinheiro é false.
    }
    std::string titulo, descricao;
    int rend, n;

    std::cout << "Titulo: ";
    std::getline(std::cin, titulo);

    std::cout << "Descricao: ";
    std::getline(std::cin, descricao);

    rend = lerInteiro("Rendimento (Numero de pessoas que a receita serve.): ");

    try{
        TemplateReceita* t = s.cadastrarTemplate(titulo, descricao, rend);

        n = lerInteiro("Quantos ingredientes? ");
        for (int i = 0; i < n; ++i) {
            std::string nome, unidade, tipo;
            double quant;
            std::cout << "-- Ingrediente " << (i+1) << " --\n";
            std::cout << "Nome: ";
            std::getline(std::cin, nome);

            quant = lerDouble("Quantidade: ");

            std::cout << "Unidade: ";
            std::getline(std::cin, unidade);

            std::cout << "Tipo: ";
            std::getline(std::cin, tipo);

            t->adicionarIngrediente(Ingrediente(nome, quant, unidade, tipo));
        }
    } catch (const std::invalid_argument& e) {
        std::cout << "Erro ao cadastrar template: " << e.what() << "\n";
        std::cout << "\a" << std::flush;
    }
}

void Menu::listarReceitasUI(Sistema& s) const {
    const auto& receitas = s.getReceitas();
    if (receitas.empty()) {
        std::cout << "Nenhuma receita.\n";
    } else {
        std::cout << "\n=== Receitas ===\n";
        int i = 1;
        for (const auto& r : receitas) {
            std::cout << i++ << ". " << r.getTitulo()
                      << " (" << r.getTempoPreparo() << " min, nota "
                      << r.calcularMediaNotas() << ")\n";
        }
    }
}

void Menu::listarTemplatesUI(Sistema& s) const {
    const auto& templates = s.getTemplates();
    if (templates.empty()) {
        std::cout << "Nenhum template.\n";
    } else {
        std::cout << "\n=== Templates ===\n";
        int i = 1;
        for (const auto& [chave, temp] : templates) { //aqui eu estou assumindo que a chave == nome do template, podemos alterar caso necessario
            std::cout << i++ << ". " << chave
                      << ", serve (" << temp.getRendimentoT() << ") pessoas.\n"
                      << "Descricao: " << temp.getDescricao() << "\n";
        }
    }
}

void Menu::buscarReceitaUI(Sistema& s) const {
    std::string titulo;
    std::cout << "Titulo a buscar: ";
    std::getline(std::cin, titulo);

    auto res = s.buscarPorTitulo(titulo);

    std::cout << res.size() << " receita(s) encontrada(s):\n";
    for (auto* r : res)
        std::cout << "- " << r->getTitulo() << "\n";
}

void Menu::filtrarDificuldadeUI(Sistema& s) const {
    int dif;
    dif = lerInteiro("Dificuldade (1=Facil, 2=Medio, 3=Dificil): ");

    Dificuldade d = (dif == 2) ? Dificuldade::Medio : (dif == 3) ? Dificuldade::Dificil : Dificuldade::Facil;

    auto res = s.filtrarPorDificuldade(d);

    std::cout << res.size() << " receita(s):\n";
    for (auto* r : res)
        std::cout << "- " << r->getTitulo() << "\n";
}

void Menu::avaliarReceitaUI(Sistema& s) const {
    if (!exigirLogin(s)) return;
    std::string titulo, comentario;
    int nota;
    std::cout << "Titulo da receita: ";
    std::getline(std::cin, titulo);

    nota = lerInteiro("Nota (1-5): ");

    std::cout << "Comentario: ";
    std::getline(std::cin, comentario);

    if (s.avaliar(titulo, nota, comentario))
        std::cout << "Avaliacao adicionada!\n";
    else{
        std::cout << "Receita nao encontrada.\n";
        std::cout << "\a" << std::flush;
    }
}

void Menu::favoritarReceitaUI(Sistema& s) const {
    if (!exigirLogin(s)) return;
    std::string titulo;
    std::cout << "Titulo: ";
    std::getline(std::cin, titulo);

    auto res = s.buscarPorTitulo(titulo);

    if (res.empty()) {
        std::cout << "Receita nao encontrada.\n";
        std::cout << "\a" << std::flush;
    } else {
        s.getUsuarioAtivo()->adicionarFavorita(res[0]);
        std::cout << "Favoritada!\n";
    }
}

void Menu::verFavoritasUI(Sistema& s) const {
    if (!exigirLogin(s)) return;
    const auto& favs = s.getUsuarioAtivo()->getFavoritas();
    if (favs.empty()) {
        std::cout << "Nenhuma favorita.\n";
    } else {
        std::cout << "Favoritas:\n";
        for (auto* r : favs)
            std::cout << "- " << r->getTitulo() << "\n";
    }
}

void Menu::salvarDadosUI(Sistema& s) const {
    s.salvar();
    std::cout << "Salvo em data/\n";
}
void Menu::adicionarIngredientesUI(Sistema& s) const {
    if (!exigirLogin(s)) return;

    int n;
    n = lerInteiro("Quantos ingredientes? ");

    for (int i = 0; i < n; ++i) {
        std::string nome, unidade, tipo;
        double quant;
        std::cout << "-- Ingrediente " << (i+1) << " --\n";
        std::cout << "Nome: ";
        std::getline(std::cin, nome);

        // valida quantidade: precisa ser estritamente positiva.
        // Repete a leitura ate o usuario digitar um valor > 0.
        quant = lerDouble("Quantidade: ");
        while (quant <= 0) {
            std::cout << "Quantidade deve ser maior que zero.\n";
            std::cout << "\a" << std::flush;
            quant = lerDouble("Quantidade: ");
        }

        std::cout << "Unidade: ";
        std::getline(std::cin, unidade);

        std::cout << "Tipo: ";
        std::getline(std::cin, tipo);

        // adicionarIngredienteDisponivel retorna false se ja houver um
        // ingrediente com esse nome na despensa (case-insensitive).
        bool ok = s.getUsuarioAtivo()->adicionarIngredienteDisponivel(
            Ingrediente(nome, quant, unidade, tipo));
        if (!ok) {
            std::cout << "Voce ja tem \"" << nome
                      << "\" na despensa; ingrediente ignorado.\n";
            std::cout << "\a" << std::flush;
        }
    }
}

void Menu::sugerirReceitasUI(Sistema& s) const {
    if (!exigirLogin(s)) return;

    try{
        std::vector<Receita*> sugestoes = s.sugerirReceitas();
        int i = 1;

        if(sugestoes.empty()){
            std::cout << "Nenhuma receita pode ser feita com os ingredientes disponiveis do usuário.\n";
        }

        std::cout << "\n=== Receitas Disponiveis ===\n";
        for (const auto& r : sugestoes) {
            std::cout << i++ << ". " << r->getTitulo()
                      << " (" << r->getTempoPreparo() << " min, nota "
                      << r->calcularMediaNotas() << ")\n";
        }
    } catch (const std::invalid_argument& e){
        std::cout << "Erro ao procurar receitas: " << e.what() << std::endl;
    }
}
void Menu::verReceitaCompletaUI(Sistema& s) const {
    std::string titulo;
    std::cout << "Titulo da receita: ";
    std::getline(std::cin, titulo);

    auto res = s.buscarPorTitulo(titulo);
    if (res.empty()) {
        std::cout << "Receita nao encontrada.\n";
        std::cout << "\a" << std::flush;
    } else {
        Receita* r = res[0];

        auto difStr = [](Dificuldade d) -> std::string {
            switch(d) {
                case Dificuldade::Facil:   return "Facil";
                case Dificuldade::Medio:   return "Medio";
                case Dificuldade::Dificil: return "Dificil";
            }
            return "?";
        };
        auto catStr = [](Categoria c) -> std::string {
            switch(c) {
                case Categoria::Doce:        return "Doce";
                case Categoria::Salgado:     return "Salgado";
                case Categoria::Vegano:      return "Vegano";
                case Categoria::Vegetariano: return "Vegetariano";
                case Categoria::Outro:       return "Outro";
            }
            return "?";
        };

        std::cout << "\n============================================\n";
        std::cout << "  " << r->getTitulo() << "\n";
        std::cout << "============================================\n";
        std::cout << "Categoria   : " << catStr(r->getCategoria()) << "\n";
        std::cout << "Dificuldade : " << difStr(r->getDificuldade()) << "\n";
        std::cout << "Tempo       : " << r->getTempoPreparo() << " min\n";
        std::cout << "Rendimento  : " << r->getRendimento() << " pessoa(s)\n";

        double media = r->calcularMediaNotas();
        int numAval  = static_cast<int>(r->getAvaliacoes().size());
        if (numAval == 0) {
            std::cout << "Nota        : Sem avaliacoes\n";
        } else {
            std::cout << "Nota        : " << std::fixed << std::setprecision(1)
                      << media << "/5 (" << numAval << " avaliacao(oes))\n";
        }

        std::cout << "\n--- Ingredientes ---\n";
        const auto& ings = r->getIngredientes();
        if (ings.empty()) {
            std::cout << "  (nenhum ingrediente cadastrado)\n";
        } else {
            for (const auto& ing : ings) {
                std::cout << "  - " << ing.getNome()
                          << ": " << ing.getQuantidade()
                          << " " << ing.getUnidade();
                if (!ing.getTipo().empty())
                    std::cout << " [" << ing.getTipo() << "]";
                std::cout << "\n";
            }
        }

        std::cout << "\n--- Instrucoes ---\n";
        const std::string& instr = r->getInstrucoes();
        if (instr.empty()) {
            std::cout << "  (sem instrucoes cadastradas)\n";
        } else {
            std::cout << "  " << instr << "\n";
        }

        std::cout << "\n--- Avaliacoes ---\n";
        const auto& avals = r->getAvaliacoes();
        if (avals.empty()) {
            std::cout << "  (nenhuma avaliacao)\n";
        } else {
            for (const auto& av : avals) {
                std::cout << "  [" << av.getNota() << "/5] "
                          << av.getAutor()->getNome() << ": "
                          << av.getComentario() << "\n";
            }
        }
        std::cout << "============================================\n";
    }
}
// Opcao exclusiva de Admin: remove um usuario do sistema pelo email.
// A checagem de privilegio acontece em duas camadas: aqui (feedback amigavel)
// e dentro de Sistema::removerUsuario (regra de negocio que protege o dominio).
void Menu::removerUsuarioUI(Sistema& s) const {
    if (!exigirLogin(s)) return;

    if (s.getAcessoUsuarioAtivo() != nivelAcesso::Admin) {
        std::cout << "Apenas administradores podem remover usuarios.\n";
        std::cout << "\a" << std::flush;
        return;
    }

    std::cout << "Email do usuario a remover: ";
    std::string email;
    std::getline(std::cin, email);

    if (s.removerUsuario(email)) {
        std::cout << "Usuario removido. A alteracao sera gravada ao salvar.\n";
    } else {
        std::cout << "Nao foi possivel remover (email inexistente ou "
                     "voce tentou remover a si mesmo).\n";
        std::cout << "\a" << std::flush;
    }
}
