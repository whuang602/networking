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

int main(int argc, char *argv[]) {

    // Set up, same as the UDP client from homework 2b
    char* ip = argv[1];
    int clientSocket = socket(AF_INET, SOCK_DGRAM,0);
    struct sockaddr_in server_address; // server == destination

    char buffer[81];

    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(atoi(argv[2])); // adding port num


    // set up struct called timeout to contain the timeout value (1 second)
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    // set up the time out value for the clientSocket
    setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)); // set time out to 1 sec

    // Set up variables to store values for use in calculating ping statistics
    int packet_received = 0;
    float total_wait = 0;
    float max_wait = 0;
    float min_wait = 1000;
    for (int i = 0 ; i < 10 ; i++) {
        // Grab the current time and create a string version of it
        time_t current_time = time(NULL);
        char time_c[26];
        ctime_r(&current_time,time_c);

        // clean up the buffer and fill it with the ping message
        bzero(buffer,sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "PING #%d %s", i+1, time_c);
        // printf("%s",buffer);

        // set up timer to calculate round trip time
        clock_t start, end;

        start = clock();
        sendto(clientSocket, buffer, sizeof(buffer) ,MSG_CONFIRM, (const struct sockaddr *) &server_address, sizeof(server_address));

        int client_length;
        int line = recvfrom(clientSocket, (char *)buffer, sizeof(buffer)-1 ,0, (struct sockaddr *) &server_address, &client_length);
        end = clock();

        float wait = 1000*(float)(end-start)/CLOCKS_PER_SEC;

        // Check if the server received the message
        // if line == -1 (meaning that the server didnt send anything back), then the original packet was lost
        // otherwise, the client did receive a message back, we accumulate the stats and print out a messages stating we received a reply
        if (line == -1) { // not received
            printf("Request timeout for seq#=%d\n",i+1);
        }
        else {
            total_wait = total_wait + wait;
            if (wait > max_wait)
                max_wait = wait;
            if (wait < min_wait)
                min_wait = wait;
            packet_received+=1;
            buffer[line] = '\0';
            snprintf(buffer, sizeof(buffer), "PING received from %s: seq#=%d time=%.3f ms\n", ip, i+1, wait);
            printf("%s", buffer);
        }

        sleep(1);
    }

    printf("--- %s ping statistics ---\n",ip);
    // Print out the ping statistic unless nothing was received
    if (packet_received > 0)
        printf("10 packets transmitted, %d received, %d%% packet loss rtt min/avg/max = %.3f %.3f %.3f ms\n",packet_received,10*(10-packet_received), roundf(1000*min_wait)/1000, roundf(1000*total_wait/packet_received)/1000, roundf(1000*max_wait)/1000);
    else
        printf("10 packets transmitted, %d received, %d%% packet loss\n", packet_received,10*(10-packet_received));
    close(clientSocket);
    return 0;
}