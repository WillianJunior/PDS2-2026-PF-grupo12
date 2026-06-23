#include "../include/Menu.hpp"
#include "../include/Sistema.hpp"

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
    std::cout << "\n  14. Salvar dados     0. Sair\n";
    std::cout << "\nOpcao: ";
}

int Menu::lerOpcao() const {
    int opcao;
    std::cin >> opcao;
    std::cin.ignore();
    return opcao;
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

        std::cout << "Escolha seu o tipo do seu perfil \n1 - Cozinheiro \n2 - Chef de cozinha: ";
        std::cin >> Acesso;
        std::cin.ignore();

        if (Acesso < 1 || Acesso > 2) {
            throw std::invalid_argument("Tipo de acesso invalido");
        }

        nivelAcesso nAcesso = (Acesso == 1) ? nivelAcesso::Cozinheiro : nivelAcesso::Chef;

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