#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <errno.h>
#define PORT 6969
#define BUFSIZE 4

int sock;
struct sockaddr_in addr;
unsigned char sendBuf[8];
unsigned char buf[BUFSIZE];

int main() {
    fprintf(stdout, "\n");
    fflush(stdout);

    float num = 12.2;

    unsigned char a[4];
    memcpy(a, (unsigned char*) (&num), 4);

    memcpy(sendBuf, a, 4);
    memcpy(sendBuf, a, 4);
    for(int i = 0; i < 4; i++) {
        sendBuf[i] = a[i];
    }
    for(int i = 0; i < 4; i++) {
        sendBuf[4 + i] = a[i];
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&addr, 0, sizeof(addr)); 
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    int s = sendto(sock, sendBuf, 8, MSG_CONFIRM, (const struct sockaddr *) &addr, sizeof(addr));
    
    int len = sizeof(addr);
    int n = recvfrom(sock, buf, BUFSIZE, MSG_WAITALL, (const struct sockaddr *) &addr, &len);
    buf[n] = '\0'; 
    if (n < 0) {
        printf("Error: ", errno);
        return 0;
    }
    printf("Recieved\n");
    float x = *(float *)buf;

    printf("Result: %f\n", x);
    close(sock);

    return 0;
}

void float2Bytes(unsigned char bytes_temp[4],float float_variable){ 
  memcpy(bytes_temp, (unsigned char*) (&float_variable), 4);
}