/**
 * @file main.cpp
 * @brief Interface CLI do Ricettario.
 */
#include <iostream>
#include <string>
#include "Sistema.hpp"


//main serve como um loop de opções. podemos incrementar ela na proxima semana
int main() {
    Sistema s;
    s.carregar();

    std::cout << "=== Ricettario - Grupo 12 ===\n";

    int opcao = -1;
    while (opcao != 0) {
        std::cout << "\n--- Menu ---\n";
        if (s.getUsuarioAtivo())
            std::cout << "(LOGADO: " << s.getUsuarioAtivo()->getNome() << ")\n";
        else
            std::cout << "(NAO LOGADO)\n";

        std::cout << "1. Cadastrar usuario\n";
        std::cout << "2. Login\n";
        std::cout << "3. Logout\n";
        std::cout << "4. Cadastrar receita\n";
        std::cout << "5. Listar receitas\n";
        std::cout << "6. Buscar receita\n";
        std::cout << "7. Filtrar por dificuldade\n";
        std::cout << "8. Avaliar receita\n";
        std::cout << "9. Favoritar receita\n";
        std::cout << "10. Ver favoritas\n";
        std::cout << "11. Salvar dados\n";
        std::cout << "0. Sair\n";
        std::cout << "Opcao: ";
        std::cin >> opcao;
        std::cin.ignore();

        if (opcao == 1) {
            std::string nome, email, senha;

            std::cout << "Nome: ";  
            std::getline(std::cin, nome);

            std::cout << "Email: "; 
            std::getline(std::cin, email);

            std::cout << "Senha: "; 
            std::getline(std::cin, senha);

            if (s.cadastrarUsuario(nome, email, senha))
                std::cout << "Usuario cadastrado!\n";
            else
                std::cout << "Falha no cadastro.\n";
        }
        else if (opcao == 2) {
            std::string email, senha;
            std::cout << "Email: "; std::getline(std::cin, email);
            std::cout << "Senha: "; std::getline(std::cin, senha);
            if (s.login(email, senha))
                std::cout << "Logado!\n";
            else
                std::cout << "Email ou senha invalidos.\n";
        }
        else if (opcao == 3) {
            s.logout();
            std::cout << "Logout feito.\n";
        }
        else if (opcao == 4) {
            if (!s.getUsuarioAtivo()) {
                std::cout << "Faca login primeiro.\n";
                continue;
            }
            std::string titulo, instrucoes;
            int tempo, dif, cat, n;

            std::cout << "Titulo: ";          
            std::getline(std::cin, titulo);

            std::cout << "Tempo (min): ";     
            std::cin >> tempo;

            std::cout << "Dificuldade (1=Facil, 2=Medio, 3=Dificil): "; 
            std::cin >> dif;

            std::cout << "Categoria (1=Doce, 2=Salgado, 3=Vegano, 4=Vegetariano, 5=Outro): "; 
            std::cin >> cat;

            std::cin.ignore();
            std::cout << "Instrucoes: ";      
            std::getline(std::cin, instrucoes);

            Dificuldade d = (dif == 2) ? Dificuldade::Medio : (dif == 3) ? Dificuldade::Dificil : Dificuldade::Facil;
            Categoria c = (cat == 2) ? Categoria::Salgado : (cat == 3) ? Categoria::Vegano :
                          (cat == 4) ? Categoria::Vegetariano : (cat == 5) ? Categoria::Outro : Categoria::Doce;

            
            try{
                Receita* r = s.cadastrarReceita(titulo, tempo, d, c);
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
            }
        }
        else if (opcao == 5) {
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
        else if (opcao == 6) {
            std::string titulo;
            std::cout << "Titulo a buscar: "; 
            std::getline(std::cin, titulo);

            auto res = s.buscarPorTitulo(titulo);
            
            std::cout << res.size() << " receita(s) encontrada(s):\n";
            for (auto* r : res)
                std::cout << "- " << r->getTitulo() << "\n";
        }
        else if (opcao == 7) {
            int dif;
            std::cout << "Dificuldade (1=Facil, 2=Medio, 3=Dificil): "; 
            std::cin >> dif;

            Dificuldade d = (dif == 2) ? Dificuldade::Medio : (dif == 3) ? Dificuldade::Dificil : Dificuldade::Facil;

            auto res = s.filtrarPorDificuldade(d);

            std::cout << res.size() << " receita(s):\n";
            for (auto* r : res)
                std::cout << "- " << r->getTitulo() << "\n";
        }
        else if (opcao == 8) {
            if (!s.getUsuarioAtivo()) {
                std::cout << "Faca login primeiro.\n";
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
            else
                std::cout << "Receita nao encontrada.\n";
        }
        else if (opcao == 9) {
            if (!s.getUsuarioAtivo()) {
                std::cout << "Faca login primeiro.\n";
                continue;
            }
            std::string titulo;
            std::cout << "Titulo: "; 
            std::getline(std::cin, titulo);

            auto res = s.buscarPorTitulo(titulo);

            if (res.empty()) {
                std::cout << "Receita nao encontrada.\n";
            } else {
                s.getUsuarioAtivo()->adicionarFavorita(res[0]);
                std::cout << "Favoritada!\n";
            }
        }
        else if (opcao == 10) {
            if (!s.getUsuarioAtivo()) {
                std::cout << "Faca login primeiro.\n";
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
        else if (opcao == 11) {
            s.salvar();
            std::cout << "Salvo em data/\n";
        }
        else if (opcao == 0) {
            s.salvar();
            std::cout << "Salvo. Ate logo!\n";
        }
        else {
            std::cout << "Opcao invalida.\n";
        }
    }

    return 0;
}