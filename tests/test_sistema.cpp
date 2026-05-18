TEST_CASE("Cadastrar usuario") {

    Sistema s;

    std::string nome = "david";
    std::string email = "davidbowie@gmail.com";
    std::string senha = "123";

    CHECK(
        s.cadastrarUsuario(
            nome,
            email,
            senha
        )
    );
}

TEST_CASE("Login usuario") {

    Sistema s;

    std::string nome = "ziggy";
    std::string email = "ziggystardust@email.com";
    std::string senha = "abc";

    s.cadastrarUsuario(
        nome,
        email,
        senha
    );

    CHECK(
        s.login(
            email,
            senha
        )
    );

    CHECK(
        s.getUsuarioAtivo()
        !=
        nullptr
    );
}

TEST_CASE("Logout") {

    Sistema s;

    std::string nome = "Joao";
    std::string email = "joaoguimaraesrosa@email.com";
    std::string senha = "123";

    s.cadastrarUsuario(
        nome,
        email,
        senha
    );

    s.login(
        email,
        senha
    );

    s.logout();

    CHECK(
        s.getUsuarioAtivo()
        ==
        nullptr
    );
}

TEST_CASE("Cadastrar receita") {

    Sistema s;

    std::string titulo = "Bolo de ninho com nutella";

    Receita* r =
        s.cadastrarReceita(
            titulo,
            60,
            Dificuldade::Facil,
            Categoria::Doce
        );

    CHECK(r != nullptr);

    CHECK(
        s.getReceitas().size()
        ==
        1
    );
}

TEST_CASE("Buscar receita") {

    Sistema s;

    std::string titulo = "Pizza";

    s.cadastrarReceita(
        titulo,
        30,
        Dificuldade::Medio,
        Categoria::Salgado
    );

    auto resultado =
        s.buscarPorTitulo(
            titulo
        );

    CHECK(
        resultado.size()
        ==
        1
    );
}

TEST_CASE("Filtrar dificuldade") {

    Sistema s;

    std::string r1 = "Bolo";
    std::string r2 = "Lasanha";

    s.cadastrarReceita(
        r1,
        20,
        Dificuldade::Facil,
        Categoria::Doce
    );

    s.cadastrarReceita(
        r2,
        90,
        Dificuldade::Dificil,
        Categoria::Salgado
    );

    auto lista =
        s.filtrarPorDificuldade(
            Dificuldade::Facil
        );

    CHECK(
        lista.size()
        ==
        1
    );
}