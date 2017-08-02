/*
 * oled.h
 *
 *  Created on: 2017年7月27日
 *      Author: Cotyledon
 */

#ifndef TOOLS_OLED_H_
#define TOOLS_OLED_H_

#define OLED_SCL(x)     U_PORT_Oled_SCL_Pin_Write(x)
#define OLED_SDA(x)     U_PORT_Oled_SDA_Pin_Write(x)
#define OLED_RST(x)     U_PORT_Oled_RST_Pin_Write(x)
#define OLED_DC(x)      U_PORT_Oled_DC_Pin_Write(x)

extern void oled_init(void);
extern void oled_CLS(void);
extern void oled_P6x8Char(unsigned char x,unsigned char y,unsigned char ch);
extern void oled_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
extern void oled_PrintValueC(unsigned char x, unsigned char y, char data);
extern void oled_PrintValueI(unsigned char x, unsigned char y, int data);
extern void oled_PrintValueFP(unsigned char x, unsigned char y, unsigned int data, unsigned char num);
extern void oled_PrintValueF(unsigned char x, unsigned char y, float data, unsigned char num);

#endif /* TOOLS_OLED_H_ */
