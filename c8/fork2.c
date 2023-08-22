#include <stdio.h>
#include <unistd.h>

int main()
{
  printf("my pid is %d first\n", getpid());
  fork();
  fork();
  fork();
  printf("my pid is %d last\n", getpid());

  return 0;
}