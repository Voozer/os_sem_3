/*Составить программу на языке Си, обрабатывающую данные в многопоточном режиме.
При обработки использовать стандартные средства создания потоков операционной системы (Windows/Unix).
При создании необходимо предусмотреть ключи,
    которые позволяли бы задать максимальное количество потоков, используемое программой.
При возможности необходимо использовать максимальное количество возможных потоков.
Ограничение потоков может быть задано или ключом запуска вашей программы, или алгоритмом.
Так же необходимо уметь продемонстрировать количество потоков,
    используемое вашей программой с помощью стандартных средств операционной системы.

Вариант 23: Коды Хэмминга.
    Необходимо реализовать 2 программы.
    Первая программа кодирует по Хэммингу нормальную последовательность байт.
    Вторая программа раскодирует и корректирует закодированную последовательность
        (корректировка осуществляется только, если было повреждение).
    Многопоточность необходимо реализовать только в программе 2
        (дешифрование и коррекция кодов).
    Количество контрольных разрядов должно задаваться пользователем.*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define TOTAL_BITS 12
#define DATA_BITS 8

int power(int base, int exp) {
    int num = 1;
    while(exp != 0) {
        num *= base;
        --exp;
    }
    return num;
}

typedef struct {
    char* code;
    char* correct;
    size_t blockNum;
    size_t pBit;
} argsT;

//gcc -g -pthread thread.c -o t

void* thrd_pBitCount(void* arguments) {
    argsT* args = (argsT*)arguments;
    size_t blockShift = TOTAL_BITS * args->blockNum;
    size_t nextBlock = TOTAL_BITS * (args->blockNum + 1);
    size_t pBitIdx = power(2, args->pBit);
    int n = args->pBit;
    int sum = 0;
    for (size_t i = blockShift; i < nextBlock; i++) {
        if (((i - blockShift + 1) & 1 << n) == power(2, n)) {
            sum ^= args->correct[i] - '0';
        }
    }
    sum == 0 ? (args->correct[pBitIdx + blockShift - 1] = '0') : (args->correct[pBitIdx + blockShift - 1] = '1');
}

char* correct(char* code) {
    size_t pBits = TOTAL_BITS - DATA_BITS;
    size_t blockNum = (strlen(code) - 1) / TOTAL_BITS;
    argsT* args = (argsT*)malloc(sizeof(argsT));
    args->code = code;
    args->correct = malloc(strlen(code));
    strcpy(args->correct, code);

    size_t pBitIdx;
    size_t blockShift;
    size_t nextBlock;
    for (size_t i = 0; i < blockNum; i++) {
        pBitIdx = 1;
        blockShift = TOTAL_BITS * i;
        for (size_t n = 0; n < pBits; n++) {
            args->correct[pBitIdx + blockShift - 1] = '0';
            pBitIdx *= 2;
        }
    }

    pthread_t thrd[blockNum * pBits];   //thread pbit count
    for (size_t i = 0; i < blockNum * pBits; i++) {
        args->blockNum = i / pBits;
        args->pBit = i % pBits;
        if (pthread_create(&(thrd[i]), NULL, thrd_pBitCount, args) != 0) {
            printf("Failed to create a thread\n");
            exit(1);
        }
        pthread_join(thrd[i], NULL); //very important!!!!
    }

    char* correct = args->correct;

    if (strcmp(code, correct) == 0) {
        return correct;
    }

    char* result = malloc(strlen(code));
    strcpy(result, code);
    for (size_t i = 0; i < blockNum; i++) { //fixing the mistake
        blockShift = TOTAL_BITS * i;
        nextBlock = TOTAL_BITS * (i + 1);
        size_t error = 0;
        for (size_t j = blockShift; j < nextBlock; j++) {
            if (code[j] != correct[j]) {
                error += j - blockShift + 1;
            }
        }
        if (error != 0) {
            result[error + blockShift - 1] == '0' ? (result[error + blockShift - 1] = '1') : (result[error + blockShift - 1] = '0');
            printf("Error at position %ld fixed!\n", error + blockShift - 1);
        }
    }
    free(correct);
    free(args);

    return result;
}

char* decode(const char* code) {

    size_t pBits = TOTAL_BITS - DATA_BITS;
    size_t blockNum = (strlen(code) - 1) / TOTAL_BITS;
    char* msg = malloc(blockNum * DATA_BITS + 1);
    msg[blockNum * DATA_BITS] = '\0';

    size_t pBitIdx;
    size_t blockShift;
    size_t nextBlock;
    for (size_t i = 0; i < blockNum; i++) { //getting rid of parityBits
        pBitIdx = 1;
        blockShift = TOTAL_BITS * i;
        nextBlock = TOTAL_BITS * (i + 1);
        for (size_t j = blockShift; j < nextBlock; j++) {
            if ((j - blockShift + 1) == pBitIdx) {
                pBitIdx *= 2;
                continue;
            }
            msg[strlen(msg)] = code[j];
        }
    }

    char* initMsg = malloc(strlen(msg) / DATA_BITS + 1);
    initMsg[strlen(msg) / DATA_BITS] = '\0';

    for (size_t i = 0; i < blockNum; i++) {
        blockShift = DATA_BITS * i;
        nextBlock = DATA_BITS * (i + 1);
        char character = 0;
        for (size_t j = blockShift; j < nextBlock; j++) {
            character += (msg[j] - '0') * power(2, DATA_BITS - (j - blockShift) - 1);
        }
        initMsg[i] = character;
    }

    return initMsg;
}

int main(int argc, char* argv[]) {

    printf("Hamming (12,8) decoder:\n");
    printf("The parity bits positions are: ");
    size_t pBitIdx = 1;
    for (size_t i = 0; i < TOTAL_BITS - DATA_BITS; i++) {
        printf("%ld, ", pBitIdx - 1);
        pBitIdx *= 2;
    }
    printf("\n");

    printf("Enter the binary message coded in Hamming (12,8):\n");
    if (argc != 3) {
        printf("Not enough arguments\n");
        exit(1);
    }

    FILE* f = fopen(argv[1], "r");

    fseek(f, 0L, SEEK_END);
    size_t sz = ftell(f);
    fseek(f, 0L, SEEK_SET);

    char code[sz + 1];
    fgets(code, sz + 1, f);

    for (size_t i = 0; i < strlen(code) - 1; i++) {
        if (code[i] != '1' && code[i] != '0') {
            printf("Invalid input! The code must be binary\n");
            exit(1);
        }
    }
    if ((strlen(code) - 1) < TOTAL_BITS || ((strlen(code) - 1) > TOTAL_BITS && (strlen(code) - 1) % TOTAL_BITS != 0)) {
        printf("Invalid input! Wrong number of bits\n");
        exit(1);
    }
    printf("Checking if the code is correct...\n");
    char* correctMsg = correct(code);
    if (strcmp(code, correctMsg) == 0) {
        printf("The given code is correct!\n\n");
    } else {
        printf("The correct code is %s\n", correctMsg);
    }
    printf("Hamming (12,8) decoding...\n");
    printf("Be aware that Hamming code can only fix one error in a block so the further message might be corrupted!\n");
    char* initMsg = decode(correctMsg);

    FILE* f1 = fopen(argv[2], "w+");
    printf("The initial message is in file\n");
    fprintf(f1, "%s\n", initMsg);

    free(correctMsg);
    free(initMsg);
    fclose(f);
    fclose(f1);

    return 0;
}
