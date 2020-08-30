#include "vector.h"
#include <string.h>

//for library usage:
//LD_LIBRARY_PATH=./
//export LD_LIBRARY_PATH

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
    vector v;
    vInit(&v);
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
            vPushBack(&v, val);
            continue;
        }
        if (strcmp(ch, "ins") == 0) {
            printf("Enter the element value and idx: ");
            scanf("%lf %ld", &val, &idx);
            vInsert(&v, val, idx);
            continue;
        }
        if (strcmp(ch, "er") == 0) {
            vErase(&v);
            continue;
        }
        if (strcmp(ch, "del") == 0) {
            printf("Enter the idx: ");
            scanf("%ld", &idx);
            vEraseAt(&v, idx);
            continue;
        }
        if (strcmp(ch, "destr") == 0) {
            vDestroy(&v);
            continue;
        }
        if (strcmp(ch, "at") == 0) {
            printf("Enter the idx: ");
            scanf("%ld", &idx);
            val = vAt(&v, idx);
            printf("%f\n", val);
            continue;
        }
        if (strcmp(ch, "size") == 0) {
            printf("%ld\n", vSize(&v));
            continue;
        }
        if (strcmp(ch, "print") == 0) {
            vPrint(&v);
            continue;
        }
        if (strcmp(ch, "quit") == 0) {
            break;
        }
        printf("Invalid option!\n");
    }

    return 0;
}
