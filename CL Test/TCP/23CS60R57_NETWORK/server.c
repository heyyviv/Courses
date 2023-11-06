#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>


 
int main(){
 
  char *ip = "127.0.0.1";
  int port = 5566;
 
  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  char buffer[1024];
  int n;
  int temp_time;
 
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0){
    perror("[-]Socket error");
    exit(1);
  }
  printf("[+]TCP server socket created.\n");
 
  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);
 
  n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (n < 0){
    perror("[-]Bind error");
    exit(1);
  }
  printf("[+]Bind to the port number: %d\n", port);
 
  listen(server_sock, 5);
  printf("Listening...\n");
 
  while(1){
    struct timeval stop, start;
    
    addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
    printf("[+]Client connected.\n");
    int untill_end=1;
    while(untill_end){
      bzero(buffer, 1024);
      recv(client_sock, buffer, sizeof(buffer), 0);
      printf("Client sent : %s\n", buffer);

      int ack_received=1;

      while(ack_received){
          //send ACK
          bzero(buffer, 1024);
          strcpy(buffer, "OK");
          printf("Server Sending : %s\n", buffer);
          gettimeofday(&start, NULL);
          send(client_sock, buffer, strlen(buffer), 0);
          //received ACK
          bzero(buffer, 1024);
          int signal_ack = recv(client_sock, buffer, sizeof(buffer), 0);
          if(signal_ack<=0){continue;}
          gettimeofday(&stop, NULL);
          printf("Client sent : %s\n", buffer);
          printf("RTT %lu micro second \n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
          temp_time = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
          printf("IP address is: %s\n", inet_ntoa(client_addr.sin_addr));
          printf("port is: %d\n", (int) ntohs(client_addr.sin_port));
          ack_received=0;
        }
      if(strcmp(buffer,"END")==0){break;}
    }

    FILE *fp;
    fp = fopen("client_detail.txt","a");
    
    fprintf(fp,"%s %d %d \n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),temp_time);
    fclose(fp);
    
    close(client_sock);
    printf("[+]Client disconnected.\n\n");
 
  }
  return 0;
}

