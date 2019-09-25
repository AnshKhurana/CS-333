#include "rwlock.h"

struct read_write_lock rwlock;

pthread_mutex_t printLock;

void *Reader(void* arg)
{
  int threadNUmber = *((int *)arg);
  
  // Occupying the Lock
  ReaderLock(&rwlock);
  pthread_mutex_lock(&printLock);
  printf("Reader: %d has acquired the lock\n", threadNUmber);
  pthread_mutex_unlock(&printLock);
  
  usleep(2000000);
  
  // Releasing the Lock
  pthread_mutex_lock(&printLock);
  ReaderUnlock(&rwlock);
  printf("Reader: %d has released the lock\n",threadNUmber);
  pthread_mutex_unlock(&printLock);
  return 0;
  
}

void *Writer(void* arg)
{
  int threadNUmber = *((int *)arg);
  
  // Occupying the Lock
  WriterLock(&rwlock);
  pthread_mutex_lock(&printLock);
  printf("Writer: %d has acquired the lock\n",threadNUmber);
  pthread_mutex_unlock(&printLock);

  
  usleep(2000000);
  
  // Releasing the Lock
  pthread_mutex_lock(&printLock);
  WriterUnlock(&rwlock);
  printf("Writer: %d has released the lock\n",threadNUmber);
  pthread_mutex_unlock(&printLock);

  return 0;
}

int main(int argc, char *argv[])
{
  printLock = PTHREAD_MUTEX_INITIALIZER;
  int *threadNUmber;
  pthread_t *threads;

  int read_num_threads;
  int write_num_threads;
  
  if (argc < 3) {
    printf("./exe #num_readers #num_writers\n");
    exit(1);
  }
  else {
    read_num_threads = atoi(argv[1]);
    write_num_threads = atoi(argv[2]);
  }
  
  InitalizeReadWriteLock(&rwlock);
  
  int num_threads = 2*read_num_threads + write_num_threads;
  
  threads = (pthread_t*) malloc(num_threads * (sizeof(pthread_t)));

  int val1[read_num_threads];
  
  int count = 0;
  for(int i=0;i<read_num_threads;i++)
    {
      
      val1[i] = i;
      int ret = pthread_create(threads+count,NULL,Reader,(void*) &val1[i]);
      if(ret)
	{
	  fprintf(stderr,"Error - pthread_create() return code: %d\n",ret);
	  exit(EXIT_FAILURE);
	}
      count++;
      usleep(1000);
    }
  
    int val2[write_num_threads];

  for(int i=0;i<write_num_threads;i++)
    {
      val2[i] = i;
      int ret = pthread_create(threads+count,NULL,Writer,(void*) &val2[i]);
      if(ret)
	{
	  fprintf(stderr,"Error - pthread_create() return code: %d\n",ret);
	  exit(EXIT_FAILURE);
	}
      count++;
      usleep(1000);
    }
  
    int val3[read_num_threads];

  for(int i=0;i<read_num_threads;i++)
    {
      
      val3[i] = read_num_threads + i;
      int ret = pthread_create(threads+count,NULL,Reader,(void*) &val3[i]);
      if(ret)
	{
	  fprintf(stderr,"Error - pthread_create() return code: %d\n",ret);
	  exit(EXIT_FAILURE);
	}
      count++;
      usleep(1000);
    }
  
  
  for(int i=0;i<num_threads; i++)
    pthread_join(threads[i],NULL);
  
  exit(1);
}
