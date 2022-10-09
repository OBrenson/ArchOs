#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/shm.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>

#define BUFSIZE 4

float *sum_res, *sqr_res_a, *sqr_res_b, *sqrt_res;

int sock;
struct sockaddr_in addr;
unsigned char sendBuf[8];
unsigned char buf[BUFSIZE];
sem_t *sum_sem, *sqr_sem, *sqrt_sem;
pid_t pid;

int wait_result_sum(float num, float num2, int port) {

    unsigned char a[4];
    unsigned char b[4];
    memcpy(a, (unsigned char*) (&num), 4);
    memcpy(b, (unsigned char*) (&num2), 4);

    for(int i = 0; i < 4; i++) {
        sendBuf[i] = a[i];
    }
    for(int i = 0; i < 4; i++) {
        sendBuf[4 + i] = b[i];
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&addr, 0, sizeof(addr)); 
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int s = sendto(sock, sendBuf, 8, MSG_CONFIRM, (const struct sockaddr *) &addr, sizeof(addr));
    
    int len = sizeof(addr);
    int n = recvfrom(sock, buf, BUFSIZE, MSG_WAITALL, (const struct sockaddr *) &addr, &len);
    buf[n] = '\0'; 
    if (n < 0) {
        printf("Error: %d\n", errno);
        return 0;
    }
    close(sock);
    *sum_res = *(float *)buf;

    return 0;
}

int wait_result(float num, int port, float *res_p) {
    unsigned char a[4];
    memcpy(a, (unsigned char*) (&num), 4);

    for(int i = 0; i < 4; i++) {
        sendBuf[i] = a[i];
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&addr, 0, sizeof(addr)); 
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int s = sendto(sock, sendBuf, 4, MSG_CONFIRM, (const struct sockaddr *) &addr, sizeof(addr));
    
    int len = sizeof(addr);
    int n = recvfrom(sock, buf, BUFSIZE, MSG_WAITALL, (const struct sockaddr *) &addr, &len);
    buf[n] = '\0'; 
    if (n < 0) {
        printf("Error: %d\n", errno);
        return 0;
    }
    close(sock);
    *res_p = *(float *)buf;

    return 0;
}

int start(char *path, char *file, char *port) {
    pid = fork ();
    if (pid < 0) {
        printf("Fork error\n");
        return -1;
    }
    if (pid == 0) {
        char *args[]={file,port, NULL};
        int err = execvp(path,args);
        if (err == -1) {
            printf("Error: %d\n", errno);
            return errno;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    key_t shmkey_a, shmkey_b, shmkey_sum, shmkey_sqrt;                
    int shmid_a, shmid_b, shmid_sum, shmid_sqrt;   

    float a = atof(argv[1]);
    float b = atof(argv[2]);

    sum_sem = sem_open ("sumSem", O_CREAT, 0644, 0); 
    sqr_sem = sem_open ("sqrSem", O_CREAT, 0644, -1); 
    sqrt_sem = sem_open ("sqrtSem", O_CREAT, 0644, 0); 

    shmkey_a = ftok ("/dev/null", 5);      
    shmid_a = shmget (shmkey_a, sizeof (float), 0666 | IPC_CREAT);
    shmkey_b = ftok ("/dev/null", 6);      
    shmid_b = shmget (shmkey_b, sizeof (float), 0666 | IPC_CREAT);
    shmkey_sum = ftok ("/dev/null", 7);      
    shmid_sum = shmget (shmkey_sum, sizeof (float), 0666 | IPC_CREAT);
    shmkey_sqrt = ftok ("/dev/null", 8);      
    shmid_sqrt = shmget (shmkey_sqrt, sizeof (float), 0666 | IPC_CREAT);

    sum_res = (float *) shmat (shmid_sum, NULL, 0); 
    sqr_res_a = (float *) shmat (shmid_a, NULL, 0); 
    sqr_res_b = (float *) shmat (shmid_b, NULL, 0); 
    sqrt_res = (float *) shmat (shmid_sqrt, NULL, 0); 

    start("/media/sf_buffer/lab1/sqr", "sqr", "6968");
    if (pid > 0) {
        start("/media/sf_buffer/lab1/sqr1", "sqr1", "6967");
        if (pid > 0 ) {
            start("/media/sf_buffer/lab1/sum", "sum", "6966");
            if (pid > 0 ) {
                start("/media/sf_buffer/lab1/sqrt", "sqrt", "6965");
                sleep(5);
                pid = fork();
                if (pid == 0){
                    wait_result(a, 6968, sqr_res_a);
                    sem_post(sqr_sem);
                } else{
                    pid = fork();
                    if (pid == 0) {
                        wait_result(b, 6967, sqr_res_b);
                        sem_post(sqr_sem);
                    } else {
                        sem_wait(sqr_sem);
                        pid = fork();
                        if (pid == 0) {
                            sem_wait(sqr_sem);
                            wait_result_sum(*sqr_res_a, *sqr_res_b, 6966);
                            sem_post(sum_sem);
                        } else {
                            sem_wait(sum_sem);
                            pid = fork();
                            if (pid == 0) {
                                wait_result(*sum_res, 6965, sqrt_res);
                                sem_post(sqrt_sem);
                            } else {
                                sem_wait(sqrt_sem);
                                printf("Result calculations: %f\n", *sqrt_res);
                            }
                        }
                    }
                }
            } 
        } 
    }
    return 0;
}