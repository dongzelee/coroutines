#include "Coroutine.h"

void coroutines_body(schedule &schedule)
{
     int id = schedule->running();//获取当前执行协程
     if(id >0)
     {
     	 coroutine* co = schedule->co[id];
     	 co->func(co->args);
     	 co->state = DEAD;
     	 co->running = -1; 
     }
}


int coroutine_create(schedule &schedule,coroutine_func func,void *arg)
{
    for(int id = 0; id < schedule.max_index; ++id ){
        if(schedule.co[id].states == DEAD){
            break;
        }
    }
    
    if (id == schedule.max_index) {
        schedule.max_index++;
    }

    coroutine *co = &(schedule.co[id]);

    co->state = READY;
    co->func = func;
    co->arg = arg;

    getcontext(&(co->ctx));
    
    co->ctx.uc_stack.ss_sp = co->stack;
    co->ctx.uc_stack.ss_size = STACK_SIZE;
    co->ctx.uc_stack.ss_flags = 0;
    co->ctx.uc_link = &(schedule.main);
    schedule.running_thread = id;
    
    makecontext(&(co->ctx),(void (*)(void))(coroutines_body),1,&schedule);
    swapcontext(&(schedule.main), &(co->ctx));
    
    return id;
}


bool coroutine_finished(const schedule &schedule)
{
	if (schedule.running_thread != -1){
        return 0;
    }else{
        for(int i = 0; i < schedule.max_index; ++i){
            if(schedule.threads[i].state != DEAD){
                return 0;
            }
        }
    }

    return 1;
}

int coroutine_resume(schedule &schedule,int id)
{

	if(id < 0 || id >max_index)
		return ;
	  coroutine* co = schedule.co[id];
	if(co->states == SUSPEND)
		swapcontext(&(co->ctx),&(schedule.main));
}


void coroutine_yield(schedule &schedule)
{
	int id = schedule->running;
	if(id!=-1)
	{
      coroutine*co = schedule[id];
      co->func(co->args);
      co->state = SUSPEND;
      schedule->running=-1;

	}
}

void coroutine_delete(schedule *schedule)
{
  free(co->stack);
  free(schedule);
}


