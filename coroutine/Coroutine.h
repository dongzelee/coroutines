#ifndef COROUTINE_H
#define COROUTINE_H

#ifdef _ON_MAC_
#include <sys/ucontext.h>
#else
#include <ucontext.h>
#endif
#define STACK_SIZE (1024*1024)//1M大小
#define MAX_UTHREAD_SIZE   1024


enum states
{
	DEAD,READY,RUNNING,SUSPEND
};

typedef void (*coroutine_func)(void *arg);

typedef struct coroutine
{
	ucontext_t ctx;//当前协程上下文状态
	coroutine_func func;//协程执行函数
	void* arg;//函数参数
	enum states state;//此coroutine的状态，是正在运行，还是挂起 
    char stack[STACK_SIZE];
};

typedef struct schedule
{
	ucontext_t main;//主函数应用上下文
	int running; // schedule状态, 正在执行哪个coroutine.. = id
    coroutine *co;//协程所有信息
    int max_index;
    schedule:running(-1),max_index(0) {
        coroutines = new coroutine[MAX_UTHREAD_SIZE];
        for (int i = 0; i < MAX_UTHREAD_SIZE; i++) {
            coroutines[i].state = DEAD;
        }
    }
};
void coroutines_body(schedule &schedule);//让一个协程在调度器内部指向
int coroutine_create(schedule &schedule,coroutine_func func,void *arg);//在一个调度器内部创建协程
bool coroutine_finished(const schedule &schedule);//销毁所有协程
int coroutine_resume(schedule &schedule,int id);//恢复id为？的协程使用
void coroutine_yield(schedule &schedule);//中断协程使用
void coroutine_delete(schedule *schedule);//销毁一个任务管理器


#endif // COROUTINE_H
