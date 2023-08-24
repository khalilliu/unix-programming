#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHILD_MSG "Child: I wanna cookie\n"
#define PAR_MSG "Parent: testing...\n"

#define oops(m, x) \
  {                \
    perror(m);     \
    exit(x);       \
  }

int main(int argc, char const *argv[])
{
  int pipe_fd[2] = {-1, -1};
  int len = 0;
  char buf[100] = {'\0'};
  int read_len = 0;

  if (pipe(pipe_fd) == -1)
  {
    oops("can not get a pipe", 1);
  }

  switch (fork())
  {
  case -1:
    oops("cannot fork", 2);
    break;
  case 0: // in the child
    len = strlen(CHILD_MSG);
    while (1)
    {
      if (write(pipe_fd[1], CHILD_MSG, len) != len)
      {
        oops("write", 3);
      }
      sleep(5);
    }
    break;
  default: // in the parent
    len = strlen(PAR_MSG);
    while (1)
    {
      if (write(pipe_fd[1], PAR_MSG, len) != len)
      {
        oops("write", 4);
      }
      sleep(1);
      read_len = read(pipe_fd[0], buf, 100);
      printf("read_len: %d\n", read_len);
      if (read_len < 0)
      {
        exit(1);
      }
      else if (read_len == 0)
      {
        break;
      }
      write(1, buf, read_len);
    }

    break;
  }
  return 0;
}
