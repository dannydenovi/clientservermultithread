#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define true 1

FILE *fp;
struct Args{
    int *arg1;
    pthread_mutex_t *arg2;
};

void *routine(void *arg);

int main(){

    int socket_descriptor, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    int i = 0;
    pthread_t threads[30];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    // Creazione socket socket()
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_descriptor < 0){
        printf("ERROR: Could not create socket.");
        return -1;
    }

    // set port and ip
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7799);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // bind port and ip
    bind(socket_descriptor, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // listen for clients listen()
    if (listen(socket_descriptor, 30) < 0){
        printf("ERROR: could not listen for incoming connections.");
        return -1;
    }

    while (true){
        client_size = sizeof(client_addr);
        // accept incoming connections
        struct Args args = {&client_sock, &mutex};
        client_sock = accept(socket_descriptor, (struct sockaddr *)&client_addr, (socklen_t *)&client_size);
        pthread_create(&threads[i], NULL, routine, (void *)&args);
        i == 30 ? i = 0 : i++;
    }

    close(socket_descriptor);
    return 0;
}

void *routine(void *arg){

    struct Args *args = (struct Args *)arg;
    int *client_sock = args->arg1;
    pthread_mutex_t *mutex = args->arg2;

    char senderMessage[2000], recvMessage[2000];
    memset(senderMessage, '\0', sizeof(senderMessage));
    memset(recvMessage, '\0', sizeof(recvMessage));

    if (*client_sock < 0){
        printf("ERROR: could not accept connections.");
        pthread_exit(NULL);
    }

    if (recv(*client_sock, recvMessage, sizeof(recvMessage), 0) < 0){
        printf("ERROR: could not receive message.");
        pthread_exit(NULL);
    }

    strcpy(senderMessage, "Hello world!");

    if (send(*client_sock, senderMessage, sizeof(senderMessage), 0) < 0){
        printf("ERROR: could not send message.");
        pthread_exit(NULL);
    }

    printf("\n%s", recvMessage);
    close(*client_sock);

    pthread_mutex_lock(mutex);
    fp = fopen("file.txt", "a+");

    if (fp == NULL){
        printf("ERROR: could not open file.");
        pthread_mutex_unlock(mutex);
        pthread_exit(NULL);
    }

    fputs(recvMessage, fp);
    fclose(fp);
    pthread_mutex_unlock(mutex);

    return NULL;
}