SRC = ./src/
INC = ./include/
OBJ = ./obj/
BIN = ./bin/

CC = gcc
CFLAGS = -Wall -std=c11
EXEC = carcassonne
OBJ = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)
	@rm -f $(OBJ)  

%.o: %.c structs.h tileFetcher.h deckFunctions.h positionFunctions.h tileFunctions.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
