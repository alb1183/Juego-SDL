#ifndef __LISTAS_H
#define __LISTAS_H

/**
\mainpage La biblioteca Lista es una biblioteca de estructuras enlazadas.
*/
/**
   \file Listas.h
   \brief  Esta bibliteca esta destinada a agrupar todos las funciones y estructuras necesarias de las estructuras enlazadas.
*/


/**
    \brief Estructura elemental de informacion del nodo.
 */
typedef struct Entidad Lista_Entidad;

/**
    \brief Tipo Nodo como un apuntador.
 */
typedef struct Nodo* Listas_NodoPtr;

/**
    \brief Funcion para crear una lista.
    \return El apuntador al nodo cabecera.
 */
Listas_NodoPtr Listas_crear();

/**
    \brief Funcion para insertar un elemento al principio de la lista.
    \param p Puntero del nodo Cabecera de la lista.
    \param x Posicion X.
    \param y Posicion Y.
    \param vel_x Velocidad X.
    \param vel_y Velocidad Y.
    \param radio Radio.
    \param dat Dato.
    \param estado Estado.
 */
void Listas_Insertar_crear(Listas_NodoPtr p, double x, double y, double vel_x, double vel_y, double radio, double dat, int estado);


/**
    \brief Función para suprimir un nodo de la lista.
    \param q Nodo que se quiere eliminar.
    \pre q no es la cabecera.
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
    \brief Funcion para obtener X.
    \param p Nodo de la lista(no nulo).
    \return Devuelve el valor de X.
 */
double Listas_obtener_x(Listas_NodoPtr p);
/**
    \brief Funcion para obtener Y.
    \param p Nodo de la lista(no nulo).
    \return Devuelve el valor de Y.
 */
double Listas_obtener_y(Listas_NodoPtr p);
/**
    \brief Funcion para obtener Vel_X.
    \param p Nodo de la lista(no nulo).
    \return Devuelve el valor de Vel_X.
 */
double Listas_obtener_vel_x(Listas_NodoPtr p);
/**
    \brief Funcion para obtener Vel_Y.
    \param p Nodo de la lista(no nulo).
    \return Devuelve el valor de Vel_Y.
 */
double Listas_obtener_vel_y(Listas_NodoPtr p);
/**
    \brief Funcion para obtener Radio.
    \param p Nodo de la lista(no nulo).
    \return Devuelve el valor de Radio.
 */
double Listas_obtener_radio(Listas_NodoPtr p);
/**
    \brief Funcion para obtener Dat.
    \param p Nodo de la lista(no nulo).
    \return Devuelve el valor de Dat.
 */
double Listas_obtener_dat(Listas_NodoPtr p);
/**
    \brief Funcion para obtener Estado.
    \param p Nodo de la lista(no nulo).
    \return Devuelve el valor de Estado.
 */
int Listas_obtener_estado(Listas_NodoPtr p);

/**
    \brief Funcion para cambiar el valor de X.
    \param p Nodo de la lista(no nulo).
    \param d Valor de X a cambiar.
 */
void Listas_cambiar_x(Listas_NodoPtr p, double d);
/**
    \brief Funcion para cambiar el valor de Y.
    \param p Nodo de la lista(no nulo).
    \param d Valor de Y a cambiar.
 */
void Listas_cambiar_y(Listas_NodoPtr p, double d);
/**
    \brief Funcion para cambiar el valor de Vel_X.
    \param p Nodo de la lista(no nulo).
    \param d Valor de Vel_X a cambiar.
 */
void Listas_cambiar_vel_x(Listas_NodoPtr p, double d);
/**
    \brief Funcion para cambiar el valor de Vel_Y.
    \param p Nodo de la lista(no nulo).
    \param d Valor de Vel_Y a cambiar.
 */
void Listas_cambiar_vel_y(Listas_NodoPtr p, double d);
/**
    \brief Funcion para cambiar el valor de Radio.
    \param p Nodo de la lista(no nulo).
    \param d Valor de Radio a cambiar.
 */
void Listas_cambiar_radio(Listas_NodoPtr p, double d);
/**
    \brief Funcion para cambiar el valor de Dat.
    \param p Nodo de la lista(no nulo).
    \param d Valor de Dat a cambiar.
 */
void Listas_cambiar_dat(Listas_NodoPtr p, double d);
/**
    \brief Funcion para cambiar el valor de Estado.
    \param p Nodo de la lista(no nulo).
    \param d Valor de Estado a cambiar.
 */
void Listas_cambiar_estado(Listas_NodoPtr p, int d);


/**
    \brief Suprime la lista y libera la memoria que utilizaba
    \param p Puntero del nodo Cabecera de la lista
 */
void Listas_liberar(Listas_NodoPtr p);

#endif
