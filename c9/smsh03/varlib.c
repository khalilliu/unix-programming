/* varlib.c: a simple storage system to store name=value pairs with facility
 *           to mark items as part of the environment
 *
 * interface:
 *     VLstore( name, value )    returns 1 for 0k, 0 for no
 *     VLlookup( name )          returns string or NULL if not there
 *     VLlist()			 prints out current table
 *
 * environment-related functions
 *     VLexport( name )		 adds name to list of env vars
 *     VLtable2environ()	 copy from table to environ
 *     VLenviron2table()         copy from environ to table
 *
 * details:
 *	the table is stored as an array of structs that
 *	contain a flag for `global' and a single string of
 *	the form name=value.  This allows EZ addition to the
 *	environment.  It makes searching pretty easy, as
 *	long as you search for "name="
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "varlib.h"
#include <string.h>

#define MAXVARS 200

struct var
{
  char *str;
  int global;
};

static struct var tab[MAXVARS];
static char *new_string(char *, char *);
static struct var *find_item(char *, int);

int VLstore(char *name, char *val)
{
  /*
   * traverse list, if found, replace it, else add at end
   * since there is no delete, a blank one is a free one
   * return 1 if trouble, 0 if ok (like a command)
   */
  struct var *itemp;
  char *s;
  int rv = 1;

  if ((itemp = find_item(name, 1)) != NULL && (s = new_string(name, val)) != NULL)
  {
    if (itemp->str)
    {
      free(itemp->str);
    }
    itemp->str = s;
    rv = 0;
  }
  return rv;
}

char *new_string(char *name, char *val)
{
  char *retval;
  retval = malloc(strlen(name) + strlen(val) + 2);
  if (retval != NULL)
  {
    sprintf(retval, "%s=%s", name, val);
  }
  return retval;
}

char *VLlookup(char *name)
{
  struct var *itemp;
  if ((itemp = find_item(name, 0)) != NULL)
  {
    return itemp->str + 1 + strlen(name);
  }
  return "";
}

int VLexport(char *name)
{
  struct var *itemp;
  int rv = 1;
  if ((itemp = find_item(name, 0)) != NULL)
  {
    itemp->global = 1;
    rv = 0;
  }
  else if (VLstore(name, "") == 1)
  {
    rv = VLexport(name);
  }
  return rv;
}

static struct var *find_item(char *name, int first_blank)
{
  int i;
  int len = strlen(name);
  char *s;
  for (i = 0; i < MAXVARS && tab[i].str != NULL; i++)
  {
    s = tab[i].str;
    if (strncmp(s, name, len) == 0 && s[len] == '=')
    {
      return &tab[i];
    }
  }
  if (i < MAXVARS && first_blank)
  {
    return &tab[i];
  }
  return NULL;
}

void VLlist()
{
  int i;
  for (i = 0; i < MAXVARS && tab[i].str != NULL; i++)
  {
    if (tab[i].global)
    {
      printf("  * %s\n", tab[i].str);
    }
    else
    {
      printf("    %s\n", tab[i].str);
    }
  }
}

int VLenviron2table(char *env[])
{
  int i;
  char *newstring;

  for (i = 0; env[i] != NULL; i++)
  {
    if (i == MAXVARS)
    {
      return 0;
    }
    newstring = malloc(1 + strlen(env[i]));
    if (newstring == NULL)
    {
      return 0;
    }
    strcpy(newstring, env[i]);
    tab[i].str = newstring;
    tab[i].global = 1;
  }
  while (i < MAXVARS)
  {
    tab[i].str = NULL;
    tab[i++].global = 0;
  }
  return 1;
}

char **VLtable2environ()
{
  int i,         /* index			*/
      j,         /* another index		*/
      n = 0;     /* counter			*/
  char **envtab; /* array of pointers		*/

  /*
   * first, count the number of global variables
   */

  for (i = 0; i < MAXVARS && tab[i].str != NULL; i++)
    if (tab[i].global == 1)
      n++;

  /* then, allocate space for that many variables	*/
  envtab = (char **)malloc((n + 1) * sizeof(char *));
  if (envtab == NULL)
    return NULL;

  /* then, load the array with pointers		*/
  for (i = 0, j = 0; i < MAXVARS && tab[i].str != NULL; i++)
    if (tab[i].global == 1)
      envtab[j++] = tab[i].str;
  envtab[j] = NULL;
  return envtab;
}