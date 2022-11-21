/*Fibonacci.c: Generacion concurrente del algoritmo de Fibonacci
-> El usuario indica una cierta cantidad de valores, los cuales seran tomados como los puntos donde se detendra la serie
Ejmplo:
./a.out 10 8 5 6 20 4 ...
-> Habra tantos hilos como cantidades indicadas por el usuario
-> Los hilos no imprimen la serie, la deberan de regresar al padre para que el la imprima
*/
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
void *Fibonacci(void *param);
typedef struct estruct Serie;
struct estruct{
    int valor;
    int *serie;
};
int main(int argc, char **argv){
    if(argc < 1){printf("Error al ingresar los datos"); exit(-1);}
    int i, j, *valores, nHilos = argc - 1;
    pthread_t *hilos;
    Serie *respuesta;
    hilos = (pthread_t*)malloc(nHilos*sizeof(pthread_t));
    valores = (int*)malloc(nHilos*sizeof(int));
    for(i = 0; i < nHilos; i++){valores[i] = atoi(argv[i+1]);}
    for(i = 0; i < nHilos; i++){if(pthread_create(&hilos[i], NULL, Fibonacci, &valores[i]) != 0){printf("No se creo correctamente los hilos");}}
    for(i = 0; i < nHilos; i++){
        pthread_join(hilos[i], (void*)&respuesta);
        printf("Valor: %d\n", respuesta->valor);
        for(j = 1; j <= respuesta->valor; j++){
            printf("%d\n", respuesta->serie[j]);
        }
        free(respuesta->serie);
        free(respuesta);
    }
    free(valores);
    free(hilos);
}
void *Fibonacci(void *param){
    int *num = (int*)param;
    Serie *resultado;
    resultado = (Serie*)malloc(sizeof(Serie));
    resultado->valor = *num;
    resultado->serie = (int*)malloc((*num)*sizeof(int));
    if(*num >= 0){
        int xF, i;
        if(*num == 0 || *num == 1){xF = (*num == 0)?0:1;}
        else{
            xF = 0;
            long x1 = 0, x2 = 1;
            for (i = 1; i <= *num; i++){
                xF = x1 + x2;
                x1 = x2;
                x2 = xF;
                resultado->serie[i] = xF;
            }
        }
    }else{printf("Error\n");}
    pthread_exit(resultado);
}