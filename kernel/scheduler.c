#include "scheduler.h"
#include "task.h"

extern void context_switch(void*, void*);

static int current = 0;

/*   Round-Robin padrão   */

static int round_robin()
{
    int next = current;
    int count = 0;

    // para pular tasks invalidas
    do {
        next = (next + 1) % task_count;
        count++;
    } while (tasks[next].entry == 0 && count < task_count);

    return next;
}

/*   Algoritmo atual   */

static sched_algo_t current_algo = round_robin;

void scheduler_set_algorithm(sched_algo_t algo)
{
    if (algo)
        current_algo = algo;
}

/*   Yield   */

void yield()
{
    int prev = current;
    int next = current_algo();

    current = next;

    context_switch(tasks[prev].regs,
                   tasks[next].regs);
}
 
/*   Início   */

void scheduler_start()
{
    if (task_count == 0)
        return;

    // procura primeira task válida
    for (int i = 0; i < task_count; i++)
    {
        if (tasks[i].entry != 0)
        {
            current = i;
            tasks[i].entry();
        }
    }
}