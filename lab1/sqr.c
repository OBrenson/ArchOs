#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <errno.h>

#define BUFSIZE 4
#define BUFNUM 4

int sock;
struct sockaddr_in addr, cliaddr;
unsigned char buf[BUFSIZE];
int bytes_read = 0;

int main(int argc, char *argv[]) {
    int port = atoi(argv[1]);

    fflush(stdout);
    printf("sqr started on port %d\n", port);
    memset(&addr, 0, sizeof(addr)); 
    memset(&cliaddr, 0, sizeof(cliaddr));

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));

    float a;
    unsigned char first_num[BUFNUM], res[BUFNUM];

    int len = sizeof(addr);
    bytes_read = recvfrom(sock, buf, BUFSIZE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    if (bytes_read == -1) {
        printf("Error %d\n", errno);
        return 0;
    }
    for (int i = 0; i < 4; i++) {
        first_num[i] = buf[i];
    }

    a = *(float *) first_num;

    float sqrt = a * a;
    memcpy(res, (unsigned char*) (&sqrt), 4);
    printf("sqr float: %f\n", sqrt);
    int s = sendto(sock, res, 4, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
    close(sock);
    return 0;
}