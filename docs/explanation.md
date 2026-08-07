# Explicação dos arquivos e classes

## Jogo Asteroids

Os arquivos do jogo ficam no diretório `Source`.

A classe principal é a `Game`, onde ocorre a instanciação e deleção dos objetos, a definição dos parâmetros do jogo e as funções principais de treinamento:

* `Reset` : Reinicia o jogo e todos os objetos, e retorna o espaço de observação inicial do jogo.

* `Step` : Recebe como parâmetro uma ação, realiza essa ação por 4 frames e retorna a recompensa obtida, o espaço de observações do próximo estado, informações se o episódio terminou e algumas métricas medidas.

Os arquivos da classe `Main` servem apenas para testes locais, e não são usados no treinamento.

O diretório `Components` possui as classes de componentes que são responsáveis pela física dos objetos, colisão de objetos e pelo desenho deles na tela.

O diretório `Actors` possui as classes dos objetos do jogo, tais como Asteroides, Nave, Laser e as partículas que são criadas ao destruir os asteroides.

O arquivo `Bindings.cpp` é responsável por configurar o módulo do jogo através do Pybind11.

O arquivo `CmakeLists.txt` foi configurado para criar tanto o módulo do jogo usado no treinamento, quanto um executável do jogo, para fins de testes. No CLion, é possível escolher qual dos dois fazer o build. O módulo do jogo é construído e colocado dentro do diretório `IA`.


## Ambiente de treinamento

O ambiente de treinamento do agente em Python fica no diretório `IA`

Nele há o arquivo `asteroids_env.py`, que, através da biblioteca Gymnasium, cria a classe AsteroidsEnv, responsável por modelar os processos relacionados ao funcionamento do jogo e controle do agente.

Além disso, há o arquivo `training.py`, responsável pelo pipeline de treinamento do agente, e o arquivo `evaluating.py`, que serve para testar a performance do agente treinado.

Há também o arquivo `asteroids_cpp.cp313-win_amd64.pyd`, que é o arquivo do motor do jogo produzido através do build do jogo, sendo utilizado para fazer o import do modulo asteroids_cpp. Esse arquivo que está disponível é da versão final treinada, com 10 asteroides e uma janela de atenção de tamanho 2. Para rodar com outras configurações, basta alterar o jogo em C++ e fazer um novo build, que este arquivo será substituído por um novo.

Na pasta `models`, há exemplos de pesos de alguns agentes que foram treinados em diferentes configurações do jogo, variando o número de asteroides existentes, e o número de asteroides presentes na janela de atenção do agente.

Na pasta `tensorBoardFiles` há os arquivos de análise dos treinamentos feitos, na qual é possível rodar através do TensorBoard e verificar algumas métricas de treinamento e performance.
