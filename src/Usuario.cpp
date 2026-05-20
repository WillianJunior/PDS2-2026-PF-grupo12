#include "../include/Usuario.hpp"

Usuario::Usuario( std::string& nome, std::string& email, std::string& senha){}

std::string Usuario::getNome(){}

std::string Usuario::getEmail(){}

bool Usuario::autenticar(const std::string& senha){}

void Usuario::alterarSenha(const std::string& senhaAtual){}

void Usuario::adicionarReceitaPropria(Receita* r){}

void Usuario::removerReceitaPropria(Receita* r){}

std::vector<Receita*>& Usuario::getReceitasProprias(){}

void Usuario::adicionarFavorita(Receita* r){}

bool Usuario::removerFavorita(Receita* r){}

bool Usuario::ehFavorita(Receita* r){}

std::vector<Receita*>& Usuario::getFavoritas(){}