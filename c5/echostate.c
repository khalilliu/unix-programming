#include <stdio.h>
#include <stdlib.h>
#include <termio.h>

int main(int argc, char const *argv[])
{
  struct termios buf;
  if (tcgetattr(0, &buf) == -1)
  {
    perror("tcgetattr");
    exit(EXIT_FAILURE);
  }
  if (buf.c_lflag & ECHO)
  {
    printf("echo is on, since its bit is 1\n");
  }
  else
  {
    printf("echo is OFF, since its bit is 0\n");
  }
  return 0;
}
