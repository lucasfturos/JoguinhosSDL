run: build
	@./Breaker
build:
	@gcc -Wall -Wextra -g -o Breaker \
			 main.c objects.c \
			 -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf \
			 -lm 
clean:
	@rm -rf Breaker
