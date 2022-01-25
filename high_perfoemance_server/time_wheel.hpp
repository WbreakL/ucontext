#ifndef TIME_WHEEL
#define TIME_WHEEL

#include<time.h>
#include<netinet/in.h>
#include<stdio.h>

#define BUFFER_SIZE 64

class tw_timer;

struct client_data
{
    /* data */
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    tw_timer* timer;
};

//定时器类
class tw_timer {
public:
    int rotation;//记录定时器在时间轮转动多少圈后生效
    int time_slot;
    void (*cb_func)(client_data*);//callback function
    client_data* user_data;//client data
    tw_timer* next;
    tw_timer* prev;
public:
    tw_timer(int rot, int ts) :next(NULL), prev(NULL), rotation(rot), time_slot(ts) {}
    
};


class time_wheel {
private:
    //时间轮上槽的数量
    static const int N = 60;
    //每一秒转动一次，即间隔为1
    static const int SI = 1;
    //时间轮上的槽
    tw_timer* slots[N];
    int cur_slot;
public:
    time_wheel() :cur_slot(0) {
        for (int i = 0;i < N;i++) {
            slots[i] = NULL;
        }
    }
    ~time_wheel() {
        for (int i = 0;i < N;i++) {
            tw_timer* tmp = slots[i];
            while (tmp) {
                slots[i] = tmp->next;
                delete tmp;
                tmp = slots[i];
            }
        }
    }

    //根据定时值timeout创建一个定时器，并把它插入到合适的槽中
    tw_timer* add_timer(int timeout) {
        if (timeout < 0) {
            return NULL;
        }
        int ticks = 0;
        if (timeout < SI) {
            ticks = 1;
        }
        else {
            ticks = timeout / SI;
        }

        //计算插入的定时器在时间轮转动多少圈后被触发
        int rotation = ticks / N;
        //计算插入的定时器应该被插入到哪个槽中
        int ts=(cur_slot+(ticks%N))%N;
        //创建新的定时器，他在时间轮转动rotation圈后被触发，且位于第ts个槽上
        tw_timer* timer = new tw_timer(rotation, ts);

        //如果第ts个槽中没有任何定时器，则把新建的定时器插入其中，并将该定时器设置为该槽的头节点
        if (!slots[ts]) {
            printf("add timer,rotatoin is %d, ts is %d,cur_slot is %d\n",
                rotation, ts, cur_slot);
            slots[ts] = timer;

        }
        //否则，将定时器插入到顶ts个槽中,从前面开始插入
        else {
            timer->next = slots[ts];
            slots[ts]->prev = timer;
            slots[ts] = timer;
        }
        return timer;
    }

    //删除目标定时器
    void del_timer(tw_timer* timer) {
        if (!timer) {
            return;
        }
        int ts = timer->time_slot;
        //slot[ts]是目标定时器的头节点，如果目标定时器就是该头节点，需要重置头节点
        if(timer==slots[ts]){
            slots[ts] = slots[ts]->next;
            if (slots[ts]){
                slots[ts]->prev=NULL;
            }
            delete timer;
        }
        else {
            timer->prev->next = timer->next;
            if (timer->next) {
                timer->next->prev = timer->prev;
            }
            delete timer;
        }
    }

    //SI时间到后，调用该函数，时间轮向前滚动一个槽的间隔
    void tick(){
        tw_timer* tmp = slots[cur_slot];
        printf("current slot is %d\n", cur_slot);
        while (tmp) {
            printf("tick the timer once\n");
            //如果定时器的rotation值大于0，则他在这一轮不起作用
            if (tmp->rotation > 0) {
                tmp->rotation--;
                tmp = tmp->next;
            }
            //否则说明该定时器已经到期，需要执行任务，然后删除任务
            else{
                tmp->cb_func(tmp->user_data);
                if (tmp == slots[cur_slot]) {
                    printf("delete header in cur_solt\n");
                    slots[cur_slot] = slots[cur_slot]->next;
                    if (slots[cur_slot]) {
                        slots[cur_slot]->prev = NULL;
                    }
                    tmp = slots[cur_slot];
                }
                else {
                    tmp->prev->next = tmp->next;
                    if (tmp->next) {
                        tmp->next->prev = tmp->prev;
                    }
                    tw_timer* tmp2 = tmp->next;
                    delete tmp;
                    tmp = tmp2;
                }
            }
        }
        cur_slot = ++cur_slot % N;//注意需要%N
    }
};

#endif // !TIME_WHEEL

