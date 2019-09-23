#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
    rw->m = PTHREAD_MUTEX_INITIALIZER;
    rw->readercv = PTHREAD_COND_INITIALIZER; 
    rw->writercv = PTHREAD_COND_INITIALIZER; 
    rw->readerscount = 0;
    rw->writerwaiting = 0;
    rw->writerPresent = false;
}

void ReaderLock(struct read_write_lock * rw)
{
  pthread_mutex_lock(&(rw->m));
  while (rw->writerPresent)
  {
    pthread_cond_wait(&(rw->readercv),  &(rw->m));
  }
  rw->readerscount++;
  pthread_mutex_unlock(&(rw->m));
}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
    pthread_mutex_lock(&(rw->m));
    rw->readerscount--;
    if (rw->readerscount == 0)
        pthread_cond_signal(&(rw->writercv));
    pthread_mutex_unlock(&(rw->m));
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
    pthread_mutex_lock(&(rw->m));
    while(rw->writerPresent || rw->readerscount > 0)
    {
        pthread_cond_wait(&(rw->writercv), &(rw->m));
    }
    rw->writerPresent = true;
    pthread_mutex_unlock(&(rw->m));
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
    pthread_mutex_lock(&(rw->m));
    rw->writerPresent = false;
    pthread_cond_broadcast(&(rw->readercv));
    pthread_cond_signal(&(rw->writercv));
    pthread_mutex_unlock(&(rw->m));
}
