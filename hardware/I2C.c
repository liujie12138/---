#include "stm32f10x.h"
#include "delay.h"

#define SDA_H GPIO_SetBits(GPIOB, GPIO_Pin_9)
#define SDA_L GPIO_ResetBits(GPIOB, GPIO_Pin_9)
#define SCL_H GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define SCL_L GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define SDA_read GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)

void I2C_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_I2C;
    GPIO_I2C.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_I2C.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_I2C.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_I2C);

    SDA_H;
    SCL_H;
}

void I2C_Start(void)
{
    SDA_H;
    SCL_H;
    delay_us(5);
    SDA_L;
    delay_us(5);
    SCL_L;
}

void I2C_Stop(void)
{
    SDA_L;
    SCL_H;
    delay_us(2);
    SDA_H;
}

uint8_t I2C_send_Byte(uint8_t data)
{
    uint8_t i;
    for(i = 0; i < 8; i++){
        if(data & 0x80){
            SDA_H;
        }else{
            SDA_L;
        }
        SCL_H;
        delay_us(2);
        SCL_L;
        data <<= 1;
    }
    // ACK
    SDA_H;
    SCL_H;
    delay_us(2);
    uint8_t ack = SDA_read;
    SCL_L;
    return ack;
}
