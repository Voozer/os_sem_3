/*Вариант 23: родительский процесс считывает две координаты, передает
их через канал дочернему процессу. Дочерний процесс определяет,
к какокй четверти относится точка, а далее передает результат
родительскому процессу.*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>

int getFile() {                     //or try with FILE*
    char* templateString = strdup("/tempXXXXXX");
    int fd = mkstemp(templateString);
    if (fd == -1) {
        perror("File couldn't be created!");
        exit(1);
    }
    if(unlink(templateString) == -1) {
        perror("Couldn't unlink filename!");
        exit(1);
    }
    free(templateString);
    if(write(fd, "test", 4) == -1) {
		perror("Couldn't test write in temp file!");
		exit(1);
	}
    return fd;
}

int main()
{
    int point[2] = {0};
    char quarter;

    int filedes = getFile();

    size_t pagesize = getpagesize();
    int* mappedData = (int*)mmap(0, pagesize, PROT_WRITE | PROT_READ, MAP_SHARED, filedes, 0);
    if (mappedData == MAP_FAILED) {
        perror("Mmap failed!");
        exit(1);
    }

//SEM MMAP? WHAT FD NEEDED?
//sem_t sem1; if sem is not pointer then use sem_init and then mmap then unmap and sem destroy
	unsigned int count = 0;
	sem_t* sem1 = sem_open("sem1", O_CREAT, 777, count);
	sem_t* sem2 = sem_open("sem2", O_CREAT, 777, count);

	if(sem1 == SEM_FAILED) {
		perror("Couldn't create semaphore1!");
		exit(1);
	}
	if(sem2 == SEM_FAILED) {
		perror("Couldn't create semaphore2!");
		exit(1);
	}

    pid_t pid = fork();
    if(pid > 0) {
        printf("Enter the coordinates: ");
        while(scanf("%d %d", &point[0], &point[1])) {
            /*if(write(mappedData[1], point, sizeof(int) * 2) == -1) {
                perror("Parent process failed to write to file!");
            }*/
			mappedData[0] = point[0];
			mappedData[1] = point[1];
            printf("parent process waiting...\n");
			sem_post(sem1);
			int c;
			sem_getvalue(sem1, &c);
			printf("1 sem1 val %d\n", c);
			sem_getvalue(sem2, &c);
			printf("2 sem2 val %d\n", c);
			sem_wait(sem2);
			printf("4\n");
            /*if(read(mappedData[0], &quarter, 1) == -1) {
                perror("Parent process failed to read from file!");
            }*/
			quarter = mappedData[2];
            printf("The answer is: %c\n", quarter);
            printf("\nEnter the coordinates: ");
        }
    } else if(pid == 0) {
        while(1) { //read(mappedData[1], point, sizeof(int) * 2)) {
			sem_wait(sem1);
			point[0] = mappedData[0];
			point[1] = mappedData[1];
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
            /*if(write(mappedData[0], &quarter, 1) == -1) {
                perror("Child procces failed to write to file!");
            }*/
			mappedData[2] = quarter;
			int c;
			sem_getvalue(sem2, &c);
			printf("3 sem2 val %d\n", c);
			sem_post(sem2);
        }
    } else if(pid == -1) {
        perror("Unable to fork!");
		sem_close(sem1); sem_close(sem2);
		sem_unlink("sem1"); sem_unlink("sem2");
		close(filedes);
        exit(1);
    }
	if(munmap(mappedData, pagesize) == -1) {
		perror("Unmapping failed!");
		sem_close(sem1); sem_close(sem2);
		sem_unlink("sem1"); sem_unlink("sem2");
		close(filedes);
		exit(1);
	}
	sem_close(sem1); sem_close(sem2);
	sem_unlink("sem1"); sem_unlink("sem2");
	close(filedes);
    return 0;
}
