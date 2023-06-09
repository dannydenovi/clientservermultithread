#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define true 1
#define PORT 7799
#define LOCALHOST "127.0.0.1"
#define CLIENT_LIMIT 30
#define BUFFER_SIZE 2000

FILE *fp;
struct Args{
    int arg1;
    pthread_mutex_t *arg2;
};

void *routine(void *arg);

int main(){

    int socket_descriptor, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    int i = 0;
    pthread_t threads[30];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_descriptor < 0){
        printf("ERROR: Could not create socket.");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(LOCALHOST);

    bind(socket_descriptor, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (listen(socket_descriptor, CLIENT_LIMIT) < 0){
        printf("ERROR: could not listen for incoming connections.");
        return -1;
    }

    while (true){
        client_size = sizeof(client_addr);
        struct Args args = {client_sock, &mutex};
        client_sock = accept(socket_descriptor, (struct sockaddr *)&client_addr, (socklen_t *)&client_size);
        pthread_create(&threads[i], NULL, routine, (void *)&args);
        i == CLIENT_LIMIT ? i = 0 : i++;
    }

    close(socket_descriptor);
    return 0;
}

void *routine(void *arg){

    struct Args *args = (struct Args *)arg;
    int client_sock = args->arg1;
    pthread_mutex_t *mutex = args->arg2;

    char senderMessage[BUFFER_SIZE], recvMessage[BUFFER_SIZE];
    memset(senderMessage, '\0', sizeof(senderMessage));
    memset(recvMessage, '\0', sizeof(recvMessage));

    if (client_sock < 0){
        printf("ERROR: could not accept connections.");
        pthread_exit(NULL);
    }

    if (recv(client_sock, recvMessage, sizeof(recvMessage), 0) < 0){
        printf("ERROR: could not receive message.");
        pthread_exit(NULL);
    }

    strcpy(senderMessage, "Hello world!");

    if (send(client_sock, senderMessage, sizeof(senderMessage), 0) < 0){
        printf("ERROR: could not send message.");
        pthread_exit(NULL);
    }

    printf("\n%s", recvMessage);
    close(client_sock);

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