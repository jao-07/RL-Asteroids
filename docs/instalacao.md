# Guia de Instalação e Configuração

Para fazer a instalação e configuração das ferramentas necessárias para a compilação do jogo e o treinamento do agente, siga os seguintes passos:

## *Jogo Asteroids em C++*

## CLion

Para compilar e executar o jogo, é recomendado utilizar a IDE CLion, pois ela já vem com um compilador de C++ na instalação, e ela facilita muito a compilação e execução do jogo.
Para instalar, acesse o site do [CLion](https://www.jetbrains.com/clion/) e siga as instruções para o seu sistema operacional.

## Biblioteca SDL

Para o desenvolvimento do jogo, foi utilizada a biblioteca SDL2 (Simple DirectMedia Layer). Foi utilizada a versão 2.32.2.
Para instalar, siga os passos de acordo com o seu sistema operacional:

### Linux

1. Acesse o repositório do [SDL](https://github.com/libsdl-org/SDL/releases/tag/release-2.32.2) e baixe o pacote `Source code.zip`

2. Extraia os arquivos no diretório temporário `/tmp/SDL2/`

3. Instale a biblioteca no diretório `/opt/SDL2/`
```bash
cd /tmp/SDL2/
 ./configure --prefix /opt/SDL2/
 make all
 sudo make install
```

### Windows (64 bits)

1. Acesse o repositório do [SDL](https://github.com/libsdl-org/SDL/releases/tag/release-2.32.2) e baixe o pacote `SDL2-devel-2.32.2-VC.zip`.

2. Extraia os arquivos no diretório `C:\Arquivos de Programas\SDL2\`

3. Após isso, copie o arquivo `SDL2.dll` do diretório `C:\Arquivos de Programas\SDL2\lib\x64` para o diretório `C:\Windows\System32\`

### Mac

1. Acesse o repositório do [SDL](https://github.com/libsdl-org/SDL/releases/tag/release-2.32.2) e baixe o pacote `SDL2-2.32.2.dmg`

2. Clique na imagem para abrí-la e copie e o pacote `SDL2.framework` para o diretório `/Library/Frameworks/`.


## *Ambiente de treinamento em Python*

Para esse projeto, foi utilizada a versão 3.13.5 do Python, mas possivelmente outras versões da linguagem também funcionarão.
Para criar o ambiente, entre no diretório `IA`, e siga os seguintes passos:

1. Crie um ambiente virtual (venv)
```bash
python -m venv venv
```

2. Faça a ativação do ambiente

   Windows:
   ```bash
   venv\Scripts\activate
   ```

   Linux ou Mac:
   ```bash
   source venv/bin/activate
   ```

3. Faça a instalação dos pacotes necessários:
   ```bash
   python -m pip install -r requirements.txt
   ```

Como o projeto utiliza a biblioteca `pybind11` para fazer a comunicação entre o Python e o C++, o arquivo `CMakeLists.txt`, que é responsável pela compilação do jogo, foi configurado para localizar o arquivo `pybind11Config.cmake` ou `pybind11-config.cmake`, para produzir o arquivo .pyd, que é o arquivo que permite com que o programa em Python faça a importação do módulo do jogo.

Para isso, deve-se fazer a seguinte configuração no CLion:
Acesse `Configurações(Settings) -> Build,Execution,Deployment -> CMake` e no campo `Cmake option` adicione a seguinte informação do pathing do arquivo do pybind11, da seguinte forma:

`-DCMAKE_PREFIX_PATH="C:\Users\joaovecruz\CLionProjects\RL-Asteroids\IA\venv\Lib\site-packages\pybind11\share\cmake\pybind11`

Mude para o caminho correto da sua máquina.

Dessa forma, o CMake deve funcionar bem e compilar o jogo