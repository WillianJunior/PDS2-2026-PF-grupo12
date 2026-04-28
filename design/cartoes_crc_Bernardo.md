### Classe: Usuario
| Responsabilidades | Colaborações |
| :---------------- | :----------- |
| Armazenar dados pessoais (nome, e-mail, senha) | Receita |
| Manter a lista de receitas de sua autoria | Avaliacao |
| Manter a lista de receitas marcadas como favoritas | ---- |
| Adicionar e remover receitas dos favoritos | ---- |
| Validar suas próprias credenciais para autenticação | ---- |



### Classe: Sistema
| Responsabilidades | Colaborações |
| :---------------- | :----------- |
| Manter a coleção de usuários e receitas cadastrados | Usuario |
| Autenticar o usuário ativo e gerenciar a sessão | Receita |
| Carregar e persistir os dados em arquivos texto | ---- |
| Coordenar as operações de cadastro, busca, edição, remoção, favoritar | ---- |
| Tratar exceções globais e validar entradas do menu | ---- |
