#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char const *argv[])
{
  int fd = 0;
  char buf[BUFSIZ];

  if (argc != 2)
  {
    fprintf(stderr, "Usage: write0 ttyname\n");
    exit(-1);
  }

  fd = open(argv[1], O_WRONLY);
  if (fd == -1)
  {
    perror(argv[1]);
    exit(-1);
  }

  while (fgets(buf, BUFSIZ, stdin) != NULL)
  {
    printf("one iteration executed\n");
    if (write(fd, buf, strlen(buf)) == -1)
    {
      break;
    }
    printf("Write one piece of contents\n");
  }
  close(fd);
  return 0;
}
