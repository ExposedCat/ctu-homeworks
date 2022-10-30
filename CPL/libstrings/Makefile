FLAGS=-pedantic -Wall -Werror -std=c99 -Wno-incompatible-library-redeclaration
BINARY=main
LIBRARY=my_string
BIN_DIR=.

all: compile clean

compile:
	clang -c $(FLAGS) $(BINARY).c -o $(BIN_DIR)/$(BINARY).o
	clang -c $(FLAGS) $(LIBRARY).c -o $(BIN_DIR)/$(LIBRARY).o
	clang $(FLAGS) $(BIN_DIR)/$(BINARY).o $(BIN_DIR)/$(LIBRARY).o -o $(BIN_DIR)/$(BINARY)

clean:
	rm $(BIN_DIR)/$(BINARY).o $(BIN_DIR)/$(LIBRARY).o