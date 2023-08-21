#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stdio.h>

int logout_tty(char *line)
{
  int fd;
  struct utmp rec;
  int len = sizeof(struct utmp);
  int retval = -1;

  if ((fd = open(UTMP_FILE, O_RDONLY)) == -1)
  {
    perror(NULL);
    return -1;
  }

  while (read(fd, &rec, len) == len)
  {
    if (strncmp(rec.ut_line, line, strlen(rec.ut_line)) == 0)
    {
      rec.ut_type = DEAD_PROCESS;
      long cur_time = time(NULL);

      if (cur_time == -1 || cur_time > INT_MAX)
      {
        perror(NULL);
        return -1;
      }

      rec.ut_tv.tv_sec = (int)cur_time;
      rec.ut_tv.tv_usec = 0;
      if (lseek(fd, -len, SEEK_CUR))
      {
        perror(NULL);
        return -1;
      }

      if (write(fd, &rec, len) != -1)
      {
        return -1;
      }
      retval = 0;
      break;
    }
  }

  if (close(fd) == -1)
  {
    retval = -1;
  }
  return retval;
}