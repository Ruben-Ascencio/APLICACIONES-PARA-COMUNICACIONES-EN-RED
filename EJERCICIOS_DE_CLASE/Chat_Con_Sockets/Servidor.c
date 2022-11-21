/*Servidor.c: Creacion de chats sockets TCP/IP.
-> El servidor estara siempre en ejecucion esperando a que un cliente se conecte, el servidor tambien podra enviar mensajes
EJECUCION: ./server (Direccion IP) (Puerto)
EJEMPLO: ./server 127.0.0.1 8080
*/
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "EncabezadoParaEjecucion.c"
#define RESTABLECER "\x1b[0m"								//RESTABLECE COLOR
#define BLANCO "\x1b[37m"									//COLOR BLANCO
#define VERDE "\x1b[32m"									//COLOR VERDE
#define CYAN "\x1b[36m"										//COLOR CYAN
#define ROJO "\x1b[31m"										//COLOR ROJO
#define AMARILLO "\x1b[33m"									//COLOR AMARILLO
#define AZUL "\x1b[34m"										//COLOR AZUL
#define MAGENTA "\x1b[35m"									//MAGENTA
void Encabezado(char *Nombre);
int main(int argc, char **argv){
    int puerto, idSocket, sizeCliente, idAccept, idRecv, idSend, idListen;
    unsigned char mensaje[100], buffer[100];
    Encabezado(__FILE__);
    if(argc != 3){printf("Faltan parametros (Dirección IP y puertos)\n");exit(-1);}
    idSocket = socket(AF_INET, SOCK_STREAM, 0);                                         //1. Crear el socket -> SOCK_STREAM: TCP, SOCK_DGRAM: UDP
    if(idSocket < 0){printf("Error al crear el socket\n");exit(-1);}
    puerto = atoi(argv[2]);
    struct sockaddr_in servidor, cliente;
    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = inet_addr(argv[1]);
    servidor.sin_port = htons(puerto);
    int idBind = bind(idSocket, (struct sockaddr *) &servidor, sizeof(servidor));       //2. Bind (asociar el socket a una dirección IP y puerto)
    if(idBind < 0){printf("Error al asociar el socket a la dirección\n");exit(-1);}
    idListen = listen(idSocket, 5);                                         //3. Escuchar conexiones entrantes
    if(idListen < 0){printf("Error al escuchar conexiones entrantes\n");exit(-1);}
    sizeCliente = sizeof(cliente);
    idAccept = accept(idSocket, (struct sockaddr *) &cliente, &sizeCliente);    // accept: crea un canal vitual para la comunicación con el cliente por cada cliente que se conecte
    if(idAccept < 0){printf("Error al aceptar conexiones entrantes\n");exit(-1);}
    while(1){
        idRecv = recv(idAccept, buffer, sizeof(buffer), 0);                 //7. Fase de comunicación con el cliente
        if(idRecv <= 0){printf("Error al recibir el mensaje\n");exit(-1);}
        printf(VERDE "C << %s\n" RESTABLECER, buffer);
        if(strcmp(buffer, "exit") == 0){
            strcpy(mensaje, "Hasta luego!");
            idSend = send(idAccept, mensaje, strlen(mensaje), 0);
            if(idSend <= 0){printf("Error al enviar el mensaje\n");exit(-1);}
            close(idAccept);                                                           //Cerramos canal de comunicacion
            sizeCliente = sizeof(cliente);
            idAccept = accept(idSocket, (struct sockaddr *) &cliente, &sizeCliente);    // accept: crea un canal vitual para la comunicación con el cliente por cada cliente que se conecte
            if(idAccept < 0){printf("Error al aceptar conexiones entrantes\n");exit(-1);}
        }else{
            printf(AMARILLO "S >> " RESTABLECER);
            gets(mensaje);
            //scanf("%s", mensaje);
            idSend = send(idAccept, mensaje, strlen(mensaje), 0);           //8. Enviar datos al cliente
            if(idSend <= 0){printf("Error al enviar el mensaje\n");exit(-1);}
        }
        bzero(mensaje, 100);
        bzero(buffer, 100);
    }
}