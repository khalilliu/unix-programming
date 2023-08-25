/*
 * lserv_funcs1.c: functions for the license server
 */
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <sys/errno.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include <errno.h>
#include "dgram.h"

#define RECLAIM_INTERVAL 60
#define SERVER_PORT 2020
#define MSG_LEN 128

// if a ticket is available, ticket_array[index] will be TICKET_AVAIL.
// Else, it will be the pid of the client
#define TICKET_AVAIL 0

#define MAX_USERS 3 // how many tickets do we have
#define oops(msg) \
  {               \
    perror(msg);  \
    exit(1);      \
  }

int ticket_array[MAX_USERS]; // ticker array
int server_sock = -1;        // server socket
int num_tickets_out = 0;     // number of tickets been given away

void free_all_tickets()
{
  for (int i = 0; i < MAX_USERS; i++)
  {
    ticket_array[i] = TICKET_AVAIL;
  }
}

int setup()
{
  server_sock = make_dgram_server_socket(SERVER_PORT);
  if (server_sock == -1)
  {
    oops("make socket");
  }
  free_all_tickets();
  return server_sock;
}

void shut_down()
{
  close(server_sock);
}

void narrate(char *msg1, char *msg2, struct sockaddr_in *client_p)
{
  fprintf(stderr, "\t\tSERVER: %s %s", msg1, msg2);

  if (client_p != NULL)
  {
    // print ip address and port number
    fprintf(stderr, "(%s: %d)", inet_ntoa(client_p->sin_addr),
            ntohs(client_p->sin_port));
  }
  putchar('\n');
}

char *do_hello(char *msg)
{
  if (num_tickets_out >= MAX_USERS)
  {
    return "FAIL no tickets avaliable";
  }

  int available_ticekt_index;
  for (available_ticekt_index = 0; available_ticekt_index < MAX_USERS && ticket_array[available_ticekt_index] != TICKET_AVAIL; available_ticekt_index++)
    ;

  if (available_ticekt_index == MAX_USERS)
  {
    narrate("database corrupt", "", NULL);
    return "FAIL database corrupt";
  }

  ticket_array[available_ticekt_index] = atoi(msg + 5);
  static char reply_buf[MSG_LEN];
  sprintf(reply_buf, "TICK %d %d", ticket_array[available_ticekt_index], available_ticekt_index);
  num_tickets_out += 1;
  return reply_buf;
}

char *do_goodbye(char *msg)
{
  uint32_t pid = 0;
  int index = 0;
  if (sscanf(msg + 5, "%d %d", &pid, &index) != 2)
  {
    narrate("Bogus ticket", msg + 5, NULL);
    return "FAIL invalid ticket";
  }

  ticket_array[index] = TICKET_AVAIL;
  num_tickets_out -= 1;

  return "THNX see ya!";
}

void handle_request(char *req, struct sockaddr_in *client, socklen_t add_len)
{
  char *response = NULL;
  int ret = 0;
  if (strncmp(req, "HELO", 4) == 0)
  {
    response = do_hello(req);
  }
  else if (strncmp(req, "GBYE", 4) == 0)
  {
    response = do_goodbye(req);
  }
  else
  {
    response = "FAIL invalid request";
  }

  narrate("SAID: ", response, client);
  assert(client->sin_family == AF_INET);
  ret = sendto(server_sock, response, strlen(response), 0, (struct sockaddr *)client, add_len);
  if (ret == -1)
  {
    perror("SERVER sendto failed");
  }
}

int is_pid_dead(pid_t pid)
{
  return kill(pid, 0) == -1 && errno == ESRCH;
}

void ticket_reclaim()
{
  char tick[BUFSIZ];

  for (int i = 0; i < MAX_USERS; i++)
  {
    if (ticket_array[i] != TICKET_AVAIL && is_pid_dead(ticket_array[i]))
    {
      sprintf(tick, "%d %d", ticket_array[i], i);
      narrate("freeing", tick, NULL);
      ticket_array[i] = TICKET_AVAIL;
      num_tickets_out -= 1;
    }
  }
  alarm(RECLAIM_INTERVAL);
}