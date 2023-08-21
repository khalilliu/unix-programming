#ifndef UTMPLIB
#define UTMPLIB

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utmp.h>
#include <unistd.h>
#include <stdlib.h>

#define NRECS 16
#define NULLUT ((struct utmp *)NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE];
static int num_recs;
static int cur_rec;
static int fd_utmp = -1;

int utmp_open(char *filename)
{
  fd_utmp = open(filename, O_RDONLY);
  cur_rec = 0;
  num_recs = 0;
  return fd_utmp;
}

int utmp_reoload()
{
  int amt_read = read(fd_utmp, &utmpbuf, NRECS * UTSIZE);
  if (amt_read == -1)
  {
    fprintf(stderr, "Error: can not read from the utmp file");
    perror("");
    exit(-1);
  }
  num_recs = amt_read / UTSIZE;
  cur_rec = 0;
  return num_recs;
}

struct utmp *utmp_next()
{
  struct utmp *recp = NULLUT;
  if (fd_utmp == -1)
  {
    return recp;
  }

  if (cur_rec == num_recs && utmp_reoload() == 0)
  {
    return recp;
  }

  recp = (struct utmp *)(&utmpbuf[cur_rec * UTSIZE]);
  cur_rec++;
  return recp;
}

void utmp_close()
{
  if (fd_utmp != -1)
  {
    close(fd_utmp);
  }
}

#endif