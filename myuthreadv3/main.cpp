#include "uthread.h"
#include <stdio.h>
#include<cstddef>
#include<unistd.h>
#include<sys/time.h>
#include<signal.h>

static schedule_t s;
extern FILE* fp;
int& iter = s.running_thread_id;
void func1(void* arg)
{
    puts("1");
    puts("11");
    puts("111");
    puts("1111");

}



void func3(void* arg)
{

    for (int i = 0;i < 100;i++) {
        // sleep(1);
        fprintf(fp, "fun%d:%d\t", *((int*)arg), i);
    }

    fprintf(fp, "\n");
    s.threads[s.running_thread_id].state = FREE;
    fprintf(fp, "fun%d:over!!!!!!!!!!!!\n", *((int*)arg));
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
        id_slots[i] = i;
        uthread_create(s, func3, (void*)&id_slots[i]);//创建时调用uthread_body();
    }


    while (!schedule_finished(s)) {


        //int& iter = s.running_thread_id;
        for (iter = 0; iter < MAX_UTHREAD_SIZE; ++iter) {//如果是可执行和挂起才执行

            getcontext(&s.main);



            // if (iter + 1 == MAX_UTHREAD_SIZE) {
            //     //iter = 0;
            // }
            printf("%d\n", iter);
            if (s.threads[iter].state == RUNNABLE) {
                s.threads[iter].state = RUNNING;
                s.threads[iter].func(s.threads[iter].arg);

            }
            else if (s.threads[iter].state == SUSPEND) {
                uthread_resume(s, iter);
            }

        }
    }
    puts("main over");
    printf("%s\n", schedule_finished(s) ? "y" : "n");



}
void sig_ticks_timer(int sig) {

    if (s.threads[iter].state == RUNNING)
        uthread_yield(s);
    setcontext(&s.main);
}

int main()
{
    s.threads = uthread_slots;

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
    fclose(fp);

    return 0;
}
