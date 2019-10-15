#include "types.h"
#include "stat.h"
#include "user.h"

int main()
{
  int ret1, ret2;
  int i;
  sem_init(0, 1);
  sem_init(1, 0);
  sem_init(2, 0);
  
  ret1 = fork();
  if(ret1 == 0)
    {
      for(i=0; i < 10; i++) {
        sem_down(0);
	printf(1, "I am child 1\n");
      sem_up(1);
      }
      exit();
    }
  else
    {
      ret2 = fork();
      if(ret2 == 0)
	{
	  for(i=0; i < 10; i++) {
	     sem_down(1);
      printf(1, "I am child 2\n");
	   sem_up(2);
    }
	  exit();
	}
      else {
	for(i=0; i < 10; i++) {
    sem_down(2);
	  printf(1, "I am parent\n");
	   sem_up(0);
  }
	wait();
	wait();
	exit();
      }
    }
}

