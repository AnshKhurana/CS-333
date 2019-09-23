/*----------xv6 sync lab----------*/
#include "types.h"
#include "x86.h"
#include "spinlock.h"
#include "defs.h"
#include "barrier.h"

//define any variables needed here
int val;
struct spinlock block;

int
barrier_init(int n)
{
  
  val = n;
  return 0;
}

int
barrier_check(void)
{
  acquire(&block);
  val--;
  while(val != 0)
  {
    sleep(&val, &block);
  }
  wakeup(&val);
  release(&block);

  // if(val!=1)
  // {
  //   val--;
  //   sleep(&val, &block);

  // }
  // else
  // {

  //   wakeup(&val);
  //   release(&block);
  // }

  return 0;
}

/*----------xv6 sync lock end----------*/
