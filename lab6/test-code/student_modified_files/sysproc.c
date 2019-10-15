#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_getNumFreePages(void)
{
  return ret_num_free_pages();
}

int 
sys_getNumPhysPages(void)
{
  struct proc *p = myproc();
  char *a, *last;
  a = (char*)PGROUNDDOWN((uint)0);
  last = (char*)PGROUNDDOWN(((uint)0) + p->sz - 1);
  int count = 0;
  pde_t *pgdir = p->pgdir;
  for(;;)
  {
    pde_t *pde;
    pde = &pgdir[PDX(a)];
    if(*pde & PTE_P)  
      count ++;
    if(a == last)
      return count;
    a += PGSIZE;
  }
  return 0;
}

int 
sys_getNumVirtPages(void)
{
  uint mysize = myproc()->sz;
  return mysize/PGSIZE;
}

int 
sys_getNumPTPages(void)
{
  int num_pages=0;
  pde_t *pgdir = myproc()->pgdir;
  for (int i = 0; i < NPDENTRIES; ++i)
  {
    if (pgdir[i] & PTE_P)
    {
      num_pages++;
    }
  }

  return num_pages + 1;
}
