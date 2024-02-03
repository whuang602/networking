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
    int clientSocket;
    struct sockaddr_in server_address; // server == destination

    char buffer[129];
    
    
    if ( (clientSocket = socket(AF_INET, SOCK_DGRAM,0)) < 0) {
        printf("Socket Creation Error");
    }

    // ------------------------------------------------------------------------------------------------------------------------
    // originally from: https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
    // the process of setting up the member variables for the struct is too complex
    // for me to figure out by myself (same as the tcp portion)
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(atoi(argv[2])); // adding port num
    // ------------------------------------------------------------------------------------------------------------------------
    
    printf("Enter string: ");
    scanf("%s", buffer);

    // ------------------------------------------------------------------------------------------------------------------------
    // Originally from https://www.scaler.com/topics/udp-server-client-implementation-in-c/ 
    // I wanted to be sure what the format of the send and receive functions look like
    sendto(clientSocket, buffer, 129 ,MSG_CONFIRM, (const struct sockaddr *) &server_address,sizeof(server_address));

    int client_length;
    int line = recvfrom(clientSocket, (char *)buffer, 129 ,MSG_WAITALL, (struct sockaddr *) &server_address, &client_length);

    buffer[line] = '\0';
    // ------------------------------------------------------------------------------------------------------------------------
	printf("%s", buffer);

    close(clientSocket);
    return 0;
}