/*Вариант 23: родительский процесс считывает две координаты, передает
их через канал дочернему процессу. Дочерний процесс определяет,
к какокй четверти относится точка, а далее передает результат
родительскому процессу.*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    int point[2] = {0};
    char quarter;

    int pipe1fd[2];
    int pipe2fd[2];
    if(pipe(pipe1fd) == -1) {
        perror("Unable to create a pipe1!");
        exit(1);
    }
    if(pipe(pipe2fd) == -1) {
        perror("Unable to create a pipe2!");
        exit(1);
    }

    pid_t pid = fork();

    if(pid > 0) {
        printf("Enter the coordinates: ");
        close(pipe1fd[0]);
        close(pipe2fd[1]);
        while(scanf("%d %d", &point[0], &point[1])) {
            if(write(pipe1fd[1], point, sizeof(int) * 2) == -1) {
                perror("Parent process failed to write to pipe!");
            }
            printf("parent process waiting...\n");
            /*if(waitpid(-1, NULL, 0) == -1) {
                perror("Parent process failed to waitpid()!");
            }*/
            if(read(pipe2fd[0], &quarter, 1) == -1) {
                perror("Parent process failed to read from pipe!");
            }
            printf("The answer is: %c\n", quarter);
            printf("\nEnter the coordinates: ");
        }
        close(pipe1fd[1]);
        close(pipe2fd[0]);
    } else if(pid == 0) {
        close(pipe1fd[1]);
        close(pipe2fd[0]);
        while(read(pipe1fd[0], point, sizeof(int) * 2)) {
            if(point[0] == 0 && point[1] == 0) {
                printf("The point is the origin! Assigning quarter to 0...\n");
                quarter = '0';
            }
            if(point[0] == 0 && point[1] != 0) {
                printf("The point lays on the Oy axis! Assigning quarter to 0...\n");
                quarter = '0';
            }
            if(point[0] != 0 && point[1] == 0) {
                printf("The point lays on the Ox axis! Assigning quarter to 0...\n");
                quarter = '0';
            }
            if(point[0] > 0 && point[1] > 0) {
                quarter = '1';
            }
            if(point[0] < 0 && point[1] > 0) {
                quarter = '2';
            }
            if(point[0] < 0 && point[1] < 0) {
                quarter = '3';
            }
            if(point[0] > 0 && point[1] < 0) {
                quarter = '4';
            }
            printf("child process writing...\n");
            if(write(pipe2fd[1], &quarter, 1) == -1) {
                perror("Child procces failed to write to pipe!");
            }
        }
        close(pipe1fd[0]);
        close(pipe2fd[1]);
    } else if(pid == -1) {
        perror("Unable to fork!");
        exit(1);
    }
    return 0;
}
