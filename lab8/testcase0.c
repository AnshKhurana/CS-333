#include "simplefs-ops.h"

int main(){
  simplefs_formatDisk();
  int ret_val = simplefs_create("fileabc");
  printf("inode number: %d\n", ret_val);
  int fd = simplefs_open("fileabc");
  printf("fd given to new created file: %d\n", fd);
  char str[] = "!-----------------------64 Bytes of Data-----------------------!";
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("block written\n");
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  char buf[BLOCKSIZE * 2 + 1];
  buf[BLOCKSIZE * 2] = '\0';
  printf("Seek: %d\n", simplefs_seek(fd, -BLOCKSIZE * 2));
  printf("Read Data %d\n", simplefs_read(fd, buf, BLOCKSIZE * 2));
  printf("Data: %s\n", buf);
  simplefs_close(fd);
  simplefs_dump();
}
