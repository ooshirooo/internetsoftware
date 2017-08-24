#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
 
 
#define SERVER_NAME "n-lab.info"
#define ERR -1
#define BUFSIZE 1024
 
void usage(void);
char *command_name;
int socket(int domain, int type, int protocol);
 
int main(int argc, char* argv[]){
  int sds;                 //ソケットディスクリプタ
  FILE *fp;
  struct sockaddr_in server;//サーバプロセスのソケットアドレス情報
  struct hostent *hp; //ホスト情報
  char buf[BUFSIZE];
  char *mes;
  
 
  if(argc != 6)
    usage(); 
  
  /*ソケットの作成*/
  if((sds = socket(PF_INET, SOCK_STREAM, 0)) == ERR){
    perror("client: socket");
    exit(1);
  }
  /*サーバの情報を格納*/
  memset((void*)&server, 0, sizeof(server));
  server.sin_family = PF_INET;
  server.sin_port = htons(25); //ポート番号25
//   printf("%d", server.sin_family);
  
  /*IPアドレスの設定*/
  hp = gethostbyname(argv[4]);
  memcpy(&(server.sin_addr), hp->h_addr_list[0], hp->h_length);
  
  /*サーバに接続*/
  if(connect(sds, (struct sockaddr *)&server, sizeof(server))==ERR){
    perror("client: connect");
    exit(1);
  }
  
  read(sds, buf, sizeof(buf));
  printf("%s", buf);
 
/* HELOの送信 */
  strcpy(buf,"HELO ");
  strcat(buf,argv[5]);
  strcat(buf,"\n");
//   strcpy(buf, "HELO ie.u-ryukyu.ac.jp \n");
  printf("書き込む内容//%s",buf);
  write(sds, buf, strlen(buf));  
  read(sds, buf, sizeof(buf));
  printf("読み込んだ内容//%s", buf);
 
  
  /* MAIL FROMの送信 */ 
  strcpy(buf, "MAIL FROM: ");
  strcat(buf,argv[1]); 
  strcat(buf, "\r\n"); 
  printf("書き込む内容//%s",buf);
  write(sds, buf, strlen(buf)); 
  read(sds, buf, sizeof(buf));
  printf("読み込んだ内容//%s", buf);
 
 
/* RCPT TOの送信 */
  strcpy(buf, "RCPT TO: ");
  strcat(buf, argv[2]);
  strcat(buf, "\r\n");
  printf("書き込む内容//%s",buf);
  write(sds, buf, strlen(buf)); 
  read(sds, buf, sizeof(buf));
  printf("読み込んだ内容//%s", buf);
  
  strcpy(buf,"DATA\r\n");
  printf("書き込む内容//%s",buf);
  write(sds, buf, strlen(buf)); 
  read(sds, buf, sizeof(buf));
  printf("読み込んだ内容//%s", buf);
  strcpy(buf,"From:");
  strcat(buf,argv[1]);
  strcat(buf,"\r\n");
  strcat(buf,"To:");
  strcat(buf,argv[2]);
  strcat(buf,"\n");
  strcat(buf,argv[3]);
  strcat(buf,"\r\n");
  printf("書き込む内容//%s",buf);
  write(sds, buf, strlen(buf)); 
  strcpy(buf,".\r\n");
  write(sds, buf, strlen(buf));
  printf("書き込む内容//%s",buf);
  read(sds, buf, sizeof(buf));
  printf("読み込んだ内容//%s", buf);

  
//   /* DATA の送信 */
//   write(sds, "DATA ", sizeof(buf)); 
//   read(sds, write, sizeof(buf));
//   
//   fp = fopen(argv[3], "r");
//   while(fgets(buf, 1024, fp) != NULL){
//     write(sds, fp, sizeof(buf));
//  }
//   
  write(sds, "QUIT", sizeof(buf));  
  read(sds, write, sizeof(buf));
  
  return 0;
}
 
void usage(void)
{
  fprintf(stderr, "Usage: %s 差出 宛先 テキスト IP ドメイン\n",command_name);
  exit(1);
}
