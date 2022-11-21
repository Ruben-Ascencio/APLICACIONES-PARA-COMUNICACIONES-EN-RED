/*LeemosArchivo.c: Ingresamos la cantidad de archivos a abrir y la cantidad de palabras a buscar, cada busqueda le correspondera un hilo.
EJECICION: ./a.out (Numero de archivos) (Numero de palabras).*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<ctype.h>
#define TAM 255
typedef struct Estructura PTH_Hilo;
struct Estructura{
	int id;
	char *archivo;
};
typedef struct Respuesta PTH_Respuesta;
struct Respuesta{
	int nPalabras;
	int *nCoincidencias;
};
char **palabras;
int nPalabras;
void *buscamosPalabras(void *arg);
int main(int argc, char **argv){
	pthread_t *hilos;
	int nHilos = atoi(argv[1]), i, j;
	char nombreAux[TAM];
	PTH_Hilo *datoArchivo;
	PTH_Respuesta *retorno;
	nPalabras = atoi(argv[2]);
	if(argc != 3){
		printf("No ha ingresado el numero de hilos\n");
		exit(-1);
	}
	hilos = (pthread_t*)malloc(nHilos*sizeof(pthread_t));
	palabras = (char**)malloc(nPalabras*sizeof(char*));
	datoArchivo = (PTH_Hilo*)malloc(nHilos*sizeof(PTH_Hilo));
	for(i = 0; i < nHilos; i++){
		printf("Ingrese el nombre del archivo %d: ", i+1);
		scanf("%s", nombreAux);
		datoArchivo[i].archivo = (char*)malloc((strlen(nombreAux)+1)*sizeof(char));
		strcpy(datoArchivo[i].archivo, nombreAux);
	}
	for(i = 0; i < nPalabras; i++){
		printf("Ingresa la palabra %d: ", i+1);
		scanf("%s", nombreAux);
		palabras[i] = (char*)malloc((strlen(nombreAux)+1)*sizeof(char));
		strcpy(palabras[i], nombreAux);
	}
	for(i = 0; i < nHilos; i++){
		datoArchivo[i].id = i;
		if(pthread_create(&hilos[i], NULL, buscamosPalabras, (void*)&datoArchivo[i]) !=0 ){
			printf("Error al crear el hilo\n");
			exit(-2);
		}
	}
	printf("\n");
	for(i = 0; i < nHilos; i++){
		pthread_join(hilos[i], (void*)&retorno);
		printf("El libro \"%s\" tiene %d palabras\n", datoArchivo[i].archivo, retorno->nPalabras);
		for(j = 0; j < nPalabras; j++){
			printf("La palabra: \"%s\". Tiene %d coincidencias\n", palabras[j], retorno->nCoincidencias[j]);
		}
		free(retorno->nCoincidencias);
		free(retorno);
	}
	free(hilos);
	free(palabras);
	free(datoArchivo);
}
void *buscamosPalabras(void *arg){
	PTH_Hilo *aux = ((PTH_Hilo*)arg);
	PTH_Respuesta *respuesta;
	FILE *archivo;
	int i, c, estado;
	char auxiliar[TAM] = "";
	respuesta = (PTH_Respuesta*)malloc(sizeof(PTH_Respuesta));
	respuesta->nPalabras = 0;
	respuesta->nCoincidencias = (int*)malloc(nPalabras*sizeof(int));
	for(i = 0; i < nPalabras; i++){respuesta->nCoincidencias[i] = 0;}
	if((archivo = fopen(aux->archivo, "r")) == NULL){exit(-3);}
	else{
		estado = 0;
		while((c = fgetc(archivo)) != EOF){
			if(isspace(c)){estado = 0;}
			else if(estado == 0 && isalpha(c)){
				estado = 1;
				respuesta->nPalabras++;
			}
		}
		rewind(archivo);
		while(!feof(archivo)){
			fscanf(archivo, "%s", auxiliar);
			for(i = 0; i < nPalabras; i++){
				if(strstr(auxiliar, palabras[i]) != NULL && !feof(archivo)){respuesta->nCoincidencias[i]++;}
			}
		}
		fclose(archivo);
	}
	pthread_exit(respuesta);
}
