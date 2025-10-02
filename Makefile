BIN = main
SRC = main.c

CFLAGS = -Wall

NP ?= 1

.PHONY: all compile run clean

all: compile run clean

compile: $(SRC)
	@mpicc $(CFLAGS) -o $(BIN) $(SRC)

run: $(BIN)
	@mpirun -np $(NP) ./$(BIN)

clean:
	@rm -f $(BIN)