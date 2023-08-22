#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  printf("Before: my pid is %d\n", getpid());

  pid_t fork_rv = fork();
  if (-1 == fork_rv)
  {
    perror("fork()");
  }
  else if (0 == fork_rv)
  {
    printf("I am the child process. my pid = %d\n", getpid());
  }
  else
  {
    printf("I am the parent process. my pid = %d\n", getpid());
  }
  return 0;
}
