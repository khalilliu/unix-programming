#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXARGS 20 /* cmd line args*/
#define ARGLEN 100 /* token length*/

char *make_string(char *buf);
int execute(char *arg_list[]);

int main(int argc, char const *argv[])
{
  int num_args = 0;
  char buf[ARGLEN + 1];
  *buf = '\0';

  char *arg_list[MAXARGS + 1];
  for (int i = 0; i < MAXARGS + 1; i++)
  {
    arg_list[i] = NULL;
  }

  while (num_args < MAXARGS)
  {
    printf("arg[%d]", num_args);
    if (fgets(buf, ARGLEN, stdin) && *buf != '\n')
    {
      arg_list[num_args++] = make_string(buf);
    }
    else
    {
      if (num_args > 0)
      {
        arg_list[num_args] = NULL;
        execute(arg_list);
        num_args = 0;
      }
    }
  }
  return 0;
}

int execute(char *arglist[])
{
  if (-1 == execvp(arglist[0], arglist))
  {
    const char *p = NULL;
    for (int i = 0; i < MAXARGS + 1; i++)
    {
      p = arglist[i];
      if (p != NULL)
      {
        fprintf(stderr, "debug: arg[%d]: %s\n", i, p);
      }
    }
    perror("execvp failed");
    exit(EXIT_FAILURE);
  }
}

char *make_string(char *buf)
{
  // trim the rightmost newline char
  buf[strlen(buf) - 1] = '\0';

  // allocate the memory
  char *cp = (char *)malloc(sizeof(strlen(buf) + 1));

  if (NULL == cp)
  {
    fprintf(stderr, "no memory available");
    exit(EXIT_FAILURE);
  }
  strncpy(cp, buf, strlen(buf) + 1);

  return cp;
}