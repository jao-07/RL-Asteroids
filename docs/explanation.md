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

