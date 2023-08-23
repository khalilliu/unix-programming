#include <unistd.h>
#include <stdio.h>

extern char **environ;

int main()
{
  char *table[3] = {
      NULL, NULL, NULL}; // don't forget that this string-array needs to be NULL-terminated
  table[0] = "TERM=vt100";
  table[1] = "/on/the/range";

  environ = table;

  execlp("env", "env", NULL);
}