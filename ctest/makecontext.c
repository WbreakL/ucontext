#include <ucontext.h>
#include <stdio.h>

void func1(void* arg)
{
    puts("1");
    puts("11");
    puts("111");
    puts("1111");

}
void context_test()
{
    char stack[1024 * 128];
    ucontext_t child, main, next;

    getcontext(&child);
    getcontext(&next);//获取当前上下文
    child.uc_stack.ss_sp = stack;//指定栈空间
    child.uc_stack.ss_size = sizeof(stack);//指定栈空间大小
    //child.uc_stack.ss_flags = 0;
    child.uc_link = &next;//设置后继上下文

    makecontext(&child, (void (*)(void))func1, 0);//修改上下文指向func1函数

    swapcontext(&main, &child);//切换到child上下文，保存当前上下文到main
    puts("main");//如果设置了后继上下文，func1函数指向完后会返回此处
}
int main()
{
    puts("66666666666662221");

    context_test();
    puts("22222222221");

    return 0;
}