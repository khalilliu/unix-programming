#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define oops(s, x) \
  {                \
    perror(s);     \
    exit(x);       \
  }

int main(int argc, char const *argv[])
{
  struct termios buf;
  if (argc == 1)
  {
    exit(EXIT_SUCCESS);
  }
  if (tcgetattr(0, &buf) == -1)
  {
    oops("tcgetattr", EXIT_FAILURE);
  }
  if (argv[1][0] == 'y')
  {
    buf.c_lflag |= ECHO;
  }
  else
  {
    buf.c_lflag &= ~ECHO;
  }
  if (tcsetattr(0, TCSANOW, &buf) == -1)
  {
    oops("tcsetattr", EXIT_FAILURE);
  }
  return 0;
}
