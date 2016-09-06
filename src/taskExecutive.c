//header files
#include <stdio.h>
#include <stdint.h>
#include "taskExecutive.h"


//globals
int32_t currentTask;



typedef struct Task_s {
void (*f)(void *data); 		// Task function 
void *data;			// Private data pointer for this task 
} Task_t;



Task_t tasks[MAX_TASKS];	// Process list (array of struct Task_s)



void taskInit(void)
{
  /* Initialize tasks */
  printf("Started Clearing tasks...\n");
  for(int i = 0; i<MAX_TASKS; i++)
    {
      tasks[i].f = NULL;
      tasks[i].data = NULL;
    }
  printf("All tasks cleared\n");
}



int32_t TaskAdd(void (*f)(void* data), void* data)
{
  //int* task_id = (int*)data;
  /* Try to find an empty slot */
  for(int i=0; i<MAX_TASKS; i++) 
   {
     if(tasks[i].f == NULL) 
      {
        tasks[i].f= f;
	tasks[i].data = data;
        printf("finished adding task %d...\n",(int)tasks[i].data);
	return i;
      }
   }

  /* No slots available, return -1 */
  return -1;
}
	


uint32_t TaskKill(uint32_t id)
{  
  /* Kill the respective task based on id */

  if(id == 0) printf("Invalid Task id\n");

  tasks[id-1].f    = NULL;
  tasks[id-1].data = NULL;
  printf("Hello Sri! Task %u deleted\n", (unsigned int)id);
  return 0;
}



int32_t TaskNext(void)
{
  /* Find the next task to run */
  int32_t i = currentTask;
  uint32_t count=0;

  do {
       i = (i+1) % MAX_TASKS;
       count++;
     } while((tasks[i].f == NULL)&& (count <= MAX_TASKS));

  return (count <= MAX_TASKS) ? i : -1;
}



int32_t TaskSwitcher(void)
{
  /* Switch to the next task in queue */
  currentTask = TaskNext(); 
 
  if(currentTask < 0) 
    {
     return -1;
    }
  
  tasks[currentTask].f(tasks[currentTask].data);
  return 0;
}



void checkForTasks(void)
{
  int i = 0;
  int flag = 0;

  printf("Searching for tasks...\n");

  while(i<MAX_TASKS)
  {
   if(tasks[i].f == NULL && tasks[i].data == NULL)
     { 
       i++;
     }

    else
     { 
       printf("Task %d is available...\n",i+1);
       i++;
       flag++;
     }
   }

   if(!flag) printf("Hey Sri! No tasks available..Please add tasks\n");
   return;
}



int8_t checkPrior_taskAdd(void (*f)(void* data))
{
  for(int i = 0; i<MAX_TASKS; i++)
    {
      if(tasks[i].f == f) return -1;
    }
  return 1;
}




