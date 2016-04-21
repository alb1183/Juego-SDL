#ifndef __PARTICULAS_H
#define __PARTICULAS_H

/**
\mainpage La biblioteca Particula es una biblioteca de particulas
*/
/**
   \file Particulas.h
   \brief  TDA dedicado a la generacion de un minisistema de particulas.
*/

/**
  \brief Crear la cadena(estructura enlazada) que almacenara las particulas.
  \param alto Alto de la pantalla.
  \param ancho Ancho de la pantalla.
 */
void Particulas_crea(int alto, int ancho);

/**
  \brief Crea una particula.
  \param tipo Tipo de particula(por ahora solo 0).
  \param puntos Puntos del jugador (para la velocidad).
 */
void Particulas_insertar(int tipo, int puntos);

/**
  \brief Dibuja las particulas de una cadena.
  \param puntos Puntos del jugador (para activar efectos en determinada puntuacion).
 */
void Particulas_dibuja(int puntos);

/**
  \brief Borra la cadena y la libera de memoria.
 */
void Particulas_libera();

#endif
