/*Cliente.c: Creacion de chats sockets TCP/IP.
-> El cliente iniciara una conversacion con nuestro servidor hasta que el cliente ingrese la cadena 'exit'.
EJECUCION: ./cliente (Direccion IP) (Puerto)
EJEMPLO: ./cliente 127.0.0.1 8080
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
    int puerto, idSocket, idSend, idRecv, idConnect;
    unsigned char mensaje[100], buffer[100];
    Encabezado(__FILE__);
    if(argc != 3){printf("Faltan parametros (Dirección IP y puertos)\n");exit(-1);}
    idSocket = socket(AF_INET, SOCK_STREAM, 0);             //1. Crear el socket -> SOCK_STREAM: TCP, SOCK_DGRAM: UDP
    if(idSocket < 0){printf("Error al crear el socket\n");exit(-1);}
    puerto = atoi(argv[2]);                                 //2. Establecer la conexión con el servidor
    struct sockaddr_in servidor;
    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = inet_addr(argv[1]);
    servidor.sin_port = htons(puerto);
    if(connect(idSocket, (struct sockaddr *) &servidor, sizeof(servidor)) < 0){printf("Error al establecer la conexión\n");exit(-1);}
    while(1){
        printf(VERDE "C >> " RESTABLECER);
        /* scanf("%s", mensaje); */
        gets(mensaje);
        idSend = send(idSocket, mensaje, strlen(mensaje), 0);           //3. Enviar datos -> send: retorna el número de bytes enviados, length: tamaño del mensaje
        if(idSend <= 0){printf("Error al enviar el mensaje\n");exit(-1);}
        idRecv = recv(idSocket, buffer, sizeof(buffer), 0);             // 4. Recibir datos -> recv retorna el número de bytes recibidos, length: tamaño del buffer
        if(idRecv <= 0){printf("Error al recibir el mensaje\n");exit(-1);}
        printf(AMARILLO "S << %s\n" RESTABLECER, buffer);
        if(strcmp(mensaje, "exit") == 0 && strcmp(buffer, "Hasta luego!") == 0){break;}
        bzero(mensaje, 100);
        bzero(buffer, 100);
    }
    close(idSocket);        //Cerramos socket
}