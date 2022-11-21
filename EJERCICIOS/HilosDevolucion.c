/*HilosDevolucion.c: Crear n cantidad de hilos, enviarle datos a cada hilo para su impresion y devolverlo de nueva cuenta al proceso padre.
EJECUCION: ./a.out (Numero de hilos).*/
#include<unistd.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
typedef struct estructura datos;
void *saludo(void *param);
struct estructura{
	int id;
	int num;
};
int main(int argc, char **argv){
	pthread_t *hilos;
	datos *dato;
	int aux, *valor;
	//void *valor;
	if(argc != 2){printf("No ha introducido el numero de hilos"); exit(-1);}
	aux = atoi(argv[1]);
	dato = (datos*)malloc(aux*sizeof(datos));
	hilos = (pthread_t*)malloc(aux*sizeof(pthread_t));
	for(int i = 0; i < aux; i++){
		dato[i].id = i + 1;
		dato[i].num = (i + 1) * 10;
		pthread_create(&hilos[i], NULL, saludo, &dato[i]);
	}
	printf("\nEl dato de cada hilo son:\n");
	for(int i = 0; i < aux; i++){
		pthread_join(hilos[i], (void**)&valor);
		printf("%d\n", *valor);
	}
	free(hilos);
	free(dato);
}
void *saludo(void *param){
	datos *datito = (datos*)param;
	printf("Hola, Este es el hilo %d, con el numero %d\n", datito->id, datito->num);
	pthread_exit((void*)&datito->id);
}
