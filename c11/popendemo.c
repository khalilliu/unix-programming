#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
  FILE *fp = NULL;
  char buf[100] = {'\0'};
  int i = 0;

  fp = popen("ls|head", "r");
  while (fgets(buf, 100, fp) != NULL)
  {
    printf("%3d %s", i++, buf);
  }
  pclose(fp);
  return 0;
}
