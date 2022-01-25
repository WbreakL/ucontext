#pragma once
#include <iostream>
#include <memory>
using namespace std;
class UContext;
class MyTimer
{
public:
    MyTimer();
    ~MyTimer();
    void    set_once(int seconds, int millseconds);
    void	set_cycle(int seconds, int millseconds, int intervalSeconds, int intervalMillSeconds);
    int		get_timerfd();
    void	stop();
    //获取距离时间到期还有多少纳秒
    int 	get_time();
private:
    void	set_time(int seconds, int millseconds, int intervalSeconds, int intervalMillSeconds);
private:
    int					timerfd_;
    struct itimerspec* timespec_;
};