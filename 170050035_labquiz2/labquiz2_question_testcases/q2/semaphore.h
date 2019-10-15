#include "spinlock.h"

/*----------xv6 sync lab----------*/
#define NSEM 10

int sem_init(int, int);
int sem_up(int);
int sem_down(int);
/*----------xv6 sync lab end----------*/

struct semaphore
{
	int counter;
	struct spinlock lk;
	int cv;
};

struct semaphore sem[NSEM];