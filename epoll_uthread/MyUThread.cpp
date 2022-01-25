#include "MyUThread.h"
#include "MyTimer.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <assert.h>
#include <functional>
#include <sys/timerfd.h>
using namespace std;
MyUThread::MyUThread(int max_uthread_count, int stack_size) :
    max_uthread_count_(max_uthread_count),
    stack_size_(get_stack_size(stack_size)),
    curr_running_index_(-1),
    curr_max_count_(0),
    idle_count_(max_uthread_count),
    shut_down_(false), thread_(0), epoll_fd_(0)
{
    memset(&main_ctx_, 0, sizeof(main_ctx_));
    ctx_items_.resize(max_uthread_count_, 0);
    epoll_fd_ = epoll_create(100);
    thread_ = new thread(bind(thread_work, (void*)this));
}
MyUThread::~MyUThread() {
    ctx_items_.clear();
    while (!ctx_ready_queue_.empty()) {
        ctx_ready_queue_.pop();
    }
    delete thread_;
}

void	MyUThread::add_task(UContextFunc func, CallBack callback) {
    auto ctx = new UContext(-1, &main_ctx_, stack_size_, epoll_fd_);
    ctx->set_func(func, callback);
    if (curr_max_count_ < max_uthread_count_) {
        ctx->ctx_index_ = curr_max_count_;
        ctx->make();
        ctx_items_[curr_max_count_] = ctx;
        ++curr_max_count_;
        --idle_count_;
    }
    else {
        if (idle_count_ > 0) {
            auto index = -1;
            for (int i = 0, j = curr_max_count_ - 1; i <= j; i++, j--) {
                if (ctx_items_[i] == nullptr) {
                    index = i;
                    break;
                }
                if (ctx_items_[j] == nullptr) {
                    index = j;
                    break;
                }
            }
            ctx->ctx_index_ = index;
            ctx->make();
            ctx_items_[index] = ctx;
            --idle_count_;
        }
        else {
            lock_guard<mutex> lock(mutex_);
            ctx_ready_queue_.push(ctx);
        }
    }
}

void	MyUThread::join() {
    while (idle_count_ < max_uthread_count_ || !ctx_ready_queue_.empty()) {
        unique_lock<mutex> lock(mutex_join_);
        cv_.wait(lock);
    }
    this->shut_down_ = true;
    this->thread_->join();
}

void	MyUThread::destory() {
    this->shut_down_ = true;
    this->thread_->join();
}

void	MyUThread::remove_uthread(int index) {
    if (index >= 0 && index < curr_max_count_) {
        delete ctx_items_[index];
        ctx_items_[index] = nullptr;
        ++idle_count_;
        cv_.notify_all();
    }
    UContext* ctx = nullptr;
    {
        lock_guard<mutex> lock(mutex_);
        if (!ctx_ready_queue_.empty()) {
            ctx = ctx_ready_queue_.front();
            ctx_ready_queue_.pop();
        }
    }
    if (ctx) {
        ctx->ctx_index_ = index;
        ctx->make();
        ctx_items_[index] = ctx;
        --idle_count_;
    }
}

int		MyUThread::get_stack_size(int stack_size) {
    auto page_size = getpagesize();
    if (stack_size < page_size) {
        return page_size;
    }
    int page_count = stack_size / page_size;
    if ((stack_size % page_size) > 0) {
        ++page_count;
    }
    return page_count * page_size;
}

void	MyUThread::thread_work(void* pvoid) {
    MyUThread* mut = (MyUThread*)pvoid;
    struct epoll_event events[1024];
    while (!mut->shut_down_) {
        int ret = epoll_wait(mut->epoll_fd_, events, 1024, 4);
        if (ret > 0) {//有事件发生
            for (int i = 0; i < ret; i++) {
                UContextRevent* ctxex = (UContextRevent*)events[i].data.ptr;
                auto ctx = ctxex->ctx_;
                if (ctx->get_timer() && ctxex->fd_ == ctx->get_timer()->get_timerfd()) {
                    ctxex->revents_ = UThreadTimerOut;
                }
                else {
                    ctxex->revents_ = events[i].events;
                }
                ctx->resume();
                if (ctx->status_ == UContext::FINISHED) {
                    mut->remove_uthread(ctx->ctx_index_);
                }
            }
        }
        else if (ret == -1) {//异常
            if (errno == EINTR) {
                continue;
            }
            else {
                perror("epoll_wait:");
                abort();
            }
        }
        else {//epoll_wait超时
            ++mut->curr_running_index_;
            if (mut->curr_running_index_ >= mut->curr_max_count_) {
                mut->curr_running_index_ = -1;
                continue;
            }
            auto ctx = mut->ctx_items_[mut->curr_running_index_];
            if (ctx == nullptr || !ctx->register_fd_set.empty()) {
                //如果该ctx协程正在等待某个异步事件，则不对他唤醒
                continue;
            }
            ctx->resume();
            if (ctx->status_ == UContext::FINISHED) {
                mut->remove_uthread(ctx->ctx_index_);
            }
        }
    }
}