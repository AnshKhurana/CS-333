#include "types.h"
#include "stat.h"
#include "user.h"

int main()
{
  int ret1, ret2;
  int i;
  
  ret1 = fork();
  if(ret1 == 0)
    {
      for(i=0; i < 10; i++) {
	printf(1, "I am child 1\n");
      }
      exit();
    }
  else
    {
      ret2 = fork();
      if(ret2 == 0)
	{
	  for(i=0; i < 10; i++) {
	    printf(1, "I am child 2\n");
	  }
	  exit();
	}
      else {
	for(i=0; i < 10; i++) {
	  printf(1, "I am parent\n");
	}
	wait();
	wait();
	exit();
      }
    }
}

