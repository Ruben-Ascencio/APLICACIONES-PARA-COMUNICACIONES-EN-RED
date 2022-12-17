#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<errno.h>

extern int errno; //para cachar el numero de error generado

#define RCVBUFFER 100

int main (int argc, char **argv)
{
  int sock;
  struct sockaddr_in broadcastAddr;
  unsigned short puertoBroadcast;
  char buffer[RCVBUFFER];
  int tamRecv;

  //Paso 0: verificaci{on de parametros de entrada
  if(argc < 2){
    printf("Faltan parametros. Uso: 1.- Puerto");
    exit(1);
  }

  puertoBroadcast = atoi(argv[1]);

  //Paso 1: Crea el socket Datagrama/UDP
  if((sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0){
    perror("Se genero el siguiente error en SOCKET ");
    printf("\nValor de errno = %d",errno);
    exit(1);
  }

  //Paso 2: LLenado de la estructura
  memset(&broadcastAddr,0,sizeof(broadcastAddr));
  broadcastAddr.sin_family = AF_INET;
  broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  broadcastAddr.sin_port = htons(puertoBroadcast);

  //Paso 3: Publicacion del puerto broadcast
  if(bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0){
    perror("Se genero el siguiente error en BIND ");
    printf("\nValor de errno = %d",errno);
    exit(1);
  }

  //paso 4: evitar que muera el receiver
  while(1){
    if((tamRecv = recvfrom(sock,buffer,RCVBUFFER, 0, NULL,0)) < 0){
      perror("Se genero el siguiente error en RECvFROM ");
      printf("\nValor de errno = %d",errno);
      exit(1);
    }

    buffer[tamRecv] = '\0';
    printf("\nMensaje = %s\n",buffer);
  }
  close(sock);
  exit(0);

}
