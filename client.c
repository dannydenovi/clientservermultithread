//crea socket
//set porta e ip
// manda richiesta al server

//invia messaggio
// ricevere risposta del server
// chiude connessione


//Import headerfiles
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    char serverMessage[2000], clientMessage[2000];
    int serverDescriptor;
    struct sockaddr_in serverAddr;

    memset(serverMessage, '\0', sizeof(serverMessage));
    memset(clientMessage, '\0', sizeof(clientMessage));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2002);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    serverDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    if(serverDescriptor < 0){
        printf("ERROR: could not create socket.");
        return -1;
    }
    
    if(connect(serverDescriptor, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
        printf("ERROR: connection error.");
        return -1;
    }

    printf("Enter message: ");
    fgets(clientMessage, sizeof(clientMessage), stdin);

    
    if(send(serverDescriptor, clientMessage, strlen(clientMessage), 0) < 0){
        printf("ERROR: could not send message.");
        return -1;
    }

    if(recv(serverDescriptor, serverMessage, sizeof(serverMessage), 0) < 0){
        printf("ERROR: could not receive message.");
        return -1;
    }

    printf("\nServer: %s\n", serverMessage);
    close(serverDescriptor);

    return 0;
}