/*Hilos.c: Crear n cantidad de hilos e imprimir su respectivo ID.
EJECUCION: ./a.out (Numero de hilos a crear).*/
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
void *saludo(void *param);
int main(int argc, char **argv){
	int num;
	pthread_t *hilos;
	int aux, *parametros;
	if(argc != 2){printf("No ha introducido el numero de hilos"); exit(-1);}
	aux = atoi(argv[1]);
	parametros = (int*)malloc(aux*sizeof(int));
	hilos = (pthread_t*)malloc(aux*sizeof(pthread_t));
	for(int i = 0; i < aux; i++){
		parametros[i] = i + 1;
		pthread_create(&hilos[i], NULL, saludo, &parametros[i]);
	}
	for(int i = 0; i < aux; i++){pthread_join(hilos[i], NULL);}
	free(parametros);
	free(hilos);
}
void *saludo(void *param){
	int *dato = (int*)param;
	printf("Hola, yo soy el hilo %d\n", *dato);
	pthread_exit(NULL);
}