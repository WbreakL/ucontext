#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <queue>
#include <ucontext.h>
#include "sys/epoll.h"
using namespace std;
#include "UContext.h"
class MyUThread {
public:
    MyUThread(int max_uthread_count, int stack_size);
    ~MyUThread();
    void	add_task(UContextFunc func, CallBack callback = 0);
    void	join();
    void	destory();
private:
    void	remove_uthread(int index);
    int		get_stack_size(int stack_size);
    static void thread_work(void* pvoid);
private:
    int					max_uthread_count_;		//最大协程数量
    int					stack_size_;			//栈大小
    volatile int		curr_running_index_;	//当前执行协程索引
    volatile int		curr_max_count_;		//当前最大协程数量
    volatile int		idle_count_;			//可用协程数量
    bool				shut_down_;				//是否退出
    vector<UContext*>	ctx_items_;				//调度的协程列表
    queue<UContext*>	ctx_ready_queue_;		//就绪队列，等待其它协程退出后，会被添加到ctx_items_中
    thread* thread_;				//线程
    ucontext_t			main_ctx_;				//主上下文
    mutex				mutex_;					//锁queue
    mutex				mutex_join_;
    condition_variable	cv_;					//使用锁和条件变量，阻塞等待协程全部执行完毕
    int					epoll_fd_;				//epoll fd
};