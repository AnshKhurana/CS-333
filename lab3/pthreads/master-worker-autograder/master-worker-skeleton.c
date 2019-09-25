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
int ind = 0;
int buffer[1000000];


pthread_mutex_t buffer_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  full_cond  = PTHREAD_COND_INITIALIZER;
pthread_cond_t  empty_cond  = PTHREAD_COND_INITIALIZER;


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
void *generate_requests_loop(void *data)
{
  int thread_id = *((int *)data);
  while(1)
    {
      
    pthread_mutex_lock(&buffer_mutex);
    if(item_to_produce >= total_items)
    {
        pthread_cond_broadcast(&empty_cond);    
        pthread_mutex_unlock(&buffer_mutex);
        break;
    }
    while (ind > max_buf_size - 1)
    {
        pthread_cond_wait(&full_cond, &buffer_mutex);
    }

    buffer[ind] = item_to_produce;
    ind = ind + 1;
    print_produced(item_to_produce);
    item_to_produce++;
    pthread_cond_signal(&empty_cond);
    pthread_mutex_unlock(&buffer_mutex);      
    }
  return 0;
}

//write function to be run by worker threads
//ensure that the workers call the function print_consumed when they consume an item

void *consume_from_buffer(void *data)
{
    int thread_id = *((int *)data);
    while(1)
    {
    
    
    pthread_mutex_lock(&buffer_mutex);
    while (ind < 1 && item_to_produce < total_items) pthread_cond_wait(&empty_cond, &buffer_mutex);
    
    if(ind < 1)
    {
        if(item_to_produce >= total_items)
        {
        // pthread_cond_signal(&full_cond);
        pthread_mutex_unlock(&buffer_mutex);
        break;
        }
        
    }
   
   
    int num = buffer[ind-1];
    ind = ind - 1;

 
    // if (ind == max_buf_size-1)
    pthread_cond_signal(&full_cond);

    print_consumed(num, thread_id);
    pthread_mutex_unlock(&buffer_mutex);
    }


    return 0;
}

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
  pthread_t worker_threads[num_workers];
  int worker_thread_id = 0;

  //create master producer thread
  pthread_create(&master_thread, NULL, generate_requests_loop, (void *)&master_thread_id);

  //create worker consumer threads
  int ids[num_workers];

  for (int i = 0; i < num_workers; ++i)
  {
      ids[i] = i;
  }
  for (int i = 0; i < num_workers; ++i)
  {
    pthread_create(&worker_threads[i], NULL, consume_from_buffer, (void *)&ids[i]);
    
  }

  //wait for all threads to complete
  pthread_join(master_thread, NULL);
  printf("master joined\n");

  for (int i = 0; i < num_workers; ++i)
      {
      pthread_join(worker_threads[i], NULL);
      // printf("worker thread %d joined\n", ids[i]);
  }

  //deallocate and free up any memory you allocated
  
  return 0;
}
