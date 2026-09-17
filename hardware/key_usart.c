#include "stm32f10x.h"
#include "stdio.h"

uint16_t serial_TXpacket[4];
uint16_t serial_RXpacket[4];
int8_t serial_RXflag = 0; // 整包状态
uint8_t serial_RXstate = 0;
uint8_t index = 0;


void key_usart_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
    GPIO_InitTypeDef GPIO_Initstructure;
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Initstructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstructure);
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Initstructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_Initstructure);

    //初始化USART
    USART_InitTypeDef USART_Initsatructure;
    USART_Initsatructure.USART_BaudRate = 9600;
    USART_Initsatructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Initsatructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
    //USART_Initsatructure.USART_Parity = USART_Parity_Odd;                        // ★修改前:奇校验
    USART_Initsatructure.USART_Parity = USART_Parity_No;                           // ★修改后:无校验(和串口助手默认 8-N-1 一致)
    USART_Initsatructure.USART_StopBits = USART_StopBits_1;
    //USART_Initsatructure.USART_WordLength = USART_WordLength_9b;                 // ★修改前:9 位 = 8 数据位 + 奇校验
    USART_Initsatructure.USART_WordLength = USART_WordLength_8b;                   // ★修改后:8 位数据
    USART_Init(USART1,&USART_Initsatructure);
    USART_Cmd(USART1,ENABLE);
}
// 发送数据函数
void serial_sendbyte(uint8_t byte)
{
    USART_SendData(USART1, byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
// 发送数组
void serial_sendarry(uint16_t cout, uint16_t *arry)
{
    uint16_t i = 0;
    for (i = 0; i < cout; i++)
    {
        USART_SendData(USART1, arry[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            ;
    }
}
// 发送字符串
void serial_sendstring(char *arry)
{
    uint8_t i = 0;
    while (arry[i] != '\0')
    {
        USART_SendData(USART1, arry[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            ;
        i++;
    }
}
// 平方函数
uint16_t serial_pow(uint16_t x, uint16_t y)
{
    uint16_t result = 1;
    while (y > 0)
    {
        result *= x;
        y--;
    }
    return result;
}
// 发送数字
void serial_sendnum(uint16_t num)
{
    uint16_t temp = num;
    uint8_t i = 0;
    do
    {
        temp /= 10;
        i++;
    } while (temp != 0);
    while (i > 0)
    {
        uint8_t data = num / serial_pow(10, i - 1) % 10;
        USART_SendData(USART1, data + '0');
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            ;
        i--;
    }
}

// 接收函数
uint16_t recive_data(void)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        ;
    uint16_t rdata = USART_ReceiveData(USART1);
    return rdata;
}
// 发送数据包
void serial_sendpacket(uint8_t i, uint16_t *serial_TXpacket)
{
    serial_sendbyte(0xFF);
    serial_sendarry(i, serial_TXpacket);
    serial_sendbyte(0xFE);
}
// 接收数据包
void serial_recivepacket(void)
{
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == 0)
    {
        return;
    }
    uint16_t data = USART_ReceiveData(USART1);
    switch (serial_RXstate)
    {
    case 0:
    {
        if (data == 0xFF)
        {
            serial_RXstate = 1;
        }
        break;
    }
    case 1:
    {

        serial_RXpacket[index] = data;
        index++;
        if (index == 4)
        {
            index = 0;
            serial_RXstate = 2;
            break;
        }

        break;
    }
    case 2:
    {
        if (data == 0xFE)
        {
            serial_RXflag = 1;
            serial_RXstate = 0; // 恢复状态
        }
        else
        {
            serial_RXflag = -1;
            serial_RXstate = 0;
        }
        break;
    }
    }
}

//printf映射到串口
//int fputc(int ch,FILE *f){
//    USART_SendData(USART1,(uint8_t) ch);
//    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == SET){          // ★原代码:等待条件写反(写 DR 后 TXE 被清 0,这里又完全不等)

//    }
//    return ch;
//}

// ★新增:printf 重定向到 USART1。printf 必须靠 fputc 才能吐到串口,
//        这一段被注释掉就等于没有重定向 —— 不勾 MicroLIB 时标准库会退回半主机(semihosting),
//        没接调试器时 printf 直接卡死;勾了 MicroLIB 也只是丢进空实现,一样打不出来。
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint8_t)ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);        // ★修改后:等 TXE 置位(数据寄存器空)再返回
    return ch;
}
