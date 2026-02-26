SRC   = ./src
INC   = ./include
OBJ   = ./obj
BIN   = ./bin

CC     = gcc
CFLAGS = -Wall -std=c11 -I$(INC)

EXEC   = carcassonne

SRCS = $(wildcard $(SRC)/*.c)

OBJS = $(SRCS:$(SRC)/%.c=$(OBJ)/%.o)

all: $(BIN)/$(EXEC)

$(BIN)/$(EXEC): $(OBJS)
	@mkdir -p $(BIN)
	$(CC) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BIN)/$(EXEC)

.PHONY: all clean
