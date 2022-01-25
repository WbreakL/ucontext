#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<assert.h>
#include<netinet/in.h>

#define SERV_PORT 9527

int main(void)
{

    int lfd, cfd;
    lfd = socket(AF_INET, SOCK_STREAM, 0);

    char buf[1024], clie_IP[BUFSIZ];
    int ret, i;
    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htonl(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(lfd, 128);

    clie_addr_len = sizeof(clie_addr);

    cfd = accept(lfd, (struct sockaddr*)&clie_addr, &clie_addr_len);
    printf("client's IP:%s  prot:%d\n",
        inet_ntop(AF_INET, &clie_addr.sin_addr, clie_IP, sizeof(clie_IP)),
        ntohs(clie_addr.sin_port));

    while (1) {
        ret = read(cfd, buf, sizeof(buf));
        for (i = 0; i < ret; i++)
        {
            buf[i] = toupper(buf[i]);/* code */
        }
        write(cfd, buf, ret);
    }

    close(lfd);
    close(cfd);

    return 0;


}
