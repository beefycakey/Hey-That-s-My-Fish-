CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRC = main.c boardgen.c interactive.c movementph.c placementph.c ai.c
OBJ = $(SRC:.c=.o)

EXEC = penguins

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
