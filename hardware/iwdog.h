#include "stdint.h"
#ifndef IWDOG_H
#define IWDOG_H

void iwdog(void);
uint8_t get_iwdogflag(void); // 喂狗函数
void feeddog(void);

#endif
