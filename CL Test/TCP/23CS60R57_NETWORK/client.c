#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    char ip[100];
    int port = 5566;
    int n_packet;
    int packet_delay;
    printf("Enter Server IP address: ");
    scanf("%s", ip);
    printf("Enter Server Port number: ");
    scanf("%d", &port);
    printf("Number of packets: ");
    scanf("%d", &n_packet);
    printf("Packet delay (in microseconds): ");
    scanf("%d", &packet_delay);

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;
    struct timeval stop, start;  // for time
    

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

    for (int i = 0; i < n_packet; i++) {
        printf("Packet %d:\n", i + 1);

        bzero(buffer, sizeof(buffer));
        strcpy(buffer, "PING");
        printf("Client Sending: %s\n", buffer);

        gettimeofday(&start, NULL);
        send(sock, buffer, strlen(buffer), 0);

        bzero(buffer, sizeof(buffer));
        int signal_ack = recv(sock, buffer, sizeof(buffer), 0);

        if (signal_ack <= 0) {
            printf("No response received.\n");
        } else {
            gettimeofday(&stop, NULL);
            printf("RTT %lu microseconds\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
            printf("Server sent: %s\n", buffer);
        }

        bzero(buffer, sizeof(buffer));
        if(n_packet-1==i){strcpy(buffer, "END");
        }else{strcpy(buffer, "OK");}

        send(sock, buffer, strlen(buffer), 0);

        if (i < n_packet - 1) {
            // Delay before sending the next packet
            usleep(packet_delay);
        }
    }

    close(sock);
    printf("Disconnected from the server.\n");

    return 0;
}
