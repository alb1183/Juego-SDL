#include "Vector.h"
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

struct Vector {
    double x, y;
};

Vector vector_new(double x, double y) {
    Vector vector = malloc(sizeof(struct Vector));
    vector->x = x;
    vector->y = y;
    return vector;
}

double Vector_get(Vector v, int t){
    return (t == 0) ? v->x : v->y;
}

Vector Vector_change(Vector v, double x, double y){
    v->x = x;
    v->y = y;
    return v;
}

Vector unitary_vector(Vector v) {
    Vector v_f = vector_new(v->x, v->y);
    double module_f = sqrt((v->x*v->x)+(v->y*v->y));

    return vector_scale(v_f, 1/module_f);
};

Vector points2vector(Vector inicial, Vector fin) {
    return vector_new(fin->x - inicial->x, fin->y - inicial->y);
}

double degree2radians(double degree) {
    return degree*(M_PI/180);
}

double radians2degree(double radians) {
    return radians/(M_PI/180);
}

Vector vector_rotate(Vector inicial, double degree) {
    double radians = degree2radians(degree);
    Vector vector = vector_new(0, 0);
    vector->x = inicial->x*cos(radians) - inicial->y*sin(radians);
    vector->y = inicial->x*sin(radians) + inicial->y*cos(radians);
    return vector;
}

Vector vector_scale(Vector inicial, double scale) {
    inicial->x *= scale;
    inicial->y *= scale;
    return inicial;
}
