#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  rw->m = PTHREAD_MUTEX_INITIALIZER;
  rw->reader = PTHREAD_COND_INITIALIZER;
  rw->writer = PTHREAD_COND_INITIALIZER;
  rw->writer_waiting = 0;
  rw->writer_present = false;
  rw->read_count = 0;
}

void ReaderLock(struct read_write_lock * rw)
{
    pthread_mutex_lock(&(rw->m));
    while(rw->writer_present)
    {
        pthread_cond_wait(&(rw->reader), &(rw->m));
    } 
    rw->read_count++;
    pthread_mutex_unlock(&(rw->m));
}

void ReaderUnlock(struct read_write_lock * rw)
{
  pthread_mutex_lock(&(rw->m));
  rw->read_count--;
  if (rw->read_count == 0)
  {
      pthread_cond_signal(&(rw->writer));
  }

  pthread_mutex_unlock(&(rw->m));
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
    pthread_mutex_lock(&(rw->m));
    while(rw->read_count>0 || rw->writer_present)
        pthread_cond_wait(&(rw->writer), &(rw->m));
    rw->writer_present = true;
    pthread_mutex_unlock(&(rw->m));

}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
    pthread_mutex_lock(&(rw->m));
    rw->writer_present =  false;
    pthread_cond_broadcast(&(rw->reader));
    pthread_cond_broadcast(&(rw->writer));
    pthread_mutex_unlock(&(rw->m));
}
