#include "stm32f10x.h"

void key_NVIC_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置中断引脚

    GPIO_InitTypeDef GPIO_Initstructure;
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstructure);

    // 初始化中断线
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    // 初始化中断
    EXTI_InitTypeDef EXTI_Initstructure;
    EXTI_Initstructure.EXTI_Line = EXTI_Line0;
    EXTI_Initstructure.EXTI_LineCmd = ENABLE; // 使能中断线
    EXTI_Initstructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_Initstructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_Initstructure);

    // 配置中断组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // 初始化NVIC
    NVIC_InitTypeDef NVIC_Initsructure;
    NVIC_Initsructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_Initsructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Initsructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_Initsructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_Initsructure);
}
// 中断函数{开启定时中断}
void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) == 1)
    {
        EXTI_ClearITPendingBit(EXTI_Line0);
        TIM_Cmd(TIM2, ENABLE);
    }
}
