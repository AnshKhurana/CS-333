#include "simplefs-ops.h"

int main(){
  // 1
  simplefs_formatDisk();
  simplefs_create("fileab1");
  int fd = simplefs_open("fileab1");
  char str[] = "!-----------------------64 Bytes of Data-----------------------!";
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  char buf[BLOCKSIZE * 4 + 1];
  buf[BLOCKSIZE * 4] = '\0';
  printf("Seek: %d\n", simplefs_seek(fd, -BLOCKSIZE * 4));
  printf("Read Data %d\n", simplefs_read(fd, buf, BLOCKSIZE * 4));
  printf("Data: %s\n", buf);
  simplefs_close(fd);
  simplefs_dump();


// 2
  
  simplefs_create("fileab2");
  fd = simplefs_open("fileab2");
  
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  
  buf[BLOCKSIZE * 4] = '\0';
  printf("Seek: %d\n", simplefs_seek(fd, -BLOCKSIZE * 4));
  printf("Read Data %d\n", simplefs_read(fd, buf, BLOCKSIZE * 4));
  printf("Data: %s\n", buf);
  simplefs_close(fd);
  simplefs_dump();


  // 3
  
  simplefs_create("fileab3");
  fd = simplefs_open("fileab3");
  
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  
  buf[BLOCKSIZE * 4] = '\0';
  printf("Seek: %d\n", simplefs_seek(fd, -BLOCKSIZE * 4));
  printf("Read Data %d\n", simplefs_read(fd, buf, BLOCKSIZE * 4));
  printf("Data: %s\n", buf);
  simplefs_close(fd);
  simplefs_dump();


  // 4
  
  simplefs_create("fileab4");
  fd = simplefs_open("fileab4");
  
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));

  buf[BLOCKSIZE * 4] = '\0';
  printf("Seek: %d\n", simplefs_seek(fd, -BLOCKSIZE * 4));
  printf("Read Data %d\n", simplefs_read(fd, buf, BLOCKSIZE * 4));
  printf("Data: %s\n", buf);
  simplefs_close(fd);
  simplefs_dump();

  // 5
  
  simplefs_create("fileab5");
  fd = simplefs_open("fileab5");
  
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  
  buf[BLOCKSIZE * 4] = '\0';
  printf("Seek: %d\n", simplefs_seek(fd, -BLOCKSIZE * 4));
  printf("Read Data %d\n", simplefs_read(fd, buf, BLOCKSIZE * 4));
  printf("Data: %s\n", buf);
  simplefs_close(fd);
  simplefs_dump();


  // 6
  
  simplefs_create("fileab6");
  fd = simplefs_open("fileab6");
  
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  
  buf[BLOCKSIZE * 4] = '\0';
  printf("Seek: %d\n", simplefs_seek(fd, -BLOCKSIZE * 4));
  printf("Read Data %d\n", simplefs_read(fd, buf, BLOCKSIZE * 4));
  printf("Data: %s\n", buf);
  simplefs_close(fd);
  simplefs_dump();


  // 7
  
  simplefs_create("fileab7");
  fd = simplefs_open("fileab7");
  
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  printf("Write Data: %d\n", simplefs_write(fd, str, BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  
  buf[BLOCKSIZE * 4] = '\0';
  printf("Seek: %d\n", simplefs_seek(fd, -BLOCKSIZE * 4));
  printf("Read Data %d\n", simplefs_read(fd, buf, BLOCKSIZE * 4));
  printf("Data: %s\n", buf);
  simplefs_close(fd);
  simplefs_dump();


  // 8
  
  simplefs_create("fileab8");
  fd = simplefs_open("fileab8");
  char strbig[] = "!-----------------------64 Bytes of Data-----------------------!!-----------------------64 Bytes of Data-----------------------!!-----------------------64 Bytes of Data-----------------------!!-----------------------64 Bytes of Data-----------------------!";
  printf("Write Data: %d\n", simplefs_write(fd, strbig, 4*BLOCKSIZE));
  printf("Seek: %d\n", simplefs_seek(fd, BLOCKSIZE));
  
  
  simplefs_close(fd);
  simplefs_dump();





}
