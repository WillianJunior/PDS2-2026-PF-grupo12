#ifndef CHEF_HPP
#define CHEF_HPP


#include "Usuario.hpp"

class Chef : public Usuario {
public:
    Chef(const std::string& nome,
         const std::string& email,
         const std::string& senha);



    bool podeCadastrarTemplate() const override;
    nivelAcesso getAcesso() const override;

    };
    
    
    #endif
