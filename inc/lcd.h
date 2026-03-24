/*
 * lcd.h
 *
 *  Created on: Dec 12, 2025
 *      Author: dipch
 */

#ifndef LCD_H_
#define LCD_H_
#include "stm32f407xx.h"

/* User exposed LCD APIs*/

void lcd_init(void);
void lcd_ins(uint8_t);
void lcd_disp_byte(uint8_t,uint8_t,uint8_t);
void lcd_disp_string(char*,uint8_t,uint8_t);




/*User exposed LCD macros */

#define LCD_GPIO_PORT  GPIOD
#define LCD_GPIO_RS	   GPIO_PIN_NO_0
#define LCD_GPIO_RW	   GPIO_PIN_NO_1
#define LCD_GPIO_EN	   GPIO_PIN_NO_2
#define LCD_GPIO_D4	   GPIO_PIN_NO_3
#define LCD_GPIO_D5	   GPIO_PIN_NO_4
#define LCD_GPIO_D6	   GPIO_PIN_NO_5
#define LCD_GPIO_D7	   GPIO_PIN_NO_6

/*LCD instructions */
#define LCD_INS_4DL_2N_5X8F  		0x28
#define LCD_INS_DON_CURON    		0x0E
#define LCD_INS_INCADD       		0x06
#define LCD_INS_DIS_CLEAR    		0X01
#define LCD_INS_DIS_RETURN_HOME  	0x02


#endif /* LCD_H_ */
