#include "Vector.h"
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

struct Vector {
    double x, y;
};

Vector Vector_new(double x, double y) {
    Vector vector = malloc(sizeof(struct Vector));
    vector->x = x;
    vector->y = y;
    return vector;
}

double Vector_get(Vector v, int t){
    return (t == 0) ? v->x : v->y;
}

void Vector_change(Vector v, double x, double y){
    v->x = x;
    v->y = y;
}

Vector Vector_unitary(Vector v) {
    Vector v_f = Vector_new(v->x, v->y);
    double module_f = sqrt((v->x*v->x)+(v->y*v->y));
    Vector_scale(v_f, 1/module_f);
    return v_f;
};

Vector Vector_points2vector(Vector inicial, Vector fin) {
    return Vector_new(fin->x - inicial->x, fin->y - inicial->y);
}

double degree2radians(double degree) {
    return degree*(M_PI/180);
}

double radians2degree(double radians) {
    return radians/(M_PI/180);
}

Vector Vector_rotate(Vector inicial, double degree) {
    double radians = degree2radians(degree);
    Vector vector = Vector_new(0, 0);
    vector->x = inicial->x*cos(radians) - inicial->y*sin(radians);
    vector->y = inicial->x*sin(radians) + inicial->y*cos(radians);
    return vector;
}

void Vector_scale(Vector inicial, double scale) {
    inicial->x *= scale;
    inicial->y *= scale;
}
