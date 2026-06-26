/**
 * @file main.cpp
 * @brief Interface CLI do Ricettario.
 */
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include "Sistema.hpp"
#include "Menu.hpp"


//main serve como um loop de opcoes que despacha cada acao para a classe Menu.
int main() {
    Sistema s;
    s.carregar();
    Menu menu;

    menu.exibirCabecalho();
    menu.telaCarregamento();

    int opcao = -1;

    while (opcao != 0) {
        menu.limparTela();

        bool logado = s.getUsuarioAtivo() != nullptr;
        std::string nomeUsuario = logado ? s.getUsuarioAtivo()->getNome() : "";
        bool ehAdmin = logado &&
                       s.getAcessoUsuarioAtivo() == nivelAcesso::Admin;
        menu.exibirOpcoes(logado, nomeUsuario, ehAdmin);

        opcao = menu.lerOpcao();
        menu.limparTela();

        switch (opcao) {
            case 1:  menu.cadastrarUsuarioUI(s);          break;
            case 2:  menu.loginUI(s);                     break;
            case 3:  menu.logoutUI(s);                    break;
            case 4:  menu.cadastrarReceitaUI(s);          break;
            case 5:  menu.cadastrarReceitaTemplateUI(s);  break;
            case 6:  menu.cadastrarTemplateUI(s);         break;
            case 7:  menu.listarReceitasUI(s);            break;
            case 8:  menu.listarTemplatesUI(s);           break;
            case 9:  menu.buscarReceitaUI(s);             break;
            case 10: menu.filtrarDificuldadeUI(s);        break;
            case 11: menu.avaliarReceitaUI(s);            break;
            case 12: menu.favoritarReceitaUI(s);          break;
            case 13: menu.verFavoritasUI(s);              break;
            case 14: menu.salvarDadosUI(s);               break;
            case 15: menu.verReceitaCompletaUI(s);        break;
            case 16: menu.adicionarIngredientesUI(s);     break;
            case 17: menu.sugerirReceitasUI(s);           break;
            case 18: menu.removerUsuarioUI(s);            break;
            case 0:
                s.salvar();
                std::cout << "Salvo. Ate logo!\n";
                break;
            default:
                std::cout << "Opcao invalida.\n";
                std::cout << "\a" << std::flush;
                break;
        }

        // pausa para o usuario ler a saida antes de o menu ser redesenhado
        // (nao pausa ao sair, opcao 0)
        if (opcao != 0) menu.pausar();
    }

    return 0;
}