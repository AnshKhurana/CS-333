#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(void)
{
  printDiskStats();
  printBufferCacheStats();
  printFileTableStats();
  printFDStats();
  
  int fd = open("tmp.txt", O_CREATE|O_RDWR);
  printf(1, "opened file\n");

  printDiskStats();
  printBufferCacheStats();
  printFileTableStats();
  printFDStats();

  char buf[1000];
  for(int i=0; i < 1000; i++)
    buf[i]='x';

  if(write(fd, buf, 1000) < 0)
    exit();
  
  printf(1, "wrote 1000 bytes to file\n");

  printDiskStats();
  printBufferCacheStats();
  printFileTableStats();
  printFDStats();

  exit();
}
