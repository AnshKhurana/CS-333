/*----------xv6 sync lab----------*/
#include "types.h"
#include "x86.h"
#include "uspinlock.h"

struct uspinlock
{
  uint locked;
};

//TODO: define an array of NLOCK uspinlocks
struct uspinlock locks[NLOCK];

int
uspinlock_init(void)
{
  for (int i = 0; i < NLOCK; ++i)
  {
    locks[i].locked = 0;
  }
  return 0;
}

int
uspinlock_acquire(int index)
{

    while(xchg(&(locks[index].locked), 1) != 0)
    ;

  return 0;
}

int
uspinlock_release(int index)
{
  asm volatile("movl $0, %0" : "+m" (locks[index].locked) : );
  return 0;
}

int
uspinlock_holding(int index)
{
  
  return locks[index].locked;
}
/*----------xv6 sync lock end----------*/
