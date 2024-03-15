CC = gcc
ARGS = -Wall

# Compiling all the dependencies
all: hangman_client hangman_server

hangman_client: hangman_client.c
	$(CC) $(ARGS) -o hangman_client hangman_client.c -lm -pthread

hangman_server: hangman_server.c
	$(CC) $(ARGS) -o hangman_server hangman_server.c -lm -pthread

clean:
	rm -f *.o hangman_client *~
	rm -f *.o hangman_server *~