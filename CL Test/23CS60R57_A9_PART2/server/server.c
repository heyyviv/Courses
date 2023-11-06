#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <dirent.h>
#define  MAXSIZE 1024
int mutex;

// Structure to hold client information
struct ClientInfo {
    int client_sock;
    struct sockaddr_in client_addr;
};

void *handle_client(void *client_info_ptr) {
    struct ClientInfo *client_info = (struct ClientInfo *)client_info_ptr;
    int client_sock = client_info->client_sock;
    struct sockaddr_in client_addr = client_info->client_addr;



    char buffer[1024];
    char currentDirectory[1024];
    char* command[100];
    char* token;
    int index=0;
    getcwd(currentDirectory, sizeof(currentDirectory));

    while(1){
      bzero(buffer, MAXSIZE);
      recv(client_sock, buffer, sizeof(buffer), 0);
      printf("Command received: %s\n", buffer);





      if (strcmp(buffer, "put") == 0) {

              bzero(buffer, MAXSIZE);
              recv(client_sock, buffer, sizeof(buffer), 0);
              // Extract the filename from the command
              char* filename = buffer;

              // Create a file to write the incoming data
              FILE* uploadedFile = fopen(filename, "w");
              printf("file name received: %s\n", buffer);


               while (1) {
                  int bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
                  printf("received %s \n",buffer);
                  if (bytes_received <= 0) {
                      break;
                  }
                  if(strcmp(buffer,"end")==0){break;}
                  bzero(buffer, MAXSIZE);
                  fwrite(buffer, 1, bytes_received, uploadedFile);
                }
              // Receive the file data from the client and write it to the file

              // Close the file and notify the client about successful upload
              fclose(uploadedFile);
              printf("File uploaded: %s\n", filename);




        } else if (strcmp(buffer, "get") == 0) {

              recv(client_sock, buffer, sizeof(buffer), 0);
            // Extract the filename from the command
            char* filename = buffer;


            // Open the requested file for reading
            FILE* requestedFile = fopen(filename, "r");


            // Read and send the file data to the client
            char line[1000];
              while (fgets(line, sizeof(line), requestedFile) != NULL) {
                send(client_sock, line, strlen(line), MSG_DONTWAIT);
                sleep(1);
            }
            send(client_sock, "end", 3, 0);


            // Close the file
            fclose(requestedFile);
            printf("File sent: %s\n", filename);


        } else if (strcmp(buffer, "close") == 0) {
              break;

        } else if (strcmp(buffer, "cd") == 0) {


              char* targetDirectory =buffer;

              // Attempt to change the current working directory
              if (chdir(targetDirectory) == 0) {
                  // Handle error: Unable to change the directory
                  getcwd(currentDirectory, sizeof(currentDirectory));
                  send(client_sock, "ok", 2, 0);
                  sleep(1);
              }else{
              send(client_sock, "incorrect", 10, 0);
              }



        } else if (strcmp(buffer, "ls") == 0) {
            // List files in the current directory
            // Implement the directory listing logic here



              // Open the current directory for listing
              DIR* dir = opendir(currentDirectory);

              // Read the directory contents and send them to the client
              struct dirent* entry;
              while ((entry = readdir(dir)) != NULL) {
                  send(client_sock, entry->d_name, strlen(entry->d_name), 0);
                  sleep(1);
              }
              send(client_sock, "end", 3, 0);
              sleep(1);

              // Close the directory
              closedir(dir);



        } else {
            printf("end");
            break;
        }




    }


    close(client_sock);
    printf("[+]Client disconnected.\n\n");

    // Clean up and exit the thread
    free(client_info_ptr);
    pthread_exit(NULL);

}

 
int main(){
 
  char *ip = "127.0.0.1";
  int port = 2121;
 
  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;

  int n;

 
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

    
    addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);

    printf("accept \n");
    // Create a thread to handle the client
    pthread_t client_thread;
    struct ClientInfo *client_info = (struct ClientInfo *)malloc(sizeof(struct ClientInfo));
    client_info->client_sock = client_sock;
    client_info->client_addr = client_addr;
    handle_client((void *)client_info);
    if (pthread_create(&client_thread, NULL, handle_client, (void *)client_info) != 0) {
            perror("[-]Thread creation error");
            exit(1);
        }

  }
  return 0;
}


