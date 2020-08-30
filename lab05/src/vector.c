#include "vector.h"

#include <stdlib.h>
#include <stdio.h>

static size_t INIT = 2;
static size_t MULT = 2;

void vInit(vector* v) {
    v->arr = (double *)malloc(sizeof(double) * INIT);
    v->size = 0;
    v->cap = INIT;
}

size_t vSize(const vector* v) {
    return v->size;
}

double vAt(const vector* v, size_t idx) {
    return v->arr[idx];
}

int vEmpty(const vector* v) {
    return v->size == 0;
}

void vPushBack(vector* v, double n) {
    if (v->size == v->cap)
        vResize(v);
    v->arr[v->size++] = n;
}

void vInsert(vector* v, double n, size_t idx) {
    if (idx <= v->size) {
        if (v->size == v->cap)
            vResize(v);
        if (!vEmpty(v)) {
            for (int i = v->size - 1; i >= idx && i >= 0; i--) {
                v->arr[i + 1] = v->arr[i];
            }
        }
        v->arr[idx] = n;
        v->size++;
    }
}

void vResize(vector* v) {
    v->cap *= MULT;
    v->arr = (double *)realloc(v->arr, sizeof(double) * v->cap);
}

void vErase(vector* v) {
    if (!vEmpty(v)) {
        v->arr[v->size - 1] = 0;
        v->size--;
    }
}

void vEraseAt(vector* v, size_t idx) {
    if (!vEmpty(v) && idx < v->size) {
        for (size_t i = idx; i < v->size - 1; i++)
            v->arr[i] = v->arr[i + 1];
        v->arr[v->size - 1] = 0;
        v->size--;
    }
}

void vDestroy(vector* v) {
    size_t s = v->size;
    for (size_t i = 0; i < s; i++) {
        vErase(v);
    }
    v->cap = 0;
}

void vPrint(const vector* v) {
    for (size_t i = 0; i < v->size; i++) {
        printf("%f ", v->arr[i]);
    }
    printf("\n");
    //printf("Thats it\n");
}
