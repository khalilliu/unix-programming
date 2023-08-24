#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int ac, char **av)
{
  if (ac != 3)
  {
    fprintf(stderr, "usage: pipe2 cmd1 cmd2\n");
    exit(1);
  }

  int pipe_fd[2] = {-1, -1};
  if (pipe(pipe_fd) == -1)
  {
    fprintf(stderr, "Can not get a pipe\n");
    perror("pipe");
    exit(1);
  }

  int pid = fork();
  if (pid == -1)
  {
    fprintf(stderr, "Can not fork\n");
    perror("fork");
    exit(1);
  }
  else if (pid == 0)
  {
    close(pipe_fd[0]);
    dup2(pipe_fd[1], 1);
    close(pipe_fd[1]);
    execlp(av[1], av[1], NULL);
    perror("can not execute cmd1 in the first child-process");
  }

  pid = fork();
  if (pid == -1)
  {
    fprintf(stderr, "Can not fork\n");
    perror("fork");
    exit(1);
  }
  else if (pid == 0)
  {
    close(pipe_fd[1]);
    dup2(pipe_fd[0], 0);
    close(pipe_fd[0]);
    execlp(av[2], av[2], NULL);
    perror("can not execute cmd2 in the first child-process");
  }

  close(pipe_fd[0]);
  close(pipe_fd[1]);

  return 0;
}