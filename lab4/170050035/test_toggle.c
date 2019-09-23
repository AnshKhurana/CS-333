#include "types.h"
#include "stat.h"
#include "user.h"

int main()
{
	int ret;
	int i;
	// int channel = 1;
	uspinlock_init();
	ucounter_init();
	ucounter_set(0, 0);

	ret = fork();
	if(ret == 0)
	{
	  for(i=0; i < 10; i++) {
	  	uspinlock_acquire(0);
	  	while(ucounter_get(0)!=0) ucv_sleep(1, 0);
	    printf(1, "I am child\n");
	    ucounter_set(0, 1);
	    ucv_wakeup(1);
	    uspinlock_release(0);
			  	
	  }
          exit();
	}
	else
	{
	  for(i=0; i < 10; i++) {
	  	uspinlock_acquire(0);
	  	while(ucounter_get(0)!=1) ucv_sleep(1, 0);
	    printf(1, "I am parent\n");
	    ucounter_set(0, 0);
	    ucv_wakeup(1);
	    uspinlock_release(0);

	  }
	  wait();
          exit();
	}
}

