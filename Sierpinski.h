#ifndef __SIERPINSKI_H
#define __SIERPINSKI_H

/**
\mainpage La biblioteca Sierpinski es una biblioteca de fractales
*/
/**
   \file Sierpinski.h
   \brief  TDA dedicado a la generacion de un fractal
*/

/**
    \brief Tipo punto
 */
typedef struct punto* Punto;

/**
    \brief Tipo triangulo
 */
typedef struct TrianguloPtr* TrianguloPtr;

/**
  \brief Crea un punto
  \param x Coordenada x(real) del punto.
  \param y Coordenada y(real) del punto.
 */
Punto Punto_crea(double x, double y);

/**
  \brief Crea un triangulo del nivel inferior
  \param m Triangulo anterior.
  \param a Punto a.
  \param b Punto b.
  \param c Punto c.
  \param level Nivel de generacion.
 */
TrianguloPtr crear_triangulo(TrianguloPtr m, Punto a, Punto b,  Punto c, int level);

/**
  \brief Hace crecer el fractal
  \param m Triangulo padre.
  \param t Triangulo actual en la iteracion.
 */
void grow_triangulo(TrianguloPtr m, TrianguloPtr t);

#endif
