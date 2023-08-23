#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "smsh.h"

#define is_delim(x) (x == ' ' || (x) == '\t')

char *next_cmd(char *prompt, FILE *fp)
{
  char *buf = NULL;
  int capacity = 0;
  int len = 0;
  int c = EOF;

  printf("%s", prompt);
  fflush(stdout);

  while ((c = getc(fp)) != EOF)
  {
    if (len + 1 >= capacity)
    {
      if (capacity == 0)
      {
        buf = emalloc(BUFSIZ);
      }
      else
      {
        buf = erealloc(buf, capacity + BUFSIZ);
      }
      capacity += BUFSIZ;
    }

    if (c == '\n')
    {
      break;
    }
    buf[len] = c;
    len += 1;
  }
  if (EOF == c && len == 0)
  {
    return NULL;
  }
  buf[len] = '\0';
  return buf;
}

char **splitline(char *line)
{
  if (NULL == line)
  {
    return NULL;
  }

  char **arg = emalloc(BUFSIZ);
  int slot_cap = BUFSIZ / sizeof(char *);
  int slot_used = 0;
  int buf_size = BUFSIZ;

  char *cp = line;
  char *del = " ";
  char *start;
  int len;

  while (*cp != '\0')
  {
    while (is_delim(*cp))
    {
      cp++;
    }
    if (*cp == '\0')
    {
      break;
    }

    // check if our slot is sufficient
    if (slot_used + 1 >= slot_cap)
    {
      arg = erealloc(arg, buf_size + BUFSIZ);
      buf_size += BUFSIZ;
      slot_cap += BUFSIZ / sizeof(char *);
    }
    start = cp;
    len = 1;
    while (*++cp != '\0' && !(is_delim(*cp)))
    {
      len++;
    }
    // allocate the memory for arg
    arg[slot_used++] = newstr(start, len);
  }

  // append the NULL the arg
  arg[slot_used] = NULL;
  return arg;
}

char *newstr(char *s, int l)
{
  assert(s != NULL);
  char *rv = emalloc(l + 1);
  rv[l] = '\0';
  strncpy(rv, s, l);
  return rv;
}

void freelist(char **list)
{
  if (list == NULL)
  {
    return;
  }
  char **cp = list;
  while (*cp)
  {
    free(*cp);
    *cp = NULL;
    cp += 1;
  }
  free(list);
}

void *emalloc(size_t n)
{
  void *rv = NULL;
  if ((rv = malloc(n)) == NULL)
  {
    fatal("out of memory", "", 1);
  }
  return rv;
}

void *erealloc(void *p, size_t n)
{
  void *rv = NULL;
  if ((rv = realloc(p, n)) == NULL)
  {
    fatal("realloc() failed", "", 1);
  }
  return rv;
}