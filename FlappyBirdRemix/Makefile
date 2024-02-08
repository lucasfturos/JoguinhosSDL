run: build
	@./Demo
build:
	@gcc -Wall -Wextra -g -o Demo \
			 main.c init_sdl.c draw.c logic.c \
			 -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lm
clean:
	@rm -rf Demo 
