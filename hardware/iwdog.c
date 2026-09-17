#include "stm32f10x.h"                  // Device header


void iwdog(void){
    //开启看门狗时钟
    RCC_LSICmd(ENABLE);

    //配置看门狗
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_32);
    IWDG_SetReload(2500 - 1);//看门狗复位时间2S
    IWDG_Enable();
}
//获取标志位函数
uint8_t get_iwdogflag(void){
    int8_t flag = RCC_GetFlagStatus(RCC_FLAG_IWDGRST);
    RCC_ClearFlag();
    return flag;
}
//喂狗函数
void feeddog (void){
    IWDG_ReloadCounter();
}
