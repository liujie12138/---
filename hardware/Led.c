#include "stm32f10x.h"                  // Device header

void led_init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_LED;
	GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_LED.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_LED);
}

void led_light_on(void){
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}
void led_dark_on(void){
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}
void led_light_off(void){
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
}
void led_dark_off(void){
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}
