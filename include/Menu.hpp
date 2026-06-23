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


    void cadastrarUsuarioUI(Sistema& s) const;
    void loginUI(Sistema& s) const;

};

#endif
