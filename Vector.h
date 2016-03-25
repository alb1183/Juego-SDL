#ifndef __VECTOR_H
#define __VECTOR_H

/**
\mainpage La biblioteca Vector es una biblioteca de vectores
*/
/**
   \file Vector.h
   \brief  TDA dedicado al calculo vectorial
*/

/**
    \brief Tipo vector
 */
typedef struct Vector* Vector;

/**
  \brief Crea un vector/punto con las coodenadas de los parametros
  \param x Coordenada x en real.
  \param y Coordenada y en real.
 */
Vector vector_new(double x, double y);

/**
  \brief Obtener las coordenadas de un vector dado
  \param v Vector.
  \param t La coordenada deseada(0 para la X, 1 para la Y).
 */
double Vector_get(Vector v, int t);

/**
  \brief Cambia las coordenadas de un vector
  \param x Coordenada x(real).
  \param y Coordenada y(real).
 */
Vector Vector_change(Vector v, double x, double y);

/**
  \brief Normaliza un vector(lo pasa a modulo 1)
  \param v Vecto a normalizar(Tipo Vector).
 */
Vector unitary_vector(Vector v);

/**
  \brief Crea el vector que une dos puntos dados
  \param inicial Punto inicial(Tipo Vector).
  \param fin Punto final(Tipo Vector).
 */
Vector points2vector(Vector inicial, Vector fin);

/**
  \brief Pasa de grados a radianes
  \param degree Grados(Real).
 */
double degree2radians(double degree);

/**
  \brief Pasa de radianes a grados
  \param radians Radianes(Real).
 */
double radians2degree(double radians);

/**
  \brief Rota un vector
  \param inicial Vector a rotar.
  \param degree Angulo a rotar en grados(Real).
 */
Vector vector_rotate(Vector inicial, double degree);

/**
  \brief Escala un vector
  \param inicial Vector a escalar.
  \param scale Escala(Real).
 */
Vector vector_scale(Vector inicial, double scale);

#endif
