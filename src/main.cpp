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


//main serve como um loop de opções. podemos incrementar ela na proxima semana
int main() {
    Sistema s;
    s.carregar();
    Menu menu;

    menu.exibirCabecalho();
    menu.telaCarregamento();
    

    int opcao = -1;
    
    while (opcao != 0) {
        menu.telaCarregamento();
        menu.limparTela();

        bool logado = s.getUsuarioAtivo() != nullptr;
        std::string nomeUsuario = logado ? s.getUsuarioAtivo()->getNome() : "";
        menu.exibirOpcoes(logado, nomeUsuario);

        opcao = menu.lerOpcao();

        menu.limparTela();

        //limpando o main e colocando as implementações em Menu.cpp
        if      (opcao == 1)  menu.cadastrarUsuarioUI(s);
        else if (opcao == 2)  menu.loginUI(s);
        else if (opcao == 3) {
            s.logout();
            std::cout << "\nLogout feito.\n";
        }
        else if (opcao == 4) {
            if (!s.getUsuarioAtivo()) {
                std::cout << "\nFaca login primeiro.\n";
                std::cout << "\a" << std::flush;

                continue;
            }
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
        else if (opcao == 5) {
            if (!s.getUsuarioAtivo()) {
                std::cout << "\nFaca login primeiro.\n";
                std::cout << "\a" << std::flush;
                continue;
            }
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
        else if (opcao == 6) {
            if (!s.getUsuarioAtivo()) {
                std::cout << "Faca login primeiro.\n";
                std::cout << "\a" << std::flush;
                continue;
            }
            else if(!s.getUsuarioAtivo()->podeCadastrarTemplate()){
                std::cout << "Você não tem o nivel de acesso para cadastrar templates!\n";
                std::cout << "\a" << std::flush;
                continue;
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
        else if (opcao == 7) {
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
        else if (opcao == 8) {
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
        else if (opcao == 9) {
            std::string titulo;
            std::cout << "Titulo a buscar: "; 
            std::getline(std::cin, titulo);

            auto res = s.buscarPorTitulo(titulo);
            
            std::cout << res.size() << " receita(s) encontrada(s):\n";
            for (auto* r : res)
                std::cout << "- " << r->getTitulo() << "\n";
        }
        else if (opcao == 10) {
            int dif;
            std::cout << "Dificuldade (1=Facil, 2=Medio, 3=Dificil): "; 
            std::cin >> dif;

            Dificuldade d = (dif == 2) ? Dificuldade::Medio : (dif == 3) ? Dificuldade::Dificil : Dificuldade::Facil;

            auto res = s.filtrarPorDificuldade(d);

            std::cout << res.size() << " receita(s):\n";
            for (auto* r : res)
                std::cout << "- " << r->getTitulo() << "\n";
        }
        else if (opcao == 11) {
            if (!s.getUsuarioAtivo()) {
                std::cout << "Faca login primeiro.\n";
                std::cout << "\a" << std::flush;
                continue;
            }
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
        else if (opcao == 12) {
            if (!s.getUsuarioAtivo()) {
                std::cout << "Faca login primeiro.\n";
                std::cout << "\a" << std::flush;
                continue;
            }
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
        else if (opcao == 13) {
            if (!s.getUsuarioAtivo()) {
                std::cout << "Faca login primeiro.\n";
                std::cout << "\a" << std::flush;
                continue;
            }
            const auto& favs = s.getUsuarioAtivo()->getFavoritas();
            if (favs.empty()) {
                std::cout << "Nenhuma favorita.\n";
            } else {
                std::cout << "Favoritas:\n";
                for (auto* r : favs)
                    std::cout << "- " << r->getTitulo() << "\n";
            }
        }
        else if (opcao == 14) {
            s.salvar();
            std::cout << "Salvo em data/\n";
        }
        else if (opcao == 0) {
            s.salvar();
            std::cout << "Salvo. Ate logo!\n";
        }
        else {
            std::cout << "Opcao invalida.\n";
            std::cout << "\a" << std::flush;
        }
    }

    return 0;
}