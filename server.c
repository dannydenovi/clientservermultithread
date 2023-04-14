//Import headerfiles
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {

    char senderMessage[2000], recvMessage[2000];
    int socket_descriptor, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;

    //pulizia del buffer memset()
    memset(senderMessage, '\0', sizeof(senderMessage));
    memset(recvMessage, '\0', sizeof(recvMessage));

    // Creazione socket socket()
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_descriptor < 0){
        printf("ERROR: Could not create socket.");
        return -1;
    }

    //set port and ip
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2002);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //bind port and ip
    bind(socket_descriptor,  (struct sockaddr*) &server_addr, sizeof(server_addr));
    
    // listen for clients listen()
    if(listen(socket_descriptor, 1) < 0){
        printf("ERROR: could not listen for incoming connections.");
        return -1;
    }

    client_size = sizeof(client_addr);
    //accept incoming connections
    client_sock = accept(socket_descriptor, (struct sockaddr*) &client_addr, &client_size);

    if(client_sock < 0){
        printf("ERROR: could not accept connections.");
        return -1;
    }

    if(recv(client_sock, recvMessage, sizeof(recvMessage), 0) < 0){
        printf("ERROR: could not receive message.");
        return -1;
    }

    strcpy(senderMessage, "Ciao dal server");

    if(send(client_sock, senderMessage, sizeof(senderMessage), 0) < 0){
        printf("ERROR: could not send message.");
        return -1;
    }
    printf("\n%s", recvMessage);

    close(socket_descriptor);
    close(client_sock);

    return 0;
}