#ifndef  LST_TIMER
#define LST_TIMER

#include<time.h>
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<signal.h>
#include<fcntl.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<assert.h>
#include<errno.h>
#include<netinet/in.h>

#define BUF_SIZE 1024
class util_timer;

//用户数据结构
struct client_data {
    sockaddr_in address;
    int sockfd;
    char buf[BUF_SIZE];
    util_timer* timer;

};
//定时器类
class util_timer {

public:
    util_timer() :prev(NULL), next(NULL) {};
public:
    time_t expire;
    void (*cb_func)(client_data*);
    client_data* user_data;
    util_timer* prev;
    util_timer* next;
};

//定时器链表
class sort_timer_lst {
public:
    sort_timer_lst() :head(NULL), tail(NULL) {}
    ~sort_timer_lst() {
        util_timer* tmp = head;
        while (tmp) {
            head = tmp->next;
            delete tmp;
            tmp = head;
        }
    }

    //add timer to list
    void add_timer(util_timer* timer) {
        if (!timer) {
            return;
        }
        if (!head) {
            head = tail = timer;
            return;
        }
        if (timer->expire < head->expire) {
            timer->next = head;
            head->prev = timer;
            head = timer;
            return;
        }
        add_timer(timer, head);
    }
    void adjust_timer(util_timer* timer) {
        if (!timer) {
            return;
        }
        util_timer* tmp = timer->next;
        if (!tmp || (timer->expire < tmp->expire)) {
            return;
        }
        if (timer == head) {
            head = head->next;
            head->prev = NULL;
            timer->next = NULL;
            add_timer(timer, head);
        }
        else
        {
            timer->prev->next = timer->next;
            timer->next->prev = timer->prev;
            add_timer(timer, head);
        }
    }
    void del_timer(util_timer* timer) {
        if (!timer) {
            return;
        }
        if ((head == timer) && (timer == tail)) {
            delete timer;
            head = NULL;
            tail = NULL;
            return;
        }
        if (timer == head) {
            head = head->next;
            head->prev = NULL;
            delete timer;
            return;
        }
        if (timer == tail) {
            tail = tail->prev;
            tail->next = NULL;
            delete timer;
            return;
        }
        //middle
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        delete timer;
        return;
    }
    //触发
    void tick() {
        if (!head) {
            return;
        }
        printf("timer tick\n");
        time_t cur = time(NULL);
        util_timer* tmp = head;

        while (tmp) {
            if (cur < tmp->expire) {
                break;
            }
            //callback
            tmp->cb_func(tmp->user_data);
            //final,delete it
            head = tmp->next;
            if (head) {
                head->prev = NULL;
            }
            delete tmp;
            tmp = head;
        }
    }
private:
    //重载add_timer();
    void add_timer(util_timer* timer, util_timer* lst_head) {
        util_timer* prev = lst_head;
        util_timer* tmp = prev->next;

        while (tmp) {
            if (timer->expire < tmp->expire) {
                prev->next = timer;
                timer->next = tmp;
                tmp->prev = timer;
                timer->prev = prev;
                break;
            }
            prev = tmp;
            tmp = tmp->next;
        }
        if (!tmp) {
            prev->next = timer;
            timer->prev = prev;
            timer->next = NULL;
            tail = timer;
        }
    }
    util_timer* head;
    util_timer* tail;
};


#endif // ! LST_TIMER

