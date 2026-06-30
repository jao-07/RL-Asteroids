# Aprendizado por Reforço no jogo digital Asteroids

O presente projeto tem como objetivo a criação de uma versão do jogo Asteroids e o treinamento de um agente de IA para aprender a jogar o jogo.
No repositório há o relatório final da primeira parte do projeto, onde está descrito algumas decisões de projeto e resultados obtidos.
Segue abaixo as instruções e recomendações de instalação das ferramentas para a compilação do jogo e para o treinamento e avaliação do agente:

## Jogo Asteroids em C++

### CLion

Para compilar e executar o jogo, é recomendado utilizar a IDE CLion, pois ela já vem com um compilador de C++ na instalação, e ela facilita muito a compilação e execução do jogo.
Para instalar, acesse o link https://www.jetbrains.com/clion/ e siga as instruções para o seu sistema operacional.

### Biblioteca SDL

Para o desenvolvimento do jogo, foi utilizada a biblioteca SDL2 (Simple DirectMedia Layer). Foi utilizada a versão 2.32.2.
Para instalar, siga os passos de acordo com o seu sistema operacional:

### Linux

1. Acesse o link https://github.com/libsdl-org/SDL/releases/tag/release-2.32.2 e baixe o pacote Source code.zip
   
2. Extraia os arquivos no diretório temporário /tmp/SDL2/
   
3. Instale a biblioteca no diretório /opt/SDL2/
```bash
cd /tmp/SDL2/
 ./configure --prefix /opt/SDL2/
 make all
 sudo make install
```

### Windows (64 bits)

1. Acesse o link https://github.com/libsdl-org/SDL/releases/tag/release-2.32.2 e baixe o pacote SDL2-devel-2.32.2-VC.zip.

2. Extraia os arquivos no diretório C:\Arquivos de Programas\SDL2\
   
3. Após isso, copie o arquivo SDL2.dll do diretório C:\Arquivos de Programas\SDL2\lib\x64 para o diretório C:\Windows\System32\

### Mac

1. Acesse o link https://github.com/libsdl-org/SDL/releases/tag/release-2.32.2 e baixe o pacote SDL2-2.32.2.dmg

2. Clique na imagem para abrí-la e copie e o pacote SDL2.framework para o diretório /Library/Frameworks/.
