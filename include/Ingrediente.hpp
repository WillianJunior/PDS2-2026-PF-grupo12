
#ifndef INGREDIENTE_HPP
#define INGREDIENTE_HPP
#include <string>

/**
 * @class Ingrediente
 * @brief Representa um ingrediente com nome, quantidade, unidade e tipo.
 */
class Ingrediente {
private:
    std::string _nome;
    double _quantidade;
    std::string _unidade;
    std::string _tipo;

public:
    Ingrediente(const std::string& nome, double quant,
                const std::string& unidade, const std::string& tipo);

    bool verificarUnidades(double quant, const std::string& unidade) const;
    bool validar() const;

    void alterarQuantidade(double razao);
    const std::string& getNome() const;
    const std::string& getUnidade() const;
    const std::string& getTipo() const;
    const double& getQuantidade() const;
};

#endif