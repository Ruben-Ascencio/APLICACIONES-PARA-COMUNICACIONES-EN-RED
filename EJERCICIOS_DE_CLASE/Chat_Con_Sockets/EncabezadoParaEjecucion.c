#include<stdio.h>
void Encabezado(char *Nombre){
    system("clear");
    printf("-------------------------------------------------------------------------------\n");
    printf("\tFecha y hora de ejecucion: %s : %s\n", __DATE__, __TIME__);
    printf("\tNombre del archivo: %s\n", Nombre);
    printf("-------------------------------------------------------------------------------\n");
}