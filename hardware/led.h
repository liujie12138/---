#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"                  // Device header
void led_light_on(void);
void led_dark_on(void);
void led_light_off(void);
void led_dark_off(void);
void led_init();
#endif