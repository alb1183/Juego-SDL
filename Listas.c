#include "Listas.h"
#include <stdlib.h>

struct Nodo {
    Lista_Entidad Elemento;
    struct Nodo* Sig;
};

Listas_NodoPtr Listas_crear_cadena() {
    Listas_NodoPtr p = malloc(sizeof(struct Nodo));
    p->Sig = NULL;

    return p;
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

void Listas_liberar_cadena(Listas_NodoPtr p) {
    while(p != NULL) {
        Listas_NodoPtr aux = p;
        p = p->Sig;
        free(aux);
    }
}
