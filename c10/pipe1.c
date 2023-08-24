#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define oops(m, x) \
  {                \
    perror(m);     \
    exit(x);       \
  }

int main(int argc, char const *argv[])
{
  int pipe_fd[2] = {-1, -1};
  if (argc != 3)
  {
    fprintf(stderr, "usage: pipe cmd1 cmd2\n");
    exit(1);
  }

  if (pipe(pipe_fd) == -1)
  {
    oops("Can not get a pipe", 1);
  }

  switch (fork())
  {
  case -1:
    oops("Can not fork", 2);
    break;
  case 0: // child process
    close(pipe_fd[0]);
    dup2(pipe_fd[1], 1);
    close(pipe_fd[1]);
    execlp(argv[1], argv[1], NULL);
    oops(argv[1], 5);
    break;
  default: // parent process
    close(pipe_fd[1]);
    dup2(pipe_fd[0], 0);
    close(pipe_fd[0]);
    execlp(argv[2], argv[2], NULL);
    oops(argv[2], 4);
    break;
  }
  return 0;
}
