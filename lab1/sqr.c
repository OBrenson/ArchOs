#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>

#define PORT 6969
#define BUFSIZE 16

int sock;
struct sockaddr_in addr;
char buf[BUFSIZE];
int bytes_read, total = 0;

int main() {
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    while(1) {
        bytes_read = recvfrom(sock, buf, BUFSIZE, 0, NULL, NULL);
        total = total + bytes_read;
        buf[bytes_read] = '\0';
        fprintf(stdout, buf);fflush(stdout);
        if (total>BUFSIZE) 
            break;
    }
    printf("\nTotal %d bytes received", total);
    return 0;
}