# Joguinhos em SDL e C

Joguinhos que fiz para estudar SDL2 e aplicar meus conhecimentos em C.

-   Cobrinha ou SnakeGame: jogo onde uma cobra come uma fruta e cresce de tamanho. Ganha se você ocupa toda a tela com o tamanho da calda da cobra;
-   Flappy Bird Remix: Jogo muito popular a um tempo atrás para dispositivos Mobile (foi feito pelo programador vietnamita Nguyễn Hà Đông e publicado pela .GEARS studios), é um jogo Arcade e bem legal para praticar programação de jogos;
    -   OBS: Créditos das imagens e fonte de inspiração estão no README presente no arquivo do jogo.
-   Quebra Blocos: você usa uma bolinha para destruir blocos de tijolos;
    -   OBS: Créditos das imagens estão no README presente no arquivo jogo.

## Como usar:

### Requisitos

-   CMake
-   GCC
-   Make
-   SDL2
-   SDL2 Image
-   SDL2 TTF

### Instruções:

1. Clone o repositório:

```
git clone https://github.com/lucasfturos/JoguinhosSDL.git && cd JoguinhosSDL
```

2. Entre na pasta do jogo e crie a pasta build:

```
cmake -S . -B build && cd build
```

3. Compile o programa:

```
make
```

4. Execute o programa:

```
./Cobrinha
```
