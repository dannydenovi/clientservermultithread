#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define CLIENT_LIMIT 30

void* routine(void* arg);

int main() {
    long int i = 0; 
    pthread_t threads[CLIENT_LIMIT]; 
    
    while (i<CLIENT_LIMIT){
        pthread_create(&threads[i], NULL, routine, (void*)(i+1));
        usleep(500);
        i++;
    }
    
    return 0;
}

void* routine(void* arg) {

    long int i = (long int) arg;
    char serverMessage[2000], clientMessage[2000];
    struct sockaddr_in serverAddr;
    int serverDescriptor; 

    serverDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    if(serverDescriptor < 0){
        printf("ERROR: could not create socket.");
        pthread_exit(NULL);
    }
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7799);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(serverDescriptor, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
        printf("\nERROR: connection error.");
        pthread_exit(NULL);
    }

    sprintf(clientMessage, "Hello from client %li\n", i);

    if(send(serverDescriptor, clientMessage, strlen(clientMessage), 0) < 0){
        printf("ERROR: could not send message.");
        pthread_exit(NULL);
    }

    if(recv(serverDescriptor, serverMessage, sizeof(serverMessage), 0) < 0){
        printf("ERROR: could not receive message.");
        pthread_exit(NULL);
    }

    printf("\nServer: %s\n", serverMessage);
    close(serverDescriptor);
    return NULL;
}