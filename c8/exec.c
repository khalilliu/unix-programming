#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  char *arglist[3];

  arglist[0] = "ls";
  arglist[1] = "-l";
  arglist[2] = NULL;

  printf("About to exec ls -l\n");

  execvp("ls", arglist);
  printf("ls is done\n");
  return 0;
}
