#include <iostream>
#include <functional>
#include <vector>
#include "MyUThread.h"
#include "MyTimer.h"
using namespace std;
using namespace placeholders;

void	test1(UContext*& ctx, int num) {
    cout << "enter test1:" << num << endl;
    MyTimer timer;
    timer.set_once(2, 0);//设置定时器2s
    UContextRevent revents(ctx, timer.get_timerfd());
    ctx->register_event(timer.get_timerfd(), EPOLLIN, &revents);//注册定时器事件
    ctx->attach_timer(&timer);
    cout << "start yield" << endl;
    ctx->yield();            //让出cpu，等待定时器事件
    if (revents.revents_ == UThreadTimerOut) {
        cout << "UThreadTimerOut" << endl;
    }
    ctx->remove_event(timer.get_timerfd());
    cout << "finished test1" << endl;
}

void	test2(UContext*& ctx) {
    cout << "test2 yield" << endl;
    ctx->yield();
    cout << "finished test2" << endl;
}

void	finished() {
    cout << "test1 callback" << endl;
}

int main()
{
    MyUThread mut(2, 8192);//参数：协程最大数量，协程栈大小
    mut.add_task(bind(test1, _1, 99999), finished);//添加任务,参数：工作函数，回调函数
    for (int i = 0; i < 10; i++) {
        mut.add_task(test2);
    }
    mut.join();
    return 0;
}