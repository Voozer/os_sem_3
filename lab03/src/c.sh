#!/bin/bash

gcc -g gen.c -o gen
gcc -g 12-8Ham.c -o ham
gcc -g -pthread threadDecode.c -o td
