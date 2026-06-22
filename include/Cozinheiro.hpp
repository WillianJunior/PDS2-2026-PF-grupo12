#ifndef COZINHEIRO_HPP
#define COZINHEIRO_HPP

#include "Usuario.hpp"

class Cozinheiro : public Usuario {
public:
    Cozinheiro(const std::string& nome,
               const std::string& email,
               const std::string& senha);

    bool podeCadastrarTemplate() const override;
    nivelAcesso getAcesso() const override;
};

#endif