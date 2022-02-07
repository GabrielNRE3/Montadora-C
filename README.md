# Montadora-C
## Projeto em C com o objetivo de simular um sistema de estoque de montadora de veículos
## Todos os dados são armazenados em arquivos .bin que simulam bancos de dados, estes arquivos são gerados caso não existam, e modificações realizadas em um deles afetam os demais, como um sistema de chaves primárias e chaves estrangeiras.
### Ao iniciar o programa, um menu é aberto:
#### [1] Cadastrar Concessionárias - Realiza o Cadastro de uma concessionária no sistema, o limite máximo de concessionárias registradas é 5. Inserir dados como : Telefone, email e nome já possuíndo um outro no banco de dados, causará uma notificação ao usuário e o cadastro será cancelado
#### [2] Acesso de Concessionária - De acordo com o CNPJ inserido, mostra informações de nome, vendidos, reservados e lista de reservados.
#### [3] Registrar Novo Carro - Realiza o Cadastro de um carro no sistema, o limite máximo registrado é 50.
#### [4] Acesso ao Acervo de Carros - Abre um menu para pesquisar o acervo de carros - Os carros são listados exibindo nome, cor, modelo, status e número de registro.
##### [4 - 1] Total - Exibe todos os carros não vendidos do acervo.
##### [4 - 2] Parcial: Livre ou Reservado - Exibe os carros livres, ou reservados de acordo com a opção selecionada.
##### [4 - 3] Parcial: Modelo - Exibe todos os carros não vendidos do acervo que compartilhem o modelo solicitado.
##### [4 - 4] Retornar ao Menu Principal - Retorna ao menu inicial.
#### [5] Reservas - Abre um menu responsável por conter todas as ações relacionadas à reservas.
##### [5 - 1] Acervo de Carros Total - Exibe todos os carros não vendidos do acervo.
##### [5 - 2] Verificar Reservas - Exibe todos os carros reservados.
##### [5 - 3] Reservar Novo Carro - Permite reservar um carro para uma concessionária, desde que o carro não esteja reservado no momento, e a concessionária tenha menos de 3 carros reservados no momento
##### [5 - 4] Gerenciar Reservas - Permite liberar, ou vender um carro reservado para a concessionária que o reservou, o carro é selecionado baseado no número de registro.
##### [5 - 5] Retornar ao Menu Principal - Retorna ao menu inicial.
#### [6] Verificar Histórico - Abre um menu responsável por conter todas as ações relacionadas ao histórico de vendas.
##### [6 - 1] Consulta por CNPJ - De acordo com o CNPJ inserido, é mostrado todos os carros vendidos para a concessionária, incluindo cor, modelo e preço.
##### [6 - 2] Consulta por Modelo - De acordo com o modelo inserido, é mostrado todos os carros vendidos daquele modelo, incluido cor, preço e o CNPJ da concessionária.
#### [6 - 3] Retornar ao Menu Principal - Retorna ao menu inicial.
##### [7] Alterar Registros - Abre um menu responsável por realizar alterações nos dados de carros e concessionárias.
##### [7 - 1] Concessionária - Permite alterar os dados registrados da concessionária através de seu CNPJ (com excessão de vendidos e reservados).
##### [7 - 2] Carro - Permite alterar os dados registrados no carro através de seu número de registro (com excessão de vendido ou reservado).
##### [7 - 3] Retornar ao Menu Principal - Retorna ao menu inicial.
#### [8] Encerrar - Fecha a Aplicação.

