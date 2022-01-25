#ifndef MY_UTHREAD_CPP
#define MY_UTHREAD_CPP


#include "uthread.h"
//#include <stdio.h>

void uthread_resume(schedule_t& schedule, int id)
{
    if (id < 0 || id >= schedule.max_index) {//无效id
        return;
    }

    uthread_t* t = &(schedule.threads[id]);

    if (t->state == SUSPEND) {//如果是挂起状态，就恢复
        swapcontext(&(schedule.main), &(t->ctx));
    }
    if (t->state == FREE) {
        schedule.running_thread_id += 1;
    }
}

void uthread_yield(schedule_t& schedule)
{
    if (schedule.running_thread_id != -1) {//判断是否无效id
        uthread_t* t = &(schedule.threads[schedule.running_thread_id]);
        t->state = SUSPEND;
        schedule.running_thread_id += 1;

        swapcontext(&(t->ctx), &(schedule.main));
    }
}

void uthread_body(schedule_t* ps)//执行
{
    int id = ps->running_thread_id;

    if (id != -1) {
        uthread_t* t = &(ps->threads[id]);

        //t->func(t->arg);

        t->state = RUNNABLE;

        //ps->running_thread_id = -1;
    }
}

int uthread_create(schedule_t& schedule, Fun func, void* arg)
{
    static int id = 0;

    uthread_t* t = &(schedule.threads[id]);

    t->state = RUNNABLE;
    t->func = func;
    t->arg = arg;
    t->pid = id;
    getcontext(&(t->ctx));

    t->ctx.uc_stack.ss_sp = t->stack;
    t->ctx.uc_stack.ss_size = DEFAULT_STACK_SZIE;
    t->ctx.uc_stack.ss_flags = 0;
    t->ctx.uc_link = &(schedule.main);
    schedule.running_thread_id = id++;

    makecontext(&(t->ctx), (void (*)(void))(uthread_body), 1, &schedule);
    swapcontext(&(schedule.main), &(t->ctx));

    return id;
}

int schedule_finished(const schedule_t& schedule)//sucess for 1,no 0 
{

    for (int i = 0; i < MAX_UTHREAD_SIZE; ++i) {
        if (schedule.threads[i].state != FREE) {
            return 0;
        }
    }


    return 1;
}

#endif
