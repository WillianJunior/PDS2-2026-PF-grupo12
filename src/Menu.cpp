#include "../include/Menu.hpp"
#include "../include/Sistema.hpp"
#include "../include/Ingrediente.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>

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

void Menu::exibirOpcoes(bool logado, const std::string& nome) const {
    
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
    std::cout << "\n  INGREDIENTES\n";
    std::cout << "  15. Adicionar meus ingredientes   16. Sugerir receitas\n";
    std::cout << "\n  14. Salvar dados     0. Sair\n";
    std::cout << "\nOpcao: ";
}

int Menu::lerOpcao() const {
    int opcao;
    std::cin >> opcao;
    std::cin.ignore();
    return opcao;
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

        std::cout << "Escolha seu o tipo do seu perfil \n1 - Cozinheiro \n2 - Chef de cozinha \n3 - Administrador: ";
        std::cin >> Acesso;
        std::cin.ignore();

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

    std::cout << "Tempo (min): ";
    std::cin >> tempo;

    std::cout << "Dificuldade (1=Facil, 2=Medio, 3=Dificil): ";
    std::cin >> dif;

    std::cout << "Categoria (1=Doce, 2=Salgado, 3=Vegano, 4=Vegetariano, 5=Outro): ";
    std::cin >> cat;

    std::cout << "Rendimento (Numero de pessoas que a receita serve.)";
    std::cin >> rend;

    std::cin.ignore();
    std::cout << "Instrucoes: ";
    std::getline(std::cin, instrucoes);

    Dificuldade d = (dif == 2) ? Dificuldade::Medio : (dif == 3) ? Dificuldade::Dificil : Dificuldade::Facil;
    Categoria c = (cat == 2) ? Categoria::Salgado : (cat == 3) ? Categoria::Vegano :
                  (cat == 4) ? Categoria::Vegetariano : (cat == 5) ? Categoria::Outro : Categoria::Doce;

    try{
        Receita* r = s.cadastrarReceita(titulo, tempo, d, c, rend);
        r->definirInstrucoes(instrucoes);

        std::cout << "Quantos ingredientes? "; std::cin >> n;
        std::cin.ignore();
        for (int i = 0; i < n; ++i) {
            std::string nome, unidade, tipo;
            int quant;
            std::cout << "-- Ingrediente " << (i+1) << " --\n";
            std::cout << "Nome: ";
            std::getline(std::cin, nome);

            std::cout << "Quantidade: ";
            std::cin >> quant;

            std::cin.ignore();
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

    std::cout << "Tempo (min): ";
    std::cin >> tempo;

    std::cout << "Dificuldade (1=Facil, 2=Medio, 3=Dificil): ";
    std::cin >> dif;

    std::cout << "Categoria (1=Doce, 2=Salgado, 3=Vegano, 4=Vegetariano, 5=Outro): ";
    std::cin >> cat;

    std::cout << "Rendimento (Numero de pessoas que a receita serve.): ";
    std::cin >> rend;

    std::cin.ignore();
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

    std::cout << "Rendimento (Numero de pessoas que a receita serve.): ";
    std::cin >> rend;

    std::cin.ignore();

    try{
        TemplateReceita* t = s.cadastrarTemplate(titulo, descricao, rend);

        std::cout << "Quantos ingredientes? "; std::cin >> n;
        std::cin.ignore();
        for (int i = 0; i < n; ++i) {
            std::string nome, unidade, tipo;
            int quant;
            std::cout << "-- Ingrediente " << (i+1) << " --\n";
            std::cout << "Nome: ";
            std::getline(std::cin, nome);

            std::cout << "Quantidade: ";
            std::cin >> quant;

            std::cin.ignore();
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
    std::cout << "Dificuldade (1=Facil, 2=Medio, 3=Dificil): ";
    std::cin >> dif;

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

    std::cout << "Nota (1-5): ";
    std::cin >> nota;

    std::cin.ignore();
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
    std::cout << "Quantos ingredientes? "; std::cin >> n;
    std::cin.ignore();

    for (int i = 0; i < n; ++i) {
        std::string nome, unidade, tipo;
        int quant;
        std::cout << "-- Ingrediente " << (i+1) << " --\n";
        std::cout << "Nome: ";
        std::getline(std::cin, nome);

        std::cout << "Quantidade: ";
        std::cin >> quant;

        std::cin.ignore();
        std::cout << "Unidade: ";
        std::getline(std::cin, unidade);

        std::cout << "Tipo: ";
        std::getline(std::cin, tipo);

        s.getUsuarioAtivo()->adicionarIngredienteDisponivel(Ingrediente(nome, quant, unidade, tipo));
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