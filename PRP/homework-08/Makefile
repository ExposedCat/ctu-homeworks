FLAGS= -pedantic -Wall -Werror -Wextra -std=c99 -O3
HW=main

all:
	clang -c $(FLAGS) queue.c -o libqueue.so
	clang $(FLAGS) $(HW).c -o $(HW) ./libqueue.so
