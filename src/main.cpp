/**
 * @file main.cpp
 * @brief Interface CLI do Ricettario.
 */
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include "Sistema.hpp"

//adicionando 3 novas funcoes para fazer um menu bonito.

//cabeçalho do novo menu
void imprimirCabecalho() {
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


//efeito de loading
void telaCarregamento(){
    std::cout << "\nCarregando";
    for (int i = 0; i < 3; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        std::cout << "." << std::flush;   
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    std::cout << "\n\n";
}
//limpar terminal na hora de selecionar opção
void limparTela() {
    system("clear");
}

//main serve como um loop de opções. podemos incrementar ela na proxima semana
int main() {
    Sistema s;
    s.carregar();

    imprimirCabecalho();
    telaCarregamento();
    

    int opcao = -1;
    while (opcao != 0) {
        telaCarregamento();
        limparTela();
//                                   
                                  
        std::cout << "██▄  ▄██ ██████ ███  ██ ██  ██\n";
        std::cout << "██ ▀▀ ██ ██▄▄   ██ ▀▄██ ██  ██\n";
        std::cout << "██    ██ ██▄▄▄▄ ██   ██ ▀████▀\n";
        //                                   
        // std::cout << "\n--- MENU ---\n";
        
        if (s.getUsuarioAtivo())
            std::cout << "\n(LOGADO: " << s.getUsuarioAtivo()->getNome() << ")\n\n";
        else
            std::cout << "\n(NAO LOGADO)\n\n";

        std::cout << "1. Cadastrar usuario\n";
        std::cout << "2. Login\n";
        std::cout << "3. Logout\n";
        std::cout << "4. Cadastrar receita\n";
        std::cout << "5. Cadastrar receita via template\n";
        std::cout << "6. Cadastrar Template\n";
        std::cout << "7. Listar receitas\n";
        std::cout << "8. Listar templates\n";
        std::cout << "9. Buscar receita\n";
        std::cout << "10. Filtrar por dificuldade\n";
        std::cout << "11. Avaliar receita\n";
        std::cout << "12. Favoritar receita\n";
        std::cout << "13. Ver favoritas\n";
        std::cout << "14. Salvar dados\n";
        std::cout << "15. Ver receita completa\n";
        std::cout << "0. Sair\n";
        std::cout << "\nOpcao: ";
        std::cin >> opcao;
        std::cin.ignore();

        limparTela();

        if (opcao == 1) {
            try{
                std::string nome, email, senha;
                int Acesso;

                std::cout << "Nome: ";  
                std::getline(std::cin, nome);

                std::cout << "Email: "; 
                std::getline(std::cin, email);

                std::cout << "Senha: "; 
                std::getline(std::cin, senha);

                std::cout << "Escolha seu o tipo do seu perfil \n1 - Cozinheiro \n2 - Chef de cozinha: \n3 - Administrador: ";
                std::cin >> Acesso;
                std::cin.ignore();

                if(Acesso < 1 || Acesso > 3){
                    throw(std::invalid_argument("Tipo de acesso inválido"));
                    std::cout << "\a" << std::flush;
                    
                }

                nivelAcesso nAcesso;
                if (Acesso == 1) nAcesso = nivelAcesso::Cozinheiro; 
                else if (Acesso == 2) nAcesso = nivelAcesso::Chef; 
                else nAcesso = nivelAcesso::Admin; 

                if (s.cadastrarUsuario(nome, email, senha, nAcesso))
                    std::cout << "Usuario cadastrado!\n";
                else{
                    std::cout << "Falha no cadastro.\n";
                    std::cout << "\a" << std::flush;

                }
            } catch(const std::invalid_argument& e){
                
                std::cout << "Falha no cadastro: " << e.what() << std::endl;
                std::cout << "\a" << std::flush;
                
            }
        }
        else if (opcao == 2) {
            std::string email, senha;
            std::cout << "Email: "; std::getline(std::cin, email);
            std::cout << "Senha: "; std::getline(std::cin, senha);
            if (s.login(email, senha)){
                std::cout << "\nLogado!\n";
            }
                
            else {
                std::cout << "\nEmail ou senha invalidos.\n";
                std::cout << "\a" << std::flush;
            }    
        }
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
                    double media = r.calcularMediaNotas();
                    int numAval = static_cast<int>(r.getAvaliacoes().size());
                    std::cout << i++ << ". " << r.getTitulo()
                              << " | " << r.getTempoPreparo() << " min";
                    if (numAval == 0) {
                        std::cout << " | Sem avaliacoes";
                    } else {
                        std::cout << " | Nota: " << std::fixed << std::setprecision(1)
                                  << media << "/5 (" << numAval << "avaliacoes)";
                    }
                    std::cout <<"\n";
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

            // Lista receitas disponíveis
            const auto& receitas = s.getReceitas();
            if (receitas.empty()) {
                std::cout << "Nenhuma receita cadastrada.\n";
                continue;
            }
            std::cout << "\n=== Receitas disponíveis ===\n";
            for (const auto& r : receitas) {
                std::cout << "  - " << r.getTitulo() << "\n";
            }
            std::cout << "\n";

            std::string titulo, comentario;
            int nota;
            std::cout << "Titulo da receita: "; 
            std::getline(std::cin, titulo);

            std::cout << "Nota (1-5): "; 
            std::cin >> nota;

            std::cin.ignore();
            std::cout << "Comentario: "; 
            std::getline(std::cin, comentario);

            if (s.avaliar(titulo, nota, comentario)){
                std::cout << "Avaliacao adicionada!\n";
                // exibe nova media atualizada
                auto res = s.buscarPorTitulo(titulo);
                if (!res.empty()) {
                    double novaMedia = res[0]->calcularMediaNotas();
                    int total = static_cast<int>(res[0]->getAvaliacoes().size());
                    std::cout << "Nova nota media de \"" << res[0]->getTitulo()
                              << "\": " << std::fixed << std::setprecision(1)
                              << novaMedia << "/5 (" << total << " avaliacoes)\n";
                }
            } else{
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
        else if (opcao == 15) {
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