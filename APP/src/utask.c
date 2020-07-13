#include "utask.h"


//开始任务优先级
#define START_TASK_PRIO 1
//开始任务堆栈大小
#define START_STK_SIZE 128
//开始任务句柄
TaskHandle_t StartTask_Handler;
//开始任务函数
void start_task(void *pvParameters);

//系统状态任务优先级
#define SYSTEM_STATE_TASK_PRIO 2
//系统状态任务堆栈大小
#define SYSTEM_STATE_STK_SIZE 128
//系统状态任务句柄
TaskHandle_t SystemStateTask_Handler;
//系统状态任务函数
void system_state_task(void *pvParameters);

//网络任务优先级
#define NETWORK_TASK_PRIO 3
//网络任务堆栈大小
#define NETWORK_STK_SIZE 128
//网络任务句柄
TaskHandle_t NetworkTask_Handler;
//网络任务函数
void network_task(void *pvParameters);


//系统状态任务函数
void system_state_task(void *pvParameters)
{
  while (1)
  {
    LED1 = 0;
    vTaskDelay(500);
    LED1 = 1;
    vTaskDelay(500);
  }
}

//网络任务函数
void network_task(void *pvParameters)
{
  while (1)
  {
	my_debug("led1 task running!\r\n");
	vTaskDelay(1000);   
  }
}


//开始任务任务函数
void start_task(void *pvParameters)
{
  taskENTER_CRITICAL(); //进入临界区
	
  //创建系统状态任务
  xTaskCreate((TaskFunction_t)system_state_task,
              (const char *)"task1_task",
              (uint16_t)SYSTEM_STATE_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)SYSTEM_STATE_TASK_PRIO,
              (TaskHandle_t *)&SystemStateTask_Handler);
  //创建网络任务
  xTaskCreate((TaskFunction_t)network_task,
              (const char *)"network_task",
              (uint16_t)NETWORK_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)NETWORK_TASK_PRIO,
              (TaskHandle_t *)&NetworkTask_Handler);
  vTaskDelete(StartTask_Handler); //删除开始任务
  taskEXIT_CRITICAL();            //退出临界区
}


void utask_init(void)
{
	//TODO:在这里做初始化工作
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置系统中断优先级分组4
  delay_init(168);                                //初始化延时函数
  uart_init(115200);                              //初始化串口
  LED_Init();                                     //初始化LED端口

  //创建开始任务
  xTaskCreate((TaskFunction_t)start_task,          //任务函数
              (const char *)"start_task",          //任务名称
              (uint16_t)START_STK_SIZE,            //任务堆栈大小
              (void *)NULL,                        //传递给任务函数的参数
              (UBaseType_t)START_TASK_PRIO,        //任务优先级
              (TaskHandle_t *)&StartTask_Handler); //任务句柄
  vTaskStartScheduler();                           //开启任务调度
}
