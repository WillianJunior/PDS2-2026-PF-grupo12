#ifndef MENU_HPP
#define MENU_HPP
#include <string>

class Sistema; 

class Menu{

public:
    void exibirCabecalho() const;                              
    void exibirOpcoes(bool logado, const std::string& nome) const;  
    int  lerOpcao() const;                                     
    void limparTela() const;                                   
    void telaCarregamento() const;    

    bool exigirLogin(Sistema& s) const;

    void cadastrarUsuarioUI(Sistema& s) const;
    void loginUI(Sistema& s) const;
    void logoutUI(Sistema& s) const;
    void cadastrarReceitaUI(Sistema& s) const;
    void cadastrarReceitaTemplateUI(Sistema& s) const;
    void cadastrarTemplateUI(Sistema& s) const;
    void listarReceitasUI(Sistema& s) const;
    void listarTemplatesUI(Sistema& s) const;
    void buscarReceitaUI(Sistema& s) const;
    void filtrarDificuldadeUI(Sistema& s) const;
    void avaliarReceitaUI(Sistema& s) const;
    void favoritarReceitaUI(Sistema& s) const;
    void verFavoritasUI(Sistema& s) const;
    void salvarDadosUI(Sistema& s) const;
    void verReceitaCompletaUI(Sistema& s) const;
    void adicionarIngredientesUI(Sistema& s) const;
    void sugerirReceitasUI(Sistema& s) const;
    


};

#endif