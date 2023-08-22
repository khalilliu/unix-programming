
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>
#include <fcntl.h>
#include <termio.h>

#define TRIES 3
#define SLEEPTIME 2
#define QUESTION "Do you want another transaction"
#define BEEP putchar('\a');

int get_ok_char()
{
  int c;
  while ((c = getchar()) != EOF && strchr("ynYN", c) == NULL)
  {
    continue;
  }
  assert(c >= -1 &&
         c <= 127);
  return c;
}

int get_response(char *question, int max_tries)
{
  int input;
  printf("%s (y/n)", question);
  fflush(stdout);

  while (1)
  {
    sleep(SLEEPTIME);
    input = tolower(get_ok_char());
    max_tries -= 1;
    if (input == (int)'y')
    {
      return 0;
    }
    if (input == (int)'n')
    {
      return 1;
    }
    if (0 == max_tries)
    {
      return 2;
    }
    BEEP;
  }
}

void set_cr_noecho_mode()
{
  struct termios ttyinfo;
  if (-1 == tcgetattr(0, &ttyinfo))
  {
    perror("tcgetattr");
    exit(EXIT_FAILURE);
  }

  ttyinfo.c_lflag &= ~ICANON; // enter noncanonical mode
  ttyinfo.c_lflag &= ~ECHO;   // disable ECHO bit
  ttyinfo.c_cc[VMIN] = 1;     // set minimum number of bytes been read to 1

  if (-1 == tcsetattr(0, TCSANOW, &ttyinfo))
  {
    perror("tcsetattr");
    exit(EXIT_FAILURE);
  }
}

void set_non_blocking_mode()
{
  int terflags;
  terflags = fcntl(0, F_GETFL);
  terflags |= O_NONBLOCK;
  fcntl(0, F_SETFL, terflags);
}

void tty_mode(int how)
{
  static struct termios orig_mode;
  static int orig_flags;
  if (how == 0)
  {
    tcgetattr(0, &orig_mode);
    orig_flags = fcntl(0, F_GETFL);
  }
  else if (how == 1)
  {
    tcsetattr(0, TCSANOW, &orig_mode);
    fcntl(0, F_SETFL, orig_flags);
  }
}

int main()
{
  int response = 0;
  tty_mode(0);
  set_cr_noecho_mode();
  set_non_blocking_mode();
  response = get_response(QUESTION, TRIES);
  tty_mode(1);
  return response;
}