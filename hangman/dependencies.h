#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H

struct client_header {
    int msg_length;
    char data;
};

struct server_header {
    int msg_flag;
    char word_length;
    char num_incorrect;
    char data[30];
};

#endif