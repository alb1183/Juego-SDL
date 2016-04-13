#ifndef __LISTAS_H
#define __LISTAS_H

/**
\mainpage La biblioteca Lista es una biblioteca de estructuras enlazadas.
*/
/**
   \file Listas.h
   \brief  Esta bibliteca esta destinada a agrupar todos las funciones y estructuras necesarias de las estructuras enlazadas.
*/

struct Entidad {
    double x,y,vel_x,vel_y,radio,dat;
    int estado;
};

/**
    \brief Estructura elemental de informacion del nodo
 */
typedef struct Entidad Lista_Entidad;

/**
    \brief Tipo Nodo como un apuntador
 */
typedef struct Nodo* Listas_NodoPtr;

/**
    \brief Funcion para crear una cadena de nodos(crea el nodo cabecera con memoria dinamica)
    \return Devuelve una cadena vacia.
 */
Listas_NodoPtr Listas_crear_cadena();

/**
    \brief Funcion para insertar un elemento al principio de la cadena
    \param p Puntero del nodo Cabecera de la cadena
    \param e Elemento a insertar
 */
void Listas_Insertar_Inicio(Listas_NodoPtr p, Lista_Entidad e);

/**
    \brief Funcion para suprimir un nodo de la cadena
    \param q Nodo que se quiere eliminar(como precondicion q no es la cabecera)
 */
void Listas_SuprimeNodo(Listas_NodoPtr q);

/**
    \brief Funcion para obtener el puntero del siguiente elemento de la lista
    \param p Nodo de la lista(no nulo)
    \return Devuelve un puntero de nodo.
 */
Listas_NodoPtr Listas_siguiente(Listas_NodoPtr p);

/**
    \brief Funcion para obtener el puntero de un elemento de un nodo(para poder acceder a el).
    \param p Nodo de la lista(no nulo).
    \return Devuelve un puntero a el elemento de un nodo.
 */
Lista_Entidad* Lista_elemento(Listas_NodoPtr p);

/**
    \brief Suprime la cadena y libera la memoria que utilizaba
    \param p Puntero del nodo Cabecera de la cadena
 */
void Listas_liberar_cadena(Listas_NodoPtr p);

#endif
