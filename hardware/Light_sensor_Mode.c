#include "stm32f10x.h"
#include "delay.h"

void L_Sensor_M_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_LSM;
    GPIO_LSM.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_LSM.GPIO_Pin = GPIO_Pin_13;
    GPIO_LSM.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_LSM);
}

int LSM_Getval(void){
    uint8_t val = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
    return val;
}
