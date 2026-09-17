#include "stdint.h"

#ifndef __OLED_H
#define __OLED_H

void OLED_WriteData(uint8_t data);
void OLED_WriteCmd(uint8_t cmd);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t x, uint8_t y, char ch);
void OLED_ShowString(uint8_t x, uint8_t y, char *str);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len);
void OLED_ShowChinese(uint8_t x, uint8_t y, const char *ch);

#endif
