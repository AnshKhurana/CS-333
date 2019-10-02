#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>

int item_to_produce,item_to_consume,numitems;
int total_items, max_buf_size, num_workers,size,num_masters;
int* arr; 
pthread_mutex_t lock;
pthread_cond_t empty,full;
// declare any global data structures, variables, etc that are required
// e.g buffer to store items, pthread variables, etc

void print_produced(int num,int producer) {

  printf("Produced %d by producer %d\n", num, producer);
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
	
	
    int master_thread_id = *((int *)data);
	while(1)
	{
		pthread_mutex_lock(&lock);

		if(item_to_produce >= total_items){
			pthread_cond_broadcast(&empty);
			pthread_mutex_unlock(&lock);
			break;
		}

		while(size==max_buf_size)
		{
			if(item_to_produce >= total_items) break;
			pthread_cond_broadcast(&empty);
			pthread_cond_wait(&full, &lock);
		}

		if(item_to_produce >= total_items){
			pthread_cond_broadcast(&empty);
			pthread_mutex_unlock(&lock);
			break;
		}
		size++;
		arr[size-1]=item_to_produce;
		print_produced(item_to_produce,master_thread_id);
		item_to_produce++;
		pthread_cond_broadcast(&full);
		pthread_cond_broadcast(&empty);
		pthread_mutex_unlock(&lock);
	}
	
	return 0;
}


void *consumer(void *data1)
{
	
	

	while(1)
	{
		pthread_mutex_lock(&lock);
		int thread_id = *((int *)data1);
		if(numitems>=total_items)
		{
			pthread_cond_broadcast(&full);
			pthread_mutex_unlock(&lock);
			break;
		}
		while(size==0)
		{
			if(numitems>=total_items)
			{
				//pthread_mutex_unlock(&lock);
				break;
			}    
			pthread_cond_broadcast(&full);
			pthread_cond_wait(&empty,&lock);
		}
		
		if(numitems>=total_items)
		{
			pthread_cond_broadcast(&full);
			pthread_mutex_unlock(&lock);
			break;
		}

		item_to_consume=arr[size-1];
		size--;
		print_consumed(item_to_consume,thread_id);
		numitems++;
		pthread_cond_broadcast(&full);
		pthread_mutex_unlock(&lock);
	}
	
	return 0;

}
//write function to be run by worker threads
//ensure that the workers call the function print_consumed when they consume an item

int main(int argc, char *argv[])
{

  int master_thread_id = 0;
  
  item_to_produce = 0;
  item_to_consume = 0;
  numitems=0;
  
  if (argc < 4) {
	printf("./master-worker #total_items #max_buf_size #num_workers e.g. ./exe 10000 1000 4\n");
	exit(1);
  }
  else {
  	num_masters = atoi(argv[4]);
	num_workers = atoi(argv[3]);
	total_items = atoi(argv[1]);
	max_buf_size = atoi(argv[2]);
  }
  
  // Initlization code for any data structures, variables, etc
  arr = (int*)malloc(max_buf_size*sizeof(int));
  size = 0;
  pthread_mutex_init(&lock,NULL);
  pthread_cond_init(&empty,NULL);
  pthread_cond_init(&full,NULL);
  //create master producer thread
  pthread_t master_thread[num_masters];
  int master_id[num_masters];
  for (int i = 0; i < num_masters; ++i)
  {
  	/* code */
  	master_id[i] = i;

  }
  for(int i=0;i<num_masters;i++){
  	pthread_create(&master_thread[i], NULL, generate_requests_loop, (void *)&master_id[i]);
	}
  //create worker consumer threads
  pthread_t worker_thread[num_workers];
  int worker_id[num_workers];
  for (int i = 0; i < num_workers; ++i)
  {
  	worker_id[i]=i+num_masters+1;
  	/* code */
  }
  for(int i=0;i<num_workers;i++)
  {
	pthread_create(&worker_thread[i], NULL, consumer, (void *)&worker_id[i]);
  }

  //wait for all threads to complete
  for(int i=0;i<num_masters;i++){
  	pthread_join(master_thread[i], NULL);
  }
  printf("master joined\n");
  pthread_cond_broadcast(&empty);
  for(int i=0;i<num_workers;i++)
  {
  	pthread_join(worker_thread[i],NULL);
  }
  //deallocate and free up any memory you allocated
  
  return 0;
}
