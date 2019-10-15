/*----------xv6 sync lab----------*/
#include "types.h"
#include "x86.h"
#include "defs.h"
#include "semaphore.h"

int
sem_init(int index, int val)
{
  sem[index].counter = val;
  sem[index].cv = 0;
  // init spinlock?
  initlock(&(sem[index].lk), "abc");
  return 0;
}

int
sem_up(int index)
{
  acquire(&(sem[index].lk));
  sem[index].counter++;
  wakeup(&(sem[index].cv));
  release(&(sem[index].lk));
  return 0;
}

int
sem_down(int index)
{
  acquire(&(sem[index].lk));
  sem[index].counter--;

  if (sem[index].counter < 0)
  	sleep(&(sem[index].cv), &(sem[index].lk));

  release(&(sem[index].lk));
  return 0;
}

/*----------xv6 sync lab end----------*/
