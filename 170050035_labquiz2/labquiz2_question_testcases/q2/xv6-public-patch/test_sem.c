#include "types.h"
#include "stat.h"
#include "user.h"

int main()
{
  int ret1, ret2;

  sem_init(0, 0); //using one semaphore
  
  ret1 = fork();
  if(ret1 == 0)
    {
      sem_down(0);
      printf(1, "I am child 1\n");
      exit();
    }
  else
    {
      ret2 = fork();
      if(ret2 == 0)
	{
	  sem_down(0);
	  printf(1, "I am child 2\n");
	  exit();
	}
      else {

	sleep(2);

	//inspite of the sleep, parent should print first
	printf(1, "I am parent\n");

	sem_up(0);
	sem_up(0);
	
	wait();
	wait();
	
	exit();
      }
    }
}

