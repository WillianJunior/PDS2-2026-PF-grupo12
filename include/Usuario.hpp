#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <vector>
#include <string>

class Receita;

/**
 * @class Usuario
 * @brief Representa um usuário cadastrado, com suas receitas próprias e favoritas.
 */
class Usuario {
private:
    std::string nome;
    std::string email;
    std::string senha;
    std::vector<Receita*> receitasProprias;
    std::vector<Receita*> favoritas;

public:
    Usuario( std::string& nome,
             std::string& email,
             std::string& senha);

    std::string getNome();
    std::string getEmail();

    bool autenticar(const std::string& senha);
    void alterarSenha(const std::string& senhaAtual);

    void adicionarReceitaPropria(Receita* r);
    void removerReceitaPropria(Receita* r);
    std::vector<Receita*>& getReceitasProprias();

    void adicionarFavorita(Receita* r);
    bool removerFavorita(Receita* r);
    bool ehFavorita(Receita* r);
    std::vector<Receita*>& getFavoritas();
};

#endif