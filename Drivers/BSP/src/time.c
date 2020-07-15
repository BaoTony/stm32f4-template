/*
 * @Author: TonyJia
 * @Date: 2020-07-15 20:30:11
 * @LastEditTime: 2020-07-15 21:14:38
 * @LastEditors: TonyJia
 * @Description: 
 */
#include "time.h"

volatile uint32_t msTicks; /* counts 100ms timeTicks */

/**
 * @Name: Timer_Interrupts_Config
 * @Fuction: 初始化定时器中断
 * @Param: None
 * @Return: None
 */
void Timer_Interrupts_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @Name: time3_init
 * @Fuction: init time3
 * @Param: None
 * @Return: None
 */
void time3_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_DeInit(TIM3); //复位TIM3定时器

    /* TIM3 configuration */
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;                // 100ms
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;             // 分频8399
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数方向向上计数
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* Clear TIM3 update pending flag[清除TIM3溢出中断标志] */
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);

    /* Enable TIM3 Update interrupt [TIM3溢出中断允许]*/
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    /* TIM3计数器清零*/
    TIM3->CNT = 0;
    /* TIM3 enable counter [允许TIM3计数]*/
    TIM_Cmd(TIM3, DISABLE);
    /*Config interrupts*/
    Timer_Interrupts_Config();
}

/**
 * @Name: Timer_Start
 * @Fuction: 清除定时器计数器寄存器计数值并启动定时器
 * @Param: None
 * @Return: None
 */
void Timer_Start(void)
{
    TIM3->CNT = 0;         //清除计数器寄存器的值，可以减小丢帧的情况
    TIM_Cmd(TIM3, ENABLE); /* Enable the TIM Counter */
}

/**
 * @Name: Timer_Stop
 * @Fuction: 停止定时器并清除定时器的计数值
 * @Param: None
 * @Return: None
 */
void Timer_Stop(void)
{
    TIM_Cmd(TIM3, DISABLE); /* Disable the TIM Counter */
}

/**
  * @brief  定时器3中断处理函数，在该函数中将ADC1转换标志置位
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检测是否发生溢出更新事件
    {
        TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update); //清除TIM2的中断待处理位
        msTicks++;                                    /* increment counter necessary in Delay()*/
        //SHOULD BE Added DNS Timer Handler your 1s tick timer
        if ((msTicks % 10) == 0)
        {
            DNS_time_handler();
        }
    }
}

/*********************************END OF FILE**********************************/
