#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

// Function declarations
bool isValidString(char string[]);
int sumUp(char string[]);

int main(int argc, char *argv[]) {

    int port = atoi(argv[1]); // store commandline input for port number
    int serverSocket; // sockfd equivalent
    char buffer[129];
    bzero(buffer,128); // makes sure the buffer is 0
    int client_length;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    // Socket Creation
    serverSocket = socket(AF_INET,SOCK_DGRAM,0);
    bzero((char *) &server_address, sizeof(server_address)); // cleans up the server_address struct first

    // Set up the address struct
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    // Bind that bad boy
    bind(serverSocket, (struct sockaddr *) &server_address, sizeof(server_address));

    printf("UDP Server Set-Up Complete\n");

    // Listen
    listen(serverSocket,5);

    int newSocket, line;

    // Did everything in here myself, arguably took too long
    do {
        printf("UDP Server Ready to Receive Inputs\n");
        client_length = sizeof(client_address);
        int line = recvfrom(serverSocket, buffer, 1024, MSG_WAITALL, ( struct sockaddr *) &client_address, &client_length); // get message from client


        printf("Message received: %s\n", buffer);

        if (!isValidString(buffer)) {
            sendto(serverSocket, "From server: Sorry, cannot compute!\n", 37, MSG_CONFIRM, (const struct sockaddr *) &client_address, client_length);
        }
        else {
            int x = sumUp(buffer);
            char response[150];
            bzero(response,150);
            char final_string[500] = "";

            while (x>9) {
                snprintf(response, sizeof(response), "From server: %d\n", x);
                strcat(final_string,response);
                snprintf(buffer, sizeof(buffer), "%d", x);
                x = sumUp(buffer);
            }
            snprintf(response, sizeof(response), "From server: %d\n", x);
            strcat(final_string,response);
            sendto(serverSocket, final_string, strlen(final_string), MSG_CONFIRM, (const struct sockaddr *) &client_address, client_length);
        }
    } while (true);

    close(serverSocket);

    return 0;
}

// Checks if the input string is entirely only digits
// returns: true if the string is only digits
bool isValidString(char string[]) {

    int i = 0; 
    while ( string[i] != '\0') {
        if (!isdigit(string[i]))
            return false;
        i+=1;
    }
    return true;
}

// it sums up every character of the string (assumes all characters are digits)
int sumUp(char string[]) {
    int sum = 0;
    while (*string) {
        sum += *string - '0';
        string++;
    }
    return sum;
}