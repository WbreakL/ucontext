#include "UContext.h"
#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>
#include <sys/epoll.h>
#include "MyTimer.h"

UContext::UContext(int index, ucontext_t* main_ctx, int stack_size, int epoll_fd)
    :ctx_index_(index), main_ctx_(main_ctx), ctx_(nullptr),
    raw_stack_(nullptr), stack_(nullptr), func_(nullptr), callback_(nullptr),
    stack_size_(stack_size),
    status_(READY), epoll_fd_(epoll_fd),
    mytimer_(nullptr) {
    //创建协程私有栈
    auto page_size = getpagesize();
    raw_stack_ = (char*)mmap(nullptr, stack_size_ + page_size * 2,
        PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    assert(raw_stack_ != nullptr);
    stack_ = raw_stack_ + page_size;
    assert(mprotect(raw_stack_, page_size, PROT_NONE) == 0);
    assert(mprotect(raw_stack_ + stack_size_ + page_size, page_size, PROT_NONE) == 0);

    ctx_ = new ucontext_t;
    ctx_->uc_flags = 0;
    ctx_->uc_link = main_ctx;
    ctx_->uc_stack.ss_sp = stack_;
    ctx_->uc_stack.ss_size = stack_size_;
    getcontext(ctx_);
}
UContext::~UContext() {
    delete ctx_;
    munmap(raw_stack_, stack_size_ + getpagesize() * 2);
}

void	UContext::set_func(UContextFunc func, CallBack callback) {
    func_ = func;
    callback_ = callback;
}

void	UContext::make() {
    auto ptr = (uintptr_t)this;
    makecontext(ctx_, (void(*)(void))work_func, 2, (uint32_t)ptr, (uint32_t)(ptr >> 32));
}

void	UContext::resume() {
    status_ = RUNNING;
    swapcontext(main_ctx_, ctx_);
}

void	UContext::yield() {
    status_ = SUSPEND;
    swapcontext(ctx_, main_ctx_);
}

void	UContext::attach_timer(MyTimer* timer) {
    mytimer_ = timer;
}

MyTimer* UContext::get_timer() {
    return mytimer_;
}

void	UContext::register_event(int fd, int events, UContextRevent* revents) {
    epoll_event ev;
    ev.data.fd = fd;
    ev.events = events;
    ev.data.ptr = revents;
    epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
    register_fd_set.insert(fd);
}

void	UContext::remove_event(int fd) {
    epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr);
    if (mytimer_ && mytimer_->get_timerfd() == fd) {
        this->attach_timer(nullptr);
    }
    register_fd_set.erase(fd);
}

void	UContext::work_func(uint32_t low32, uint32_t high32) {
    uintptr_t ptr = (uintptr_t)low32 | ((uintptr_t)high32 << 32);
    UContext* uc = (UContext*)ptr;
    if (uc->func_) {
        uc->func_(uc);
        if (uc->callback_) {
            uc->callback_();
        }
    }
    uc->status_ = FINISHED;
}