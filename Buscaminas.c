#include "Buscaminas.h"
#include "Pantalla.h"
#include <stdlib.h>

int mapa[22][22];
int mapa_d[22][22];
int mouse_pulsado = 0;
double mouse_x;
double mouse_y;

void Buscaminas_neighbour(int i, int j) {
    if(i >= 0 && i < 22 && j >= 0 && j < 22 && mapa[i][j] != -1)
        mapa[i][j]++;
}

void Buscaminas_iniciar() {
    // Inicializo los mapas
    for(int i = 0; i < 22; i++) {
        for(int j = 0; j < 22; j++) {
             mapa[i][j] = 0;
             mapa_d[i][j] = 1;
             //mapa_d[i][j] = 0;
        }
    }

    // Metodo 1
    /*
    int i, random;
    srand(time(0));
    for (i = 0; i < 22; i++) {
        random = rand() % (22);
        mapa_d[random][i] = 0;
    }
    */
    // Metodo 2
    int Minas_num = 30;
    srand(time(0));
    for(int i = 0; i < Minas_num; i++){
        int row = rand() % 22;
        int col = rand() % 22;
        while(mapa[row][col] == -1){
            row = (rand() % 22);
            col = (rand() % 22);
        }
        mapa[row][col] = -1;
    }

    // Genero los contadores de minas
    for(int i = 0; i < 22; i++) {
        for(int j = 0; j < 22; j++) {
             if(mapa[i][j] == -1) {
                Buscaminas_neighbour(i-1, j+1);
                Buscaminas_neighbour(i-1, j);
                Buscaminas_neighbour(i-1, j-1);


                Buscaminas_neighbour(i, j+1);
                Buscaminas_neighbour(i, j-1);

                Buscaminas_neighbour(i+1, j+1);
                Buscaminas_neighbour(i+1, j);
                Buscaminas_neighbour(i+1, j-1);
             }
        }
    }
}

void Buscaminas_discover(int i, int j) {
    if(mapa_d[i][j] != 0 && i >= 0 && i < 22 && j >= 0 && j < 22) {
        mapa_d[i][j] = 0;
        if(mapa[i][j] == 0) {
            Buscaminas_discover(i-1, j-1);
            Buscaminas_discover(i-1, j);
            Buscaminas_discover(i-1, j+1);

            Buscaminas_discover(i, j-1);
            Buscaminas_discover(i, j+1);

            Buscaminas_discover(i+1, j-1);
            Buscaminas_discover(i+1, j);
            Buscaminas_discover(i+1, j+1);
        }
    }
}

void Buscaminas_click(double mouse_x, double mouse_y) {
    int i_x = (int) floor((mouse_x-204)/20.0);
    int j_y = (int) floor((mouse_y-20)/20.0);

    Buscaminas_discover(i_x, j_y);
}

void Buscaminas_loop() {
    Pantalla_RatonCoordenadas(&mouse_x, &mouse_y);
    if(Pantalla_RatonBotonPulsado(SDL_BUTTON_LEFT)) {
        if(!mouse_pulsado) {
            Buscaminas_click(mouse_x, mouse_y);
        }
        mouse_pulsado = 1;
    }else{
        mouse_pulsado = 0;
    }
}

void Buscaminas_draw() {
    for(int i = 0; i < 22; i++) {
        for(int j = 0; j < 22; j++) {
            Pantalla_ColorTrazo(150, 150, 150, 255);
            if(mapa_d[i][j] == 0) {
                Pantalla_ColorRelleno(255, 255, 255, 255);
                Pantalla_DibujaRectangulo(204+(i*20), 20+(j*20), 20,20);
                if(mapa[i][j] != 0) {
                    char num[3];
                    sprintf(num, "%d", mapa[i][j]);
                    Pantalla_DibujaTexto(num, 208+(i*20), 24+(j*20));
                }
            }else{
                Pantalla_ColorRelleno(80, 80, 80, 255);
                Pantalla_DibujaRectangulo(204+(i*20), 20+(j*20), 20,20);
            }
        }
    }
}
