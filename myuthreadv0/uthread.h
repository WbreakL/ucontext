/**
* @file  thread.h
* @author chenxueyou
* @version 0.1
* @brief   :A asymmetric coroutine library for C++
* History
*      1. Date: 2014-12-12
*          Author: chenxueyou
*          Modification: this file was created
*/

#ifndef MY_UTHREAD_H
#define MY_UTHREAD_H


#include <ucontext.h>
#include <vector>

#define DEFAULT_STACK_SZIE (1024*128)
#define MAX_UTHREAD_SIZE   1024

enum ThreadState { FREE, RUNNABLE, RUNNING, SUSPEND };

struct schedule_t;

typedef void (*Fun)(void* arg);


typedef struct uthread_t
{
    ucontext_t ctx;
    Fun func;
    void* arg;
    enum ThreadState state;
    char stack[DEFAULT_STACK_SZIE];
}uthread_t;
static uthread_t uthread_slots[MAX_UTHREAD_SIZE];
typedef struct schedule_t
{
    ucontext_t main;
    int running_thread_id;
    uthread_t* threads;
    int max_index; // 曾经使用到的最大的index + 1

    schedule_t() :running_thread_id(-1), max_index(0),
        threads(uthread_slots) {

        for (int i = 0; i < MAX_UTHREAD_SIZE; i++) {
            threads[i].state = FREE;
        }
    }

    ~schedule_t() {
        //delete[] threads;
    }
}schedule_t;

/*help the thread running in the schedule*/
static void uthread_body(schedule_t* ps);


int  uthread_create(schedule_t& schedule, Fun func, void* arg);

/* Hang the currently running thread, switch to main thread */
void uthread_yield(schedule_t& schedule);

/* resume the thread which index equal id*/
void uthread_resume(schedule_t& schedule, int id);


int  schedule_finished(const schedule_t& schedule);

#endif
