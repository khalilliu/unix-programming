/*
 * selectdemo.c: watch for input on two devices
 *
 * usage: ./selectdemo dev1 dev2 timeout
 *
 * action: reports on input from each file, and reports timeouts
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

void show_data(char *, int);

#define oops(m, x) \
  {                \
    perror(m);     \
    exit(x);       \
  }

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    fprintf(stderr, "usage: ./selectdemo file1 file2 timeout");
    exit(1);
  }

  int fd1 = open(argv[1], O_RDONLY);
  if (fd1 == -1)
  {
    oops(argv[1], 2);
  }
  int fd2 = open(argv[2], O_RDONLY);
  if (fd2 == -1)
  {
    oops(argv[2], 3);
  }
  int max_fd = 1 + (fd1 > fd2 ? fd1 : fd2);

  fd_set read_fds;
  struct timeval timeout;
  int ret_val = -1;

  while (1)
  {
    FD_ZERO(&read_fds);
    FD_SET(fd1, &read_fds);
    FD_SET(fd2, &read_fds);

    timeout.tv_sec = (long)atoi(argv[3]);
    timeout.tv_usec = 0;

    // wait for input
    ret_val = select(max_fd, &read_fds, NULL, NULL, &timeout);
    if (ret_val == -1)
    {
      oops("select", 4);
    }

    if (ret_val > 0)
    {
      if (FD_ISSET(fd1, &read_fds))
      {
        show_data(argv[1], fd1);
      }
      if (FD_ISSET(fd2, &read_fds))
      {
        show_data(argv[2], fd2);
      }
    }
    else
    {
      printf("no input after %d seconds\n", atoi(argv[3]));
    }
  }

  return 0;
}

void show_data(char *file_name, int fd)
{
  char buf[BUFSIZ];

  printf("%s: ", file_name);
  fflush(stdout);
  int n = read(fd, buf, BUFSIZ);
  if (n == -1)
  {
    oops(file_name, 5);
  }

  write(1, buf, n);
  write(1, "\n", 1);
}