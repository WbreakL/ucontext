#include "uthread.h"
#include <stdio.h>
#include<cstddef>
#include<sys/time.h>

schedule_t s;
void func1(void* arg)
{
    puts("1");
    puts("11");
    puts("111");
    puts("1111");

}

void func2(void* arg)
{
    puts("22");
    puts("22");
    uthread_yield(*(schedule_t*)arg);
    puts("22");
    puts("22");
}

void func3(void* arg)
{
    for (int i = 0;1;i++)
        printf("fun%d:%d\t", *((int*)arg), i);

}

void context_test()
{
    char stack[1024 * 128];
    ucontext_t uc1, ucmain;

    getcontext(&uc1);
    uc1.uc_stack.ss_sp = stack;
    uc1.uc_stack.ss_size = 1024 * 128;
    uc1.uc_stack.ss_flags = 0;
    uc1.uc_link = &ucmain;

    makecontext(&uc1, (void (*)(void))func1, 0);

    swapcontext(&ucmain, &uc1);
    puts("main");
}

void schedule_test(schedule_t& s)
{

    int id_slots[MAX_UTHREAD_SIZE];
    for (int i = 0;i < MAX_UTHREAD_SIZE;i++) {
        id_slots[i] = uthread_create(s, func3, (void*)&id_slots[i]);//创建时调用uthread_body();
    }
    while (!schedule_finished(s)) {
        for (int i = 0;i < MAX_UTHREAD_SIZE;i++) {
            if (s.)
                uthread_resume(s, id_slots[i]);

        }
        puts("main over");

    }
    void sig_ticks_timer(int sig) {

        // if (s.threads[s.running_thread_id].state == RUNNING)
        uthread_yield(s);
        //setcontext(&s.main);
    }
    int main()
    {


        struct itimerval ticks_timer;
        //context_test();
        puts("----------------");
        signal(SIGALRM, sig_ticks_timer);
        ticks_timer.it_interval.tv_sec = 0;
        ticks_timer.it_interval.tv_usec = 100;
        ticks_timer.it_value.tv_sec = 0;
        ticks_timer.it_value.tv_usec = 10;
        setitimer(ITIMER_REAL, &ticks_timer, NULL);

        schedule_test(s);

        return 0;
    }
