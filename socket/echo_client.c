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


#define BUF_SIZE 1024
void error_handing(char* message);
int main(int argc, char* argv[]) {

    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;

    if (argc != 3) {
        printf("usage : %s<IP><PORT>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handing("socket error");
    }
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = PF_INET;
    inet_aton(argv[1], &serv_adr.sin_addr);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handing("connect error");
    }
    else {
        printf("connected..........");
    }

    while (1) {
        fputs("input message(Q to quit):", stdout);
        fgets(message, BUF_SIZE, stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }
        write(sock, message, strlen(message));
        str_len = read(sock, message, BUF_SIZE - 1);
        message[str_len] = '\0';
        printf("message from service : %s", message);

    }
    close(sock);


    return 0;
}

void error_handing(char* message) {
    perror("message");
    exit(1);
}