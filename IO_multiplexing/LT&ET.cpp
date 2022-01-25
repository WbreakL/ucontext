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
#include<sys/types.h>
#include<fcntl.h>
#include<pthread.h>
#include<sys/epoll.h>


#define MAX_EVENT_NUMBER 1024
#define BUF_SIZE 10

//将文件描述符设置成为非阻塞的
int setnoblocking(int fd) {
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

/*将文件描述符fd上的EPOLLIN注册到epollfd指示的epoll内核事件表中，参数enable_et指定是否对fd启用ET模式*/
void addfd(int epollfd, int fd, bool enable_et) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    if (enable_et) {
        event.events |= EPOLLET;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnoblocking(fd);
}

//LT模式的工作流程
void lt(epoll_event* events, int number, int epollfd, int listenfd) {
    char buf[BUF_SIZE];
    for (int i = 0;i < number;i++) {
        int sockfd = events[i].data.fd;
        if (sockfd == listenfd) {
            struct sockaddr_in client_address;
            socklen_t client_addrlength = sizeof(client_address);
            int connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
            addfd(epollfd, connfd, false);
        }
        else if (events[i].events & EPOLLIN) {
            //只要socket读缓存中还有未读出的数据，这段代码就被触发
            printf("event trigger once\n");
            memset(buf, '\0', BUF_SIZE);
            int ret = recv(sockfd, buf, BUF_SIZE - 1, 0);
            if (ret <= 0) {
                close(sockfd);
                continue;
            }
            printf("get %d bytes of content: %s\n", ret, buf);
        }
        else {
            printf("something else happened \n");
        }
    }
}

//ET模式的工作流程
void et(epoll_event* events, int number, int epolllfd, int listenfd) {
    char buf[BUF_SIZE];
    for (int i = 0;i < number;i++) {
        int sockfd = events[i].data.fd;
        if (sockfd == listenfd) {
            struct sockaddr_in client_address;
            socklen_t client_addrlength = sizeof(client_address);
            int connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
        }
        else if (events[i].events & EPOLLIN) {
            //这段代码不会被触发，所以我们循环读取数据，以确保把socket读缓存中的所有数据读出
            printf("event trigger once \n");
            while (1) {
                memset(buf, '\0', BUF_SIZE);
                int ret = recv(sockfd, buf, BUF_SIZE - 1, 0);
                if (ret < 0) {
                    /*对于非阻塞IO，下面的条件成立表示数据已经全部读取完毕，此后，epoll就能再次出发sockfd上的EPOLLIN事件，以驱动下一次读操作*/
                    if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                        printf("read later\n");
                        break;
                    }
                }
            }
        }
    }
}





void error_handing(char* message);


int main(int argc, char* argv[]) {

    return 0;
}
void error_handing(char* message) {
    perror("message");
    exit(1);
}