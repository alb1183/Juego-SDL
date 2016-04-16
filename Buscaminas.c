#include "Buscaminas.h"
#include "Pantalla.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys\timeb.h>
#include <time.h>
#include <math.h>

int mapa[22][22];
int mapa_d[22][22];
int mouse_pulsado = 0;
int tecla_pulsada = 0;
double mouse_x;
double mouse_y;
int terminada;
int minas = 30;
int colores_numeros[8][3] = {{0, 0, 255},
                             {0, 130, 0},
                             {255, 0, 0},
                             {0, 0, 130},
                             {130, 0, 0},
                             {0, 130, 130},
                             {0, 0, 0},
                             {130, 130, 130}};
// Time
struct timeb start, end;
int diff;

Imagen mina_img;
Imagen mina_img_b;
Imagen mina_img_p;
Imagen bandera_img;

void Buscaminas_neighbour(int i, int j) {
    if(i >= 0 && i < 22 && j >= 0 && j < 22 && mapa[i][j] != -1)
        mapa[i][j]++;
}

void Buscaminas_iniciar() {
    mina_img = Pantalla_ImagenLee("Datos/Imagenes/mina.bmp", 0);
    mina_img_b = Pantalla_ImagenLee("Datos/Imagenes/mina_b.bmp", 0);
    mina_img_p = Pantalla_ImagenLee("Datos/Imagenes/mina_p.bmp", 0);
    bandera_img = Pantalla_ImagenLee("Datos/Imagenes/bandera.bmp", 0);

    terminada = 0;
    ftime(&start);

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
    srand(time(0));
    for(int i = 0; i < minas; i++){
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

void Buscaminas_minas_discover(int modo) {
    for(int i = 0; i < 22; i++) {
        for(int j = 0; j < 22; j++) {
            if(modo == 1) {
                if(mapa[i][j] == -1) {
                    if(mapa_d[i][j] == -1)
                        mapa[i][j] = -3;
                    mapa_d[i][j] = 0;
                }
            }else if(modo == 2) {
                if(mapa[i][j] == -1)
                    mapa_d[i][j] = -1;
                else
                    mapa_d[i][j] = 0;
            }
        }
    }
}

void Buscaminas_discover(int i, int j) {
    if(mapa_d[i][j] != 0 && i >= 0 && i < 22 && j >= 0 && j < 22) {
        if(mapa_d[i][j] != -1) {
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
}

void Buscaminas_flags(int i, int j) {
    if(mapa_d[i][j] != 0)
        mapa_d[i][j] = (mapa_d[i][j] == 1) ? -1 : 1;
}

void Buscaminas_win() {
    int fin = 1;
    for(int i = 0; i < 22 && fin == 1; i++) {
        for(int j = 0; j < 22 && fin == 1; j++) {
            if(mapa_d[i][j] == 1 && mapa[i][j] >= 0) {
                fin = 0;
            }
        }
    }

    if(fin) {
        Buscaminas_minas_discover(2);
        terminada = 2;
    }
}

void Buscaminas_click(double mouse_x, double mouse_y, int modo) {
    if(mouse_x >= 204 && mouse_x <= 644 && mouse_y >= 20 && mouse_y <= 460) {
        int i_x = (int) floor((mouse_x-204)/20.0);
        int j_y = (int) floor((mouse_y-20)/20.0);

        if(modo == 1) {
            if(mapa[i_x][j_y] == -1 && mapa_d[i_x][j_y] == 1) {
                Buscaminas_minas_discover(1);
                mapa[i_x][j_y] = -2;
                terminada = 1;
            }else if(mapa_d[i_x][j_y] == 1){
                Buscaminas_discover(i_x, j_y);
                Buscaminas_win();
            }
        } else {
            Buscaminas_flags(i_x, j_y);
        }
    }
}

void Buscaminas_loop() {
    Pantalla_RatonCoordenadas(&mouse_x, &mouse_y);

    if(Pantalla_TeclaPulsada(SDL_SCANCODE_R)) {
        if(!tecla_pulsada)
           Buscaminas_iniciar();
        tecla_pulsada = 1;
    }else if(Pantalla_TeclaPulsada(SDL_SCANCODE_UP)) {
        if(!tecla_pulsada)
            if(minas < 22*22-1)
                minas++;
        tecla_pulsada = 1;
    }else if(Pantalla_TeclaPulsada(SDL_SCANCODE_DOWN)) {
        if(!tecla_pulsada)
            if(minas > 1)
                minas--;
        tecla_pulsada = 1;
    }else if(Pantalla_TeclaPulsada(SDL_SCANCODE_W)) {
        if(!tecla_pulsada){
            Buscaminas_minas_discover(2);
            terminada = 2;
        }
        tecla_pulsada = 1;
    }else if(Pantalla_TeclaPulsada(SDL_SCANCODE_S)) {
        if(!tecla_pulsada){
            Buscaminas_minas_discover(1);
            terminada = 1;
        }
        tecla_pulsada = 1;
    }else{
        tecla_pulsada = 0;
    }

    if(Pantalla_RatonBotonPulsado(SDL_BUTTON_LEFT)) {
        if(!mouse_pulsado) {
            if(!terminada)
                Buscaminas_click(mouse_x, mouse_y, 1);
        }
        mouse_pulsado = 1;
    }else if(Pantalla_RatonBotonPulsado(SDL_BUTTON_RIGHT)) {
        if(!mouse_pulsado) {
            if(!terminada)
                Buscaminas_click(mouse_x, mouse_y, 2);
        }
        mouse_pulsado = 1;
    }else{
        mouse_pulsado = 0;
    }
}

void Buscaminas_draw() {
    if(!terminada)
        ftime(&end);
    diff = (int) end.time - start.time;
    Pantalla_ColorTrazo(0,200,255, 255);
    char texto[16];
    sprintf(texto, "Tiempo: %d", diff);
    Pantalla_DibujaTexto(texto, 20, 20);
    sprintf(texto, "Minas: %d", minas);
    Pantalla_DibujaTexto(texto, 20, 40);

    if(terminada == 1) {
        Pantalla_ColorTrazo(255,80,80, 255);
        Pantalla_DibujaTexto("Has perdido!! D:", 20, 60);
    }else if(terminada == 2) {
        Pantalla_ColorTrazo(255,80,80, 255);
        Pantalla_DibujaTexto("Has ganado!! :D", 20, 60);
    }

    // Dibujo el mapa
    for(int i = 0; i < 22; i++) {
        for(int j = 0; j < 22; j++) {
            Pantalla_ColorTrazo(150, 150, 150, 255);
            if(mapa_d[i][j] == 0) {
                Pantalla_ColorRelleno(255, 255, 255, 255);
                Pantalla_DibujaRectangulo(204+(i*20), 20+(j*20), 20,20);
                if(mapa[i][j] != 0) {
                    if(mapa[i][j] == -1){
                        Pantalla_DibujaImagen(mina_img, 204+(i*20), 20+(j*20) ,20,20);
                    }else if(mapa[i][j] == -2){
                        Pantalla_DibujaImagen(mina_img_b, 204+(i*20), 20+(j*20) ,20,20);
                    }else if(mapa[i][j] == -3){
                        Pantalla_DibujaImagen(mina_img_p, 204+(i*20), 20+(j*20) ,20,20);
                    }else{
                        char num[3];
                        sprintf(num, "%d", mapa[i][j]);
                        Pantalla_ColorTrazo(colores_numeros[mapa[i][j]-1][0], colores_numeros[mapa[i][j]-1][1], colores_numeros[mapa[i][j]-1][2], 255);
                        Pantalla_DibujaTexto(num, 210+(i*20), 23+(j*20));
                    }
                }
            }else if(mapa_d[i][j] == -1){
                Pantalla_DibujaImagen(bandera_img, 204+(i*20), 20+(j*20) ,20,20);
            }else{
                Pantalla_ColorRelleno(100, 100, 100, 255);
                Pantalla_DibujaRectangulo(204+(i*20), 20+(j*20), 20,20);
            }
        }
    }
}
