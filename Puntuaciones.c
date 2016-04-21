#include "Puntuaciones.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Puntuaciones {
    int puntos;
    char nombre[26];
};

const char* data_text_puntos = "Datos/Puntuaciones.txt";
Puntuaciones puntos_array_f;
int puntuaciones_num;
int puntuacion_escrita = 0;

Puntuaciones ordenar_puntuaciones(Puntuaciones puntuaciones_array, int longitud) {
    // TODO: Intentar hacerlo unicamente por apuntadores
    for(int j = 0; j < longitud-1; j++) {
        int minimo_valor = puntuaciones_array[j].puntos;
        int minimo_posicion = j;
        char minimo_nombre[26];
        strcpy(minimo_nombre, puntuaciones_array[j].nombre);
        for(int i = j+1; i < longitud; i++) {
            if(puntuaciones_array[i].puntos > minimo_valor) {
                minimo_valor = puntuaciones_array[i].puntos;
                strcpy(minimo_nombre, puntuaciones_array[i].nombre);
                minimo_posicion = i;
            }
        }
        puntuaciones_array[minimo_posicion] = puntuaciones_array[j];
        puntuaciones_array[j].puntos = minimo_valor;
        strcpy(puntuaciones_array[j].nombre, minimo_nombre);
    }
    return puntuaciones_array;
}

void Puntuaciones_leer() {
    FILE * f = fopen(data_text_puntos,"r");

    int longitud = 0;
    char buff[32];

    // TODO: Metodo un poco raro, O(2n) intentar O(n)
    while(fgets(buff, 32, f)) {
        longitud++;
    }

    rewind(f);
    Puntuaciones puntos_array = malloc(sizeof(struct Puntuaciones) * longitud);

    for(int i = 0; i < longitud; i++) {
        char nombre[26];
        int puntos;
        fscanf(f, "%s %d", nombre, &puntos);
        strcpy(puntos_array[i].nombre, nombre);
        puntos_array[i].puntos = puntos;
    }
    fclose(f);
    puntos_array_f = ordenar_puntuaciones(puntos_array, longitud);
    puntuaciones_num = longitud;
}

void Puntuaciones_escribir(char nombre[26], int puntos) {
    if(!puntuacion_escrita) {
        puntuacion_escrita = 1;
        FILE * f_o = fopen(data_text_puntos,"a");
        fprintf(f_o, "\n%s %d", nombre, puntos);
        fclose(f_o);
    }
}

int Puntuaciones_numero() {
    return puntuaciones_num;
}

char* Puntuaciones_array_nombre(int i) {
    return puntos_array_f[i].nombre;
}

int Puntuaciones_array_puntos(int i) {
    return puntos_array_f[i].puntos;
}

void Puntuaciones_liberar() {
    free(puntos_array_f);
}
