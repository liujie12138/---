#include "stm32f10x.h" // Device header
#include "delay.h"

void led_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_Initstructure;
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Initstructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_Initstructure);
}

void led_status_LP(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
    delay_ms(40);
    GPIO_SetBits(GPIOB, GPIO_Pin_13);
    delay_ms(40);
    GPIO_SetBits(GPIOB, GPIO_Pin_14);
    delay_ms(40);
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    delay_ms(40);
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);
    delay_ms(40);
    GPIO_ResetBits(GPIOB, GPIO_Pin_14);
    delay_ms(40);
}
void led_status0(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
}
void led_status1(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
}
void led_status2(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
    delay_ms(40);
    GPIO_SetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
    delay_ms(40);
}