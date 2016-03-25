#ifndef __SIERPINSKI_H
#define __SIERPINSKI_H

typedef struct punto* Punto;
typedef struct TrianguloPtr* TrianguloPtr;
Punto Punto_crea(double x, double y);
TrianguloPtr crear_triangulo(TrianguloPtr m, Punto a, Punto b,  Punto c, int level);
void grow_triangulo(TrianguloPtr m, TrianguloPtr t);


#endif
