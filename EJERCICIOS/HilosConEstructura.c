/*HilosConEstructura.c: Crear n cantidad de hilos. A cada hilo se le enviara dos parametros dentro de una estructura y los imprimira.
EJECUCION: ./a.out (Numero de hilos a crear).*/
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
typedef struct estructura datos;
void *saludo(void *param);
struct estructura{
	int id;
	char *caract;
};
int main(int argc, char **argv){
	pthread_t *hilos;
	datos *dato;
	int aux;
	char *mensaje = "Este es mi mensaje";
	if(argc != 2){printf("No ha introducido el numero de hilos"); exit(-1);}
	aux = atoi(argv[1]);
	dato = (datos*)malloc(aux*sizeof(datos));
	hilos = (pthread_t*)malloc(aux*sizeof(pthread_t));
	for(int i = 0; i < aux; i++){
		dato[i].id = i + 1;
		dato[i].caract = mensaje;
		pthread_create(&hilos[i], NULL, saludo, (void*)&dato[i]);
	}
	for(int i = 0; i < aux; i++){pthread_join(hilos[i], NULL);}
	free(dato);
	free(hilos);
}
void *saludo(void *param){
	datos *datito = (datos*)param;
	printf("Este hilo es %d, con el mensaje: %s\n", datito->id, datito->caract);
	pthread_exit(NULL);
}
