FLAGS+= -pedantic
FLAGS+= -Wall -Werror -Wextra
FLAGS+= -Wno-unused-variable
FLAGS+= -std=c99 -O3
NAME=graph

all: prepare compile_lib
full: prepare compile_lib compile_main link

prepare:
	mkdir -p .bin

compile_lib:
	clang -c $(FLAGS) $(NAME).c -o .bin/$(NAME).o

compile_main:
	clang -c $(FLAGS) main.c -o .bin/main.o

link:
	clang $(FLAGS) .bin/main.o .bin/$(NAME).o -o .bin/main

validate:
	clear && valgrind --leak-check=full --show-leak-kinds=all -s ./.bin/main $(READ)