#include "Listas.h"
#include <stdlib.h>

struct Entidad {
    double x,y,vel_x,vel_y,radio,dat;
    int estado;
};

struct Nodo {
    Lista_Entidad Elemento;
    struct Nodo* Sig;
};

Listas_NodoPtr Listas_crear() {
    Listas_NodoPtr p = malloc(sizeof(struct Nodo));
    p->Sig = NULL;

    return p;
}

void Listas_Insertar_crear(Listas_NodoPtr p, double x, double y, double vel_x, double vel_y, double radio, double dat, int estado) {
    Lista_Entidad e;
    e.x = x;
    e.y = y;
    e.vel_x = vel_x;
    e.vel_y = vel_y;
    e.radio = radio;
    e.dat= dat;
    e.estado = estado;

    Listas_Insertar_Inicio(p, e);
}

void Listas_Insertar_Inicio(Listas_NodoPtr p, Lista_Entidad e) {
    Listas_NodoPtr aux = malloc(sizeof(struct Nodo));
    aux->Elemento = e;
    aux->Sig = p->Sig;
    p->Sig = aux;
}

void Listas_SuprimeNodo(Listas_NodoPtr q) {
    Listas_NodoPtr aux = q->Sig;
    q->Sig = q->Sig->Sig;
    free(aux);
}

Listas_NodoPtr Listas_siguiente(Listas_NodoPtr p) {
    return p->Sig;
}

Lista_Entidad* Lista_elemento(Listas_NodoPtr p) {
    return &(p->Sig->Elemento);
}

/*
    double x,y,vel_x,vel_y,radio,dat;
    int estado;
*/

double Listas_obtener_x(Listas_NodoPtr p) {
    return p->Sig->Elemento.x;
}
double Listas_obtener_y(Listas_NodoPtr p) {
    return p->Sig->Elemento.y;
}
double Listas_obtener_vel_x(Listas_NodoPtr p) {
    return p->Sig->Elemento.vel_x;
}
double Listas_obtener_vel_y(Listas_NodoPtr p) {
    return p->Sig->Elemento.vel_y;
}
double Listas_obtener_radio(Listas_NodoPtr p) {
    return p->Sig->Elemento.radio;
}
double Listas_obtener_dat(Listas_NodoPtr p) {
    return p->Sig->Elemento.dat;
}
int Listas_obtener_estado(Listas_NodoPtr p) {
    return p->Sig->Elemento.estado;
}


void Listas_cambiar_x(Listas_NodoPtr p, double d) {
    p->Sig->Elemento.x = d;
}
void Listas_cambiar_y(Listas_NodoPtr p, double d) {
    p->Sig->Elemento.y = d;
}
void Listas_cambiar_vel_x(Listas_NodoPtr p, double d) {
    p->Sig->Elemento.vel_x = d;
}
void Listas_cambiar_vel_y(Listas_NodoPtr p, double d) {
    p->Sig->Elemento.vel_y = d;
}
void Listas_cambiar_radio(Listas_NodoPtr p, double d) {
    p->Sig->Elemento.radio = d;
}
void Listas_cambiar_dat(Listas_NodoPtr p, double d) {
    p->Sig->Elemento.dat = d;
}
void Listas_cambiar_estado(Listas_NodoPtr p, int d) {
    p->Sig->Elemento.estado = d;
}

//Lista_Entidad Listas_crear_entidad(double x, double y, double vel_x double vel_y, double radio, double dat, int estado);

void Listas_liberar(Listas_NodoPtr p) {
    while(p != NULL) {
        Listas_NodoPtr aux = p;
        p = p->Sig;
        free(aux);
    }
}
