#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

void* compare_lines(void*);
int main(int, char**);

int NUM_THREADS;
char **bufferArray;
char **retArray;

pthread_barrier_t barrier;
