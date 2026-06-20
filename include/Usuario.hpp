#ifndef USUARIO_HPP
#define USUARIO_HPP
#include <vector>
#include <string>

class Receita; 
enum class nivelAcesso {Chef, Cozinheiro};

/**
 * @class Usuario
 * @brief Representa um usuário cadastrado, com receitas próprias e favoritas.
 */
class Usuario {
private:
    std::string _nome;
    std::string _email;
    std::string _senha;
    std::vector<Receita*> _receitasProprias;
    std::vector<Receita*> _favoritas;
    nivelAcesso _nivelAcesso;

public:
    Usuario(const std::string& nome,
            const std::string& email,
            const std::string& senha,
            const nivelAcesso& nAcesso);

    const std::string& getNome() const;
    const std::string& getEmail() const;
    const std::string& getSenha() const;
    const nivelAcesso& getAcesso() const;


    bool autenticar(const std::string& senha) const;
    bool alterarSenha(const std::string& senhaAtual,
                      const std::string& novaSenha);

    void adicionarReceitaPropria(Receita* r);
    void removerReceitaPropria(Receita* r);
    const std::vector<Receita*>& getReceitasProprias() const;

    void adicionarFavorita(Receita* r);
    bool removerFavorita(Receita* r);
    bool ehFavorita(Receita* r) const;
    const std::vector<Receita*>& getFavoritas() const;
};

#endif
