#include "Sierpinski.h"
#include "Pantalla.h"
#include <stdlib.h>
#include <math.h>

struct punto {
    double x,y;
};

struct Sierpinski_TrianguloPtr {
    int count,level;
    Sierpinski_Punto a,b,c;
    struct Sierpinski_TrianguloPtr* padre;
    struct Sierpinski_TrianguloPtr* ap;
    struct Sierpinski_TrianguloPtr* bp;
    struct Sierpinski_TrianguloPtr* cp;
};

int triang_level;
int tiempo = 30; // Tiempo entre iteracion(en ciclos)
int niveles = 6; // Maximo de niveles de profundidad del arbol

Sierpinski_Punto Sierpinski_Punto_crea(double x, double y) {
    Sierpinski_Punto punto = malloc(sizeof(struct punto));
    punto->x = x;
    punto->y = y;
    return punto;
}

Sierpinski_Punto Sierpinski_PuntoMedio(Sierpinski_Punto a, Sierpinski_Punto b) {
    return Sierpinski_Punto_crea((a->x + b->x)/2, (a->y + b->y)/2);
}

Sierpinski_TrianguloPtr Sierpinski_crear_triangulo(Sierpinski_TrianguloPtr m, Sierpinski_Punto a, Sierpinski_Punto b,  Sierpinski_Punto c, int level) {
    Sierpinski_TrianguloPtr p = malloc(sizeof(struct Sierpinski_TrianguloPtr));
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

void libera_triangulo(Sierpinski_TrianguloPtr t) {
    if(t != NULL){
        libera_triangulo(t->ap);
        libera_triangulo(t->bp);
        libera_triangulo(t->cp);
        free(t);
    }
}

void Sierpinski_grow_triangulo(Sierpinski_TrianguloPtr m, Sierpinski_TrianguloPtr t) {
    // Dibujo el triangulo inicial solo en el caso base
    if(m == t) {
        Pantalla_DibujaLinea(m->a->x, m->a->y, m->b->x, m->b->y);
        Pantalla_DibujaLinea(m->a->x, m->a->y, m->c->x, m->c->y);
        Pantalla_DibujaLinea(m->b->x, m->b->y, m->c->x, m->c->y);
    }

    Sierpinski_Punto a = t->a;
    Sierpinski_Punto b = t->b;
    Sierpinski_Punto c = t->c;
    Sierpinski_Punto ap = Sierpinski_PuntoMedio(a, b);
    Sierpinski_Punto bp = Sierpinski_PuntoMedio(a, c);
    Sierpinski_Punto cp = Sierpinski_PuntoMedio(b, c);
    int count = t->count;
    int level = t->level;
    Pantalla_DibujaLinea(ap->x, ap->y, bp->x, bp->y);
    Pantalla_DibujaLinea(ap->x, ap->y, cp->x, cp->y);
    Pantalla_DibujaLinea(bp->x, bp->y, cp->x, cp->y);

    // Crear
    if(count <= tiempo){
        t->count++;
    }else if(t->ap == NULL && t->bp == NULL && t->cp == NULL) {
        if(level < niveles) {
            t->ap = Sierpinski_crear_triangulo(t, a, ap, bp, level+1);
            t->bp = Sierpinski_crear_triangulo(t, ap, b, cp, level+1);
            t->cp = Sierpinski_crear_triangulo(t, bp, cp, c, level+1);
        }else{
            //libera_triangulo(m);
        }
    }

    // Seguir recorriendo
    if(t->ap != NULL && t->bp != NULL && t->cp != NULL) {
        Sierpinski_grow_triangulo(m, t->ap);
        Sierpinski_grow_triangulo(m, t->bp);
        Sierpinski_grow_triangulo(m, t->cp);
    }
}
