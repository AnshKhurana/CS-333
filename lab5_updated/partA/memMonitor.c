#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

#define PAGESIZE 409600 //size of memory to allocate from OS
#define MINALLOC 8 //allocations will be 8 bytes or multiples of it

int main(int argc, char const *argv[])
{
    /* code */
    printf("%d\n", (int)getpid());
    void* addr = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1,0);
    getchar();

    char* ptr = (char *) addr;
    // strcpy(ptr, "Hello world");
    // printf("%s\n", ptr);
    for (int i = 0; i < PAGESIZE; ++i)
    {
        ptr[i] = 'a';
    }
    getchar();
    getchar();  

    
    return 0;
}