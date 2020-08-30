CC = gcc

CCKEYS = -Wall -Werror -g

all: link load

link: linking.c libvector.so
	$(CC) $(CCKEYS) linking.c -o link -L. -lvector

load: loading.c libvector.so
	$(CC) $(CCKEYS) -rdynamic loading.c -o load -ldl

libvector.so: vector.o
	$(CC) $(CCKEYS) -shared -o libvector.so vector.o

vector.o: vector.c vector.h
	$(CC) $(CCKEYS) vector.c -c -fpic

cleanL:
	rm -rf libvector.so vector.o

cleanE:
	rm -rf link load
