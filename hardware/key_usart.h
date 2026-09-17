#include "stdint.h"

#ifndef KEY_USART_H
#define KEY_USART_H


extern uint16_t serial_TXpacket[4];
extern uint16_t serial_RXpacket[4];
extern int8_t serial_RXflag;
void key_usart_init(void);
void serial_sendbyte(uint8_t byte);
void serial_sendarry(uint16_t cout,uint16_t * arry);
void serial_sendstring(char*arry);
uint16_t recive_data(void);
void serial_sendnum(uint16_t num);
void serial_sendpacket(uint8_t i,uint16_t* serial_TXpacket);
void serial_recivepacket(void);

#endif