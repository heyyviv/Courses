#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define  MAXSIZE 1024

int set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        return -1;
    }

    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    char ip[100];
    int port = 2141;
    int n_packet;
    int packet_delay;
    printf("Enter Server IP address: ");
    scanf("%s", ip);
    printf("Enter Server Port number: ");
    scanf("%d", &port);
    char ff;
    scanf("%c",&ff);


    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;
    struct timeval stop, start;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);
    while(1){
        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("Connect failed");
            continue;
        }else{
            break;
        }
    }




    while(1){
        printf("server>>");

        bzero(buffer, sizeof(buffer));
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            int index = 0;
            char* command[100];
            char* token;
            buffer[strlen(buffer)-1]='\0';


            token = strtok(buffer, " ");
            while (token) {
                command[index] = token;
                token = strtok(NULL, " ");
                index++;
            }
            //command[index++] = NULL;



            // Handle different commands based on the 'command' array
            if (strcmp(command[0], "put") == 0) {
                 send(sock, command[0], strlen(command[0]), MSG_DONTWAIT);
                 sleep(1);

                char* filename = command[1];
                send(sock, command[1], strlen(command[1]), MSG_DONTWAIT);
                sleep(1);

                // Open the local file for reading
                FILE* localFile = fopen(filename, "r");

                if (localFile == NULL) {
                    perror("Error opening local file");
                    send(sock, "end", 3, 0);
                } else {
                    while (1) {
                        size_t bytes_read = fread(buffer, 1, sizeof(buffer), localFile);
                        if (bytes_read <= 0) {
                            break;
                        }
                        send(sock, buffer, bytes_read, 0);
                        sleep(1);
                    }

                    // Send "end" to indicate the end of the file
                    send(sock, "end", 3, 0);

                    // Close the local file
                    fclose(localFile);
                    printf("file sent\n");
    }
            } else if (strcmp(command[0], "get") == 0) {
                // Implement the 'get' command logic
                send(sock, command[0], strlen(command[0]), 0);
                sleep(1);

                char* filename = command[1];

                send(sock, command[1], strlen(command[1]), 0);
                sleep(1);
              // Create a file to write the incoming data
              FILE* uploadedFile = fopen(filename, "w");
                printf("file name : %s\n",command[1]);
                size_t bytes_received;
                while (1) {
                    bytes_received = recv(sock, buffer, sizeof(buffer),0);
                    if(strcmp(buffer,"end")==0){break;}
                    fprintf(uploadedFile,"%s",buffer);
                    bzero(buffer, sizeof(buffer));
                  //fwrite(text, size, count, file);
                    }

            fclose(uploadedFile);
            } else if (strcmp(command[0], "close") == 0) {
                // Implement the 'close' command logic
                send(sock, command[0], strlen(command[0]), 0);
                break;
            } else if (strcmp(command[0], "cd") == 0) {
                // Implement the 'cd' command logic
                send(sock, command[0], strlen(command[0]), 0);
                recv(sock, buffer, sizeof(buffer), 0);
                printf("%s\n",buffer);

            } else if (strcmp(command[0], "ls") == 0) {
                // Implement the 'ls' command logic
                send(sock, command[0], strlen(command[0]), 0);

                while(1){
                recv(sock, buffer, sizeof(buffer), 0);
                if(strcmp(buffer,"end")==0){break;}
                printf("%s \n",buffer);
                bzero(buffer, sizeof(buffer));
                }

            } else {
                // Handle other commands
                break;
            }
        }
    }




    close(sock);
    printf("Disconnected from the server.\n");

    return 0;
}

