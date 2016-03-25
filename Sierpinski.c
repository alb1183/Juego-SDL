#include "Sierpinski.h"
#include "Pantalla.h"
#include <stdlib.h>
#include <math.h>

struct punto {
    double x,y;
};

struct TrianguloPtr {
    int count,level;
    Punto a,b,c;
    struct TrianguloPtr* padre;
    struct TrianguloPtr* ap;
    struct TrianguloPtr* bp;
    struct TrianguloPtr* cp;
};

int triang_level;
int tiempo = 30; // Tiempo entre iteracion(en ciclos)
int niveles = 6; // Maximo de niveles en la iteracion

Punto Punto_crea(double x, double y) {
    Punto punto = malloc(sizeof(struct punto));
    punto->x = x;
    punto->y = y;
    return punto;
}

Punto PuntoMedio(Punto a, Punto b) {
    return Punto_crea((a->x + b->x)/2, (a->y + b->y)/2);
}

TrianguloPtr crear_triangulo(TrianguloPtr m, Punto a, Punto b,  Punto c, int level) {
    TrianguloPtr p = malloc(sizeof(struct TrianguloPtr));
    p->count = 0;
    p->level = level;
    p->a = a;
    p->b = b;
    p->c = c;
    p->padre = m;
    p->ap = NULL;
    p->bp = NULL;
    p->cp = NULL;

    return p;
}

void libera_triangulo(TrianguloPtr t) {
    if(t->padre != NULL){
        TrianguloPtr aux = t->padre;
        free(aux->ap);
        free(aux->bp);
        free(aux->cp);
        libera_triangulo(aux);
    }else{
        free(t);
    }

}


/*void grow_triangulo(TrianguloPtr m, TrianguloPtr t) {
    Punto a = t->a;
    Punto b = t->b;
    Punto c = t->c;
    int count = t->count;
    int level = t->level;
    Pantalla_DibujaLinea(a.x, a.y, b.x, b.y);
    Pantalla_DibujaLinea(a.x, a.y, c.x, c.y);
    Pantalla_DibujaLinea(b.x, b.y, c.x, c.y);

    if(count <= 30){
        t->count++;
    }else if(t->ap == NULL && t->bp == NULL && t->cp == NULL) {
        if(level < 7) {
            t->ap = crear_triangulo(t, a, PuntoMedio(a, b), PuntoMedio(a, c), level+1);
            t->bp = crear_triangulo(t, PuntoMedio(b, a), b, PuntoMedio(b, c), level+1);
            t->cp = crear_triangulo(t, PuntoMedio(a, c), PuntoMedio(b, c), c, level+1);
            triang_level = level+1;
        }
    }

    if(t->ap != NULL && t->bp != NULL && t->cp != NULL) {
        grow_triangulo(m, t->ap);
        grow_triangulo(m, t->bp);
        grow_triangulo(m, t->cp);
    }
}*/

void grow_triangulo(TrianguloPtr m, TrianguloPtr t) {
    // Dibujo el triangulo inicial solo en el caso base
    if(m == t) {
        Pantalla_DibujaLinea(m->a->x, m->a->y, m->b->x, m->b->y);
        Pantalla_DibujaLinea(m->a->x, m->a->y, m->c->x, m->c->y);
        Pantalla_DibujaLinea(m->b->x, m->b->y, m->c->x, m->c->y);
    }

    Punto a = t->a;
    Punto b = t->b;
    Punto c = t->c;
    Punto ap = PuntoMedio(a, b);
    Punto bp = PuntoMedio(a, c);
    Punto cp = PuntoMedio(b, c);
    int count = t->count;
    int level = t->level;
    Pantalla_DibujaLinea(ap->x, ap->y, bp->x, bp->y);
    Pantalla_DibujaLinea(ap->x, ap->y, cp->x, cp->y);
    Pantalla_DibujaLinea(bp->x, bp->y, cp->x, cp->y);

    if(count <= tiempo){
        t->count++;
    }else if(t->ap == NULL && t->bp == NULL && t->cp == NULL) {
        if(level < niveles) {
            t->ap = crear_triangulo(t, a, ap, bp, level+1);
            t->bp = crear_triangulo(t, ap, b, cp, level+1);
            t->cp = crear_triangulo(t, bp, cp, c, level+1);
        }
    }

    if(t->ap != NULL && t->bp != NULL && t->cp != NULL) {
        grow_triangulo(m, t->ap);
        grow_triangulo(m, t->bp);
        grow_triangulo(m, t->cp);
    }
}
