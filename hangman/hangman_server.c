#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "dependencies.h"

int client_count = 0;

// Function declarations
void *hangman(void *arg);

int main(int argc, char *argv[]) {

    int port = atoi(argv[1]); // store commandline input for port number
    int serverSocket; // sockfd equivalent
    struct sockaddr_in server_address;

    // The function and general outline is inspired by the same website as client
    // https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
    // basically the: socket creation, info set up, bind, listen, etc

    // Socket Creation
    serverSocket = socket(AF_INET,SOCK_STREAM,0);
    bzero((char *) &server_address, sizeof(server_address)); // cleans up the server_address struct first

    // Set up the address struct
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    // Bind that bad boy
    bind(serverSocket, (struct sockaddr *) &server_address, sizeof(server_address));

    printf("TCP Server Set-Up Complete\n");


    // listen(serverSocket,5);

    // hangman(serverSocket); // tempoary

    listen(serverSocket,10);

    // insert multi threading code
    while (true) {
        struct sockaddr_in client_address;
        int client_length = sizeof(client_address);

        printf("Active client: %d\n", client_count);

        int newSocket = accept(serverSocket, (struct sockaddr*) &client_address, &client_length);
        struct server_header server_message;
        if (client_count == 3) {
            printf("Server overloaded, sorry");
            server_message.msg_flag = 17;
            strcpy(server_message.data, "server-overloaded");
            send(newSocket, &server_message, sizeof(server_message) , 0);
            close(newSocket);
            continue;
        }
        else { 
            server_message.msg_flag = 0;
            send(newSocket, &server_message, sizeof(server_message) , 0);
        }
        pthread_t thread;
        pthread_create(&thread, NULL, hangman, (void *)&newSocket);
        client_count+=1;
        pthread_detach(thread);
    }

    close(serverSocket);

    return 0;
}

void *hangman(void *arg) {
    int newSocket = *((int*)arg);
    struct client_header received_client_message;
    int client_message = recv(newSocket, &received_client_message, sizeof(received_client_message), 0);
    printf("First message received (%d)\n", received_client_message.msg_length);
    int guess_count = 0;

    // select random word --------------------------------------------------
    FILE *f;
    char word[20];
    bzero(&word, sizeof(word));
    int fileSize = 0;
    f = fopen("hangman_words.txt","r");
    while (fgets(word, sizeof(word), f) != NULL) {
        fileSize++;
    }
    printf("file size: %d\n",fileSize);

    int randomLine = (rand() % fileSize)+1;
    printf("Random line: %d\n",randomLine);
    rewind(f);

    for (int i = 0 ; i < randomLine ; i++ ) {
        fgets(word, sizeof(word), f);
        // printf("Random: %s", word);
    }

    fclose(f);
    // random word selection done -------------------------------------------

    int length = strlen(word)-1;
    char start[length];
    bzero(&start,sizeof(start));
    for (int i = 0 ; i < length ; i++)
        start[i] = '_';

    struct server_header server_message;
    server_message.msg_flag = 0;
    server_message.word_length = length;
    server_message.num_incorrect = 0;
    bzero(server_message.data, sizeof(server_message.data));
    strcpy(server_message.data,start);

    printf("The word is: %s and its length is %d\n", word, length);

    bool found = false;
    int counter = 0;

    printf("Ready to play!\n");
    do {
        send(newSocket, &server_message, sizeof(server_message) , 0);
        client_message = recv(newSocket, &received_client_message, sizeof(received_client_message), 0);
        printf("Output: %c\n", received_client_message.data);

        for (int i = 0 ; i < length ; i++) {
            if (received_client_message.data == server_message.data[i]) {
                found = true;
                break;
            }
            if (received_client_message.data == word[i]) {
                server_message.data[i] = received_client_message.data;
                counter+=1;
                found = true;
            }
        }
        if (!found) {
            server_message.data[length+server_message.num_incorrect] = received_client_message.data;
            server_message.num_incorrect += 1;
        }
        found = false;

        // Check win or lose

        if (server_message.num_incorrect > 5) {
            // too many incorrect guesses
            printf("Client Lost!\n");
            strcpy(server_message.data, "The word was ");
            strcat(server_message.data, word);
            server_message.msg_flag = 13 + length;
            send(newSocket, &server_message, sizeof(server_message) , 0);
            strcpy(server_message.data,"You Lose!");
            server_message.msg_flag = 9;
            send(newSocket, &server_message, sizeof(server_message) , 0);
            strcpy(server_message.data,"Game Over!");
            server_message.msg_flag = 10;
            send(newSocket, &server_message, sizeof(server_message) , 0);
            break;
        }

        if (counter == length) {
            // they guessed the word
            printf("Client Won!\n");
            strcpy(server_message.data, "The word was ");
            strcat(server_message.data, word);
            server_message.msg_flag = 13 + length;
            send(newSocket, &server_message, sizeof(server_message) , 0);
            strcpy(server_message.data,"You Win!");
            server_message.msg_flag = 8;
            send(newSocket, &server_message, sizeof(server_message) , 0);
            strcpy(server_message.data,"Game Over!");
            server_message.msg_flag = 10;
            send(newSocket, &server_message, sizeof(server_message) , 0);
            break;
        }
        printf("Incorrect: %d\n", server_message.num_incorrect);
        printf("Counter: %d\n", counter);

        printf("Current data: ");
        for (int i = 0 ; i < length+server_message.num_incorrect ; i++) {
            printf("%c",server_message.data[i]);
        }
        printf("\n");



    } while (client_message > 0);
    printf("Game Done!\n");
    close(newSocket);
    client_count-=1;
    pthread_exit(NULL);
    return;

}