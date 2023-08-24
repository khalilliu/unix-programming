#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define oops(m, x) \
  {                \
    perror(m);     \
    exit(x);       \
  }

void fatal(char *);
void be_dc(int *, int *);
void be_bc(int *, int *);

int main(int argc, char const *argv[])
{
  int pid = -1;
  int to_dc[2] = {-1, -1};
  int from_dc[2] = {-1, -1};

  if (pipe(to_dc) == -1 || pipe(from_dc) == -1)
  {
    oops("pipe failed", 1)
  }

  pid = fork();
  if (pid == -1)
  {
  }
  else if (pid == 0) // child proces
  {
    be_dc(to_dc, from_dc);
  }
  else // parent process
  {
    be_bc(to_dc, from_dc);
    wait(NULL);
  }
  return 0;
}

void be_dc(int to_dc[2], int from_dc[2])
{
  if (dup2(to_dc[0], 0) == -1)
  {
    oops("dc: cannot redirect stdin", 3);
  }
  close(to_dc[0]);
  close(to_dc[1]);

  if (dup2(from_dc[1], 1) == -1)
  {
    oops("dc: cannot redirect stdout", 4)
  }
  close(from_dc[0]);
  close(from_dc[1]);

  // now execl `dc` with `-` option (to make it read from stdin)
  execlp("dc", "dc", "-", NULL);
  oops("cannot run dc", 5);
}

void be_bc(int to_dc[2], int from_dc[2])
{
  int num1 = 0;
  int num2 = 0;
  char operation[BUFSIZ] = {'\0'};
  char message[BUFSIZ] = {'\0'};
  FILE *fpout = NULL;
  FILE *fpin = NULL;

  close(to_dc[0]);
  close(from_dc[1]);

  fpout = fdopen(to_dc[1], "w");
  fpin = fdopen(from_dc[0], "r");
  if (fpout == NULL || fpin == NULL)
  {
    fatal("Error converting pipes to streams");
  }

  printf("tinybc: ");
  while (fgets(message, BUFSIZ, stdin) != NULL)
  {
    if (sscanf(message, "%d %s %d", &num1, operation, &num2) != 3)
    {
      printf("syntax error\n");
      continue;
    }
    if (fprintf(fpout, "%d\n%d\n%c\np\n", num1, num2, *operation) == EOF)
    {
      fatal("Error writing");
    }
    fflush(fpout);

    if (fgets(message, BUFSIZ, fpin) == NULL)
    {
      break;
    }

    printf("%d %c %d = %s", num1, *operation, num2, message);
    printf("tinybc: ");
  }
  fclose(fpout);
  fclose(fpin);
}

void fatal(char *msg)
{
  fprintf(stderr, "Error: %s\n", msg);
  exit(1);
}