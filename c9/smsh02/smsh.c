#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "smsh.h"

#define DFL_PROMPT "> "

void fatal(char *s1, char *s2, int n)
{
  fprintf(stderr, "Error: %s, %s\n", s1, s2);
  exit(n);
}

void setup()
{
  signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
}

int main(int argc, char const *argv[])
{
  char *cmdline = NULL;
  char *prompt = DFL_PROMPT;
  char **arg = NULL;
  int result, process(char **);
  ;

  setup();
  while ((cmdline = next_cmd(prompt, stdin)) != NULL)
  {
    if ((arg = splitline(cmdline)) != NULL)
    {
      result = process(arg);
      freelist(arg);
    }
    free(cmdline);
  }

  return 0;
}
