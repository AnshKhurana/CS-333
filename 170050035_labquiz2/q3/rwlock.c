#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock *rw)
{
  zem_init(&(rw->reader_can_enter), 1);
  zem_init(&(rw->writer_can_enter), 1);
  zem_init(&(rw->lock), 1);
  rw->reader_count = 0;

}

void ReaderLock(struct read_write_lock *rw)
{
  zem_down(&(rw->lock));
  zem_down(&(rw->reader_can_enter));
  rw->reader_count++;
  if (rw->reader_count==1)
  	{
  	zem_up(&(rw->lock));
  	zem_down(&(rw->writer_can_enter));
  	}
  zem_up(&(rw->reader_can_enter));
  zem_up(&(rw->lock));
}

void ReaderUnlock(struct read_write_lock *rw)
{
  //	Write the code for releasing read-write lock by the reader.
  // zem_down(&(rw->lock));
  zem_down(&(rw->lock));
  rw->reader_count--;
  if (rw->reader_count == 0)
  {
  	zem_up(&(rw->reader_can_enter));
  	zem_up(&(rw->writer_can_enter));
  }
  else
  {
  zem_up(&(rw->reader_can_enter));
  }
  zem_up(&(rw->lock));
}

void WriterLock(struct read_write_lock *rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  // zem_down(&(rw->lock));
  	
  	zem_down(&(rw->writer_can_enter));
  	zem_down(&(rw->reader_can_enter));
  // zem_up(&(rw->lock));
}

void WriterUnlock(struct read_write_lock *rw)
{
  //	Write the code for releasing read-write lock by the writer.
  // zem_down(&(rw->lock));
  	zem_up(&(rw->reader_can_enter));
  	zem_up(&(rw->writer_can_enter));
  // zem_up(&(rw->lock));
}
