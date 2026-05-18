#include "doctest.h"
#include "../include/Ingrediente.hpp"

TEST_CASE("Criacao de ingrediente") {

    Ingrediente i(
        "Acucar",
        500,
        "g",
        "Doce"
    );

    CHECK(i._getnomeI() == "Acucar");

    CHECK(i._getquantI() == 500);

    CHECK(i._getunidadeI() == "g");

    CHECK(i._getTipoI() == "Doce");
}

TEST_CASE("Validacao de quantidade") {

    Ingrediente i(
        "Farinha",
        1000,
        "g",
        "Massa"
    );

    CHECK(
        i.verificar_unidades(
            500,
            "g"
        )
    );
}