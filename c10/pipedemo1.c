#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  int a_pipe[2] = {0};
  char buf[101] = {'\0'};
  int len = 0;

  if (pipe(a_pipe) == -1)
  {
    perror("pipe");
    exit(1);
  }
  printf("get a pipeits file descriptors are: %d & %d\n", a_pipe[0],
         a_pipe[1]);

  fgets(buf, 100, stdin);
  len = strlen(buf);

  if (len != write(a_pipe[1], buf, len))
  {
    perror("write");
    exit(1);
  }
  close(a_pipe[1]);
  for (int i = 0; i < len; i += 1)
  {
    buf[i] = '\0';
  }

  len = read(a_pipe[0], buf, 100);
  if (len == -1)
  {
    perror("read");
    exit(1);
  }
  close(a_pipe[0]);
  printf("read from a_pipe[0]: %s\n", buf);
  return 0;
}
