#ifndef __PUNTOS_H
#define __PUNTOS_H

/**
\mainpage La biblioteca Puntos es una biblioteca de puntuaciones.
*/
/**
   \file Puntuaciones.h
   \brief  TDA dedicado a la lectura y manipulacion de la puntuacion apartir de un fichero.
*/

/**
  \brief Tipo de la puntuacion(array).
 */
typedef struct Puntuaciones* Puntuaciones;

/**
  \brief Funcion para leer las puntuaciones del fichero.
 */
void Puntuaciones_leer();

/**
  \brief Funcion de escritura de la puntuacion en el fichero.
  \param nombre String del nombre a escribir.
  \param puntos Puntuacion a escribir.
 */
void Puntuaciones_escribir(char nombre[26], int puntos);

/**
  \brief Numero de jugadores en la puntuacion.
  \return El numero de puntuaciones.
 */
int Puntuaciones_numero();

/**
  \brief Funcion para obtener el nombre del array de las puntuaciones en un determinado indice.
  \param i Indice del array.
  \return Un nombre de un indice dado.
 */
char* Puntuaciones_array_nombre(int i);

/**
  \brief Funcion para obtener los puntos del array de las puntuaciones en un determinado indice.
  \param i Indice del array.
  \return La puntuacion del indice dado.
 */
int Puntuaciones_array_puntos(int i);

/**
  \brief Funcion para liberar la memoria del array puntuaciones.
 */
void Puntuaciones_liberar();

#endif
