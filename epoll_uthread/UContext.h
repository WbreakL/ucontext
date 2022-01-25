#include <functional>
#include <ucontext.h>
#include <memory>
#include <unordered_set>
using namespace std;
/*
EPOLLIN:1
EPOLLOUT:4
EPOLLRDHUP:8192
EPOLLPRI:2
EPOLLERR:8
EPOLLHUP:16
定时器超时标志
*/
#define UThreadTimerOut 3
class UContext;
typedef function<void(UContext*&)>      UContextFunc;
typedef function<void()>                CallBack;
class MyTimer;
struct UContextRevent {
    UContext* ctx_;
    int          fd_;
    int          revents_;
    UContextRevent(UContext* ctx, int fd, int revents = 0) :
        ctx_(ctx), fd_(fd), revents_(revents) {}
};
class UContext {
public:
    UContext(int index, ucontext_t* main_ctx, int stack_size, int epoll_fd);
    ~UContext();
    void	set_func(UContextFunc func, CallBack callback = 0);
    void	make();
    void	resume();
    void	yield();
    void	attach_timer(MyTimer*);
    void	register_event(int fd, int events, UContextRevent* revents);
    void	remove_event(int fd);
    MyTimer* get_timer();
private:
    static void	work_func(uint32_t low32, uint32_t high32);
public:
    int             ctx_index_;		//当前对象在MyUThread对象的数组中保存的索引
    ucontext_t* main_ctx_;		//线程主逻辑上下文
    ucontext_t* ctx_;			//当前上下文
    char* raw_stack_;		//上下文使用栈空间
    char* stack_;			//栈空间（安全保护）
    UContextFunc    func_;			//用户任务
    CallBack        callback_;		//任务回调
    int             stack_size_;	//栈大小
    enum { READY = 0, RUNNING, SUSPEND, FINISHED };
    int             status_;		//当前协程状态
    int             epoll_fd_;		//epoll fd
    MyTimer* mytimer_;
    unordered_set<int>    register_fd_set;
};