#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

#define PAGESIZE -1 //size of memory to allocate from OS
#define MINALLOC 8 //allocations will be 8 bytes or multiples of it

// You can declare any data structures required here
int status[PAGESIZE/MINALLOC];
void *addr;


// function declarations
int init();
int cleanup();
char *alloc(int);
void dealloc(char *);
