// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

void freerange(void *vstart, void *vend);
// extern int ret_num_free_pages(void);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
  int pgcount[PHYSTOP/PGSIZE];
} kmem;

int num_free_pages = 0; // Should be 2 GB worth of pages
// PGSIZE 
// 
// struct spinlock count_lock;


void incrementCount(int pgno)
{
 
  if (kmem.use_lock)
  {
    acquire(&kmem.lock);
  }
   
  kmem.pgcount[pgno]++;

  if (kmem.use_lock)
  {
    release(&kmem.lock);
  }

  
}
void decrementCount(int pgno)
{
  if (kmem.use_lock)
  {
    acquire(&kmem.lock);
  }
  
  kmem.pgcount[pgno]--;
  
  if (kmem.use_lock)
  {
    release(&kmem.lock);
  }

}

int getCount(int pgno)
{
  int refCount=0;
  if (kmem.use_lock)
  {
      acquire(&kmem.lock);
  }
  
  refCount = kmem.pgcount[pgno];
  if (kmem.use_lock)
  {
    release(&kmem.lock);
  }
  return refCount;
}

// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the kphysical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  kmem.use_lock = 0;
  for (int i = 0; i < PHYSTOP/PGSIZE; ++i)
  {
    kmem.pgcount[i] = 0;
  }
  // num_free_pages = num_free_pages - ( (char *) vend - (char *) vstart )/PGSIZE;
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  // num_free_pages = num_free_pages - ( (char *) vend - (char *) vstart )/PGSIZE;
  freerange(vstart, vend);
  kmem.use_lock = 1;
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
    kfree(p);
}
//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");

  if (kmem.use_lock)
  {
    acquire(&kmem.lock);
  }


  
  if (kmem.pgcount[(uint)V2P(v) / PGSIZE] > 0)
  {
      kmem.pgcount[(uint)V2P(v) / PGSIZE]--;
  }
  if (kmem.pgcount[(uint)V2P(v)/ PGSIZE] == 0)
  {
    /* code */
    // Fill with junk to catch dangling refs.
  memset(v, 1, PGSIZE);
  // if(kmem.use_lock)
  //   acquire(&kmem.lock);
  r = (struct run*)v;
  r->next = kmem.freelist;
  kmem.freelist = r;
  num_free_pages = num_free_pages + 1;
  
  }

  if(kmem.use_lock)
    release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;

  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  // cprintf("Found a free list head\n");   
  num_free_pages = num_free_pages -1;

  kmem.pgcount[(uint)V2P((char*) r) / PGSIZE] = 1;
  if(kmem.use_lock)
    release(&kmem.lock);
  return (char*)r;
}

int ret_num_free_pages()
{
  return num_free_pages;
}