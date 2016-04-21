#include "Particulas.h"
#include "Pantalla.h"
#include "Vector.h"
#include <stdlib.h>
#include <math.h>

struct particula {
    double x,y,vel_x,vel_y,radio;
    int estado;
};
typedef struct particula Particula;

struct Nodo {
    Particula Elemento;
    struct Nodo* Sig;
};
typedef struct Nodo* ParticulasPtr;

int pantalla_alto;
int pantalla_ancho;
ParticulasPtr Cadena_particulas;

void Particulas_crea(int alto, int ancho) {
    ParticulasPtr p = malloc(sizeof(struct Nodo));
    p->Sig = NULL;
    pantalla_alto = alto;
    pantalla_ancho = ancho;

    Cadena_particulas = p;
}

void Particulas_Insertar_Inicio(ParticulasPtr p, Particula e) {
    ParticulasPtr aux = malloc(sizeof(struct Nodo));
    aux->Elemento = e;
    aux->Sig = p->Sig;
    p->Sig = aux;
}
void Particulas_Suprimir(ParticulasPtr q) {
    ParticulasPtr aux = q->Sig;
    q->Sig = q->Sig->Sig;
    free(aux);
}

void Particulas_insertar(int tipo, int puntos) {
    if(tipo == 0) {
        Particula particula;
        particula.estado = rand()%1786;
        particula.radio = 1+rand()%5;
        particula.x = 0;
        particula.y = rand()%pantalla_alto;

        double vel_mult = (puntos < 80) ? 1 : (puntos/60);
        particula.vel_x = (1+rand()%8) * vel_mult;
        particula.vel_y = 0;

        if(rand()%8 == 1)
            particula.vel_y = -4+rand()%9;

        Particulas_Insertar_Inicio(Cadena_particulas, particula);
    }
}


void Particulas_dibuja(int puntos) {
    ParticulasPtr q = Cadena_particulas;
    while(q->Sig != NULL) {
        double pos_x = q->Sig->Elemento.x;
        double pos_y = q->Sig->Elemento.y;
        double radio = q->Sig->Elemento.radio;
        Vector vel_v = Vector_unitary(Vector_new(q->Sig->Elemento.vel_x, q->Sig->Elemento.vel_y));
        double pos_x_2 = pos_x + (Vector_get(vel_v, 0) * radio);
        double pos_y_2 = pos_y + (Vector_get(vel_v, 1) * radio);

        int estado = q->Sig->Elemento.estado;
        int color_vel = 0;
        if(puntos < 80) {
            color_vel = 0;
        }else if(puntos < 100) {
            color_vel = 1;
        }else if (puntos < 120) {
            color_vel = 5;
        }else if (puntos < 140) {
            color_vel = 7;
        }else if (puntos < 160) {
            color_vel = 15;
        }else if (puntos < 180) {
            color_vel = 17;
        }else {
            color_vel = 21;
        };
        q->Sig->Elemento.estado = (q->Sig->Elemento.estado+color_vel) % 1786;
        int red = 0;
        int green = 0;
        int blue = 0;
        if(estado <= 255) {
            red = 255;
            green = 0;
            blue = estado;
        }else if(estado <= 510) {
            red = 765-estado;
            green = 0;
            blue = 255;
        }else if(estado <= 765) {
            red = 0;
            green = estado-765;
            blue = 255;
        }else if(estado <= 1020) {
            red = 0;
            green = 255;
            blue = 1275-estado;
        }else if(estado <= 1275) {
            red = estado-1275;
            green = 255;
            blue = 0;
        }else if(estado <= 1530) {
            red = 255;
            green = 1785-estado;
            blue = 0;
        }
        if(color_vel == 0)
            Pantalla_ColorTrazo(255, 255, 255, 255);
            //Pantalla_ColorTrazo(222, 222, 222, 255);
        else
            Pantalla_ColorTrazo(red, green, blue, 255);
        Pantalla_DibujaLinea(pos_x, pos_y, pos_x_2, pos_y_2);
        q->Sig->Elemento.x += q->Sig->Elemento.vel_x;
        q->Sig->Elemento.y += q->Sig->Elemento.vel_y;
        // Mueve la particula de forma aleatoria cada 1/25
        if(rand()%25 == 1)
            q->Sig->Elemento.vel_y = -4+rand()%9;

        if(pos_x > pantalla_ancho || pos_x < 0 || pos_y > pantalla_alto || pos_y < 0)
            Particulas_Suprimir(q);
        else
            q = q->Sig;

    }
    Pantalla_ColorTrazo(0,0,255, 255); // Color de los trazos default
}


void Particulas_libera() {
    ParticulasPtr p = Cadena_particulas;
    while(p != NULL) {
        ParticulasPtr aux = p;
        p = p->Sig;
        free(aux);
    }
}
