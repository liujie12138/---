#include "stm32f10x.h"
#include "I2C.h"
#include "delay.h"
#include "font.h"
#include "font_cn.h"

void OLED_WriteCmdMulti(uint8_t *cmds, uint8_t len)
{
    uint8_t i;
    I2C_Start();
    I2C_send_Byte(0x78);
    I2C_send_Byte(0x00);
    for(i = 0; i < len; i++){
        I2C_send_Byte(cmds[i]);
    }
    I2C_Stop();
}

void OLED_WriteDataMulti(uint8_t *data, uint8_t len)
{
    uint8_t i;
    I2C_Start();
    I2C_send_Byte(0x78);
    I2C_send_Byte(0x40);
    for(i = 0; i < len; i++){
        I2C_send_Byte(data[i]);
    }
    I2C_Stop();
}

void OLED_WriteCmd(uint8_t cmd)
{
    OLED_WriteCmdMulti(&cmd, 1);
}

void OLED_WriteData(uint8_t data)
{
    OLED_WriteDataMulti(&data, 1);
}

void OLED_Init(void)
{
    delay_ms(100);

    OLED_WriteCmd(0xAE);       // display off
    OLED_WriteCmd(0x20);       // memory addressing mode
    OLED_WriteCmd(0x10);       // 0x10=page mode
    OLED_WriteCmd(0xC8);       // COM remap
    OLED_WriteCmd(0x40);       // start line
    OLED_WriteCmd(0x81);       // contrast
    OLED_WriteCmd(0xFF);       // max
    OLED_WriteCmd(0xA1);       // segment remap
    OLED_WriteCmd(0xA6);       // normal display
    OLED_WriteCmd(0xA8);       // multiplex ratio
    OLED_WriteCmd(0x3F);       // 1/64
    OLED_WriteCmd(0xA4);       // display on following ram
    OLED_WriteCmd(0xD3);       // display offset
    OLED_WriteCmd(0x00);
    OLED_WriteCmd(0xD5);       // clock divide
    OLED_WriteCmd(0xF0);
    OLED_WriteCmd(0xD9);       // precharge
    OLED_WriteCmd(0x22);
    OLED_WriteCmd(0xDA);       // com pins
    OLED_WriteCmd(0x12);
    OLED_WriteCmd(0xDB);       // vcomh
    OLED_WriteCmd(0x20);
    OLED_WriteCmd(0x8D);       // charge pump
    OLED_WriteCmd(0x14);       // enable
    OLED_WriteCmd(0xAF);       // display on
}

void OLED_Clear(void)
{
    uint8_t i, j;
    for(i = 0; i < 8; i++){
        OLED_WriteCmd(0xB0 + i);
        OLED_WriteCmd(0x00);
        OLED_WriteCmd(0x10);
        for(j = 0; j < 128; j++){
            OLED_WriteData(0x00);
        }
    }
}

void OLED_ShowChar(uint8_t x, uint8_t y, char ch)
{
    uint8_t i;
    uint8_t data[6];
    if(ch < 32 || ch > 126){
        ch = 32;
    }
    OLED_WriteCmd(0xB0 + y);
    OLED_WriteCmd(x & 0x0F);
    OLED_WriteCmd(0x10 | (x >> 4));
    for(i = 0; i < 6; i++){
        data[i] = Font_6x8[ch - 32][i];
    }
    OLED_WriteDataMulti(data, 6);
}

void OLED_ShowString(uint8_t x, uint8_t y, char *str)
{
    while(*str){
        OLED_ShowChar(x, y, *str);
        x += 6;
        if(x > 122){
            x = 0;
            y++;
        }
        str++;
    }
}

void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len)
{
    uint8_t i;
    uint8_t digits[10];
    for(i = 0; i < len; i++){
        digits[i] = num % 10;
        num /= 10;
    }
    for(i = 0; i < len; i++){
        OLED_ShowChar(x + (len - 1 - i) * 6, y, '0' + digits[i]);
    }
}

void OLED_ShowChinese(uint8_t x, uint8_t y, const char *ch)
{
    uint16_t i, idx;
    uint8_t found;

    while(*ch){
        // 在 CN_Font_Chars_UTF8 中查找匹配的3字节UTF-8序列
        found = 0;
        for(i = 0; i < CN_FONT_CHAR_COUNT; i++){
            if(ch[0] == CN_Font_Chars_UTF8[i*3] &&
               ch[1] == CN_Font_Chars_UTF8[i*3+1] &&
               ch[2] == CN_Font_Chars_UTF8[i*3+2]){
                idx = i;
                found = 1;
                break;
            }
        }
        if(!found){
            ch += 3;
            continue;
        }

        // 上半: page y, 写 data[0..15]
        OLED_WriteCmd(0xB0 + y);
        OLED_WriteCmd(x & 0x0F);
        OLED_WriteCmd(0x10 | (x >> 4));
        OLED_WriteDataMulti((uint8_t *)CN_Font[idx], 16);

        // 下半: page y+1, 写 data[16..31]
        OLED_WriteCmd(0xB0 + y + 1);
        OLED_WriteCmd(x & 0x0F);
        OLED_WriteCmd(0x10 | (x >> 4));
        OLED_WriteDataMulti((uint8_t *)CN_Font[idx] + 16, 16);

        x += 16;
        if(x > 112){
            x = 0;
            y += 2;
        }
        ch += 3;  // 跳到下一个汉字 (UTF-8 3字节)
    }
}
