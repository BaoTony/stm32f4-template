#include "utask.h"


//��ʼ�������ȼ�
#define START_TASK_PRIO 1
//��ʼ�����ջ��С
#define START_STK_SIZE 128
//��ʼ������
TaskHandle_t StartTask_Handler;
//��ʼ������
void start_task(void *pvParameters);

//ϵͳ״̬�������ȼ�
#define SYSTEM_STATE_TASK_PRIO 2
//ϵͳ״̬�����ջ��С
#define SYSTEM_STATE_STK_SIZE 128
//ϵͳ״̬������
TaskHandle_t SystemStateTask_Handler;
//ϵͳ״̬������
void system_state_task(void *pvParameters);

//�����������ȼ�
#define NETWORK_TASK_PRIO 3
//���������ջ��С
#define NETWORK_STK_SIZE 128
//����������
TaskHandle_t NetworkTask_Handler;
//����������
void network_task(void *pvParameters);


//ϵͳ״̬������
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

//����������
void network_task(void *pvParameters)
{
  while (1)
  {
	my_debug("led1 task running!\r\n");
	vTaskDelay(1000);   
  }
}


//��ʼ����������
void start_task(void *pvParameters)
{
  taskENTER_CRITICAL(); //�����ٽ���
	
  //����ϵͳ״̬����
  xTaskCreate((TaskFunction_t)system_state_task,
              (const char *)"task1_task",
              (uint16_t)SYSTEM_STATE_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)SYSTEM_STATE_TASK_PRIO,
              (TaskHandle_t *)&SystemStateTask_Handler);
  //������������
  xTaskCreate((TaskFunction_t)network_task,
              (const char *)"network_task",
              (uint16_t)NETWORK_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)NETWORK_TASK_PRIO,
              (TaskHandle_t *)&NetworkTask_Handler);
  vTaskDelete(StartTask_Handler); //ɾ����ʼ����
  taskEXIT_CRITICAL();            //�˳��ٽ���
}


void utask_init(void)
{
	//TODO:����������ʼ������
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //����ϵͳ�ж����ȼ�����4
  delay_init(168);                                //��ʼ����ʱ����
  uart_init(115200);                              //��ʼ������
  LED_Init();                                     //��ʼ��LED�˿�

  //������ʼ����
  xTaskCreate((TaskFunction_t)start_task,          //������
              (const char *)"start_task",          //��������
              (uint16_t)START_STK_SIZE,            //�����ջ��С
              (void *)NULL,                        //���ݸ��������Ĳ���
              (UBaseType_t)START_TASK_PRIO,        //�������ȼ�
              (TaskHandle_t *)&StartTask_Handler); //������
  vTaskStartScheduler();                           //�����������
}
