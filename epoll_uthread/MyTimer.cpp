#include "MyTimer.h"
#include "UContext.h"
#include <cstring>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <fcntl.h>

MyTimer::MyTimer() :timerfd_(-1), timespec_(nullptr)
{
    timerfd_ = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (timerfd_ == -1) {
        abort();
    }
    auto flags = fcntl(timerfd_, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(timerfd_, F_SETFL, flags);
    timespec_ = new struct itimerspec;
    memset(timespec_, 0, sizeof(struct itimerspec));
}

MyTimer::~MyTimer()
{
    if (timespec_) {
        delete timespec_;
    }
    close(timerfd_);
}

void    MyTimer::set_once(int seconds, int millseconds) {
    this->set_time(seconds, millseconds, 0, 0);
}

void	MyTimer::set_cycle(int seconds, int millseconds, int intervalSeconds, int intervalMillSeconds) {
    this->set_time(seconds, millseconds, intervalSeconds, intervalMillSeconds);
}

int		MyTimer::get_timerfd() {
    return timerfd_;
}

void	MyTimer::set_time(int seconds, int millseconds, int intervalSeconds, int intervalMillSeconds) {
    if (timerfd_ == -1 || timespec_ == nullptr)
        return;
    timespec_->it_value.tv_sec = seconds;
    timespec_->it_value.tv_nsec = millseconds * 1000;
    timespec_->it_interval.tv_sec = intervalSeconds;
    timespec_->it_interval.tv_nsec = intervalMillSeconds * 1000;
    if (-1 == timerfd_settime(timerfd_, 0, timespec_, nullptr)) {
        abort();
    }
}

void	MyTimer::stop() {
    this->set_time(0, 0, 0, 0);
}

int 	MyTimer::get_time() {
    struct itimerspec t;
    timerfd_gettime(timerfd_, &t);
    return t.it_value.tv_sec * 10001000 + t.it_value.tv_nsec;
}