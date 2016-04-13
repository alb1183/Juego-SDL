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
typedef struct punto* Sierpinski_Punto;

/**
    \brief Tipo triangulo
 */
typedef struct Sierpinski_TrianguloPtr* Sierpinski_TrianguloPtr;

/**
  \brief Crea un punto
  \param x Coordenada x(real) del punto.
  \param y Coordenada y(real) del punto.
  \return Devuelve el punto
 */
Sierpinski_Punto Sierpinski_Punto_crea(double x, double y);

/**
  \brief Crea un triangulo del nivel inferior
  \param m Triangulo anterior.
  \param a Punto a.
  \param b Punto b.
  \param c Punto c.
  \param level Nivel de generacion.
  \return Devuelve un triangulo
 */
Sierpinski_TrianguloPtr Sierpinski_crear_triangulo(Sierpinski_TrianguloPtr m, Sierpinski_Punto a, Sierpinski_Punto b, Sierpinski_Punto c, int level);

/**
  \brief Hace crecer el fractal
  \param m Triangulo padre.
  \param t Triangulo actual en la iteracion.
 */
void Sierpinski_grow_triangulo(Sierpinski_TrianguloPtr m, Sierpinski_TrianguloPtr t);

#endif
