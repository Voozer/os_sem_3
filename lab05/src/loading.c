#include "vector.h"
#include <string.h>
#include <dlfcn.h>
#include <stdlib.h>

//в чем смысл библиотек (статических) в сравнении с хэдэрами

void* getFunc(void* handle, char* fName) {
    void* f = dlsym(handle, fName);
    if (f == NULL) {
        printf("%s\n", dlerror());
        exit(1);
    }
    return f;
}

void menu() {
    printf("Double vector. Type in:\n");
    printf("'menu' for menu;\n");
    printf("'push' to pushback;\n");
    printf("'ins' to insert;\n");
    printf("'er' to erase;\n");
    printf("'del' to erase at idx;\n");
    printf("'destr' to destroy;\n");
    printf("'at' to get element at idx;\n");
    printf("'size' for size;\n");
    printf("'print' to print;\n");
    printf("'quit' to quit\n\n");
}

int main() {
    void* libHandle = dlopen("./libvector.so", RTLD_LAZY);
    if (!libHandle) {
        printf("%s\n", dlerror());
        exit(1);
    }
    vector v;
    void (*init)(vector*) = getFunc(libHandle, "vInit");
    size_t (*size)(const vector*) = getFunc(libHandle, "vSize");
    double (*at)(const vector*, size_t) = getFunc(libHandle, "vAt");
    void (*pushback)(vector*, double) = getFunc(libHandle, "vPushBack");
    void (*insert)(vector*, double, size_t) = getFunc(libHandle, "vInsert");
    void (*erase)(vector*) = getFunc(libHandle, "vErase");
    void (*delete)(vector*, size_t) = getFunc(libHandle, "vEraseAt");
    void (*destroy)(vector*) = getFunc(libHandle, "vDestroy");
    void (*print)(const vector*) = getFunc(libHandle, "vPrint");

    (*init)(&v);
    menu();
    char ch[16];
    double val;
    size_t idx;

    while (scanf("%15s", ch) == 1) {
        if (strcmp(ch, "menu") == 0) {
            menu();
            continue;
        }
        if (strcmp(ch, "push") == 0) {
            printf("Enter the element value: ");
            scanf("%lf", &val);
            (*pushback)(&v, val);
            continue;
        }
        if (strcmp(ch, "ins") == 0) {
            printf("Enter the element value and idx: ");
            scanf("%lf %ld", &val, &idx);
            (*insert)(&v, val, idx);
            continue;
        }
        if (strcmp(ch, "er") == 0) {
            (*erase)(&v);
            continue;
        }
        if (strcmp(ch, "del") == 0) {
            printf("Enter the idx: ");
            scanf("%ld", &idx);
            (*delete)(&v, idx);
            continue;
        }
        if (strcmp(ch, "destr") == 0) {
            (*destroy)(&v);
            continue;
        }
        if (strcmp(ch, "at") == 0) {
            printf("Enter the idx: ");
            scanf("%ld", &idx);
            val = (*at)(&v, idx);
            printf("%f\n", val);
            continue;
        }
        if (strcmp(ch, "size") == 0) {
            printf("%ld\n", (*size)(&v));
            continue;
        }
        if (strcmp(ch, "print") == 0) {
            (*print)(&v);
            continue;
        }
        if (strcmp(ch, "quit") == 0) {
            break;
        }
        printf("Invalid option!\n");
    }
    dlclose(libHandle);
    return 0;
}
