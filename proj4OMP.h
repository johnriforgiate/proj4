#define _GNU_SOURCE
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


char **bufferArray;
char **retArray;
void compare_lines(int);
int main(int, char**);