#ifndef __PARTICULAS_H
#define __PARTICULAS_H

/**
\mainpage La biblioteca Particula es una biblioteca de particulas
*/
/**
   \file Particulas.h
   \brief  TDA dedicado a la generacion de un minisistema de particulas
*/

/**
    \brief Tipo particula
 */
typedef struct Nodo* ParticulaPtr;

/**
  \brief Crear la cadena(estructura enlazada) que almacenara las particulas
  \param alto Alto de la pantalla.
  \param ancho Ancho de la pantalla.
 */
ParticulaPtr Particulas_cadena(int alto, int ancho);

/**
  \brief Crea una particula en la cadena especificada
  \param p Nodo cabecera de la cadena.
  \param tipo Tipo de particula(por ahora solo 0).
  \param puntos Puntos del jugador (para la velocidad).
 */
void Particulas_crear(ParticulaPtr p, int tipo, int puntos);

/**
  \brief Dibuja las particulas de una cadena
  \param p Nodo cabecera de la cadena.
  \param puntos Puntos del jugador (para activar efectos en determinada puntuacion).
 */
void Particulas_dibuja(ParticulaPtr q, int puntos);

/**
  \brief Borra la cadena y la libera de memoria
  \param p Nodo cabecera de la cadena.
 */
void Particulas_libera(ParticulaPtr p);

#endif
