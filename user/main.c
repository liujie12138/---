#include "stm32f10x.h" // Device header
#include "key_NVIC.h"
#include "tim_nvic.h"
#include "delay.h"
#include "I2C.h"
#include "oled.h"
#include "key_led.h"
#include "iwdog.h"
#include "key_usart.h" // ★新增:key_usart_init/serial_sendbyte 的声明(否则是隐式声明)
#include "stdio.h"

int main()
{
	delay_init();
	I2C_init();
	OLED_Init();
	key_NVIC_init();
	tim_NVIC_init();
	OLED_Clear();
	led_init();
	key_usart_init();
	setvbuf(stdout, NULL, _IONBF, 0); // ★新增:关掉 stdout 缓冲,不勾 MicroLIB 也能立刻打印出来
	iwdog();
	feeddog();

	printf("%d\r\n", get_iwdogflag());

	while (1)
	{
		uint8_t iwdogflag = get_iwdogflag(); // ★新增:每轮只读一次(该函数内部会 RCC_ClearFlag 清标志)
		OLED_ShowNum(0, 0, key_num, 4);
		switch (key_num)
		{
		case -1:
		{
			led_status_LP();
		}
		case 0:
		{
			led_status0();
			break;
		}
		case 1:
		{
			led_status1();
			break;
		}
		case 2:
		{
			led_status2();
			break;
		}
		default:
		{
			key_num = 0;
			break;
		}
		}
		if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == 1)
		{
			if (recive_data() == 0x02)
			{
				delay_ms(3000);
			}
		}
		feeddog();
//		printf("%d\r\n", iwdogflag);
//		delay_ms(1000);
	}
}
