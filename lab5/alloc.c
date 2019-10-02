#include "alloc.h"

/* Code to allocate page of 4KB size with mmap() call and
* initialization of other necessary data structures.
* return 0 on success and 1 on failure (e.g if mmap() fails)
*/
int init()
{
	// Write your code below
    addr = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1,0);
    if (addr == MAP_FAILED)
      {
          return 1;
      }      
    
    for (int i = 0; i < PAGESIZE/MINALLOC; ++i)
    {
        status[i] = 0;
    }
    return 0;
}


/* optional cleanup with munmap() call
* return 0 on success and 1 on failure (if munmap() fails)
*/
int cleanup()
{

	// Write your code below - just remove the chunk
    for (int i = 0; i < PAGESIZE/MINALLOC; ++i)
    {
        status[i] = 0;
    }

    if (munmap(addr, PAGESIZE) == -1)
    {
        return 1;
    }
    return 0;
}

/* Function to allocate memory of given size
* argument: bufSize - size of the buffer
* return value: on success - returns pointer to starting address of allocated memory
*               on failure (not able to allocate) - returns NULL
*/
char *alloc(int bufSize)
{
	// write your code below
    bufSize = bufSize/8;
    for (int i = 0; i < PAGESIZE/MINALLOC - bufSize +1 ; ++i)
    {
        
        int found = 0;
        if (status[i]==0)
        {
            int count = 1;

            for (int j = i; j < i+bufSize; ++j)
            {
                if (status[j] > 0)
                {
                    count = 1;
                    break;
                }
            }

            if (count)
            {
                found = 1;
            }
            else
            {
                continue;
            }
        // printf("%d\n", found);
        if (found)
        {
            status[i] = bufSize;
            for (int j = i+1; j < i+bufSize; ++j)
            {
                status[j] = 1;
            }

            return (char*) (addr+i*8);
        }
        }
    }

    return NULL;
}


/* Function to free the memory
* argument: takes the starting address of an allocated buffer
*/
void dealloc(char *memAddr)
{
	// write your code below

    int start_pos = (memAddr - (char *) addr)/8;

    int bufSize = status[start_pos];
    for (int i = start_pos; i < start_pos + bufSize ; ++i)
    {
        status[i] = 0;
    }
}
