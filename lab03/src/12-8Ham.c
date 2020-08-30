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

//Hamming (12,8)
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

char* encode(const char* msg) {
    size_t pBits = TOTAL_BITS - DATA_BITS;
    size_t blockNum = strlen(msg) / DATA_BITS;
    char* code = malloc(blockNum * TOTAL_BITS + 1);
    code[blockNum * TOTAL_BITS] = '\0';
    strcpy(code, msg);

    size_t pBitIdx;
    size_t blockShift;
    size_t nextBlock;
    printf("num %ld\n", blockNum);
    for (size_t i = 0; i < blockNum; i++) {
        if (i % 1000 == 0) {
            printf("block num %ld\n", i); //1000 blocks/25 sec i.e 40 blocks/sec
        }
        pBitIdx = 1;
        blockShift = TOTAL_BITS * i;
        for (size_t n = 0; n < pBits; n++) {
            for (size_t j = strlen(code); j != pBitIdx + blockShift - 1; j--) {
                code[j] = code[j - 1];
            }
            code[pBitIdx + blockShift - 1] = '0';
            pBitIdx *= 2;
        }

        pBitIdx = 1;
        for (size_t n = 0; n < pBits; n++) {
            nextBlock = TOTAL_BITS * (i + 1);
            int sum = 0;
            for (size_t j = blockShift; j < nextBlock; j++) {
                if (((j - blockShift + 1) & 1 << n) == power(2, n)) {
                    sum ^= code[j] - '0';
                }
            }
            sum == 0 ? (code[pBitIdx + blockShift - 1] = '0') : (code[pBitIdx + blockShift - 1] = '1');
            pBitIdx *= 2;
        }
    }

    return code;
}

int main(int argc, char* argv[]) {
     printf("Hamming code:\nEnter the message you want to encode:\n");

     if (argc != 3) {
         printf("Not enough arguments\n");
         exit(1);
     }

     FILE* f = fopen(argv[1], "r");

     fseek(f, 0L, SEEK_END);
     size_t sz = ftell(f);
     fseek(f, 0L, SEEK_SET);

     char buf[10000] = {0};
      char msg[sz];
      while(fgets(buf, 10000, f) != NULL) {
          strcat(msg, buf);
      }

     size_t msgLen = strlen(msg) - 1;
     size_t binMsgSize = msgLen * 8 + 1;
     char* binMsg = malloc(binMsgSize);

     size_t n = 0;
     for(size_t i = 0; i < msgLen; i++) {
         for(size_t j = 8; j != 0; j--)
            (msg[i] & 1 << j - 1) ? (binMsg[n++] = '1') : (binMsg[n++] = '0');
     }
     binMsg[msgLen * 8] = '\0';
    // printf("Input binary message:\n%s\n", binMsg);

     printf("Hamming (12,8) encoding...\n");
     char* code = encode(binMsg);

     FILE* f1 = fopen(argv[2], "w+");
     printf("Encoded message is in file\n");
     fprintf(f1, "%s\n", code);

     free(binMsg);
     free(code);
     fclose(f);
     fclose(f1);


     return 0;
}
