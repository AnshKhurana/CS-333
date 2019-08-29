#include "types.h"
#include "stat.h"
#include "user.h"
#include "processInfo.h"
int main(void)
{
    struct processInfo info;
    int pid;
    printf(1, "PID\tSIZE\tNumber of Context Switch\n");
    for(int i=1; i<=getMaxPid(); i++)
    {
        pid = i;
        if(getProcInfo(pid, &info) == 0)
	  printf(1, "%d\t%d\t%d\n", info.pid, info.psize, info.numberContextSwitches);
    }
    exit();
}
