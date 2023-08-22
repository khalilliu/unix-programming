#include <stdio.h>

#define QUESTION "Do you want another transaction"

int get_response(char *question)
{
  printf("%s (y/n)", question);
  while (1)
  {
    switch (getchar())
    {
    case 'y':
    case 'Y':
      return 0;
    case 'n':
    case 'N':
    case EOF:
      return 1;
    }
  }
}

int main(int argc, char const *argv[])
{
  int response = get_response(QUESTION);
  return response;
}
