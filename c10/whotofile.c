#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

void redirect(int new_fd, char *filename)
{
  int old_fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
  assert(old_fd != -1);
  assert(new_fd == dup2(old_fd, new_fd));
  assert(close(old_fd) != -1);
}

int main(int argc, char const *argv[])
{
  int pid;
  int fd;

  printf("Abount to run who into a file\n");
  if ((pid = fork()) == -1)
  {
    perror("fork");
    exit(1);
  }

  if (pid == 0)
  {
    // redirect(1, "userlist");
    // if (-1 == execlp("who", "who", NULL))
    // {
    //   perror("execlp");
    //   exit(1);
    // }

    close(1);
    fd = open("userlist", O_CREAT | O_RDWR | O_TRUNC, 0644);
    execlp("who", "who", NULL);
    exit(1);
  }

  if (pid != 0)
  {
    wait(NULL);
    printf("Done running who\n");
  }

  return 0;
}
