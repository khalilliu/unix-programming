#include <time.h>
#include <string.h>
#include <ctype.h>
#include "utmplib.h"

void show_time(int);
void show_info(struct utmp *);
char *trim(char *);

void show_info(struct utmp *utbufp)
{
  if (utbufp->ut_type != USER_PROCESS)
  {
    return;
  }
  printf("%-8.8s", utbufp->ut_user);
  printf(" ");
  printf("%-8.8s", utbufp->ut_line);
  printf(" ");
  show_time(utbufp->ut_tv.tv_sec);
  if (utbufp->ut_host[0] != '\0')
  {
    printf("(%s)", utbufp->ut_host);
  }
  printf("\n");
}

char *trim(char *s)
{
  char *ptr;
  if (!s)
    return NULL; // handle NULL string
  if (!*s || strlen(s) == 0)
    return s; // handle empty string
  for (ptr = s + strlen(s) - 1; (ptr >= s) && isspace(*ptr); --ptr)
    ;
  ptr[1] = '\0';
  return s;
}

void show_time(int seconds)
{
  time_t sec = (time_t)seconds;
  time_t *p = &sec;
  char *s = trim(ctime(p));
  printf("%s", s + 4);
}

int main()
{
  struct utmp *utbufp;
  if (utmp_open(UTMP_FILE) == -1)
  {
    perror(UTMP_FILE);
    exit(1);
  }
  while ((utbufp = utmp_next()) != NULL)
  {
    show_info(utbufp);
  }
  utmp_close();
  return 0;
}