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
#include<netdb.h>
#include<sys/time.h>
#include<sys/select.h>


#define BUF_SiZE 100

void error_handing(char* message);


int main(int argc, char* argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    fd_set reads, cpy_reads;

    socklen_t adr_sz;
    int fd_max, str_len, fd_num, i;
    char buf[BUF_SiZE];
    if (argc != 2) {
        printf("usage : %s<PORT>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
    {
        /* data */
        error_handing("bind error");
    };

    if (listen(serv_sock, 5) == -1) {
        error_handing("listen error");
    }


    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    while (1) {

        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;
        if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
            break;
        if (fd_num == 0)
            continue;

        for (i = 0;i < fd_max + 1;i++) {
            if (FD_ISSET(i, &cpy_reads)) {          //如果为真，则说明i对应的文件一定是监视对象并且发生了变化
                if (i == serv_sock) {
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                    FD_SET(clnt_sock, &reads);
                    if (fd_max < clnt_sock)
                        fd_max = clnt_sock;
                    printf("connect client:%d\n", clnt_sock);
                }
                else {
                    str_len = read(i, buf, BUF_SiZE);
                    if (str_len == 0) {
                        FD_CLR(i, &reads);
                        close(i);
                        printf("close client:%d \n", i);
                    }
                    else
                    {
                        write(i, buf, BUF_SiZE);
                    }
                }
            }
        }


    }


    close(serv_sock);

    return 0;
}
void error_handing(char* message) {
    perror("message");
    exit(1);
}