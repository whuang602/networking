Tools/Resources used:

ChatGPT for C syntax checking

StackOverFlow also for C syntax checking

Most (99%) of the code was written by me from scratch

Basic Run Down of the Program:

    - The Client runs on the UDP protocol, meaning that it doesn't attempt to initiate a handshake. 
        It just sends a packet and hopes that it receives an ack (or some packet in return)

    In short the code runs like this:
    - set up and make sure we have the server info set up in the socket
    - we send a series of 10 pings to the server
    - if the ping reaches the server and isn't "dropped," the server sends back a reply
    - if the packet was dropped, the client will wait until the timeout limit was reached
    - After all 10 pings were sent, the final statistics is calculated

    Note that the makefile needed to link the .c file to math.h otherwise certain rounding functionality won't work

How to Compile and Run:

- run make

- start up the respective servers first with the port number included in the code, e.g. python3 UDPPingerServer.py

- start up the client and include the IP AND the port number, eg ./PingClient 123.123.123.123 12000 (where 123.123.123.123 is your actual host IP address)

- watch magic unfold

- make clean if you want to clean