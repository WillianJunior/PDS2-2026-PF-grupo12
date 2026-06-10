#ifndef SISTEMA_HPP
#define SISTEMA_HPP

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Receita.hpp"   
#include "Usuario.hpp"
/**
 * @class Sistema
 * @brief O Sistema gerencia usuários, receitas e a sessão ativa.
 */
class Sistema {
private:
    std::vector<Usuario> _usuarios;
    std::vector<Receita> _receitas;
    Usuario* _usuarioAtivo = nullptr;  
    
    static std::string dificuldadeParaStr(Dificuldade d);
    static std::string categoriaParaStr(Categoria c);
    static Dificuldade strParaDificuldade(const std::string& s);
    static Categoria   strParaCategoria(const std::string& s);

public:
    Sistema();

    bool cadastrarUsuario( std::string& nome,std::string& email, std::string& senha);

    bool login( std::string& email,  std::string& senha);
    void logout();

    void salvar();
    void carregar();

    Usuario* getUsuarioAtivo() ;

    Receita* cadastrarReceita( std::string& titulo, int tempoPreparo, Dificuldade dificuldade, Categoria categoria);

    void removerReceita(std::string& titulo);

    std::vector<Receita*> buscarPorTitulo( std::string& titulo);
    std::vector<Receita*> filtrarPorDificuldade(Dificuldade d);
    std::vector<Receita*> filtrarPorNotaMinima(double notaMinima);

    void avaliar( std::string& tituloReceita, int nota,  std::string& comentario);

    std::vector<Receita>& getReceitas();
};

#endif