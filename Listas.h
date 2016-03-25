#ifndef __LISTAS_H
#define __LISTAS_H

/**
\mainpage La biblioteca Lista es una biblioteca de Listas, Estructuras enlazadas, Arboles, etc..
*/
/**
   \file Listas.h
   \brief  Esta bibliteca esta destinada a agrupar todos las funciones y estructuras necesarias de las listas, estructuras enlazadas y arboles
*/


/**
    \brief Estructura elemental de informacion del nodo
 */
struct Entidad {
    double x,y,vel_x,vel_y,radio;
    int estado;
};
typedef struct Entidad Entidad;

/**
    \brief Estructura unitaria de nodo(enlazada)
 */
struct Nodo {
    Entidad Elemento;
    struct Nodo* Sig;
};
/**
    \brief Tipo Nodo como un apuntador
 */
typedef struct Nodo* NodoPtr;

/*
struct Rama {
    Entidad Elemento;
    struct Rama* Izq;
    struct Rama* Der;
};
typedef struct Rama* RamaPtr;*/

/**
    \brief Funcion para crear una cadena de nodos(crea el nodo cabecera con memoria dinamica)
 */
NodoPtr crear_cadena();

//RamaPtr crear_arbor();

/**
    \brief Funcion para insertar un elemento al principio de la cadena
    \param p Puntero del nodo Cabecera de la cadena
    \param e Elemento a insertar
 */
void Insertar_Inicio(NodoPtr p, Entidad e);

/**
    \brief Funcion para suprimir un nodo de la cadena
    \param q Nodo que se quiere eliminar(como precondicion q no es la cabecera)
 */
void SuprimeNodo(NodoPtr q);

/**
    \brief Suprime la cadena y libera la memoria que utilizaba
    \param p Puntero del nodo Cabecera de la cadena
 */
void liberar_cadena(NodoPtr p);

#endif
