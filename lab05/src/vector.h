#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>

typedef struct {
    double* arr;
    size_t size;
    size_t cap;
} vector;

void vInit(vector* v);
size_t vSize(const vector* v);
double vAt(const vector* v, size_t i);
int vEmpty(const vector* v);
void vPushBack(vector* v, double n);
void vInsert(vector* v, double n, size_t idx);
void vResize(vector* v);
void vErase(vector* v);
void vEraseAt(vector* v, size_t idx);
void vDestroy(vector* v);
void vPrint(const vector* v);

#endif
