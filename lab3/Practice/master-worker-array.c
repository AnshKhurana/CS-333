#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>

int item_to_produce;
int total_items, max_buf_size, num_workers;
// declare any global data structures, variables, etc that are required
// e.g buffer to store items, pthread variables, etc
int* buffer;
int sz = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full, empty = PTHREAD_COND_INITIALIZER;


void print_produced(int num) {

  printf("Produced %d\n", num);
}

void print_consumed(int num, int worker) {

  printf("Consumed %d by worker %d\n", num, worker);
  
}


/* produce items and place in buffer (array or linked list)
 * add more code here to add items to the buffer (these items will be consumed
 * by worker threads)
 * use locks and condvars suitably
 */

void *consume_request(void *data)
{
  int thread_id = *((int *)data);
  
  while(1)
  {
    pthread_mutex_lock(&m);
    if (item_to_produce >= total_items && sz <=0)
    {
      pthread_cond_broadcast(&empty);
      pthread_mutex_unlock(&m);
      return 0;
    }
    while(sz <= 0)
    {
      pthread_cond_wait(&empty, &m);
      if (item_to_produce >= total_items && sz <=0)
    {
      pthread_cond_broadcast(&empty);
      pthread_mutex_unlock(&m);
      return 0;
    } 
      
    }
    sz--;
    
    print_consumed(buffer[sz], thread_id);
    
    pthread_cond_signal(&full);
    pthread_mutex_unlock(&m);
  }
  return 0; 
}

void *generate_requests_loop(void *data)
{
  int thread_id = *((int *)data);

  while(1)
    {

      pthread_mutex_lock(&m);
      if(item_to_produce >= total_items)
      {
        pthread_cond_broadcast(&empty);
        pthread_mutex_unlock(&m);
        break;
      }
      while(sz >= max_buf_size)
      {
        pthread_cond_wait(&full, &m);
      }
      print_produced(item_to_produce);
      buffer[sz] = item_to_produce;
      item_to_produce++;
      sz++;
      pthread_cond_signal(&empty);
      pthread_mutex_unlock(&m);
    }
  return 0;
}

//write function to be run by worker threads
//ensure that the workers call the function print_consumed when they consume an item

int main(int argc, char *argv[])
{
 
  int master_thread_id = 0;
  pthread_t master_thread;
  item_to_produce = 0;

  
  if (argc < 4) {
    printf("./master-worker #total_items #max_buf_size #num_workers e.g. ./exe 10000 1000 4\n");
    exit(1);
  }
  else {
    num_workers = atoi(argv[3]);
    total_items = atoi(argv[1]);
    max_buf_size = atoi(argv[2]);
  }
  
  // Initlization code for any data structures, variables, etc
  buffer = (int*) malloc(max_buf_size * sizeof(int));

  //create master producer thread
  pthread_create(&master_thread, NULL, generate_requests_loop, (void *)&master_thread_id);

  //create worker consumer threads
  pthread_t worker[num_workers];
  int worker_ids[num_workers];
  for (int i = 0; i < num_workers; ++i)
  {
    worker_ids[i] = i;
  }

  for (int i = 0; i < num_workers; ++i)
  {
    pthread_create(&worker[i], NULL, consume_request, (void*) &worker_ids[i]);
  }
  //wait for all threads to complete
  pthread_join(master_thread, NULL);
  printf("master joined\n");
  for (int i = 0; i < num_workers; ++i)
  {
    pthread_join(worker[i], NULL);
  }

  //deallocate and free up any memory you allocated
  free(buffer);
  return 0;
}
  