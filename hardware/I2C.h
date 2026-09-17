#include "stdint.h"

#ifndef __I2C_H
#define __I2C_H

void I2C_init(void);
void I2C_Start(void);
void I2C_Stop(void);
uint8_t I2C_send_Byte(uint8_t data);

#endif
