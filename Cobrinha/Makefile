run: build
	@clear
	@./cobra
build:
	@gcc -Wall -Wextra -g -o cobra \
			 main.c obj.c init_sdl.c \
			 -lSDL2 -lSDL2main -lSDL2_ttf -lm
clean:
	@rm -rf cobra
