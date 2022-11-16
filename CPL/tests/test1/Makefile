FLAGS=-pedantic -Wall -Werror -Wextra -std=c99 -Wno-incompatible-library-redeclaration

BIN_DIR=bin
BINARY=main
LIBRARY=decipher
HELPERS_LIBRARY=helpers

FILESET=example.txt example.perm example.lang

all: prepare clean compile

validate: prepare clean compile validate_memory

prepare:
	mkdir -p $(BIN_DIR)

compile:
	clang -c $(FLAGS) $(BINARY).c -o $(BIN_DIR)/$(BINARY).o
	clang -c $(FLAGS) $(LIBRARY).c -o $(BIN_DIR)/$(LIBRARY).o
	clang -c $(FLAGS) $(HELPERS_LIBRARY).c -o $(BIN_DIR)/$(HELPERS_LIBRARY).o
	clang $(FLAGS) \
		$(BIN_DIR)/$(BINARY).o \
		$(BIN_DIR)/$(LIBRARY).o \
		$(BIN_DIR)/$(HELPERS_LIBRARY).o \
	-o $(BIN_DIR)/$(BINARY)

clean:
	rm -f $(BIN_DIR)/$(BINARY) \
		$(BIN_DIR)/$(BINARY).o \
		$(BIN_DIR)/$(LIBRARY).o \
		$(BIN_DIR)/$(HELPERS_LIBRARY).o

validate_memory:
	valgrind \
		--leak-check=full \
		--quiet \
		$(BIN_DIR)/$(BINARY) $(FILESET)