#include "../include/Sistema.hpp"

Sistema::Sistema(){}

bool Sistema::cadastrarUsuario( std::string& nome,std::string& email, std::string& senha){}

bool Sistema::login( std::string& email,  std::string& senha){}
void Sistema::logout(){}

Usuario* Sistema::getUsuarioAtivo(){}

Receita* Sistema::cadastrarReceita( std::string& titulo, int tempoPreparo, Dificuldade dificuldade, Categoria categoria){}

void Sistema::removerReceita(std::string& titulo){}

std::vector<Receita*> Sistema::buscarPorTitulo( std::string& titulo){}
std::vector<Receita*> Sistema::filtrarPorDificuldade(Dificuldade d){}
std::vector<Receita*> Sistema::filtrarPorNotaMinima(double notaMinima){}

void Sistema::avaliar( std::string& tituloReceita, int nota,  std::string& comentario){}

std::vector<Receita>& Sistema::getReceitas(){}