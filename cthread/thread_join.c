#include <stdio.h>
#include <errno.h>   //使用宏 ESRCH
#include <pthread.h>
//线程执行的函数
void *ThreadFun(void *arg)
{
    pthread_exit("because love");
}
int main()
{
    int res;
    void * thread_result;
    pthread_t myThread;
    //创建 myThread 线程
    res = pthread_create(&myThread, NULL, ThreadFun, NULL);
    if (res != 0) {
        printf("线程创建失败");
        return 0;
    }
    //阻塞主线程，等待 myThread 线程执行结束
    res = pthread_join(myThread, &thread_result);
    if (res != 0) {
        printf("1：等待线程失败");
    }
    //输出获取到的 myThread 线程的返回值
    printf("%s\n", (char*)thread_result);

    //尝试再次获取 myThread 线程的返回值
    res = pthread_join(myThread, &thread_result);
    if (res == ESRCH) {
        printf("2：等待线程失败，线程不存在");
    }
    return 0;
}
