#include "stm32f10x.h"                  // Device header

void buzzer_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_buzzer;
	GPIO_buzzer.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_buzzer.GPIO_Pin = GPIO_Pin_12;
	GPIO_buzzer.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_buzzer);
}
void buzzer_on(){
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	
}
void buzzer_off(){
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}
