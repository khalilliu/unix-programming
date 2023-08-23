#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <assert.h>
#include <unistd.h>
#include "smsh.h"
#include "varlib.h"

extern char **environ;

int execute(char *argv[])
{
  pid_t pid;
  int child_info = -1;

  if (argv[0] == NULL)
  {
    return 0;
  }

  if ((pid = fork()) == -1)
  {
    perror("fork()");
    return -1;
  }
  else if (pid == 0)
  {
    environ = VLtable2environ();
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    if (execvp(argv[0], argv) == -1)
    {
      perror("execvp()");
      exit(-1);
    }
  }
  else
  {
    if (wait(&child_info) == -1)
    {
      perror("wait()");
      return -1;
    }
  }
  return child_info;
}