#ifndef __PUNTOS_H
#define __PUNTOS_H

/**
\mainpage La biblioteca Puntos es una biblioteca de puntuaciones
*/
/**
   \file Puntos.h
   \brief  TDA dedicado a la lectura y manipulacion de la puntuacion apartir de un fichero
*/

/**
  \brief Tipo de la puntuacion(array)
 */
typedef struct Puntuaciones* Puntuaciones;

/**
  \brief Funcion para leer las puntuaciones del fichero
 */
void leer_puntuaciones();

/**
  \brief Funcion de escritura de la puntuacion en el fichero
  \param nombre String del nombre a escribir.
  \param puntos Puntuacion a escribir.
 */
void escribir_puntuaciones(char nombre[26], int puntos);

/**
  \brief Numero de jugadors en la puntuacion
 */
int puntos_num();

/**
  \brief Funcion para obtener el nombre del array de las puntuaciones en un determinado indice
  \param i Indice del array.
 */
char* puntos_array_nombre(int i);

/**
  \brief Funcion para obtener los puntos del array de las puntuaciones en un determinado indice
  \param i Indice del array.
 */
int puntos_array_puntos(int i);

/**
  \brief Funcion para liberar la memoria del array puntuaciones
 */
void liberar_puntos();

#endif
