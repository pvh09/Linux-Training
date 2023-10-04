CC = gcc
INC = -I./inc
SRC = ./src

all: run

run: main
	./main

main: $(SRC)
	$(CC) $(INC) $(SRC)/server.c -o server -lpthread
	$(CC) $(INC) $(SRC)/client.c -o client

clean:
	rm -rf main server client