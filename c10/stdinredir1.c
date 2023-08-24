#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  char buf[100] = {'\0'};

  fgets(buf, 100, stdin);
  printf("%s", buf);
  fgets(buf, 100, stdin);
  printf("%s", buf);
  fgets(buf, 100, stdin);
  printf("%s\n", buf);
  close(0);

  int fd = open("/etc/passwd", O_RDONLY);
  if (fd != 0)
  {
    fprintf(stderr, "Can not open data as fd 0");
    exit(1);
  }
  fgets(buf, 100, stdin);
  printf("%s", buf);
  fgets(buf, 100, stdin);
  printf("%s", buf);
  fgets(buf, 100, stdin);
  printf("%s\n", buf);
  close(fd);
  return 0;
}
