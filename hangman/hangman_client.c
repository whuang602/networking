#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#include "dependencies.h"

int main(int argc, char *argv[]) {

    // Set up, same as the UDP client from homework 2b
    char* ip = argv[1];
    int clientSocket = socket(AF_INET, SOCK_STREAM,0);
    struct sockaddr_in server_address; // server == destination
    struct hostent *server = gethostbyname(ip);

    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(atoi(argv[2])); // adding port num
    memcpy((char *)&server_address.sin_addr.s_addr, (char *)server->h_addr_list[0], server->h_length); 
    connect(clientSocket, (struct sockaddr *) &server_address, sizeof(server_address));

    struct server_header received_server_message;
    recv(clientSocket, &received_server_message, sizeof(received_server_message), 0);
    if ( received_server_message.msg_flag != 0) {
        printf(">>>%s\n", received_server_message.data);
    }
    else {
        char input;
        int result;
        printf(">>>Ready to start game? (y/n): ");
        scanf("%c",&input);
        if (input == 'y') {
            // do the game
            struct client_header header;
            header.msg_length = 0;
            header.data = ' ';
            send(clientSocket, &header, sizeof(header) , 0); // empty message
            int incorrect_guess = 0; // count up to 6
            char guess[100];
            bzero(&guess,sizeof(guess));
            while ( incorrect_guess < 6 ) {
                recv(clientSocket, &received_server_message, sizeof(received_server_message), 0);
                if (received_server_message.msg_flag == 0) {
                    printf(">>>");
                    for (int i = 0 ; i < received_server_message.word_length ; i++) {
                        printf("%c",received_server_message.data[i]);
                        if (i != received_server_message.word_length-1)
                            printf(" ");
                    }
                    printf("\n>>>Incorrect Guesses: ");
                    for (int i = 0 ; i < received_server_message.num_incorrect ; i++ ) {
                        printf("%c",received_server_message.data[received_server_message.word_length+i]);
                        if (i != received_server_message.num_incorrect-1)
                            printf(" ");
                    }
                    printf("\n>>>\n");
                }
                else {
                    printf(">>>%s",received_server_message.data);
                    recv(clientSocket, &received_server_message, sizeof(received_server_message), 0);
                    printf(">>>%s\n",received_server_message.data);
                    recv(clientSocket, &received_server_message, sizeof(received_server_message), 0);
                    printf(">>>%s\n",received_server_message.data);
                    break;
                }
                incorrect_guess = received_server_message.num_incorrect;
                printf(">>>Letter to guess: ");
                result = scanf("%s", &guess);
                if (strlen(guess) == 0 || result == EOF) { // EOF check should be return value of scanf
                    printf("\n");
                    close(clientSocket);
                    exit(0);
                }
                else {
                    while (strlen(guess) > 1 || !isalpha(guess[0])) {
                        printf(">>>Error! Please guess one letter.\n");
                        printf(">>>Letter to guess: ");
                        result = scanf("%s", &guess);
                        if (strlen(guess) == 0 || result == EOF) {
                            printf("\n");
                            close(clientSocket);
                            exit(0);
                        }
                    }
                    if (strlen(guess) == 1 && guess[0] != EOF) {
                        header.msg_length = 1;
                        header.data = tolower(guess[0]);
                        send(clientSocket, &header, sizeof(header) , 0);
                    }
                }
            }
        }
    }
    close(clientSocket);
    return 0;
}