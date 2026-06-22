#ifndef ADMIN_HPP
#define ADMIN_HPP

#include "Usuario.hpp"

/**
 * @class Admin
 * @brief Subclasse de Usuario com privilégios administrativos.
 *
 * O Admin pode cadastrar templates (como Chef) e também remover
 * receitas e usuários do sistema — funcionalidades exclusivas deste nível.
 */
class Admin : public Usuario {
public:
    Admin(const std::string& nome,
          const std::string& email,
          const std::string& senha);

    bool podeCadastrarTemplate() const override;
    nivelAcesso getAcesso() const override;
};

#endif
