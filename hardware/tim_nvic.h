#ifndef TIM_NVIC_H
#define TIM_NVIC_H
#include "stdint.h"

//extern uint8_t key_num;                                                              // ★修改前:与 tim_NVIC.c 里的 int8_t 定义不一致(-1 会被 main 读成 255,case -1 永远不成立)
extern int8_t key_num;                                                                 // ★修改后:与定义一致(tim_NVIC.c 用的是 int8_t)
void TIM2_IRQHandler(void);
void tim_NVIC_init(void);


#endif 
