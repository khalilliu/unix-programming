#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

FILE *my_popen(const char *command, char *type)
{
  assert(command != NULL);
  assert(type != NULL);

  // get a pipe
  int pipe_fd[2] = {-1, -1};
  if (pipe(pipe_fd) == -1)
  {
    perror("Can not pipe");
    return NULL;
  }

  // fork a process
  int pid = fork();
  if (pid == -1)
  {
    perror("Can not fork");
    return NULL;
  }
  else if (pid == 0)
  {
    if (*type == 'r')
    {
      close(pipe_fd[0]);
      // redirect stdout to the pipe write end
      dup2(pipe_fd[1], 1);
    }
    else if (*type == 'w')
    {
      close(pipe_fd[1]);
      // redirect stdin to the pipe read end
      dup2(pipe_fd[0], 0);
    }
    else
    {
      fprintf(stderr, "invalid type argument");
      return NULL;
    }
    execl("/usr/bin/bash", "sh", "-c", command, NULL);
  }
  else
  {
    if (*type == 'r')
    {
      close(pipe_fd[1]); // close the write end
      return fdopen(pipe_fd[0], type);
    }
    else if (*type == 'w')
    {
      close(pipe_fd[0]);
      return fdopen(pipe_fd[1], type);
    }
    else
    {
      fprintf(stderr, "invalid type argument");
      return NULL;
    }
  }
}

int main(int argc, char const *argv[])
{
  FILE *fp = NULL;
  char buf[100] = {'\0'};
  int i = 0;

  fp = my_popen("ls|head", "r");

  while (fgets(buf, 100, fp) != NULL)
  {
    printf("%3d %s", i++, buf);
  }
  pclose(fp);

  return 0;
}
