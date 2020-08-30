#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Not enough arguments\n");
        exit(1);
    }

    FILE* f = fopen(argv[1], "w+");

    printf("the num of tests: ");
    size_t num;
    scanf("%ld", &num);
    for (size_t c = 0; c < num; c++) {
        fprintf(f, "I am Hamming code test %ld\n", c);
    }
    fclose(f);
    return 0;
}
