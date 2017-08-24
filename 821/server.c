/* ********************************** */
/* sample program of tcp-server v0.94 */
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
  struct sockaddr_storage ss; /* address of server */
  int len;                    /* length of address */
  char mesg[BUFFER];          /* message words */
  char recv_buf[BUFFER] ;     /* recieve buffer */
  char send_buf[BUFFER];      /* send buffer */

  /* check of argument */

  if (argc != 2) {
    fprintf(stderr, "Usage: %s (port) \n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* create and bind socket */

  s = make_socket(argv[1]);

  /* waiting for connection */

  listen(s, 5);

  /* main loop */

  while (1) {

    /* accept of connection */

    len = sizeof(ss);
    if ((s = accept(s, (struct sockaddr*) &ss, &len)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    while (1) {
      int i = 0;  /* count of receiving character */

      n = sprintf(send_buf, "TCP> ");
      send(s, send_buf, n, 0);

      /* data processing of stream type */

      for(i=0; i < BUFFER - 1; i++){
        if ((recv(s, &recv_buf[i], 1, 0)) < 0) {
          perror("while receiving data");
	  break;
        }
	
        /* recieve processing by new-line */

        if (recv_buf[i] == '\n') {
          break;
        }
      }

      recv_buf[i] = '\0';
      printf("receive '%s'\n", recv_buf);

      /* process of recieving message */

      if((sscanf(recv_buf, "%s", mesg)) <= 0)
	continue;

      n = sprintf(send_buf, "%s\n", mesg);

      if (n == 0)
        n = sprintf(send_buf, "\n");

      if (send(s, send_buf, n, 0) < 0)
        break;
      printf("send %s", send_buf);

    }
    printf("connection closed.\n");
    close(s);

  }
  return(EXIT_SUCCESS);

}

int make_socket(const char *port)
{

  struct addrinfo hints;     /* address of server */
  struct addrinfo *ai;       /* address information */
  int s;                     /* file descriptor for socket */

  /* resolve of server's IP address */

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  if ((getaddrinfo(NULL, port, &hints, &ai)) != 0) {
    fprintf(stderr, "can't resolve address\n");
    exit(EXIT_FAILURE);
  }

  /* create socket */

  if ((s = socket(ai->ai_family, ai->ai_socktype, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  /* bind socket */

  if (bind(s, ai->ai_addr, ai->ai_addrlen) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(ai);
  return(s);

}
