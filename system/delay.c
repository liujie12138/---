#include "stm32f10x.h"
static uint16_t fac_us;
static uint16_t fac_ms;
// ??? SysTick
void delay_init(void) {
   SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); // HCLK/8
   fac_us = SystemCoreClock / 8000000; // ?? 1us ????
   fac_ms = fac_us * 1000; // ?? 1ms ????
}
// ????
void delay_us(uint32_t nus) {
   uint32_t temp;
   SysTick->LOAD = fac_us * nus;
   SysTick->VAL = 0x00;
   SysTick->CTRL = 0x01; // ??
   do {
       temp = SysTick->CTRL;
   } while ((temp & 0x01) && !(temp & (1 << 16)));
   SysTick->CTRL = 0x00;
   SysTick->VAL = 0x00;
}
// ????
void delay_ms(uint16_t nms) {
   uint32_t temp;
   SysTick->LOAD = fac_ms * nms;
   SysTick->VAL = 0x00;
   SysTick->CTRL = 0x01;
   do {
       temp = SysTick->CTRL;
   } while ((temp & 0x01) && !(temp & (1 << 16)));
   SysTick->CTRL = 0x00;
   SysTick->VAL = 0x00;
}
