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




void error_handing(char* message);


int main(int argc, char* argv[]) {
    int i;
    struct hostent* host;
    struct sockaddr_in addr;
    {
        /* data */
    };

    if (argc != 2) {
        printf("usage:%s<name>\n", argv[0]);
        exit(1);
    }


    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    host = gethostbyaddr((const char*)&addr, 4, AF_INET);
    if (!host) {
        error_handing("gethost error");
    }

    printf("offical name:%s\n", host->h_name);
    for (i = 0;host->h_aliases[i];i++)
        printf("aliases %d:%s\n", host->h_aliases);
    printf("address type:%s\n",
        ((host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6"));
    for (i = 0;host->h_addr_list[i];i++)
        printf("IP address %d:%s\n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

    return 0;
}
void error_handing(char* message) {
    perror("message");
    exit(1);
}