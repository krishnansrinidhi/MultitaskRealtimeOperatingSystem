//header files
#include <stdio.h>
#include <stdint.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"
#include "stm32f3_discovery_accelerometer.h"
#include "stm32f3_discovery_gyroscope.h"
#include "taskExecutive.h"
#include "common.h"



//globals
extern int32_t currentTask;
int32_t type = 1;



//function prototypes
void task1(void* data);
void task2(void* data);
void task3(void* data);
void task4(void* data);
void CmdTaskSwitch(int mode);
void CmdTaskAdd(int mode);
void CmdTaskKill(int mode);
void CmdKill_allTasks(int mode);



void task1(void* data)
{
  printf("Hello Sri! I am task1 \n");
  BSP_LED_Init(3);

  printf("blinking led 3...\n");
  for(int x = 0; x<5; x++)
    {
     BSP_LED_Toggle(3);
     HAL_Delay(100);
    }

  if(type==0) 
    {
     printf("Self destruction is activated for task 1...\n");
     TaskKill(1);
    }
  type = 1;
}



void task2(void* data)
{
  printf("Hello Sri! I am task2 \n");
  printf("reading accelerometer data...\n");
  int16_t xyz[3];
  BSP_ACCELERO_Init();
  BSP_ACCELERO_GetXYZ(xyz);
  printf("Accelerometer returns:\n"
	 "   X: %d\n"
	 "   Y: %d\n"
	 "   Z: %d\n",
	 xyz[0],xyz[1],xyz[2]);

  if(type==0) 
    {
     printf("Self destruction is activated for task 2...\n");
     TaskKill(2);
    }
  type = 1;
}



void task3(void* data)
{
  printf("Hello Sri! I am task3 \n");
  printf("Hello World!\n");
  if(type==0) 
    {
     printf("Self destruction is activated for task 3...\n");
     TaskKill(3);
    }
  type = 1;
}



void task4(void* data)
{
  printf("Hello Sri! I am task4 \n");
  printf("reading gyroscope data...\n");
  float xyz[3];
  BSP_GYRO_Init();
  BSP_GYRO_GetXYZ(xyz);
  printf("Gyroscope returns:\n"
	 "   X: %d\n"
	 "   Y: %d\n"
	 "   Z: %d\n",
	 (int)(xyz[0]*256),
	 (int)(xyz[1]*256),
	 (int)(xyz[2]*256));

  if(type==0) 
    {
     printf("Self destruction is activated for task 4...\n");
     TaskKill(4);
    }
  type = 1;
}



void CmdTaskAdd(int mode)
{
  int rc;
  int32_t id;
  int status = 0;

  if(mode != CMD_INTERACTIVE) 
   {
     return;
   }

  rc = fetch_int32_arg(&id);

  if(rc) 
   {
    printf("Missing task id\n");
    return;
   }

  fetch_int32_arg(&type);

  switch(id)
  {
    case 1:  status = checkPrior_taskAdd(&task1);
             if(status == 1) TaskAdd(task1, (void*)id);
	     else if(status == -1) printf("Error: task %d is already present\n",(signed int)id);
             break;

    case 2:  status = checkPrior_taskAdd(&task2);
             if(status == 1) TaskAdd(task2, (void*)id);
 	     else if(status == -1) printf("Error: task %d is already present\n",(signed int)id);
             break;

    case 3:  status = checkPrior_taskAdd(&task3);
	     if(status == 1) TaskAdd(task3, (void*)id);
             else if(status == -1) printf("Error: task %d is already present\n",(signed int)id);
             break;

    case 4:  status = checkPrior_taskAdd(&task4);
	     if(status == 1) TaskAdd(task4, (void*)id);
             else if(status == -1) printf("Error: task %d is already present\n",(signed int)id);
             break;

    default: if(!(id>0 && id<=MAX_TASKS)) printf("Please enter a task id from 1..%d\n",MAX_TASKS);
  }
  return;
}
ADD_CMD("addtask",CmdTaskAdd,"<task id type>	Add a task to the queue, type = 0 for self destruction");



void CmdTaskKill(int mode)
{
  int rc;
  int32_t id;
  
  if(mode != CMD_INTERACTIVE) 
   {
     return;
   }

  rc = fetch_int32_arg(&id);

  if(rc) 
   {
    printf("Missing task id\n");
    return;
   }

  switch(id)
  {
    case 1:  TaskKill(id);
             break;

    case 2:  TaskKill(id);
             break;

    case 3:  TaskKill(id);
             break;
    
    case 4:  TaskKill(id);
             break;

    default: if(!(id>0 && id<=MAX_TASKS)) printf("Please enter a task id from 1..%d\n",MAX_TASKS);
  }
  return;
}
ADD_CMD("killtask",CmdTaskKill,"<task id> 	Kill a task in the queue");



void CmdTaskSwitch(int mode)
{

 uint32_t Loop_Cnt = 0;
 int rc;

 if(mode != CMD_INTERACTIVE) 
   {
     return;
   }

 rc = fetch_uint32_arg(&Loop_Cnt);
 if(rc) 
   {
    printf("Missing Loop/Iteration count\n");
    return;
   }

 //check if tasks are vailable
 checkForTasks(); 

 //reset condition to start from 1st task
 currentTask = MAX_TASKS-1;

 for(int i = 0; i<Loop_Cnt; i++)
   {
     TaskSwitcher();
   }
 return;
}
ADD_CMD("runtask",CmdTaskSwitch,"<iteration val> Switch from a task to another task");



void CmdKill_allTasks(int mode)
{
 if(mode != CMD_INTERACTIVE) 
   {
     return;
   }

 taskInit();
 return;
}
ADD_CMD("clralltasks",CmdKill_allTasks,"		Kills all the tasks in the queue");
