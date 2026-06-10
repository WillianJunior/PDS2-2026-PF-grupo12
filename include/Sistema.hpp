#ifndef SISTEMA_HPP
#define SISTEMA_HPP
#include <string>
#include <list>
#include <vector>
#include "Receita.hpp"
#include "Usuario.hpp"

/**
 * @class Sistema
 * @brief Fachada que gerencia usuários, receitas e a sessão ativa.
 *
 * Após o refactor: usa std::list para armazenar usuários e receitas,
 * pois list NÃO invalida ponteiros para elementos quando há inserções
 * ou remoções de outros elementos (diferente de vector). Isso conserta
 * o problema do _usuarioAtivo e dos Receita* guardados em favoritos
 * / receitas próprias do Usuario.
 */
class Sistema {
private:
    std::list<Usuario> _usuarios;
    std::list<Receita> _receitas;
    Usuario* _usuarioAtivo = nullptr;

    static std::string dificuldadeParaStr(Dificuldade d);
    static std::string categoriaParaStr(Categoria c);
    static Dificuldade strParaDificuldade(const std::string& s);
    static Categoria   strParaCategoria(const std::string& s);

public:
    Sistema();

    bool cadastrarUsuario(const std::string& nome,
                          const std::string& email,
                          const std::string& senha);

    bool login(const std::string& email, const std::string& senha);
    void logout();
    Usuario* getUsuarioAtivo() const;

    Receita* cadastrarReceita(const std::string& titulo, int tempoPreparo,
                              Dificuldade dificuldade, Categoria categoria);

    bool removerReceita(const std::string& titulo);

    std::vector<Receita*> buscarPorTitulo(const std::string& titulo);
    std::vector<Receita*> filtrarPorDificuldade(Dificuldade d);
    std::vector<Receita*> filtrarPorNotaMinima(double notaMinima);

    bool avaliar(const std::string& tituloReceita, int nota,
                 const std::string& comentario);

    std::list<Receita>& getReceitas();
    const std::list<Receita>& getReceitas() const;
    std::list<Usuario>& getUsuarios();
    const std::list<Usuario>& getUsuarios() const;

    void salvar();
    void carregar();
};

#endif
