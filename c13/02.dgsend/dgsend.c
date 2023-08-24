#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "dgram.h"

#define oops(msg) \
  {               \
    perror(msg);  \
    exit(1);      \
  }

int main(int ac, char *av[])
{
  if (ac != 4)
  {
    fprintf(stderr, "usage: ./dgsend hostname port_num message");
    exit(1);
  }
  int client_fd = make_dgram_client_socket();
  if (client_fd == -1)
  {
    oops("socket");
  }
  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  make_internet_address(av[1], (uint16_t)atoi(av[2]), &server_address);

  if (-1 == sendto(client_fd, av[3], strlen(av[3]), 0,
                   (struct sockaddr *)&server_address,
                   sizeof(server_address)))
  {
    oops("sendto failed");
  }

  char buf[BUFSIZ];
  memset(buf, 0, BUFSIZ);
  int msglen = 0;

  struct sockaddr_in client_address;
  memset(&client_address, 0, sizeof(client_address));
  socklen_t address_len = 0;
  while ((msglen = recvfrom(client_fd, buf, BUFSIZ, 0,
                            (struct sockaddr *)&client_address,
                            &address_len)) > 0)
  {
    printf("dgrecv: got a message: %s\n", buf);
  }
  return 0;
}
