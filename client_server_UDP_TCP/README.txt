Tools/Sources used:

https://www.geeksforgeeks.org/socket-programming-cc/

https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html

ChatGPT for C syntax checking

https://www.scaler.com/topics/udp-server-client-implementation-in-c/ 



Basic Run Down of the program:

    Summary:
    - the client sends a string to the server, the server will reject the string if it contains any non-numerical characters
    - however, if the string only contains numbers, then the server will calculate the sum of the string (and the sum of the sum string, etc)
    until the sum is only a one-digit string
    - Then the connection will close, but the server will remain open

    TCP:
    - you create a socket
    - set up a struct to hold all the host info (i.e. IP, port)
    - bind the socket to the struct to give it all the critical address and port info (this fully sets up the socket itself)
    - now you can have the socket listen so that it passively waits for external requests
    - here we set up a loop so that the main server socket doesnt close after receiving one requests
    - inside the loop, you create a "new socket" to accept incoming requests and make a handshake (as per TCP guideline)
    - until all calculation completes, the connection will stay open

    UDP:
    - similar to above where you create a socket, set up the info struct, bind the info and socket, and passively listen
    - however, instead of making an connection and accepting a handshake, the UDP server can simply grab the incoming message if it receives one
    - same calculation with sums apply
    - at the end, the server send a response back and continues listening to other requests
    - 



How to Compile and Run:

- run "make" (or make "specific_name" to recompile specific parts of the program)

- start up the respective servers first with the port number included in the command line, eg. ./server_c_tcp 4000

- start up the respective client and include the IP AND the port number, eg ./client_c_udp 123.123.123.123 4000 (where 123.123.123.123 is your actual host IP address)

- watch magic unfold