#Co-operative Multi-task RTOS

A simple Task Executive is implemented that supports the following primitives:
int32_t TaskAdd(void (*f)(void *data), void *data);
int32_t TaskKill(int32_t id);
int32_t TaskCurrent(void);
int32_t TaskSwitcher(void);


A set of test tasks is implemented that should at various times:
● Create a new task.
● Kill another task.
● Kill themselves.
