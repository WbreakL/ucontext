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




#define BUF_SIZE 1024
void error_handing(char* message);
int main(int argc, char* argv[]) {

    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len, i;

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if (argc != 2) {
        printf("usage :%s<port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handing("socket error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));


    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handing("bind error");


    if (listen(serv_sock, 5) == -1)
        error_handing("listen error");

    clnt_adr_sz = sizeof(clnt_adr);

    for (i = 0;i < 5;i++) {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if (clnt_sock == -1) {
            error_handing("accept error");
        }
        else {
            printf("connected Client %d\n", i + 1);
            printf("client IP=%s\n", inet_ntoa(clnt_adr.sin_addr));
            printf("client port=%d\n", ntohs(clnt_adr.sin_port));
        }
        while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) {
            printf("message from client: %s", message);
            message[str_len] = '\0';
            write(clnt_sock, message, str_len);
        }
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

void error_handing(char* message) {
    perror("message");
    exit(1);
}