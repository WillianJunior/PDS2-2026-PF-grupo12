#ifndef USUARIO_HPP
#define USUARIO_HPP
#include <vector>
#include <string>

class Receita; 
enum class nivelAcesso {Chef, Cozinheiro, Admin};

/**
 * @class Usuario
 * @brief Representa um usuário cadastrado, com receitas próprias e favoritas.
 */
class Usuario {
protected:
    std::string _nome;
    std::string _email;
    std::string _senha;
    std::vector<Receita*> _receitasProprias;
    std::vector<Receita*> _favoritas;
    nivelAcesso _nivelAcesso;

    Usuario(const std::string& nome,
            const std::string& email,
            const std::string& senha);
    //o construtor so pode ser chamado quando as subclasses de niveis de acesso forem instanciadas 

public:
   
    virtual ~Usuario() = default;

    virtual bool podeCadastrarTemplate() const = 0;
    virtual nivelAcesso getAcesso() const =0 ;

    const std::string& getNome() const;
    const std::string& getEmail() const;
    const std::string& getSenha() const;
    const std::vector<Receita*>& getReceitasProprias() const;
    const std::vector<Receita*>& getFavoritas() const;
    //colocando os getters junto


    bool autenticar(const std::string& senha) const;
    bool alterarSenha(const std::string& senhaAtual,
                      const std::string& novaSenha);

    void adicionarReceitaPropria(Receita* r);
    void removerReceitaPropria(Receita* r);
    

    void adicionarFavorita(Receita* r);
    bool removerFavorita(Receita* r);
    bool ehFavorita(Receita* r) const;
    
};

#endif
