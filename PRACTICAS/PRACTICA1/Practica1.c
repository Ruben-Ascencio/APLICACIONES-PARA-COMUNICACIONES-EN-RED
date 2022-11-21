/*Practica1.c: Realizar un programa que nos permita saturar el filtro RGB de las imagenes, utilizando hilos.
Para esta practica se necesita de imagenes en formato .bmp, estas imagenes deben de ser de un tamano de NxN (imagenes cuadradas). En esta carpeta se encuentran 4 imagenes para probar.
EJECUCION: ./a.out (Numero de imagenes).*/
#include<stdio.h>		//DEFINIMOS NUESTRAS LIBRERIAS
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<ctype.h>
#define TAM 255
#define nHilos 2
typedef struct Estruct2 BMP_Imagen;	//LE DAMOS UN ALIAS A NUESTRA ESTRUCTURA
struct Estruct2{			//CREAMOS NUESTRA ESTRUCTURA DONDE CONTENDRA LA INFORMACION DE CADA IMAGEN
	char *archivo;
	char bm[2];
	int tamano;
	int reservado;
	int offset;
	int tamanoMetadato;
	int alto;
	int ancho;
	short int numeroPlano;
	short int profundidad;
	int tipoCompres;
	int tamanoEstructura;
	int pixXh;
	int pixXv;
	int coloresUsados;
	int coloresImportantes;
	char **R;
	char **G;
	char **B;
	char **Rn;
	char **Gn;
	char **Bn;
};
void *construimosImagenP1(void *arg);
void *construimosImagenP2(void *arg);
int main(int argc, char **argv){
	pthread_t *hilo1, *hilo2;		//CREAMOS NUESTROS HILOS
	FILE *imagen;			//CREAMOS UN PUNTERO PARA LEER NUESTRAS IMAGENES
	int i = 0, j = 0, k = 0, nImagenes = atoi(argv[1]);
	char nombreAux[TAM];
	BMP_Imagen *datosImagen;	//CREAMOS UN PUNTERO A NUESTRA ESTRUCTURA
	if(argc != 2){printf("No ingreso el numero de archivos\n");exit(-1);}
	datosImagen = (BMP_Imagen*)malloc(nImagenes*sizeof(BMP_Imagen));
	hilo1 = (pthread_t*)malloc(nImagenes*sizeof(pthread_t));
	hilo2 = (pthread_t*)malloc(nImagenes*sizeof(pthread_t));
	//CAPTURAMOS EL NOMBRE Y LOS DATOS DE CADA IMAGEN QUE INGRESEMOS
	for(i = 0; i < nImagenes; i++){
		printf("Ingrese el nombre de la imagen %d: ",i+1);
		scanf("%s", nombreAux);
		datosImagen[i].archivo = (char*)malloc((strlen(nombreAux)+1)*sizeof(char));
		strcpy(datosImagen[i].archivo, nombreAux);
		imagen = fopen(datosImagen[i].archivo, "rb");
		if(imagen == NULL){printf("La imagen no se encuentra\n");exit(-2);}
		fseek(imagen, 0, SEEK_SET);
		fread(&datosImagen[i].bm, sizeof(char), 2, imagen);
		fread(&datosImagen[i].tamano, sizeof(int), 1, imagen);
		fread(&datosImagen[i].reservado, sizeof(int), 1, imagen);
		fread(&datosImagen[i].offset, sizeof(int), 1, imagen);
		fread(&datosImagen[i].tamanoMetadato, sizeof(int), 1, imagen);
		fread(&datosImagen[i].alto, sizeof(int), 1, imagen);
		fread(&datosImagen[i].ancho, sizeof(int), 1, imagen);
		fread(&datosImagen[i].numeroPlano, sizeof(short int), 1, imagen);
		fread(&datosImagen[i].profundidad, sizeof(short int), 1, imagen);
		fread(&datosImagen[i].tipoCompres, sizeof(int), 1, imagen);
		fread(&datosImagen[i].tamanoEstructura, sizeof(int), 1, imagen);
		fread(&datosImagen[i].pixXh, sizeof(int), 1, imagen);
		fread(&datosImagen[i].pixXv, sizeof(int), 1, imagen);
		fread(&datosImagen[i].coloresUsados, sizeof(int), 1, imagen);
		fread(&datosImagen[i].coloresImportantes, sizeof(int), 1, imagen);
		//CREAMOS ESPACIO EN MEMORIA
		datosImagen[i].R = (char**)malloc(datosImagen[i].alto*sizeof(char*));
		datosImagen[i].B = (char**)malloc(datosImagen[i].alto*sizeof(char*));
		datosImagen[i].G = (char**)malloc(datosImagen[i].alto*sizeof(char*));
		datosImagen[i].Rn = (char**)malloc(datosImagen[i].alto*sizeof(char*));
		datosImagen[i].Bn = (char**)malloc(datosImagen[i].alto*sizeof(char*));
		datosImagen[i].Gn = (char**)malloc(datosImagen[i].alto*sizeof(char*));
		for(j = 0; j < datosImagen[i].alto; j++){
			datosImagen[i].R[j] = (char*)malloc(datosImagen[i].ancho*sizeof(char));
			datosImagen[i].G[j] = (char*)malloc(datosImagen[i].ancho*sizeof(char));
			datosImagen[i].B[j] = (char*)malloc(datosImagen[i].ancho*sizeof(char));
			datosImagen[i].Rn[j] = (char*)malloc(datosImagen[i].ancho*sizeof(char));
			datosImagen[i].Gn[j] = (char*)malloc(datosImagen[i].ancho*sizeof(char));
			datosImagen[i].Bn[j] = (char*)malloc(datosImagen[i].ancho*sizeof(char));
		}
		//LEEMOS LA MATRIZ DE PIXELES DE NUESTRA IMAGEN, ALMACENANDO EL COLOR DE CADA PIXEL EN FORMATO HEXADECIMAL
		for(j = 0; j < datosImagen[i].alto; j++){
			for(k = 0; k < datosImagen[i].ancho; k++){
				fread(&datosImagen[i].B[j][k], sizeof(char), 1, imagen);
				fread(&datosImagen[i].G[j][k], sizeof(char), 1, imagen);
				fread(&datosImagen[i].R[j][k], sizeof(char), 1, imagen);
				datosImagen[i].Rn[j][k] = datosImagen[i].R[j][k] * 0;
				datosImagen[i].Gn[j][k] = datosImagen[i].G[j][k] * 0;
				datosImagen[i].Bn[j][k] = datosImagen[i].B[j][k] * 0;
			}
		}
		fclose(imagen);
	}
	//CICLO DONDE UTILIZAMOS NUESTROS DOS HILOS POR CADA IMAGEN
	for(i = 0; i < nImagenes; i++){
		pthread_create(&hilo1[i], NULL, construimosImagenP1, (void*)&datosImagen[i]);
		pthread_create(&hilo2[i], NULL, construimosImagenP2, (void*)&datosImagen[i]);
	}
	//CICLO DONDE ESPERAMOS A QUE TERMINEN NUESTROS HILOS
	for(i = 0; i < nImagenes; i++){
		pthread_join(hilo1[i], NULL);
		pthread_join(hilo2[i], NULL);
	}
	//LIBERAMOS EL ESPACIO DE NUESTRAS ESTRUCTURAS
	free(datosImagen);
	free(hilo1);
	free(hilo2);
	printf("IMAGEN(ES) .BPM CREADA CON EXITO!!\n");
}
void *construimosImagenP2(void *arg){		//METODO QUE EJECUTA EL HILO 2, AQUI CREAMOS NUESTRAS IMAGENES CON EL FILTRO R Y B.
	BMP_Imagen *aux = ((BMP_Imagen*)arg);
	FILE *a2, *a3;
	int i , j;
	char *nombre1, *nombre2;		//PUNTEROS UTILIZADOS PARA PONERLE UN NOMBRE A CADA ARCHIVO
	nombre1 = (char*)malloc((strlen("R_")+strlen(aux->archivo)+1)*sizeof(char));
	nombre2 = (char*)malloc((strlen("B_")+strlen(aux->archivo)+1)*sizeof(char));
	strcpy(nombre1, "R_");
	strcat(nombre1, aux->archivo);
	strcpy(nombre2, "B_");
	strcat(nombre2, aux->archivo);
	a2 = fopen(nombre1, "wb");		//ABRIMOS LOS ARCHIVOS
	a3 = fopen(nombre2, "wb");
	if(a2 == NULL){fclose(a2);printf("No se pudo crear la imagen roja\n");exit(-5);}
	if(a3 == NULL){fclose(a3);printf("No se pudo crear la imagen azul\n");exit(-6);}
	//ESCRIBIMOS EL ENCABEZADO Y LA MATRIZ DE PIXELES PARA CREAR NUESTRA IMAGEN (CON FILTRO ROJO)
	fseek(a2, 0, SEEK_SET);
	fwrite(&aux->bm, sizeof(char), 2, a2);
	fwrite(&aux->tamano, sizeof(int), 1, a2);
	fwrite(&aux->reservado, sizeof(int), 1, a2);
	fwrite(&aux->offset, sizeof(int), 1, a2);
	fwrite(&aux->tamanoMetadato, sizeof(int), 1, a2);
	fwrite(&aux->alto, sizeof(int), 1, a2);
	fwrite(&aux->ancho, sizeof(int), 1, a2);
	fwrite(&aux->numeroPlano, sizeof(short int), 1, a2);
	fwrite(&aux->profundidad, sizeof(short int), 1, a2);
	fwrite(&aux->tipoCompres, sizeof(int), 1, a2);
	fwrite(&aux->tamanoEstructura, sizeof(int), 1, a2);
	fwrite(&aux->pixXh, sizeof(int), 1, a2);
	fwrite(&aux->pixXv, sizeof(int), 1, a2);
	fwrite(&aux->coloresUsados, sizeof(int), 1, a2);
	fwrite(&aux->coloresImportantes, sizeof(int), 1, a2);
	for(i = 0; i < aux->alto; i++){		//ESCRIBIMOS LA MATRIZ DE PIXELES PARA CREAR NUESTRA IMAGEN (AQUI HACEMOS EL FILTRO ROJO)
		for(j = 0; j < aux->ancho; j++){
			fwrite(&aux->Bn[i][j], sizeof(char), 1, a2);
			fwrite(&aux->Gn[i][j], sizeof(char), 1, a2);
			fwrite(&aux->R[i][j], sizeof(char), 1, a2);
		}
	}
	//ESCRIBIMOS EL ENCABEZADO Y LA MATRIZ DE PIXELES PARA CREAR NUESTRA IMAGEN (CON FILTRO AZUL)
	fseek(a3, 0, SEEK_SET);
	fwrite(&aux->bm, sizeof(char), 2, a3);
	fwrite(&aux->tamano, sizeof(int), 1, a3);
	fwrite(&aux->reservado, sizeof(int), 1, a3);
	fwrite(&aux->offset, sizeof(int), 1, a3);
	fwrite(&aux->tamanoMetadato, sizeof(int), 1, a3);
	fwrite(&aux->alto, sizeof(int), 1, a3);
	fwrite(&aux->ancho, sizeof(int), 1, a3);
	fwrite(&aux->numeroPlano, sizeof(short int), 1, a3);
	fwrite(&aux->profundidad, sizeof(short int), 1, a3);
	fwrite(&aux->tipoCompres, sizeof(int), 1, a3);
	fwrite(&aux->tamanoEstructura, sizeof(int), 1, a3);
	fwrite(&aux->pixXh, sizeof(int), 1, a3);
	fwrite(&aux->pixXv, sizeof(int), 1, a3);
	fwrite(&aux->coloresUsados, sizeof(int), 1, a3);
	fwrite(&aux->coloresImportantes, sizeof(int), 1, a3);
	for(i = 0; i < aux->alto; i++){			//ESCRIBIMOS LA MATRIZ DE PIXELES PARA CREAR NUESTRA IMAGEN (AQUI HACEMOS EL FILTRO AZUL)
		for(j = 0; j < aux->ancho; j++){
			fwrite(&aux->B[i][j], sizeof(char), 1, a3);
			fwrite(&aux->Gn[i][j], sizeof(char), 1, a3);
			fwrite(&aux->Rn[i][j], sizeof(char), 1, a3);
		}
	}
	//LIBERAMOS MEMORIA
	free(nombre1);
	free(nombre2);
	fclose(a2);
	fclose(a3);
	pthread_exit(NULL);
}
void *construimosImagenP1(void *arg){		//METODO QUE EJECUTA EL HILO 2, AQUI CREAMOS NUESTRAS IMAGENES CON EL FILTRO G
	BMP_Imagen *aux = ((BMP_Imagen*)arg);
	FILE *a1;
	int i , j;
	char *nombre;
	nombre = (char*)malloc((strlen("G_")+strlen(aux->archivo)+1)*sizeof(char));
	strcpy(nombre, "G_");
	strcat(nombre, aux->archivo);
	a1 = fopen(nombre, "wb");
	if(a1 == NULL){fclose(a1);printf("No se pudo crear la imagen verde\n");exit(-4);}
	//ESCRIBIMOS EL ENCABEZADO Y LA MATRIZ DE PIXELES PARA CREAR NUESTRA IMAGEN (CON FILTRO VERDE)
	fseek(a1, 0, SEEK_SET);
	fwrite(&aux->bm, sizeof(char), 2, a1);
	fwrite(&aux->tamano, sizeof(int), 1, a1);
	fwrite(&aux->reservado, sizeof(int), 1, a1);
	fwrite(&aux->offset, sizeof(int), 1, a1);
	fwrite(&aux->tamanoMetadato, sizeof(int), 1, a1);
	fwrite(&aux->alto, sizeof(int), 1, a1);
	fwrite(&aux->ancho, sizeof(int), 1, a1);
	fwrite(&aux->numeroPlano, sizeof(short int), 1, a1);
	fwrite(&aux->profundidad, sizeof(short int), 1, a1);
	fwrite(&aux->tipoCompres, sizeof(int), 1, a1);
	fwrite(&aux->tamanoEstructura, sizeof(int), 1, a1);
	fwrite(&aux->pixXh, sizeof(int), 1, a1);
	fwrite(&aux->pixXv, sizeof(int), 1, a1);
	fwrite(&aux->coloresUsados, sizeof(int), 1, a1);
	fwrite(&aux->coloresImportantes, sizeof(int), 1, a1);
	for(i = 0; i < aux->alto; i++){			//ESCRIBIMOS LA MATRIZ DE PIXELES PARA CREAR NUESTRA IMAGEN (AQUI HACEMOS EL FILTRO VERDE)
		for(j = 0; j < aux->ancho; j++){
			fwrite(&aux->Bn[i][j], sizeof(char), 1, a1);
			fwrite(&aux->G[i][j], sizeof(char), 1, a1);
			fwrite(&aux->Rn[i][j], sizeof(char), 1, a1);
		}
	}
	fclose(a1);
	free(nombre);
	pthread_exit(NULL);
}
