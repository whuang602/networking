#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[]) {

    char* ip = argv[1];
    int port = atoi(argv[2]);
    int clientSocket;
    struct sockaddr_in server_address; // server == destination
    struct hostent *server = gethostbyname(ip);

    char buffer[129];
    clientSocket = socket(AF_INET, SOCK_STREAM,0);

    // ---------------------------------------------------------------------------------------------
    // originally from: https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
    // the process of setting up the member variables for the struct is too complex
    // for me to figure out by myself
    bzero((char *) &server_address, sizeof(server_address)); // clean up the address space
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    memcpy((char *)&server_address.sin_addr.s_addr, (char *)server->h_addr, server->h_length); // copy over all the data from the host to the server struct
    connect(clientSocket, (struct sockaddr *) &server_address, sizeof(server_address)); // make a connection
    // ---------------------------------------------------------------------------------------------
    
    // Everything below is myself, from hours of testings and reading documentation
    printf("Enter string: ");
    scanf("%s", buffer);

    int line = write(clientSocket, buffer, 129);
    line = read(clientSocket, buffer, 129);
    printf("%s",buffer);

    close(clientSocket);
    return 0;
}