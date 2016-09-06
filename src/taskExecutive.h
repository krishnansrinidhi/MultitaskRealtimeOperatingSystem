#ifndef TASK_H_
#define TASK_H_

//constants
#define MAX_TASKS 4 

//prototypes
void taskInit(void);
int32_t TaskAdd(void (*f)(void* data), void* data);
uint32_t TaskKill(uint32_t id);
int32_t TaskSwitcher(void);
int32_t TaskNext(void);
void checkForTasks(void);
int8_t checkPrior_taskAdd(void (*f)(void* data));

#endif
