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
    if (argc != 2) {
        printf("usage:%s<address>\n", argv[0]);
        exit(1);
    }

    host = gethostbyname(argv[1]);
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