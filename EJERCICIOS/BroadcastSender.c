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
  char buffer[RCVBUFFER], *IPBroadcast;
  int permisoBroadcast, tamMensaje;

  //Paso 0: verificaci{on de parametros de entrada
  if(argc < 4){
    printf("Faltan parametros. Uso: 1.- IP 2.- Puerto 3.- Mensaje");
    exit(1);
  }

  IPBroadcast = argv[1];
  puertoBroadcast = atoi(argv[2]);
  strcpy(buffer,argv[3]);
  tamMensaje = strlen(buffer);

  //Paso 1: Crea el socket Datagrama/UDP
  if((sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0){
    perror("Se genero el siguiente error en SOCKET ");
    printf("\nValor de errno = %d",errno);
    exit(1);
  }

  //paso 2: Permitirle al socket realizar broadcast
  permisoBroadcast = 1;
  if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *)&permisoBroadcast, sizeof(permisoBroadcast)) < 0){
    perror("Se genero un error en Setsockopt");
    printf("\nValor de errno = %d",errno);
    exit(1);
  }

  //Paso 3: LLenar la estructura de la direcci{on
  memset(&broadcastAddr,0,sizeof(broadcastAddr));
  broadcastAddr.sin_family = AF_INET;
  broadcastAddr.sin_addr.s_addr = inet_addr(IPBroadcast);
  broadcastAddr.sin_port = htons(puertoBroadcast);

  //Paso 4: hacer que no muera el servidor
  while(1){
    //Paso 5: Se envia un broadcast cada 3 segundos a los clientes pertenecientes al grupo
    if(sendto(sock, buffer,tamMensaje,0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) != tamMensaje){
      perror("Se genero un error en Sendto");
      printf("\nValor de errno = %d",errno);
      exit(1);
    }
    sleep(3);
  }


}
