#include "Listas.h"
#include <stdlib.h>

NodoPtr crear_cadena() {
    NodoPtr p = malloc(sizeof(struct Nodo));
    p->Sig = NULL;

    return p;
}

void Insertar_Inicio(NodoPtr p, Entidad e) {
    NodoPtr aux = malloc(sizeof(struct Nodo));
    aux->Elemento = e;
    aux->Sig = p->Sig;
    p->Sig = aux;
}

void SuprimeNodo(NodoPtr q) {
    NodoPtr aux = q->Sig;
    q->Sig = q->Sig->Sig;
    free(aux);
}

void liberar_cadena(NodoPtr p) {
    while(p != NULL) {
        NodoPtr aux = p;
        p = p->Sig;
        free(aux);
    }
}
