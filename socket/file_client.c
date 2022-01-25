#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<assert.h>
#include<errno.h>
#include<netinet/in.h>




#define BUF_SIZE 30
void error_handing(char* message);
int main(int argc, char* argv[]) {

    int serv_sd;
    FILE* fp;
    char buf[BUF_SIZE];
    int read_cnt;

    struct sockaddr_in serv_adr;
    socklen_t serv_adr_sz;

    if (argc != 3) {
        printf("usage :%s<IP><port>\n", argv[0]);
        exit(1);
    }
    fp = fopen("receive.dat", "wt");
    serv_sd = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sd == -1)
        error_handing("socket error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[1]));


    if (connect(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
    {
        error_handing("Connect error");
        /* data */
    }
    while ((read_cnt = read(serv_sd, buf, BUF_SIZE)) != 0)
        fwrite((void*)buf, 1, read_cnt, fp);

    puts("receivefile date");
    write(serv_sd, "thank you", 10);

    fclose(fp);
    close(serv_sd);
    return 0;
}

void error_handing(char* message) {
    perror("message");
    exit(1);
}