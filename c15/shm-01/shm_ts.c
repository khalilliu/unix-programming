/*
 * shm_ts.c: the time server using shared memory, a bizarre application
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <time.h>

#define TIME_MEM_KEY 99        // identifier, like a filename
#define SEG_SIZE ((size_t)100) // size of segment
#define oops(m, x) \
  {                \
    perror(m);     \
    exit(x);       \
  }

int main(int argc, char const *argv[])
{
  int seg_id;
  char *mem_ptr = NULL;
  long now;
  int n;

  seg_id = shmget(TIME_MEM_KEY, SEG_SIZE, IPC_CREAT | 0777);
  if (seg_id == -1)
  {
    oops("shmget", 1);
  }

  mem_ptr = shmat(seg_id, NULL, 0);

  if (mem_ptr == (void *)-1)
  {
    oops("shmat", 2);
  }

  for (n = 0; n < 60; n++)
  {
    time(&now);
    strcpy(mem_ptr, ctime(&now));
    sleep(1);
  }

  shmctl(seg_id, IPC_RMID, NULL);
  return 0;
}
