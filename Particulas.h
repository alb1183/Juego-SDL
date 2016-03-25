#ifndef __PARTICULAS_H
#define __PARTICULAS_H

typedef struct Nodo* ParticulaPtr;
ParticulaPtr Particulas_cadena(int alto, int ancho);
void Particulas_crear(ParticulaPtr p, int tipo, int puntos);
void Particulas_dibuja(ParticulaPtr q, int puntos);
void Particulas_libera(ParticulaPtr p);

#endif
