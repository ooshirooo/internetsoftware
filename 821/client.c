/* ********************************** */
/* sample program of tcp-client v0.94 */
/*   for basic-network-programming    */
/*              by T.Nagata.          */
/* ********************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER 1024

int main(int argc, char *argv[])
{

  int s;                      /* file descriptor for socket */
  int n;                      /* bytes of message */
  struct timeval tv;          /* time value of select */
  fd_set readfd;              /* file descriptor for select */
  char recv_buf[BUFFER];      /* recieve buffer */
  char send_buf[BUFFER];      /* send buffer */

  /* check of argument */

  if (argc != 3) {
    fprintf(stderr, "Usage: %s (hostname) (port) \n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* create socket and connect to server */

  s = connect_server(argv[1], argv[2]);

  /* main loop */

  while (1) {

    /* set of select timeout value */

    tv.tv_sec  = 30;
    tv.tv_usec = 0;

    /* check of input from standard input or server */

    FD_ZERO(&readfd);
    FD_SET(0, &readfd);
    FD_SET(s, &readfd);
    if ((select(s + 1, &readfd, NULL, NULL, &tv)) <= 0) {
      fprintf(stderr, "\nTimeout\n");
      break;
    }

    /* input from standard input */

    if (FD_ISSET(0, &readfd)) {
      if ((n = read(0, recv_buf, BUFFER - 1)) <= 0)
        break;
      recv_buf[n]='\0';
      sscanf(recv_buf, "%s", send_buf);
      if (strcmp(send_buf, "quit") == 0)
        break;
      if (send(s, recv_buf, n, 0) <= 0)
        break;
    }

    /* recieve from server */

    if (FD_ISSET(s, &readfd)) {
      if ((n = recv(s, recv_buf, BUFFER - 1, 0)) <= 0) {
        fprintf(stderr, "connection closed.\n");
        exit(EXIT_FAILURE);
      }
      recv_buf[n]='\0';
      printf("%s", recv_buf);
      fflush(stdout);
    }

  }
  strcpy(recv_buf, "quit");
  send(s, recv_buf, n, 0);
  close(s);

  return(EXIT_SUCCESS);

}

int connect_server(const char *hostname, const char *port)
{

  struct addrinfo hints;     /* address of server */
  struct addrinfo *ai;       /* address information */
  int s;                     /* file descriptor for socket */

  /* resolve of server's IP address */

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  if ((getaddrinfo(hostname, port, &hints, &ai)) != 0) {
    fprintf(stderr, "can't resolve address\n");
    exit(EXIT_FAILURE);
  }

  /* create socket */

  if ((s = socket(ai->ai_family, ai->ai_socktype, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  /* connect to server */

  if (connect(s, ai->ai_addr, ai->ai_addrlen) < 0) {
    perror("connect");
    exit(EXIT_FAILURE);
  }
  printf("connected to '%s'\n", hostname);

  freeaddrinfo(ai);
  return(s);

}
