/*
 * timeclnt.c: a client for timeserv
 *
 * usage: timeclnt 127.0.0.1 PORT_NUM(13000)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define oops(msg) \
  {               \
    perror(msg);  \
    exit(1);      \
  }

int main(int argc, char const *argv[])
{

  assert(argc == 3);

  int clnt_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (clnt_fd == -1)
  {
    oops("socket");
  }

  // connect to the server
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_port = htons(atoi(argv[2]));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  if (connect(clnt_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
  {
    oops("connect");
  }
  // transfer data from server, then hang up
  char buf[BUFSIZ] = {'\0'};
  int len = read(clnt_fd, buf, BUFSIZ);
  if (len == 1)
  {
    oops("read");
  }
  if (len != write(1, buf, len))
  {
    oops("write");
  }
  close(clnt_fd);
  return 0;
}
