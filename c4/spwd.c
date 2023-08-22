#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

ino_t get_inode(char *);
void print_path_to(ino_t);
void inode_to_name(ino_t, char *, int);

int main(int argc, char const *argv[])
{
  print_path_to(get_inode("."));
  putchar('\n');
  return 0;
}

ino_t get_inode(char *filename)
{
  struct stat buf;
  if (stat(filename, &buf) == -1)
  {
    fprintf(stderr, "cannot stat  ");
    perror(NULL);
    exit(-1);
  }
  return buf.st_ino;
}

void inode_to_name(ino_t this_inode, char *namebuf, int buflen)
{
  DIR *dir_ptr = NULL;
  struct dirent *dirent_ptr = NULL;

  if ((dir_ptr = opendir(".")) == NULL)
  {
    perror(NULL);
    exit(-1);
  }
  while ((dirent_ptr = readdir(dir_ptr)) != NULL)
  {
    struct stat buf;
    stat(dirent_ptr->d_name, &buf);
    if (buf.st_ino == this_inode)
    {
      strncpy(namebuf, dirent_ptr->d_name, buflen);
      closedir(dir_ptr);
      namebuf[buflen - 1] = '\0';
      return;
    }
  }
  fprintf(stderr, "error looking for inode: %ld\n", this_inode);
  exit(1);
}

void print_path_to(ino_t inode)
{
  ino_t my_inode = 0;
  char its_name[256];
  if (get_inode("..") != inode)
  {
    chdir("..");
    inode_to_name(inode, its_name, 256);
    my_inode = get_inode(".");
    print_path_to(my_inode);
    printf("/%s", its_name);
  }
}
