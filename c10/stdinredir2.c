#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  char buf[100] = {'\0'};
  int fd;
  int newfd;

  fgets(buf, 100, stdin);
  printf("%s", buf);
  fgets(buf, 100, stdin);
  printf("%s", buf);
  fgets(buf, 100, stdin);
  printf("%s\n", buf);

  fd = open("/etc/passwd", O_RDONLY);
  // close(0);
  // newfd = dup(fd);
  newfd = dup2(fd, 0);

  if (newfd != 0)
  {
    fprintf(stderr, "Can not open data as fd 0");
    exit(1);
  }

  close(fd);

  fgets(buf, 100, stdin);
  printf("%s", buf);
  fgets(buf, 100, stdin);
  printf("%s", buf);
  fgets(buf, 100, stdin);
  printf("%s\n", buf);
  close(newfd);
  return 0;
}
